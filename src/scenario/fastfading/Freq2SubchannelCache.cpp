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

#include <RISE/scenario/fastfading/Freq2SubchannelCache.hpp>
#include <RISE/medium/Medium.hpp>

using namespace rise::scenario::fastfading;

Freq2SubchannelCache::Freq2SubchannelCache(unsigned int numberOfSubchannels) : 
    logger_(wns::logger::Logger("RISE", "FastFading.Freq2SubchannelCache")),
    numberOfSubchannels_(numberOfSubchannels)
{
}

Freq2SubchannelCache::~Freq2SubchannelCache()
{
}

unsigned int
Freq2SubchannelCache::getSubchannelIndex(const wns::Frequency& frequency) const
{
    unsigned int sc;
    try
    {
        sc = frequencyToSubchannel_.find(frequency);
    }
    catch(wns::container::Registry<double, unsigned int>::UnknownKeyValue&)
    {
        sc = rise::medium::Medium::getInstance()->getPhysicalResourceIndex(frequency);

        /* For FDD we can have 2 x numSubchannels */
        assure(sc < 2 * numberOfSubchannels_, "Subchannel out of range");
        sc %= numberOfSubchannels_;

        frequencyToSubchannel_.insert(frequency, sc);

        MESSAGE_SINGLE(NORMAL, logger_, "Frequency " << frequency
            << " mapped to subchannel index " << sc);
    }
    return sc;
}



