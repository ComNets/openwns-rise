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

#include <RISE/transceiver/ReceiverBF.hpp>
#include <RISE/stations/station.hpp>
#include <RISE/transceiver/cache/propcacheentry.hpp>
#include <RISE/manager/systemmanager.hpp>
#include <RISE/antenna/Beamforming.hpp>
#include <RISE/transceiver/transmitter.hpp>
#include <RISE/transceiver/cache/propagationcache.hpp>
#include <RISE/transmissionobjects/transmissionobjectbf.hpp>
#include <RISE/scenario/Scenario.hpp>

using namespace rise;

ReceiverBF::ReceiverBF(Station* s,
		       antenna::Beamforming* a,
		       Demodulator *demodulator,
		       Decoder *decoder,
		       wns::Ratio rnf)
	: ReceiverBase(s, demodulator, decoder, rnf),
	  antenna(a)
{}

ReceiverBF::~ReceiverBF()
{}

wns::Power ReceiverBF::getRxPower(const TransmissionObjectPtr& t,
								  const TransmissionObjectPtr& whenListeningToTO)
{
	//transmit power
	wns::Power rxPower = t->getTxPower();
	//antenna gain - transmitter side, at the posision of the receiver
	rxPower += t->getTransmittersAntennaGain(getAntenna()->getPosition());
	//antenna gain - receiver side, at the position of the transmitter
	//jem : separate beamforming and nonbeamforming transmitters,
	//      receivers... everything to avoid dynamic casts
	rxPower += getAntenna()->getGain(t->getTransmitter()->getAntenna()->getPosition(),
									 whenListeningToTO);
	PhysicalResourceIterator itr,itrEND;
	itr = prc.begin();
	itrEND = prc.end();
	while((!(*itr)->contains(t)) && itr != itrEND)
	{
		++itr;
	}
	assert(itr!=itrEND);
	double freq = (*itr)->getFrequency();
	rxPower -= propCache->getLoss(t->getTransmitter(),freq);
	return rxPower;
}

wns::Power ReceiverBF::getInterference(const TransmissionObjectPtr& t)
{
  	wns::Power rxPower = getRxPower(t, t);
  	assert(this->contains(t));
 	wns::Power Interference = getTotalRxPower(t) - rxPower;
 	assert(Interference>=getNoise()*.99);
	return Interference;
}


wns::Power ReceiverBF::getTotalRxPower(const TransmissionObjectPtr& whenListeningToTO)
{
	assure(active, "Receiver is not active! Should we return 0 W here?");
	totalRxPower = getNoise();
	PhysicalResourceIterator itr,itrEND;
	itrEND = prc.end();
	itr = prc.begin();
	while(itr != itrEND)
	{
		medium::PhysicalResource::TransmissionObjectIterator TOI,TOI_END;
		TOI_END = (*itr)->getTOEnd();
		for(TOI = (*itr)->getTOBegin(); TOI != TOI_END; ++TOI) {
			totalRxPower += getRxPower(*TOI, whenListeningToTO);
		}
		++itr;
	}
	return totalRxPower;
}

void ReceiverBF::writeCacheEntry(PropCacheEntry& cacheEntry,
								 Transmitter* t,
								 double freq)
{
	wns::Ratio pl = getPathloss(*t, freq);
	wns::Ratio sh = getShadowing(*t);

	antenna::Antenna* txA = t->getAntenna();
	Station* RxStation = antenna->getStation();
	Station* TxStation = txA->getStation();
	MESSAGE_BEGIN(NORMAL, log, m, "");
	m  << TxStation->getStationId() << TxStation->getPosition()
	   << " -> "
	   << RxStation->getStationId() << RxStation->getPosition()
	   << " pathloss: " << pl;
	MESSAGE_END();
	cacheEntry.setPathloss(pl);
	cacheEntry.setShadowing(sh);
	// antenna gain is not cached, but calculated when needed
  	cacheEntry.setAntennaGain(wns::Ratio::from_dB(0));
	cacheEntry.setValid(true);
}
