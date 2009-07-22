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

#include <sstream>
#include <cassert>

#include <RISE/transceiver/receiver.hpp>
#include <RISE/transceiver/transmitter.hpp>
#include <RISE/antenna/Antenna.hpp>
#include <RISE/manager/systemmanager.hpp>
#include <RISE/scenario/Scenario.hpp>
#include <RISE/medium/PhysicalResource.hpp>
#include <WNS/PowerRatio.hpp>
#include <RISE/transmissionobjects/transmissionobject.hpp>
#include <RISE/stations/station.hpp>
#include <RISE/medium/Medium.hpp>
#include <WNS/TimeWeightedAverage.hpp>
#include <RISE/transceiver/cache/hashcache.hpp>
#include <RISE/transceiver/cache/idvectorcache.hpp>
#include <RISE/transceiver/cache/vectorcache.hpp>
#include <RISE/transceiver/cache/nocache.hpp>
#include <RISE/transceiver/cache/h2cache.hpp>
#include <RISE/RISE.hpp>

using namespace std;
using namespace rise;

ReceiverBase::ReceiverBase(Station* s,
						   Demodulator *demodulator,
						   Decoder *decoder,
						   wns::Ratio rnf) :
	receiverId(nextid++),
	demodulator(demodulator),
	decoder(decoder),
	totalRxPower(),
	propCache(0),
	receiverNoiseFigure(rnf),
	active(false),
	midFrequency(0),
	log("ReceiverBase")
	//log(config.getView("logger")) // no wns::pyconfig::View here !
{
	assure(s, "No valid station provided");
	totalRxPower.setDirty();
	configureLogger(); // name="ReceiverBase(receiverId)"
	this->startObserving(s);
	initiatePropCache();
}

ReceiverBase::~ReceiverBase()
{
	MESSAGE_BEGIN(NORMAL, log, m, "Shutting down ...");
	MESSAGE_END();
	delete propCache;
}

void ReceiverBase::initiatePropCache()
{
	MESSAGE_BEGIN(NORMAL, log, m, "Created ...");
	MESSAGE_END();
	wns::pyconfig::View pyConfigView = RISE::getPyConfigView();
	std::string cache = pyConfigView.get<std::string>("propagationCache.cache");
	if(cache==std::string("hash"))
		propCache = new HashCache(this);
	else if(cache==std::string("H2"))
		propCache = new H2Cache(this);
	else if(cache==std::string("idVector"))
		propCache = new IdVectorCache(this);
	else if(cache==std::string("vector"))
		propCache = new VectorCache(this);
	else if(cache==std::string("no"))
		propCache = new NoCache(this);
	else {
		throw(wns::Exception(std::string("No such cache strategy available: ")+cache));
	}
}


void ReceiverBase::configureLogger() {
	stringstream location;
	location << "ReceiverBase(" << receiverId << ")";
	log = RISELogger(location.str());

	if(RISE::getPyConfigView().get<bool>("debug.receiver"))
	{
		log.switchOn();
		pd_debugFlag = true;
		MESSAGE_SINGLE(NORMAL, log, "configureLogger(): switchOn. receiverId="<<receiverId);
	} else {
		log.switchOff();
		pd_debugFlag = false;
	};
}

void ReceiverBase::startReceiving()
{
	MESSAGE_BEGIN(NORMAL, log, m, "Started receiving ...");
	MESSAGE_END();
	//std::cout << "rise::ReceiverBase::startReceiving" << std::endl;
	assure(!active, "Receiver already active!");
	int32_t nSB = prc.size();
	for(int32_t i=0; i < nSB; ++i) {
		assert(prc.at(i));
		prc.at(i)->attach(this);
	}
	active = true;
}

void ReceiverBase::stopReceiving()
{
	MESSAGE_BEGIN(NORMAL, log, m, "Stop receiving.");
	MESSAGE_END();
	//std::cout << "rise::ReceiverBase::stopReceiving" << std::endl;
	assure(active, "Receiver is not active!");
	int32_t nSC = prc.size();
	for(int32_t i = 0; i < nSC; ++i) {
		assert(prc.at(i));
		prc.at(i)->detach(this);
	}
	active = false;
	// remove all transmission objects and their according averaging objects
	transmissionObjects.clear();
	removeAll();
}

wns::Power ReceiverBase::getTotalRxPower()
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
			totalRxPower += getRxPower(*TOI);
		}
		++itr;
	}
	return totalRxPower;
}

wns::Power ReceiverBase::getInterference(const TransmissionObjectPtr& t)
{
	wns::Power rxPower = getRxPower(t);
	assert(this->contains(t));
	wns::Power Interference = getTotalRxPower() - rxPower;
	assert(Interference>=getNoise()*.99); // should at least contain Noise
	return Interference; // I+N
}

wns::Power ReceiverBase::getRxPower(const TransmissionObjectPtr& t)
{
	wns::Power rxPower = t->getTxPower();
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


wns::Power ReceiverBase::getNoise()
{
	double receiver_band = 0;
	PhysicalResourceIterator itr,itrEND;
	itrEND = prc.end();
	for(itr = prc.begin(); itr != itrEND; ++itr) {
		receiver_band += (*itr)->getBandwidth();
	}
	wns::Power bg = wns::Power::from_dBm(-174);
	bg += receiverNoiseFigure;
	bg += wns::Ratio::from_factor(receiver_band*1E6);
	return bg;
}

void ReceiverBase::writeCacheEntry(PropCacheEntry& cacheEntry,
								   Transmitter* t,
								   double freq)
{
	wns::Ratio pl = getPathloss(*t, freq);
	wns::Ratio sh = getShadowing(*t);

	antenna::Antenna* txA = t->getAntenna();
	Station* RxStation = getAntenna()->getStation();
	Station* TxStation = txA->getStation();

	MESSAGE_BEGIN(NORMAL, log, m, "");
	m  << TxStation->getStationId() << TxStation->getPosition()
	   << " -> "
	   << RxStation->getStationId() << RxStation->getPosition()
	   << " pathloss: " << pl;
	MESSAGE_END();
	cacheEntry.setPathloss(pl);
	cacheEntry.setShadowing(sh);
	// receive antenna gain  at transmitter's position
	// which beam does receiver use to listen to the transmitter
	wns::Ratio r1 = getAntenna()->getGain(txA->getPosition(),
									 rise::antenna::PatternPtr());
	// transmit antenna gain at receiver's position
	// which beam does transmitter use to send to this receiver
	wns::Ratio r2 = txA->getGain(getAntenna()->getPosition(),
								 rise::antenna::PatternPtr());
	cacheEntry.setAntennaGain(r1 + r2);
	cacheEntry.setValid(true);
}

void ReceiverBase::tune(double f,double b,int32_t numberOfSubCarriers)
{
	assure(!active, "Tuning not possible.Transceiver is active!");
	assure(f>0, "No negative frequencies allowed.");
	assure(b>0, "No negative bandwidth allowed.");
	assure(numberOfSubCarriers>0, "No negative number of sub-carriers allowed.");

	MESSAGE_SINGLE(NORMAL, log, "rise::ReceiverBase::tune(f="<<f<<",b="<<b<<",#SC="<<numberOfSubCarriers<<")");

	midFrequency = f;
	double subCarrierBandwidth = b/numberOfSubCarriers;
	double lowerFrequency = f - b/2;
	for(int32_t i = 0; i < numberOfSubCarriers; ++i) {
		double subCarrierCenterFrequency = lowerFrequency + (2*i + 1) * subCarrierBandwidth/2;
		prc.push_back(medium::Medium::getInstance()->getPhysicalResource(subCarrierCenterFrequency, subCarrierBandwidth));
		// TODO [rs]: way to give the subChannel number into the PhysicalResource
	}
}

void ReceiverBase::positionChanged()
{
	propCache->invalidatePropagationEntries();
}

void ReceiverBase::positionWillChange()
{
	/* do nothing */
}

void ReceiverBase::mobilityUpdate(Transmitter* transmitter)
{
	propCache->invalidatePropagationEntries(transmitter);
	totalRxPower.setDirty();
	signalLevelsChange();
}

bool ReceiverBase::contains(TransmissionObjectPtr t)
{
	return find(transmissionObjects.begin(), transmissionObjects.end(), t) != transmissionObjects.end();
}

uint32_t ReceiverBase::nextid = 0;

Receiver::Receiver(Station* s,
				   antenna::Antenna* a,
				   Demodulator *demodulator,
				   Decoder *decoder,
				   wns::Ratio rnf)
	:ReceiverBase(s, demodulator, decoder, rnf),
	 antenna(a)
{}
Receiver::~Receiver()
{
	antenna = 0;
}
//! @todo Die mittenfrequenzen der sub-b�nder aufzuaddieren ist nicht so eine tolle idee!
wns::Ratio Receiver::getLoss(Transmitter* t)
{
	double freq = 0;
	int32_t nSB = prc.size();
	for(int32_t i = 0; i < nSB; ++i)
		freq += prc[i]->getFrequency();
	return propCache->getLoss(t, freq);
}




