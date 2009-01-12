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
#include <RISE/transceiver/cache/vectorcache.hpp>
#include <RISE/transceiver/transmitter.hpp>
#include <RISE/transceiver/receiver.hpp>
#include <RISE/transmissionobjects/transmissionobject.hpp>
#include <RISE/antenna/Antenna.hpp>
#include <RISE/stations/station.hpp>
#include <WNS/Position.hpp>
#include <RISE/manager/systemmanager.hpp>
#include <RISE/transceiver/cache/propcacheentry.hpp>
#include <WNS/PowerRatio.hpp>

using namespace rise;
using namespace std;

VectorCache::VectorCache(receiver::ReceiverInterface* r)
	: PropagationCache(r, "Vector"),
	  pathlossShadowGain(FrequencyHash())
{
}

VectorCache::~VectorCache()
{
}

wns::Ratio VectorCache::getLoss(Transmitter* t, double freq) {
	//if frequency is not in hash attach it
	if(pathlossShadowGain.find(freq)==pathlossShadowGain.end())
		attachFrequency(freq);
	int tIndex = getIndex(t);
	PropCacheEntry& vectorEntry = pathlossShadowGain[freq].at(tIndex);
    if(!vectorEntry.isValid()){
		updatePropEntry(vectorEntry, t, freq);
    }
	return vectorEntry.getTotalLoss();
}


int VectorCache::getIndex(Transmitter* t)
{
	PointerHashMap<Transmitter*, int32_t>::iterator itr=transmitter2Index.find(t);
 	if(itr==transmitter2Index.end())
	{
 		uint32_t index = 0;
		FrequencyIterator fItrEnd = frequencies.end();
		assert(frequencies.size()>0);
		transmitters.push_back(t);
		for(FrequencyIterator fItr = frequencies.begin();
 			fItr!=fItrEnd;
			++fItr)
 		{
			index = pathlossShadowGain[*fItr].size();
			CacheVector p = std::vector<PropCacheEntry>(index+1);
			pathlossShadowGain[*fItr] = p;
		}
 		transmitter2Index[t] = index;
 		invalidatePropagationEntries(t);
		itr=transmitter2Index.find(t);
 	}
	return itr->second;

}


void VectorCache::invalidatePropagationEntries(Transmitter* t)
{
	if(frequencies.size()>0)
	{
		uint32_t index = getIndex(t);
		FrequencyIterator itrFEnd = frequencies.end();
		for(FrequencyIterator itrF = frequencies.begin();itrF!=itrFEnd;++itrF)
		{
			PropCacheEntry& vectorEntry = pathlossShadowGain[*itrF].at(index);
			updatePropEntry(vectorEntry, t, (*itrF));
		}
	}
}


void VectorCache::invalidatePropagationEntries()
{
	FrequencyIterator itrFEnd = frequencies.end();
	for(FrequencyIterator itrF = frequencies.begin();itrF!=itrFEnd;++itrF)
	{
		TransmitterIterator itrTEnd = transmitters.end();
	   for(TransmitterIterator itrT = transmitters.begin();itrT!=itrTEnd;++itrT)
	   {
		   uint32_t index = getIndex(*itrT);
		   PropCacheEntry& vectorEntry = pathlossShadowGain[*itrF].at(index);
		   updatePropEntry(vectorEntry, *itrT, (*itrF));
		}
	}
}
//! Create a new vector for a new frequency
void VectorCache::attachFrequency(double freq)
{
	uint32_t numTx = (transmitter2Index.size()+1);
	pathlossShadowGain[freq] = std::vector<PropCacheEntry>(numTx);
	TransmitterIterator itrTEnd = transmitters.end();

	for(TransmitterIterator itrT = transmitters.begin();itrT!=itrTEnd;++itrT)
	{
		uint32_t index = getIndex(*itrT);
		PropCacheEntry& vectorEntry = pathlossShadowGain[freq].at(index);
		updatePropEntry(vectorEntry, *itrT, freq);
	}
	frequencies.push_back(freq);
}


