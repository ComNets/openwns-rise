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

#include <RISE/transceiver/cache/propcacheentry.hpp>
#include <RISE/transceiver/cache/idvectorcache.hpp>
#include <RISE/transceiver/transmitter.hpp>
#include <RISE/transceiver/receiver.hpp>
#include <RISE/transmissionobjects/transmissionobject.hpp>
#include <RISE/antenna/Antenna.hpp>
#include <RISE/stations/station.hpp>
#include <WNS/Position.hpp>
#include <RISE/manager/systemmanager.hpp>
#include <WNS/PowerRatio.hpp>

using namespace std;
using namespace __gnu_cxx;
using namespace rise;

IdVectorCache::IdVectorCache(receiver::ReceiverInterface* r)
    : PropagationCache(r, "IdVector"),
      pathlossShadowGain(CacheVector())
{}

IdVectorCache::~IdVectorCache()
{}

wns::Ratio IdVectorCache::getLoss(Transmitter* t, double frequency)
{
	unsigned long int tId  = t->getTransmitterId();
	if(!transmitters.contains(t))
		attach(t, tId);
	PropCacheEntry& vectorEntry = pathlossShadowGain.at(tId);
	if(!vectorEntry.isValid()){
		updatePropEntry(vectorEntry, t, frequency);
	}
	return vectorEntry.getTotalLoss();
}

void IdVectorCache::attach(Transmitter *t, unsigned long int tId)
{
	transmitters.push_front(t);
 	if(pathlossShadowGain.size() <= tId)
 	{
 		unsigned long int size = tId+1;
 		pathlossShadowGain = vector<PropCacheEntry>(size);
 	}

}

void IdVectorCache::invalidatePropagationEntries( Transmitter* t)
{
	unsigned long int tId = t->getTransmitterId();
	if(pathlossShadowGain.size() <= tId)
		attach(t, tId);
	PropCacheEntry& vectorEntry = pathlossShadowGain.at(tId);
	vectorEntry.setValid(false);
}

void IdVectorCache::invalidatePropagationEntries()
{
    TransmitterIterator itrTEnd = transmitters.end();
    for(TransmitterIterator itrT = transmitters.begin();
		itrT!=itrTEnd;
		++itrT)
    {
	    unsigned long int tId = (*itrT)->getTransmitterId();
		PropCacheEntry& vectorEntry = pathlossShadowGain.at(tId);
		vectorEntry.setValid(false);
    }
}


