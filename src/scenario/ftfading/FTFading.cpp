/*******************************************************************************
 * This file is part of openWNS (open Wireless Network Simulator)
 * _____________________________________________________________________________
 *
 * Copyright (C) 2004-2007
 * Chair of Communication Networks (ComNets)
 * Kopernikusstr. 5, D-52074 Aachen, Germany
 * phone: ++49-241-80-27910,
 * fax: ++49-241-80-22242
 * email: info@openwns.org
 * www: http://www.openwns.org
 * _____________________________________________________________________________
 *
 * openWNS is free software; you can redistribute it and/or modify it under the
 * terms of the GNU Lesser General Public License version 2 as published by the
 * Free Software Foundation;
 *
 * openWNS is distributed in the hope that it will be useful, but WITHOUT ANY
 * WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR
 * A PARTICULAR PURPOSE.  See the GNU Lesser General Public License for more
 * details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 ******************************************************************************/

#include <RISE/scenario/ftfading/FTFading.hpp>

#include <math.h>

using namespace rise;
using namespace rise::scenario;
using namespace rise::scenario::ftfading;



STATIC_FACTORY_REGISTER_WITH_CREATOR( FTFadingOff, FTFading, "rise.scenario.ftfading.FTFadingOff",  wns::PyConfigViewCreator);
STATIC_FACTORY_REGISTER_WITH_CREATOR( FTFadingFflat, FTFading, "rise.scenario.ftfading.FTFadingFflat",  wns::PyConfigViewCreator);
STATIC_FACTORY_REGISTER_WITH_CREATOR( FTFadingFuncorrelated, FTFading, "rise.scenario.ftfading.FTFadingFuncorrelated",  wns::PyConfigViewCreator);
STATIC_FACTORY_REGISTER_WITH_CREATOR( FTFadingFneighbourCorrelation, FTFading, "rise.scenario.ftfading.FTFadingFneighbourCorrelation",  wns::PyConfigViewCreator);

FTFading::FTFading(const wns::pyconfig::View& config)
	: logger(config.getView("logger")),
	  numberOfSubChannels(config.get<int>("numSubCarriers"))
{
	assure(!config.isNone("samplingTime"),"FTFading: samplingTime is None");
	samplingTime = config.get<double>("samplingTime");
	assure(samplingTime>=0.0,"samplingTime must be positive: t="<<samplingTime);
	//samplingFrequency = 1.0/samplingTime;
	// numberOfSubChannels can be 0, e.g. for FTFadingOff
}

FTFadingOff::FTFadingOff(const wns::pyconfig::View& config)
	: FTFading(config)
{
	MESSAGE_SINGLE(NORMAL, logger, "Using FTFadingOff strategy" );
}

wns::Ratio
FTFadingOff::getFTFading(int /*subCarrier*/)
{
	wns::Ratio OneFading = wns::Ratio(); // ::from_factor(1);
	return OneFading;
}

FTFadingJakes::FTFadingJakes(const wns::pyconfig::View& config)
	: FTFading(config),
	  _dopFreq(config.get<double>("dopFreq")),
	  //_sampFreq(config.get<double>("sampFreq")),
	  _numWaves(config.get<int>("numWaves")),
	  scheduler(wns::simulator::getEventScheduler())
{
	assure(numberOfSubChannels>0,"wrong numberOfSubChannels="<<numberOfSubChannels);
}

FTFadingFflat::FTFadingFflat(const wns::pyconfig::View& config)
	: FTFadingJakes(config)
{
	MESSAGE_SINGLE(NORMAL, logger, "Using FTFadingFflat strategy" );

	//samplePeriod = 1/_sampFreq; // new: samplingTime
	currentPeriodTime = 0;
	subCarrierFading = 0;

	jakesVector.push_back(new JakesFadingGenerator());
	jakesVector[0]->initJakes(_dopFreq,samplingTime,_numWaves);
	subCarrierFading = jakesVector[0]->getNextFadingValue(); // initial value
	CurrentFading = wns::Ratio::from_factor(subCarrierFading);
}

FTFadingFflat::~FTFadingFflat()
{
	delete jakesVector[0]; jakesVector[0]=NULL;
	jakesVector.clear();
}

wns::Ratio
FTFadingFflat::getFTFading(int /*subCarrier*/)
{
	simTimeType now = Now();
	if ( (now/samplingTime) >= currentPeriodTime){
		double diffPeriodTime = (floor(now/samplingTime)+1) - currentPeriodTime;
		// ^ should be int !
		for (int i = 0; i<diffPeriodTime; ++i){
			subCarrierFading = jakesVector[0]->getNextFadingValue();
		}
		CurrentFading = wns::Ratio::from_factor(subCarrierFading);
		currentPeriodTime = static_cast<int>(currentPeriodTime + diffPeriodTime);
	} else {
		// do nothing, i.e. return the same value
	}
	return CurrentFading;
}



FTFadingFuncorrelated::FTFadingFuncorrelated(const wns::pyconfig::View& config)
	: FTFadingJakes(config)
{
	MESSAGE_SINGLE(NORMAL, logger, "Using FTFadingFuncorrelated strategy" );

	for (int subCarrierCounter = 0; subCarrierCounter < numberOfSubChannels; ++subCarrierCounter){
		jakesVector.push_back(new JakesFadingGenerator());
		jakesVector[subCarrierCounter]->initJakes(_dopFreq,samplingTime,_numWaves);
		subCarrierFading.push_back(jakesVector[subCarrierCounter]->getNextFadingValue());
		currentFading.push_back(wns::Ratio::from_factor(subCarrierFading[subCarrierCounter]));
		sampleIndex.push_back(1);
	}
}

wns::Ratio
FTFadingFuncorrelated::getFTFading(int _subCarrier)
{
	assure( _subCarrier < numberOfSubChannels, "Exceeded Max Number of subcarriers!");
	simTimeType now = Now();

	if ( (now/samplingTime) >= (sampleIndex[_subCarrier]) ) {

		double diffPeriodTime = (floor(now/samplingTime)+1) - sampleIndex[_subCarrier];
		// ^ should be int !

		for (int i = 0; i<diffPeriodTime; ++i) {
		        subCarrierFading[_subCarrier] = jakesVector[_subCarrier]->getNextFadingValue();
		        currentFading[_subCarrier] = wns::Ratio::from_factor(subCarrierFading[_subCarrier]);
		}
		sampleIndex[_subCarrier] = static_cast<int>(sampleIndex[_subCarrier] + diffPeriodTime);
	}
	return currentFading[_subCarrier];
}



/** brief Correlation in time and only correlation between neighbour subacarriers in
 * frequency. The amount of correlation from the subcarriers which are applied
 * to form the fading is choosen  by neighbourCorrelationFactor**/
FTFadingFneighbourCorrelation::FTFadingFneighbourCorrelation(const wns::pyconfig::View& config)
	: FTFadingJakes(config),
	  _neighbourCorrelationFactor(config.get<double>("neighbourCorrelationFactor"))
{
	MESSAGE_SINGLE(NORMAL, logger, "Using FTFadingFneighbourCorrelation strategy with correlation factor= "<< _neighbourCorrelationFactor);
	assure(_neighbourCorrelationFactor <= 1, "Correlation factor out of order (bigger than 1)!");

	currentSampleIndex = 1;

	for (int subCarrierCounter = 0; subCarrierCounter < numberOfSubChannels; ++subCarrierCounter){
		jakesVector.push_back(new JakesFadingGenerator());
		jakesVector[subCarrierCounter]->initJakes(_dopFreq,samplingTime,_numWaves);
		subCarrierFading.push_back(jakesVector[subCarrierCounter]->getNextFadingValue());
		sampleIndex.push_back(1);
	}

	double correlatedFadingFirstSubcarrier = (1-_neighbourCorrelationFactor)*subCarrierFading[0] + _neighbourCorrelationFactor*subCarrierFading[numberOfSubChannels-1];
	currentFading.push_back(wns::Ratio::from_factor(correlatedFadingFirstSubcarrier));
	for (int subCarrierCounter = 1; subCarrierCounter < numberOfSubChannels; ++subCarrierCounter){
		double correlatedFading = (1-_neighbourCorrelationFactor)*subCarrierFading[subCarrierCounter] + _neighbourCorrelationFactor*currentFading[subCarrierCounter-1].get_factor();
	      	currentFading.push_back(wns::Ratio::from_factor(correlatedFading));
	}
}

wns::Ratio
FTFadingFneighbourCorrelation::getFTFading(int _subCarrier)
{
	simTimeType now = Now();

	if ( (now/samplingTime) >= (sampleIndex[_subCarrier]) ) {
		if (sampleIndex[_subCarrier] == currentSampleIndex) {
			double diffPeriodTime = (floor(now/samplingTime)+1) - (sampleIndex[_subCarrier]);
			// ^ should be int !
			for (int i = 0; i<diffPeriodTime; ++i){
				for (int subCarrierCounter = 0; subCarrierCounter <  numberOfSubChannels; ++subCarrierCounter) {
					subCarrierFading[subCarrierCounter] = jakesVector[subCarrierCounter]->getNextFadingValue();
				}

				double correlatedFadingFirstSubcarrier = (1-_neighbourCorrelationFactor)*subCarrierFading[0] + _neighbourCorrelationFactor*subCarrierFading[numberOfSubChannels-1];
				currentFading[0] = wns::Ratio::from_factor(correlatedFadingFirstSubcarrier);
				for (int subCarrierCounter = 1; subCarrierCounter < numberOfSubChannels; ++subCarrierCounter){
					double correlatedFading = (1-_neighbourCorrelationFactor)*subCarrierFading[subCarrierCounter] + _neighbourCorrelationFactor*currentFading[subCarrierCounter-1].get_factor();
					currentFading[subCarrierCounter] = wns::Ratio::from_factor(correlatedFading);
				}
			}

			sampleIndex[_subCarrier] = static_cast<int>(sampleIndex[_subCarrier] + diffPeriodTime);
			currentSampleIndex = sampleIndex[_subCarrier];
		} else if (sampleIndex[_subCarrier] < currentSampleIndex) {
			sampleIndex[_subCarrier] = currentSampleIndex;
		}
	}
	return currentFading[_subCarrier];
}

