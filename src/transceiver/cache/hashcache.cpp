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

#include <RISE/transceiver/cache/hashcache.hpp>
#include <RISE/transceiver/cache/propcacheentry.hpp>
#include <RISE/transceiver/transmitter.hpp>
#include <RISE/transceiver/receiver.hpp>
#include <RISE/transmissionobjects/transmissionobject.hpp>
#include <RISE/antenna/Antenna.hpp>
#include <RISE/stations/station.hpp>
#include <RISE/manager/systemmanager.hpp>

using namespace std;
using namespace rise;

HashCache::HashCache(ReceiverBase* r)
	:PropagationCache(r, "Hash"),
	 propHash(Level1Hash())
{}

HashCache::~HashCache()
{
}

wns::Ratio HashCache::getLoss(Transmitter* t, double f) 
{
	PropCacheEntry& hashEntry = propHash[t][f];
    if(!hashEntry.isValid()){
		updatePropEntry(hashEntry, t, f);
	}
	return hashEntry.getTotalLoss();
}

void HashCache::invalidatePropagationEntries( Transmitter* t)
{

	FrequencyIterator itrFEnd = propHash[t].end();
	for(FrequencyIterator itrF=propHash[t].begin(); itrF!=itrFEnd; ++itrF){
		itrF->second.setValid(false);
	}
}

void HashCache::invalidatePropagationEntries(){

	TransmitterIterator itrTEnd = propHash.end();
	for(TransmitterIterator itrT = propHash.begin(); itrT!=itrTEnd; ++itrT){
		FrequencyIterator itrFEnd = itrT->second.end();
		
		for(FrequencyIterator itrF = itrT->second.begin();itrF!=itrFEnd; ++itrF)
		{
			itrF->second.setValid(false);
		}
	}
}


