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

#include <RISE/scenario/pathloss/RangeChecked.hpp>
#include <RISE/scenario/pathloss/defs.hpp>
#include <RISE/antenna/Antenna.hpp>

#include <WNS/Types.hpp>
#include <WNS/Assure.hpp>

using namespace rise::scenario::pathloss;

RangeChecked::RangeChecked(const wns::pyconfig::View& config)
    : DistanceDependent(config)
{}

void RangeChecked::addRange(const DistanceRange& distances, const Pathloss* model)
{
    distanceMap.insert(distances, model);
}

void RangeChecked::addRange(const FrequencyRange& frequencies, const bool& allowed)
{
    frequencyMap.insert(frequencies, allowed);
}

bool RangeChecked::knowsDistance(const wns::Distance& distance) const
{
    return distanceMap.has(distance);
}

bool RangeChecked::knowsFrequency(const wns::Frequency& frequency) const
{
    return frequencyMap.has(frequency);
}

const Pathloss* RangeChecked::getModel(const wns::Distance& distance) const
{
	return distanceMap.get(distance);
}

wns::Ratio RangeChecked::calculatePathloss(const antenna::Antenna& source,
					   const antenna::Antenna& target,
					   const wns::Frequency& frequency) const
{
    const wns::Distance distance = calculateDistance(source, target);
    assure(frequencyMap.get(frequency), "Frequency out of range in RangeChecked::calculatePathloss().");
    return distanceMap.get(distance)->calculatePathloss(source, target, frequency, distance);
}

/* Idea:
wns::Ratio RangeChecked::calculatePathloss(const antenna::Antenna& source,
					   const antenna::Antenna& target,
					   const wns::Frequency& frequency,
					   const wns::PositionOffset shift) const
					   //const wns::PositionOffset shift = wns::PositionOffset(0,0,0)) const
{
    const wns::Distance distance = calculateDistance(source, target);
    assure(frequencyMap.get(frequency), "Frequency out of range in RangeChecked::calculatePathloss().");
    return distanceMap.get(distance)->calculatePathloss(source, target, frequency, distance);
}
*/
