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

#include <RISE/receiver/SignalAveragingStrategy.hpp>

using namespace rise;
using namespace rise::receiver;

NoTransmissionAveraging::NoTransmissionAveraging() :
	carrier(),
	interference(),
	calledEOT(),
	lastAveraging(-1.0)
{}

NoTransmissionAveraging::~NoTransmissionAveraging()
{
	removeAll();
}

void NoTransmissionAveraging::signalLevelsChange()
{
	if(lastAveraging < wns::simulator::getEventScheduler()->getTime()) {
		lastAveraging = wns::simulator::getEventScheduler()->getTime();
		CurrentSignalMap::iterator itrEnd;
		// update the carriers
		itrEnd = carrier.end();
		for(CurrentSignalMap::iterator itr = carrier.begin(); itr != itrEnd; ++itr) {
			// As long as transmission is active
			if(itr->first->getIsStart()) {
				itr->second = getRxPower(itr->first);
			}
		}

		// update the interference
		itrEnd = interference.end();
		for(CurrentSignalMap::iterator itr = interference.begin(); itr != itrEnd; ++itr) {
			// As long as transmission is active
			if(itr->first->getIsStart()) {
				itr->second = getInterference(itr->first);
			}
		}
	}
}

void NoTransmissionAveraging::endOfTransmission(const TransmissionObjectPtr& t)
{
	assure(carrier.find(t) != carrier.end(), "Not in map carrier!");
	assure(interference.find(t) != interference.end(), "Not in map interference!");
	assure(calledEOT.find(t) != calledEOT.end(), "Not in map calledEOT!");
	assure(calledEOT[t] = true, "Intentionally in assure statement!");
	carrier[t] = getRxPower(t);
	interference[t] = getInterference(t);
}

void NoTransmissionAveraging::add(const TransmissionObjectPtr& t)
{
	assure(carrier.find(t) == carrier.end(), "Already added to carrier!");
	assure(interference.find(t) == interference.end(), "Already added to interference!");
	assure(calledEOT.find(t) == calledEOT.end(), "Already added to calledEOT!");
	assure((calledEOT[t] = false) || true, "Intentionally in assure statement!");
	carrier[t];
	interference[t];
}

void NoTransmissionAveraging::remove(const TransmissionObjectPtr& t)
{
	assure(carrier.find(t) != carrier.end(), "Not in map carrier!");
	assure(interference.find(t) != interference.end(), "Not in map interference!");
	assure(calledEOT.find(t) != calledEOT.end(), "Not in map calledEOT!");
	assure(calledEOT.erase(t), "Intentionally in assure statement!");
	carrier.erase(t);
	interference.erase(t);
}

void NoTransmissionAveraging::removeAll()
{
	lastAveraging = -1.0;

	carrier.clear();
	interference.clear();
	calledEOT.clear();
}

wns::Power NoTransmissionAveraging::getAveragedRxPower(const TransmissionObjectPtr& t)
{
	assure(carrier.find(t) != carrier.end(), "Not in map carrier!");
	assure(interference.find(t) != interference.end(), "Not in map interference!");
	assure(calledEOT.find(t) != calledEOT.end(), "Not in map calledEOT!");
	assure( t->getIsStart() || calledEOT[t], "Transmission is not active any longer, but endOfTransmission has not been called)");

	signalLevelsChange();
	return carrier[t];
}

wns::Power NoTransmissionAveraging::getAveragedInterference(const TransmissionObjectPtr& t)
{
	assure(carrier.find(t) != carrier.end(), "Not in map carrier!");
	assure(interference.find(t) != interference.end(), "Not in map interference!");
	assure(calledEOT.find(t) != calledEOT.end(), "Not in map calledEOT!");
	assure( t->getIsStart() || calledEOT[t], "Transmission is not active any longer, but endOfTransmission has not been called)");

	signalLevelsChange();
	return interference[t];
}

wns::Ratio NoTransmissionAveraging::getAveragedCIR(const TransmissionObjectPtr& t)
{
	return getAveragedRxPower(t)/getAveragedInterference(t);
}

TimeWeightedTransmissionAveraging::TimeWeightedTransmissionAveraging() :
	carrier(),
	interference(),
	calledEOT(),
	lastAveraging(-1.0)
{
}

TimeWeightedTransmissionAveraging::~TimeWeightedTransmissionAveraging()
{
	removeAll();
}

void TimeWeightedTransmissionAveraging::signalLevelsChange()
{
	if(lastAveraging < wns::simulator::getEventScheduler()->getTime()) {
		lastAveraging = wns::simulator::getEventScheduler()->getTime();
		AveragedSignalMap::iterator itrEnd;
		// update the carriers
		itrEnd = carrier.end();
		for(AveragedSignalMap::iterator itr = carrier.begin(); itr != itrEnd; ++itr) {
			// As long as transmission is active
			if(itr->first->getIsStart()) {
				itr->second.put(getRxPower(itr->first));
			}
		}

		// update the interference
		itrEnd = interference.end();
		for(AveragedSignalMap::iterator itr = interference.begin(); itr != itrEnd; ++itr) {
			// As long as transmission is active
			if(itr->first->getIsStart()) {
				itr->second.put(getInterference(itr->first));
			}
		}
	}
}

void TimeWeightedTransmissionAveraging::endOfTransmission(const TransmissionObjectPtr& t)
{
	assure(carrier.find(t) != carrier.end(), "Not in map carrier!");
	assure(interference.find(t) != interference.end(), "Not in map interference!");
	assure(calledEOT.find(t) != calledEOT.end(), "Not in map calledEOT!");
	assure(calledEOT[t] = true, "Intentionally in assure statement!");
	carrier[t].put(getRxPower(t));
	interference[t].put(getInterference(t));
}

void TimeWeightedTransmissionAveraging::add(const TransmissionObjectPtr& t)
{
	assure(carrier.find(t) == carrier.end(), "Already added to carrier!");
	assure(interference.find(t) == interference.end(), "Already added to interference!");
	assure(calledEOT.find(t) == calledEOT.end(), "Already added to calledEOT!");
	assure((calledEOT[t] = false) || true, "Intentionally in assure statement!");
	SignalAverage& c = carrier[t];
	SignalAverage& i = interference[t];
	c.setClock(wns::simulator::getEventScheduler());
	i.setClock(wns::simulator::getEventScheduler());
	c.reset();
	i.reset();
}

void TimeWeightedTransmissionAveraging::remove(const TransmissionObjectPtr& t)
{
	assure(carrier.find(t) != carrier.end(), "Not in map carrier!");
	assure(interference.find(t) != interference.end(), "Not in map interference!");
	assure(calledEOT.find(t) != calledEOT.end(), "Not in map calledEOT!");
	assure(calledEOT.erase(t), "Intentionally in assure statement!");
	carrier.erase(t);
	interference.erase(t);
}

void TimeWeightedTransmissionAveraging::removeAll()
{
	lastAveraging = -1.0;

	carrier.clear();
	interference.clear();
	calledEOT.clear();
}

wns::Power TimeWeightedTransmissionAveraging::getAveragedRxPower(const TransmissionObjectPtr& t)
{
	assure(carrier.find(t) != carrier.end(), "Not in map carrier!");
	assure(interference.find(t) != interference.end(), "Not in map interference!");
	assure(calledEOT.find(t) != calledEOT.end(), "Not in map calledEOT!");
	assure( t->getIsStart() || calledEOT[t], "Transmission is not active any longer, but endOfTransmission has not been called)");

	// Call to do last averaging
	signalLevelsChange();

	assure(carrier[t].get() != wns::Power(), "Signal is 0 mW. This probably means you've just added this transmission for averaging and no time is elapsed so far. Mathematically this is correct. If you need the signal level call getRxPower(t)");

	return carrier[t].get();
}

wns::Power TimeWeightedTransmissionAveraging::getAveragedInterference(const TransmissionObjectPtr& t)
{
	assure(carrier.find(t) != carrier.end(), "Not in map carrier!");
	assure(interference.find(t) != interference.end(), "Not in map interference!");
	assure(calledEOT.find(t) != calledEOT.end(), "Not in map calledEOT!");
	assure( t->getIsStart() || calledEOT[t], "Transmission is not active any longer, but endOfTransmission has not been called)");

	// Call to do last averaging
	signalLevelsChange();

	assure(interference[t].get() != wns::Power(), "Interference is 0 dBm. This probably means you've just added this transmission for averaging and no time is elapsed so far. Mathematically this is correct. If you need the interference level call getInterference(t)");

	return interference[t].get();
}

wns::Ratio TimeWeightedTransmissionAveraging::getAveragedCIR(const TransmissionObjectPtr& t)
{
	return getAveragedRxPower(t)/getAveragedInterference(t);
}



