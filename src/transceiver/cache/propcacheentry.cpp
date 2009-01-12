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

using namespace rise;
using namespace wns;

PropCacheEntry::PropCacheEntry()
	:totalLoss(wns::Ratio()),
	 pathloss(wns::Ratio()),
	 shadowing(wns::Ratio()),
	 antennaGain(wns::Ratio()),
	 valid(false)
{
}

PropCacheEntry::~PropCacheEntry()
{
}

wns::Ratio PropCacheEntry::getTotalLoss() const
{
	return totalLoss;
}

wns::Ratio PropCacheEntry::getPathloss() const
{
	return pathloss;
}

wns::Ratio PropCacheEntry::getShadowing() const
{
	return shadowing;
}

wns::Ratio PropCacheEntry::getAntennaGain() const
{
	return antennaGain;
}

bool PropCacheEntry::isValid() const
{
	return valid;
}


void PropCacheEntry::setPathloss(wns::Ratio loss)
{
	pathloss = loss;
	recomputeTotalLoss();
}

void PropCacheEntry::setShadowing(wns::Ratio shadow)
{
	shadowing = shadow;
	recomputeTotalLoss();
}

void PropCacheEntry::setAntennaGain(wns::Ratio gain)
{
	antennaGain = gain;
	recomputeTotalLoss();
}

void PropCacheEntry::setValid(bool v)
{
	valid = v;
}


