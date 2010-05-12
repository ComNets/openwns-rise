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

#include <RISE/stations/station.hpp>
#include <RISE/transmissionobjects/transmissionobjectbf.hpp>
#include <RISE/antenna/Beamforming.hpp>
#include <WNS/rng/RNGen.hpp>
#include <boost/numeric/ublas/blas.hpp>
#include <boost/numeric/ublas/lu.hpp>

#include <cmath>
#include <algorithm>
#include <fstream>

using namespace rise::antenna;

Beamforming::Beamforming(const wns::pyconfig::View& pyConfigView,
						 Station* const station)
	: Antenna(pyConfigView, station),
	  pd_noAntennaElements(1),
	  pd_lastPowerReceived(),
	  pd_receivedTO2PatternMap(),
	  arrayLayout(linear),
	  positionErrorVariance(pyConfigView.get<double>("positionErrorVariance")),
	  dis(0.0, positionErrorVariance, wns::simulator::getRNG())

{
	pd_noAntennaElements = pyConfigView.get<long int>("noOfElements");
	if (pyConfigView.get<std::string>("arrayLayout") == "circular")
		arrayLayout = circular;

	if (drawPattern == true)
		drawRadiationPattern();

	MESSAGE_BEGIN(NORMAL, log, m,"Beamforming antenna initiated with layout (linear=0): ");
	m << arrayLayout << ", " << pd_noAntennaElements <<  " elements, and "
	  << positionErrorVariance << " variance in position estimation";
	MESSAGE_END();

	// Create the steervectors for all 360 degrees once and for all These
	// steervectors only depend on the geometry of the array (noAntennaElements,
	// ArrayLayout) which is known once the BF antenna is created.
	steervectors.resize(360, 0);

	for (unsigned long int i = 0; i < 360; i++)
	{
		steervectors[i] = new boost::numeric::ublas::vector<std::complex<double> >(pd_noAntennaElements);
		for (unsigned long int j = 0; j < pd_noAntennaElements; j++)
		{
            // Now I have to add pi here!?
			std::complex<double> c = std::polar(1.0, calculatePhaseDelta(j, double(i)*M_PI/180+M_PI)); 
			(*steervectors[i])(j) = c;
        }
	}
}

Beamforming::~Beamforming()
{
	for (unsigned long int i = 0; i < 360; ++i)
	{
		delete steervectors[i];
	}
	pd_receivedTO2PatternMap.clear();
}

wns::Ratio
Beamforming::getGain(const wns::Position& pos,
					 const TransmissionObjectPtr& TO) const
{
	TO2PatternMap::const_iterator itr = pd_receivedTO2PatternMap.find(TO);
	assure(itr != pd_receivedTO2PatternMap.end(), "The TransmissionObject is not registered for receiving at this antenna!");
	return getGain(pos, itr->second);
}

wns::Ratio
Beamforming::getGain(const wns::Position& pos,
					 PatternPtr pattern) const
{
	double phi = (pos - getPosition()).getAzimuth();
	if (phi < 0) phi += M_PI + M_PI;
	unsigned long int index = (unsigned long int)(180 * phi / M_PI + 0.5); // 0.5 to round
	if(index == 360) index = 0;
	assure(index < pattern->getSize(), "pattern index out of range");

	wns::Ratio a_directivity = pd_getGainEntry(index, pattern);
	MESSAGE_BEGIN(VERBOSE, log, m,"Directivity in (");
	m << pos.getX() << "," << pos.getY() <<  "," << pos.getZ() << "): " << a_directivity.get_dB();
	MESSAGE_END();
    return a_directivity;
}

wns::Ratio
Beamforming:: pd_getGainEntry(unsigned long int index,
							  PatternPtr pattern) const
{
	assure(index < pattern->getSize(), "pattern index out of range");
	double t = pattern->pattern.at(index);
	return wns::Ratio::from_factor(t*t);
}

void
Beamforming::setPowerReceivedForStation(Station* const id,
										const wns::Power power)
{
	pd_lastPowerReceived[id] = power;
}

void
Beamforming::setTxPowerForStation(Station* const id,
								  const wns::Power power)
{
	pd_lastTxPower[id] = power;
}

void
Beamforming::startReceiving(const TransmissionObjectPtr& TO,
							PatternPtr pattern)
{
	pd_receivedTO2PatternMap[TO] = pattern;
}

void
Beamforming::stopReceiving(const TransmissionObjectPtr& TO)
{
	pd_receivedTO2PatternMap.erase(pd_receivedTO2PatternMap.find(TO));
}

std::map<rise::Station*, wns::CandI>
Beamforming::calculateCandIsTx(const std::map<Station*, wns::Power>& station2iIntercellPlusNoise,
							   wns::Power xFriendlyness,
							   wns::Power txPower,
							   bool eirpLimited)
{
	// SINR estimation in Tx case, in other words does the following: how much
	// do I interfere station A when I send simultaniously to all the other
	// stations from the given combination (B, C,...). This means that in the
	// calculation of SINR for station A, the following values, or its estimations
	// should be used:

	//		Tx power for X + pathloss from me to A + X's pattern towards A,

	// where X traverses all the stations from the given combination: in case X = A
	// the value is signal strength, and if they are different the value is summed
	// to interference.

	std::vector<Station*> combination;
	for(std::map<Station*, wns::Power>::const_iterator itr = station2iIntercellPlusNoise.begin();
		itr != station2iIntercellPlusNoise.end();
		itr ++)
	{
		combination.push_back(itr->first);
	}
	std::map <Station*, PatternPtr > outpattern;
	std::vector<Station*>::const_iterator s;
	for (s = combination.begin(); s != combination.end(); s++)
	{
		std::vector<Station*> undesired = combination;
		undesired.erase(std::find(undesired.begin(), undesired.end(), *s));
		outpattern[*s] = p_calculateBeam(*s, undesired, xFriendlyness);
	}

	double txPowerReduction = 0.0;
	if(eirpLimited){
		// if required, the txPower is restricted to the EIRP
		wns::service::phy::ofdma::SumPattern sumPattern;
		assure(sumPattern.getSize() == 0, "new sum Pattern is not empty");
		assure(!combination.empty(), "combination is empty");
		for (s = combination.begin(); s != combination.end(); s++) {
			MESSAGE_SINGLE(VERBOSE, log, "maximum gain of pattern to add: " << outpattern[*s]->getMaxGain());
			sumPattern.add(outpattern[*s]);
			MESSAGE_SINGLE(VERBOSE, log, "current maximum gain of pattern: " << sumPattern.getMaxGain());
		}
		txPowerReduction = sumPattern.getMaxGain().get_factor();
		MESSAGE_SINGLE(NORMAL, log, "Tx power reduction per concurrent stream due to EIRP 1/" << txPowerReduction);
	}
	else if (!eirpLimited){
		// in SDMA, the txPower of the antenna is equally distributed to all
		// concurrent streams
		txPowerReduction = combination.size()*1.0;
		MESSAGE_SINGLE(NORMAL, log, "Tx power reduction per concurrent stream is 1/" << txPowerReduction);
	}

	std::map<rise::Station*, wns::CandI > candis;

	for (s = combination.begin(); s != combination.end(); s++)
	{
		double signal = 0.0;
		double iIntra = 0.0;

		// the angle is fixed, traverse the patterns
		unsigned long int grad = (unsigned long int)(pd_azimuthAngles[*s]*180/M_PI + 0.5); //0.5 to round
		if (grad == 360) grad = 0;
		assure(grad < outpattern[*s]->getSize(), "pattern index (grad) out of range");

		float pathloss = pd_lastTxPower[*s].get_mW() / pd_lastPowerReceived[*s].get_mW();

		for (std::vector<Station*>::const_iterator interferer = combination.begin(); interferer != combination.end(); interferer++)
		{
			assure(pd_lastTxPower.find(*interferer) != pd_lastTxPower.end(),
				   "TX power of station not yet registred at this antenna!");

			if ((*s) == (*interferer))
				signal = outpattern[*s]->pattern.at(grad) * outpattern[*s]->pattern.at(grad) *
					txPower.get_mW() / pathloss / txPowerReduction;
			else
				iIntra += outpattern[*interferer]->pattern.at(grad) * outpattern[*interferer]->pattern.at(grad) *
					txPower.get_mW() / pathloss / txPowerReduction;
		}

		// sum up entire interference
		double interference = 0.0;
		// add inter-cell interference and noise
		interference += station2iIntercellPlusNoise.find(*s)->second.get_mW();
		// add intra-cell interference
		interference += iIntra;

		assure(interference > 0.0, "Interference + Noise is not positive!");
		assure(signal > 0.0, "Carrier Signal is not greater than zero!");

		//return value
		wns::CandI candi = wns::CandI();
		candi.sdma.iIntra = wns::Power::from_mW(iIntra);
		candi.C = wns::Power::from_mW(signal);
		candi.I = wns::Power::from_mW(interference);
		candis[*s] = candi;
	}
	return candis;
}


std::map<rise::Station*, wns::CandI >
Beamforming::calculateCandIsRx(const std::vector<Station*>& combination,
							  wns::Power iInterPlusNoise)
{
	assure(iInterPlusNoise.get_mW() > 0.0, "given interference plus noise is not positive!");

	std::map <Station*, PatternPtr > outpattern;
	std::vector<Station*>::const_iterator s;
	for (s = combination.begin(); s != combination.end(); s++)
	{
		std::vector<Station*> undesired = combination;
		undesired.erase(std::find(undesired.begin(), undesired.end(), *s));
		// in order to calculate the perfect beam, which seems to be
		// unrealistic, only the inter-cell interference should be given to
		// calculate the beam
		outpattern[*s] = p_calculateBeam(*s, undesired, iInterPlusNoise);
	}

	std::map<rise::Station*, wns::CandI > candis;

	for (s = combination.begin(); s != combination.end(); s++)
	{
		double signal = 0.0;
 		double iIntra = 0.0;

		unsigned long int grad = 0;
		// the pattern is fixed, angles change
		for (std::vector<Station*>::const_iterator interferer = combination.begin(); interferer != combination.end(); interferer++)
		{
			grad = (unsigned long int)(pd_azimuthAngles[*interferer]*180/M_PI +0.5); //0.5 to round
			if(grad == 360) grad = 0;
			assure(grad < outpattern[*s]->getSize(), "pattern index (grad) out of range");

			if ((*s) == (*interferer))
			{
				MESSAGE_BEGIN(VERBOSE, log, m, "");
				m << "amplitude factor to desired station: " << outpattern[*s]->pattern.at(grad);
				MESSAGE_END();
				signal = outpattern[*s]->pattern.at(grad) * outpattern[*s]->pattern.at(grad) * pd_lastPowerReceived[*s].get_mW();
			}
			else
			{
				MESSAGE_BEGIN(VERBOSE, log, m, "");
				m << "amplitude factor to interferer: " << outpattern[*s]->pattern.at(grad);
				MESSAGE_END();
				iIntra += outpattern[*s]->pattern.at(grad) * outpattern[*s]->pattern.at(grad) * pd_lastPowerReceived[*interferer].get_mW();
			}
		}
		//sum up the entire interference
		double interference = 0.0;
		// add omni-directional interference and noise
		interference += iInterPlusNoise.get_mW();
		//add intra-cell interference
		interference += iIntra;

		assure(interference > 0.0, "Interference is not positive!");
		assure(signal > 0.0, "Carrier Signal is not greater than zero!");

		//return value
		wns::CandI candi = wns::CandI();
		candi.sdma.iIntra = wns::Power::from_mW(iIntra);
		candi.C = wns::Power::from_mW(signal);
		candi.I = wns::Power::from_mW(interference);
		candis[*s] = candi;
	}
	return candis;
}

double
Beamforming::estimateDoA(Station* id)
{
	double azi = (id->getPosition() - this->getStation()->getPosition()).getAzimuth();
	azi += dis() * M_PI/180;
	return( azi );
}


PatternPtr
Beamforming::calculateAndSetBeam(Station* id,
								 const std::vector<Station*>& undesired,
								 wns::Power omniPower)
{
	PatternPtr beam = p_calculateBeam(id, undesired, omniPower);
	return beam;
}

double
Beamforming::calculatePhaseDelta(int antennaNo, double stationPhi) const
{
    // stationPhi in radians
	double delta = 0.0;
	double alpha = 0.0;
    // antenna spacing: lambda/2 = pi
	double antennaSpacing = M_PI;
    // radius of array is adapted to the antennaSpacing
	double r = antennaSpacing/2 / sin(M_PI / double(pd_noAntennaElements));

	switch (arrayLayout)
	{
	case circular:
        // antenna elements are assumed to be on a circle with (x=0, y=0) as
        // center, first element is located at (x=r, y=0)
		alpha = double(antennaNo) * 2.0 * M_PI / double(pd_noAntennaElements);
		delta = r*sqrt(2.0-2.0*cos(alpha))*sin(-stationPhi-alpha/2.0);

		break;
	case linear:
	default: // Linear is default
        // elements are assumed to be lined up along the y-axis, first antenna
        // element located at (x=0, y=0)
		delta = double(antennaNo)*antennaSpacing*sin(stationPhi);
	}

	return delta;
}


PatternPtr
Beamforming::p_calculateBeam(Station* id,
							 const std::vector<Station*>& undesired,
							 wns::Power omniPower)
{
	// assure that the cov matrix, which is inverted later, has
	// non-zero diagonal-elements. Otherwise the algorithms performes unpredictable
	assure(omniPower.get_mW() > 0, "beam pattern calculation requires positive level of omni-directional power");

	std::complex<double> c;
    boost::numeric::ublas::vector<std::complex<double> > steervector(pd_noAntennaElements); 
    boost::numeric::ublas::matrix<std::complex<double> > kov(pd_noAntennaElements, pd_noAntennaElements);
    kov = 
        boost::numeric::ublas::zero_matrix<std::complex<double> >(pd_noAntennaElements, pd_noAntennaElements);

	assure(getStation() != id, "Desired and beamforming station are the same!");
    //build covariance matrix:
	wns::Position myPosition = getPosition();
	for (std::vector<Station*>::const_iterator s = undesired.begin(); s != undesired.end(); s ++){
		//build steering vector:
		assure(*s != getStation(), "Undesired and beamforming station are the same!");
		assure(*s != id, "Undesired and desired station are the same!");

		pd_azimuthAngles [*s] = ((*s)->getAntenna()->getPosition() - myPosition).getAzimuth();
		//add positionError to azimuth calculation (in order to model the actual position estimation)
		pd_azimuthAngles [*s] += dis() * M_PI/180;

	 	if (pd_azimuthAngles[*s] < 0)
 			pd_azimuthAngles[*s] += M_PI + M_PI;

		for (unsigned long int i = 0; i < pd_noAntennaElements; i++)
		{
			c = std::polar(1.0, calculatePhaseDelta(i, pd_azimuthAngles[*s]));
			steervector(i) = c;
		}
		assure(pd_lastPowerReceived.find(*s) != pd_lastPowerReceived.end(), "Station is not yet registered at this antenna!");
        //Hermitian rank 1 update
        kov += pd_lastPowerReceived[*s].get_mW() 
            * boost::numeric::ublas::outer_prod(steervector, boost::numeric::ublas::conj(steervector));
    }

    //desired station as last, is also needed for weight calculation
	pd_azimuthAngles [id] = (id->getAntenna()->getPosition() - myPosition).getAzimuth();
	//add positionError to azimuth calculation (in order to model the actual position estimation)
	pd_azimuthAngles [id] += dis() * M_PI/180;

 	if (pd_azimuthAngles[id] < 0)
 		pd_azimuthAngles[id] += M_PI + M_PI;
    for (unsigned long int i = 0; i < pd_noAntennaElements; i++)
	{
		c = std::polar(1.0, calculatePhaseDelta(i, pd_azimuthAngles[id]));
		steervector(i) = c;
    }
	assure(pd_lastPowerReceived.find(id) != pd_lastPowerReceived.end(), "Station is not yet registered at this antenna!");
    //Hermitian rank 1 update
    kov += pd_lastPowerReceived[id].get_mW()
         * boost::numeric::ublas::outer_prod(steervector, boost::numeric::ublas::conj(steervector));

   //covariance matrix almost complete, now only noise entries necessary:
    std::complex<double> n = std::complex<double>(omniPower.get_mW(), 0);
    for (unsigned long int i = 0; i < pd_noAntennaElements; i++){
		kov(i, i) = kov(i, i) + n;
    }
    //Calculate Weight vector
    boost::numeric::ublas::vector<std::complex<double> > w(pd_noAntennaElements);
    // restore full matrix:
    p_herm_rest(kov, pd_noAntennaElements);
    //Solving kov*w = sttervector using LU method:
    boost::numeric::ublas::permutation_matrix<> p(pd_noAntennaElements);
    boost::numeric::ublas::lu_factorize(kov, p);
    w = steervector;
    boost::numeric::ublas::lu_substitute(kov, p, w);

    // compute antenna pattern:
    std::complex<double> pattern[360];
    boost::numeric::ublas::vector<std::complex<double> > wTrans = boost::numeric::ublas::trans(w);
    for (unsigned long int i = 0; i < 360; i++){
        pattern[i] = boost::numeric::ublas::inner_prod(wTrans, (*steervectors[i]));
	}
    //Normalize pattern to transmit / receive the same power as an omni-antenna:
	double sumPower = 0.0;
    for (unsigned long int i = 0; i < 360; i++)
		sumPower += pow(std::abs(pattern[i]),2);
	sumPower = sumPower / 360;

	PatternPtr beam(new Pattern());
	beam->pattern.reserve(360);
    for (unsigned long int i = 0; i < 360; i++)
	{
		double amplitudeFactor = std::abs(pattern[i])/sqrt(sumPower);
		beam->pattern.push_back(amplitudeFactor);
	}
	assure(beam->getSize() == 360, "pattern has no valid size");
    // the omni attenuation factor is no longer needed, it is always one
	//beam->omniAttenuation = wns::Ratio::from_factor(1);
	unsigned long int grad = (unsigned long int)(pd_azimuthAngles[id]*180/M_PI +0.5); //0.5 to round
	if(grad == 360) grad = 0;
	assure(grad < beam->pattern.size(), "pattern index (grad) out of range");
	double desiredAntennaFactor =  beam->pattern.at(grad);

    MESSAGE_BEGIN(VERBOSE, log, m, "");
	m << "antenna factor in look direction: "<< desiredAntennaFactor << "\n";
	m << "antenna factor(s) to undesired station(s): ";
	for (std::vector<Station*>::const_iterator s = undesired.begin(); s != undesired.end(); s ++){
		grad = (unsigned long int)(pd_azimuthAngles[*s]*180/M_PI +0.5); //0.5 to round
		if(grad == 360) grad = 0;
		m << beam->pattern.at(grad) << " ";
	}
	MESSAGE_END();

	for (std::vector<Station*>::const_iterator s = undesired.begin(); s != undesired.end(); s ++){
		grad = (unsigned long int)(pd_azimuthAngles[*s]*180/M_PI +0.5); //0.5 to round
		if(grad == 360) grad = 0;

		if( beam->pattern.at(grad) < desiredAntennaFactor){
			MESSAGE_BEGIN(NORMAL, log, m, "");
			m << "antenna factor in look direction: "<< desiredAntennaFactor
			  << " is smaller than antenna factor to undesired station "
			  << beam->pattern.at(grad);
			MESSAGE_END();
			drawRadiationPattern();
		}
	}
	return beam;
}


void Beamforming::p_herm_rest(boost::numeric::ublas::matrix<std::complex<double> >& c, const unsigned long int m) const
{
	//Restoration of efficiently stored hermitian matrix
	std::complex<double> z;
	double re, im;
	for (unsigned long int i = 0; i < m; i++){
		for (unsigned long int j = i+1; j < m; j++){
			re = c(i, j).real();
			im = c(i, j).imag();
			z = std::complex<double>(re, -im);
			c(j, i) = z;
			// real part symmetrically completed, imaginary part neg. symm.
		}
	}
}

void Beamforming::drawRadiationPattern(std::string file, PatternPtr pattern)
{
	const unsigned long int resolution = 360;
	double step = 2 * M_PI / resolution;
	//double zero_dB_length = radius;
    double length = 0, angle = 0;
	wns::Ratio gain = wns::Ratio::from_dB(0),
		gain_min = wns::Ratio::from_dB(-10);
	std::fstream outfile;
	outfile.open(file.c_str(), std::ios::out | std::ios::app);
	if (!outfile.is_open())
	{
		MESSAGE_BEGIN(NORMAL, log, m,"File ");
		m << file + " could not be opened." + " Simulation will continue.\n";
		MESSAGE_END();
		return;
	}
	wns::Position myPosition = getPosition();
	for (unsigned long int k=0; k<resolution; k++)
	{
		angle = step * k;
		gain = pd_getGainEntry(k, pattern);
//		if (gain<gain_min) length = 0;
//		else length = (zero_dB_length /(-gain_min.get_dB())) * gain.get_dB() + zero_dB_length;
		length = gain.get_factor();
		outfile << myPosition.getX() + length * cos(angle) <<  " "
				<< myPosition.getY() + length * sin(angle) << std::endl;
	}
	outfile << "\n";
	outfile.close();
}

void Beamforming::drawRadiationPattern() const
{
	const unsigned long int resolution = 360;
	double step = 2 * M_PI / resolution, length = radius, angle = 0;
	std::fstream outfile;
	outfile.open(patternOutputFile.c_str(), std::ios::out | std::ios::app);
	if (!outfile.is_open())
	{
		MESSAGE_BEGIN(NORMAL, log, m,"File ");
		m << patternOutputFile + " could not be opened." + " Simulation will continue.\n";
		MESSAGE_END();
		return;
	}
	wns::Position myPosition = getPosition();
	for (unsigned long int k=0; k<resolution; k++)
	{
		angle = step * k;
		outfile << myPosition.getX() + length * cos(angle) <<  " "
				<< myPosition.getY() + length * sin(angle) << std::endl;
	}
	outfile << "\n";
	outfile.close();
}


