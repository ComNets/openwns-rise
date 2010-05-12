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

#include <RISE/transceiver/cache/h2cache.hpp>
#include <RISE/transceiver/cache/propcacheentry.hpp>
#include <RISE/transceiver/transmitter.hpp>
#include <RISE/transceiver/receiver.hpp>
#include <RISE/transmissionobjects/transmissionobject.hpp>
#include <RISE/antenna/Antenna.hpp>
#include <RISE/stations/station.hpp>
#include <RISE/manager/systemmanager.hpp>
#include <WNS/PowerRatio.hpp>

using namespace std;
using namespace __gnu_cxx;
using namespace rise;

H2Cache::H2Cache(receiver::ReceiverInterface* r)
    : PropagationCache(r, "H2"),
      pathlossShadowGain(CacheVector()),
	  transmitters(TransmitterContainer())
{
	pathlossShadowGain = std::vector<PropCacheEntry>(0);
}

H2Cache::~H2Cache()
{
}

wns::Ratio H2Cache::getLoss(Transmitter* t, double freq)
{
 	unsigned long int tId = t->getTransmitterId();
 	if(!transmitters.contains(t))
 		attach(t, tId);
 	PropCacheEntry& cacheEntry = pathlossShadowGain.at(tId);
 	if(!cacheEntry.isValid()) {
 		updatePropEntry(cacheEntry,
 						t, 
  						freq);
  	}
	return cacheEntry.getTotalLoss();
}


void H2Cache::attach(Transmitter* t, unsigned long int tId) {
	
 	transmitters.push_back(t);
	if(pathlossShadowGain.size() <= tId)
 	{
  		unsigned long int numTx = tId+1;
 		pathlossShadowGain = std::vector<PropCacheEntry>(numTx);
 	}
}

void H2Cache::invalidatePropagationEntries(Transmitter* t)
{
 	unsigned long int tId = t->getTransmitterId();
	if(tId >= pathlossShadowGain.size())
		attach(t, tId);
	pathlossShadowGain.at(tId).setValid(false);
}

void H2Cache::invalidatePropagationEntries()
{
	TransmitterIterator itrTEnd = transmitters.end();
    for(TransmitterIterator itrT = transmitters.begin();
		itrT!=itrTEnd;
		++itrT)
    {
	    unsigned long int tId = (*itrT)->getTransmitterId();
		pathlossShadowGain.at(tId).setValid(false);
    }
}


