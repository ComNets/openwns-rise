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

#include <RISE/transceiver/SCReceiver.hpp>
#include <RISE/transceiver/transmitter.hpp>
#include <RISE/antenna/Antenna.hpp>
#include <RISE/manager/systemmanager.hpp>
#include <RISE/medium/PhysicalResource.hpp>
#include <RISE/transmissionobjects/transmissionobject.hpp>
#include <RISE/stations/station.hpp>
#include <RISE/medium/Medium.hpp>
#include <RISE/transceiver/cache/hashcache.hpp>
#include <RISE/transceiver/cache/idvectorcache.hpp>
#include <RISE/transceiver/cache/vectorcache.hpp>
#include <RISE/transceiver/cache/nocache.hpp>
#include <RISE/transceiver/cache/h2cache.hpp>
#include <RISE/RISE.hpp>

#include <WNS/PowerRatio.hpp>
#include <WNS/TimeWeightedAverage.hpp>
#include <WNS/Assure.hpp>
#include <WNS/pyconfig/View.hpp>

#include <sstream>


using namespace std;
using namespace rise;

SCReceiver::SCReceiver(const wns::pyconfig::View& config,
					   Station* s,
					   antenna::Antenna* a,
					   Demodulator *demodulator,
					   Decoder *decoder,
					   wns::Ratio rnf)
	: Receiver(s, a, demodulator, decoder, rnf),
	  LossCalculation(config)
{}

SCReceiver::~SCReceiver()
{}


void SCReceiver::startReceiving()
{
	assert(!active);
	assert(!prc.empty());
	prc.at(0)->attach(this);
	active = true;
}

void SCReceiver::stopReceiving()
{
	assert(active);
	assert(!prc.empty());
	prc.at(0)->detach(this);
	active = false;
	// remove all transmission objects and their according averaging objects
	transmissionObjects.clear();
	removeAll();
}

void SCReceiver::notify(TransmissionObjectPtr aTransmissionObject)
{
	totalRxPower.setDirty();
	signalLevelsChange();
	if (aTransmissionObject->getIsStart())
	{
		transmissionObjects.push_back(aTransmissionObject);
		add(aTransmissionObject);
	}
	else
	{
		transmissionObjects.remove(aTransmissionObject);
		remove(aTransmissionObject);
	}
}

wns::Power SCReceiver::getTotalRxPower() {
	assert(active);
	//	if(totalRxPower.isDirty()) {
	totalRxPower = getNoise();
	medium::PhysicalResource::TransmissionObjectIterator itr, itrEnd;
	itrEnd=prc.at(0)->getTOEnd();
	for(itr=prc.at(0)->getTOBegin();
		itr!=itrEnd;
		++itr) {
		totalRxPower += getRxPower(*itr);
	}
	//	}
	return totalRxPower;
}

wns::Power SCReceiver::getInterference(const TransmissionObjectPtr& t) {
  	wns::Power rxPower = getRxPower(t);
  	assert(prc.at(0)->contains(t));
 	wns::Power Interference = getTotalRxPower() - rxPower;
 	assert(Interference>=getNoise()*.99);
	return Interference;
}

wns::Power SCReceiver::getRxPower(const TransmissionObjectPtr& t) {
	wns::Power rxPower = t->getTxPower();
	double freq = prc.at(0)->getFrequency();
	rxPower -= propCache->getLoss(t->getTransmitter(),  freq);
	return rxPower;
}

wns::Ratio SCReceiver::getLoss(Transmitter* t)
{
	double freq = prc.at(0)->getFrequency();
	return propCache->getLoss(t, freq);
}


wns::Power SCReceiver::getNoise()
{
	wns::Power bg = wns::Power::from_dBm(-174);
	bg += receiverNoiseFigure;
	bg += wns::Ratio::from_factor(prc.at(0)->getBandwidth()*1E6);
	return bg;
}


void SCReceiver::tune(double f, double b) {
	if(active) {
		stringstream errMsg;
		errMsg << "Tuning not possible. Transceiver is active!";
		throw(errMsg.str().c_str());
	} else {
		assure(prc.size() <= 1, "Single carrier receiver may have one resource at maximum");
		if(!prc.empty()) {
			prc.clear();
		}
		prc.push_back(medium::Medium::getInstance()->getPhysicalResource(f, b));
	}
}

bool SCReceiver::contains(TransmissionObjectPtr t)
{
	if (find(transmissionObjects.begin(), transmissionObjects.end(), t)
		!= transmissionObjects.end())
	{
		return true;
	}
	return false;
}

double SCReceiver::getFrequency()
{
	assure(!prc.empty(), "SCReceiver not tuned.");
	return prc.at(0)->getFrequency();
}



