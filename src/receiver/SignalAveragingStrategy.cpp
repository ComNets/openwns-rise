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
			if(itr->second.first->getIsStart()) {
				itr->second.second = getRxPower(itr->second.first);
			}
		}

		// update the interference
		itrEnd = interference.end();
		for(CurrentSignalMap::iterator itr = interference.begin(); itr != itrEnd; ++itr) {
			// As long as transmission is active
			if(itr->second.first->getIsStart()) {
				itr->second.second = getInterference(itr->second.first);
			}
		}
	}
}

void NoTransmissionAveraging::endOfTransmission(const TransmissionObjectPtr& t)
{
	assure(carrier.find(t->getBirthmark()) != carrier.end(), "Not in map carrier!");
	assure(interference.find(t->getBirthmark()) != interference.end(), "Not in map interference!");
	assure(calledEOT.find(t->getBirthmark()) != calledEOT.end(), "Not in map calledEOT!");
	assure(calledEOT[t->getBirthmark()].second = true, "Intentionally in assure statement!");
	carrier[t->getBirthmark()].second = getRxPower(t);
	interference[t->getBirthmark()].second = getInterference(t);
}

void NoTransmissionAveraging::add(const TransmissionObjectPtr& t)
{
	assure(carrier.find(t->getBirthmark()) == carrier.end(), "Already added to carrier!");
	assure(interference.find(t->getBirthmark()) == interference.end(), "Already added to interference!");
	assure(calledEOT.find(t->getBirthmark()) == calledEOT.end(), "Already added to calledEOT!");
	assure((calledEOT[t->getBirthmark()].second = false) || true, "Intentionally in assure statement!");
	carrier[t->getBirthmark()].first = t;
	interference[t->getBirthmark()].first = t;
}

void NoTransmissionAveraging::remove(const TransmissionObjectPtr& t)
{
	assure(carrier.find(t->getBirthmark()) != carrier.end(), "Not in map carrier!");
	assure(interference.find(t->getBirthmark()) != interference.end(), "Not in map interference!");
	assure(calledEOT.find(t->getBirthmark()) != calledEOT.end(), "Not in map calledEOT!");
	assure(calledEOT.erase(t->getBirthmark()), "Intentionally in assure statement!");
	carrier.erase(t->getBirthmark());
	interference.erase(t->getBirthmark());
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
	assure(carrier.find(t->getBirthmark()) != carrier.end(), "Not in map carrier!");
	assure(interference.find(t->getBirthmark()) != interference.end(), "Not in map interference!");
	assure(calledEOT.find(t->getBirthmark()) != calledEOT.end(), "Not in map calledEOT!");
	assure( t->getIsStart() || calledEOT[t->getBirthmark()].second, "Transmission is not active any longer, but endOfTransmission has not been called)");

	signalLevelsChange();
	return carrier[t->getBirthmark()].second;
}

wns::Power NoTransmissionAveraging::getAveragedInterference(const TransmissionObjectPtr& t)
{
	assure(carrier.find(t->getBirthmark()) != carrier.end(), "Not in map carrier!");
	assure(interference.find(t->getBirthmark()) != interference.end(), "Not in map interference!");
	assure(calledEOT.find(t->getBirthmark()) != calledEOT.end(), "Not in map calledEOT!");
	assure( t->getIsStart() || calledEOT[t->getBirthmark()].second, "Transmission is not active any longer, but endOfTransmission has not been called)");

	signalLevelsChange();
	return interference[t->getBirthmark()].second;
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
			if(itr->second.first->getIsStart()) {
				itr->second.second.put(getRxPower(itr->second.first));
			}
		}

		// update the interference
		itrEnd = interference.end();
		for(AveragedSignalMap::iterator itr = interference.begin(); itr != itrEnd; ++itr) {
			// As long as transmission is active
			if(itr->second.first->getIsStart()) {
				itr->second.second.put(getInterference(itr->second.first));
			}
		}
	}
}

void TimeWeightedTransmissionAveraging::endOfTransmission(const TransmissionObjectPtr& t)
{
	assure(carrier.find(t->getBirthmark()) != carrier.end(), "Not in map carrier!");
	assure(interference.find(t->getBirthmark()) != interference.end(), "Not in map interference!");
	assure(calledEOT.find(t->getBirthmark()) != calledEOT.end(), "Not in map calledEOT!");
	assure(calledEOT[t->getBirthmark()].second = true, "Intentionally in assure statement!");
	carrier[t->getBirthmark()].second.put(getRxPower(t));
	interference[t->getBirthmark()].second.put(getInterference(t));
}

void TimeWeightedTransmissionAveraging::add(const TransmissionObjectPtr& t)
{
	assure(carrier.find(t->getBirthmark()) == carrier.end(), "Already added to carrier!");
	assure(interference.find(t->getBirthmark()) == interference.end(), "Already added to interference!");
	assure(calledEOT.find(t->getBirthmark()) == calledEOT.end(), "Already added to calledEOT!");
	assure((calledEOT[t->getBirthmark()].second = false) || true, "Intentionally in assure statement!");
    carrier[t->getBirthmark()].first = t;
    interference[t->getBirthmark()].first = t;
	SignalAverage& c = carrier[t->getBirthmark()].second;
	SignalAverage& i = interference[t->getBirthmark()].second;
	c.setClock(wns::simulator::getEventScheduler());
	i.setClock(wns::simulator::getEventScheduler());
	c.reset();
	i.reset();
}

void TimeWeightedTransmissionAveraging::remove(const TransmissionObjectPtr& t)
{
	assure(carrier.find(t->getBirthmark()) != carrier.end(), "Not in map carrier!");
	assure(interference.find(t->getBirthmark()) != interference.end(), "Not in map interference!");
	assure(calledEOT.find(t->getBirthmark()) != calledEOT.end(), "Not in map calledEOT!");
	assure(calledEOT.erase(t->getBirthmark()), "Intentionally in assure statement!");
	carrier.erase(t->getBirthmark());
	interference.erase(t->getBirthmark());
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
	assure(carrier.find(t->getBirthmark()) != carrier.end(), "Not in map carrier!");
	assure(interference.find(t->getBirthmark()) != interference.end(), "Not in map interference!");
	assure(calledEOT.find(t->getBirthmark()) != calledEOT.end(), "Not in map calledEOT!");
	assure( t->getIsStart() || calledEOT[t->getBirthmark()].second, 
        "Transmission is not active any longer, but endOfTransmission has not been called)");

	// Call to do last averaging
	signalLevelsChange();

	assure(carrier[t->getBirthmark()].second.get() != wns::Power(), "Signal is 0 mW. This probably means you've just added this transmission for averaging and no time is elapsed so far. Mathematically this is correct. If you need the signal level call getRxPower(t)");

	return carrier[t->getBirthmark()].second.get();
}

wns::Power TimeWeightedTransmissionAveraging::getAveragedInterference(const TransmissionObjectPtr& t)
{
	assure(carrier.find(t->getBirthmark()) != carrier.end(), "Not in map carrier!");
	assure(interference.find(t->getBirthmark()) != interference.end(), "Not in map interference!");
	assure(calledEOT.find(t->getBirthmark()) != calledEOT.end(), "Not in map calledEOT!");
	assure( t->getIsStart() || calledEOT[t->getBirthmark()].second, 
        "Transmission is not active any longer, but endOfTransmission has not been called)");

	// Call to do last averaging
	signalLevelsChange();

	assure(interference[t->getBirthmark()].second.get() != wns::Power(), "Interference is 0 dBm. This probably means you've just added this transmission for averaging and no time is elapsed so far. Mathematically this is correct. If you need the interference level call getInterference(t)");

	return interference[t->getBirthmark()].second.get();
}

wns::Ratio TimeWeightedTransmissionAveraging::getAveragedCIR(const TransmissionObjectPtr& t)
{
	return getAveragedRxPower(t)/getAveragedInterference(t);
}



