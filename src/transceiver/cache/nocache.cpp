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

#include <RISE/transceiver/cache/nocache.hpp>
#include <RISE/transceiver/cache/propcacheentry.hpp>
#include <RISE/antenna/Antenna.hpp>
#include <RISE/stations/station.hpp>
#include <RISE/manager/systemmanager.hpp>
#include <RISE/transceiver/receiver.hpp>

using namespace std;
using namespace rise;

NoCache::NoCache(receiver::ReceiverInterface* r)
    : PropagationCache(r, "No"),
	propObject(PropCacheEntry())
{}

NoCache::~NoCache()
{}

wns::Ratio NoCache::getLoss(Transmitter* t, double freq)
{
 	updatePropEntry(propObject, t, freq);
 	return propObject.getTotalLoss();
}

void NoCache::invalidatePropagationEntries(Transmitter*)
{
	MESSAGE_BEGIN(NORMAL, log, m, "");
	m << "No update of PropEntries (Transmitter).";
	MESSAGE_END();
}

void NoCache::invalidatePropagationEntries()
{
	MESSAGE_BEGIN(NORMAL, log, m, "");
	m << "No update of PropEntries.";
	MESSAGE_END();
}


