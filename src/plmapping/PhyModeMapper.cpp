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

#include <WNS/Assure.hpp>
#include <WNS/StaticFactory.hpp>
#include <WNS/StaticFactoryBroker.hpp>
#include <WNS/PyConfigViewCreator.hpp>
#include <WNS/service/phy/phymode/PhyModeInterface.hpp>
#include <WNS/service/phy/phymode/PhyModeMapperInterface.hpp>
#include <RISE/plmapping/PhyModeMapper.hpp>

using namespace rise::plmapping;

// Situation: There are (too) many PhyModeMapper objects instanciated,
// although they are all contain the same data.
// TODO: is this the way to make a singleton?
STATIC_FACTORY_BROKER_REGISTER(rise::plmapping::PhyModeMapper,
							   wns::service::phy::phymode::PhyModeMapperInterface,
							   "rise.plmapping.PhyModeMapper");

/*
STATIC_FACTORY_REGISTER_WITH_CREATOR(rise::plmapping::PhyModeMapper,
									 wns::service::phy::phymode::PhyModeMapperInterface,
									 "rise.plmapping.PhyModeMapper",
									 wns::PyConfigViewCreator);
*/

PhyModeMapper::PhyModeMapper(const wns::pyconfig::View& config)
	: subCarriersPerSubChannel(config.get<unsigned int>("subCarriersPerSubChannel")),
	  symbolDuration(config.get<simTimeType>("symbolDuration")),
	  phyModeCount(config.len("mapEntries")),
	  minimumSINR(config.get<double>("minimumSINR")),
	  mi2perMapper(wns::service::phy::phymode::CoderFullMappingInterface::getCoderFullMapping(config.get("mi2perMapper"))),
	  SNR2MImapper(wns::service::phy::phymode::SNR2MIInterface::getSNR2MImapper(config.get("snr2miMapper"))),
	  logger(std::string("RISE"), std::string("PhyModeMapper"))
	  //logger(config.get("logger")) // TODO
{
	// phyModeRangeMap is empty here
	// iterate through the SINR-2-PhyMode table
	for (unsigned int phymodeIndex=0; phymodeIndex < phyModeCount; ++phymodeIndex) {
		//MESSAGE_SINGLE(NORMAL, logger, "PhyModeMapper::Constructor: phymodeIndex="<<phymodeIndex);
		wns::pyconfig::View mapEntry = config.getView("mapEntries",phymodeIndex);
		// mapEntry contains "phyMode" and "sinrInterval"

		//wns::SmartPtr<rise::plmapping::PhyMode> aPhyMode =
		//	wns::SmartPtr<rise::plmapping::PhyMode>
		wns::service::phy::phymode::PhyModeInterface* phyModeInterfacePtr = wns::service::phy::phymode::createPhyModeNonConst(mapEntry.get<wns::pyconfig::View>("phyMode"));

		PhyModePtr aPhyMode =
			PhyModePtr(dynamic_cast<rise::plmapping::PhyMode*>(phyModeInterfacePtr));
		MESSAGE_SINGLE(NORMAL, logger, "PhyModeMapper::Constructor: aPhyMode["<<phymodeIndex<<"]="<<*aPhyMode);

		aPhyMode->setSubCarriersPerSubChannel(subCarriersPerSubChannel);
		aPhyMode->setSymbolDuration(symbolDuration);
		assure(aPhyMode->dataRateIsValid(),"unknown dataRate for PhyMode "<< *aPhyMode);
		phyModeVector.push_back(aPhyMode);
		wns::service::phy::phymode::SINRRange sinrRange =
			wns::service::phy::phymode::SINRRange::CreateFrom(mapEntry.get<wns::pyconfig::View>("sinrInterval"));
		phyModeRangeMap.insert( sinrRange, phymodeIndex );
		phyModeSINRRangeRegistry.insert( *aPhyMode,sinrRange );
	}
}

wns::service::phy::phymode::PhyModeInterfacePtr
PhyModeMapper::getBestPhyMode(const wns::Ratio& sinr) const
{
	return getBestPhyMode(sinr.get_dB());
}

wns::service::phy::phymode::PhyModeInterfacePtr
PhyModeMapper::getBestPhyMode(double sinr /* dB */) const
{
	unsigned int phymodeIndex;
	try {
		phymodeIndex=phyModeRangeMap.get(sinr);
        MESSAGE_SINGLE(NORMAL, logger,"phymodeIndex: "<<phymodeIndex);

	} catch ( ... ) { // out of range
		if (sinr<0) { return getLowestPhyMode(); }
		else { return getHighestPhyMode(); }
	}
	return phyModeVector[phymodeIndex];
}

int
PhyModeMapper::getBestPhyModeIndex(double sinr /* dB */) const
{
	return phyModeRangeMap.get(sinr);
}

wns::Ratio
PhyModeMapper::getMinSINRRatio(const wns::service::phy::phymode::PhyModeInterfacePtr phyMode) const
{
	assure(phyMode != wns::service::phy::phymode::PhyModeInterfacePtr(),"invalid phyModePtr");
	wns::Ratio ratio;
	ratio.set_dB(getMinSINR(phyMode)); // double dB
	return ratio;
}

/** @brief find required (minimum) SINR [dB] for a certain PhyMode */
double
PhyModeMapper::getMinSINR(const wns::service::phy::phymode::PhyModeInterfacePtr phyMode) const
{
	assure(phyMode != wns::service::phy::phymode::PhyModeInterfacePtr(),"invalid phyModePtr");
	const rise::plmapping::PhyMode& aPhyMode = static_cast<const rise::plmapping::PhyMode&>(*phyMode);
	double minSINR = phyModeSINRRangeRegistry.find(aPhyMode).min();
	if (minSINR<minimumSINR) { minSINR=minimumSINR; }
	return minSINR;
}

/** @brief find possible SINR range for a certain PhyMode */
wns::service::phy::phymode::SINRRange
PhyModeMapper::getSINRRange(const wns::service::phy::phymode::PhyModeInterfacePtr phyMode) const
{
	assure(phyMode != wns::service::phy::phymode::PhyModeInterfacePtr(),"invalid phyModePtr");
	const rise::plmapping::PhyMode& aPhyMode = static_cast<const rise::plmapping::PhyMode&>(*phyMode);
	return phyModeSINRRangeRegistry.find(aPhyMode);
}

const wns::service::phy::phymode::PhyModeInterfacePtr
PhyModeMapper::getHighestPhyMode() const
{
	return phyModeVector.back();
}

const wns::service::phy::phymode::PhyModeInterfacePtr
PhyModeMapper::getLowestPhyMode() const
{
	return phyModeVector.front();
}

int
PhyModeMapper::getPhyModeCount() const
{
	return (int)phyModeCount;
}

PhyModeMapper::PhyModeVector
PhyModeMapper::getListOfPhyModes() const
{
	return phyModeVector;
}

const std::vector< wns::service::phy::phymode::PhyModeInterfacePtr >
PhyModeMapper::getListOfPhyModePtr() const
{
	std::vector< wns::service::phy::phymode::PhyModeInterfacePtr > phyModePtrVector(phyModeCount);
	for(unsigned int i=0; i<phyModeCount; i++) {
		phyModePtrVector[i] = phyModeVector[i];
	}
	return phyModePtrVector;
}

wns::service::phy::phymode::PhyModeInterfacePtr
PhyModeMapper::getPhyModeForIndex(int index) const
{
	assure(index>=0,"invalid index="<<index);
	return phyModeVector[index];
}

int
PhyModeMapper::getIndexForPhyMode(const wns::service::phy::phymode::PhyModeInterface& phyMode) const
{
	for (unsigned int phymodeIndex=0; phymodeIndex < phyModeCount; ++phymodeIndex) {
		if (*(phyModeVector[phymodeIndex]) == phyMode) return phymodeIndex;
	}
	// this helps debugging, in case the phyMode is not found:
	/*
	MESSAGE_SINGLE(NORMAL, logger, "getIndexForPhyMode(PhyMode="<<phyMode<<" @ "<<&phyMode<<"): not found");
	for (unsigned int phymodeIndex=0; phymodeIndex < phyModeCount; ++phymodeIndex) {
		MESSAGE_SINGLE(NORMAL, logger, "getIndexForPhyMode(PhyMode="<<phyMode<<" @ "<<&phyMode<<"): compared to phyModeVector["<<phymodeIndex<<"]="<<*(phyModeVector[phymodeIndex])<<" @ "<<(phyModeVector[phymodeIndex]));
	}
	*/
	assure(-1,"getIndexForPhyMode("<<phyMode<<"): not found");
	return wns::service::phy::phymode::UNDEFINED_PHYMODEINDEX; // (-1) = not found
}

void
PhyModeMapper::calculateSINRRanges(double targetPER, unsigned int bl)
{
	//std::cout<<"calculateSINRRanges(targetPER="<<targetPER<<", bl="<<bl<<"):"<<std::endl;
	// ::From(from).To(to);
	// ::FromIncluding(from).ToExcluding(to);
	// ::FromExcluding(from).ToIncluding(to);
	phyModeRangeMap = PhyModeRangeMap(); // empty phyModeRangeMap
	phyModeSINRRangeRegistry = PhyModeSINRRangeRegistry(); // empty phyModeSINRRangeRegistry
	double from = -200.0;
	double to;
	// This method can only collaborate with a rise::plmapping::SNR2MImapper
	rise::plmapping::SNR2MI* snr2miMapper = dynamic_cast<rise::plmapping::SNR2MI*>(SNR2MImapper);
	assure(snr2miMapper, "SNR2MI Mapper is not of type rise::plmapping::SNR2MI*");
	rise::plmapping::CoderFullMapping* coderMapper = dynamic_cast<rise::plmapping::CoderFullMapping*>(mi2perMapper);
	assure(coderMapper, "mi2perMapper is not of type rise::plmapping::CoderFullMapping*");

	PhyMode previousPhyMode = *(phyModeVector[0]); // worst PhyMode (BPSK*)
	{
		double mib = coderMapper->PER2MIB(targetPER, bl, previousPhyMode.getCoding()); // inversion
        minimumSINR = snr2miMapper->convertMIB2SNR(mib,previousPhyMode.getModulation()).get_dB();
		//std::cout<<"  minimumSINR("<<previousPhyMode<<") = "<<minimumSINR<<std::endl;
	}
	for(unsigned int phymodeIndex=1; phymodeIndex<phyModeCount; phymodeIndex++) { // start with 2nd
		wns::SmartPtr<PhyMode> phyModePtr = phyModeVector[phymodeIndex];
		double mib = coderMapper->PER2MIB(targetPER, bl, phyModePtr->getCoding()); // inversion
        to = snr2miMapper->convertMIB2SNR(mib,phyModePtr->getModulation()).get_dB();
		// sinr range for previous PhyMode
		wns::service::phy::phymode::SINRRange sinrRange =
			wns::service::phy::phymode::SINRRange::FromExcluding(from).ToIncluding(to);
		phyModeRangeMap.insert( sinrRange, phymodeIndex-1 );
		phyModeSINRRangeRegistry.insert( previousPhyMode,sinrRange );
		MESSAGE_SINGLE(NORMAL, logger, "  SINRRange("<<previousPhyMode<<") = "<<from<<"..."<<to);
		from = to;
		previousPhyMode = *phyModePtr; // copy
	}
	to = 200.0;
	wns::service::phy::phymode::SINRRange sinrRange =
			wns::service::phy::phymode::SINRRange::FromExcluding(from).ToIncluding(to);
	phyModeRangeMap.insert( sinrRange, phyModeCount-1 );
	phyModeSINRRangeRegistry.insert( previousPhyMode,sinrRange );
	//std::cout<<"  SINRRange("<<previousPhyMode<<") = "<<from<<"..."<<to<<std::endl;
}

std::string
PhyModeMapper::printSwitchingPoints() const
{
	std::stringstream line;
	// don't start with index 0; get lowest switching point from getMinimumSINR()
	line << "[ "<<getMinimumSINR()<<" ";
	for(unsigned int phymodeIndex=1; phymodeIndex<phyModeCount; phymodeIndex++) {
		wns::service::phy::phymode::PhyModeInterfacePtr phyModePtr = getPhyModeForIndex(phymodeIndex);
		double min = getSINRRange(phyModePtr).min();
		line <<min<<" ";
	}
	line<<"]";
	return line.str();
}

std::string
PhyModeMapper::printPhyModeNames() const
{
	std::stringstream line;
	line << "char( ";
	for(unsigned int phymodeIndex=0; phymodeIndex<phyModeCount; phymodeIndex++) {
		wns::service::phy::phymode::PhyModeInterfacePtr phyModePtr = getPhyModeForIndex(phymodeIndex);
		line <<"'"<< phyModePtr->getString() <<"', ";
	}
	line<<"'invalid' )";
	return line.str();
}


