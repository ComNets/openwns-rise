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

#include <RISE/scenario/pathloss/FreeSpace.hpp>

#include <WNS/PyConfigViewCreator.hpp>
#include <WNS/StaticFactoryBroker.hpp>
#include <WNS/pyconfig/View.hpp>
#include <WNS/PowerRatio.hpp>
#include <WNS/Types.hpp>

#include <limits>
#include <algorithm>

using namespace rise::scenario::pathloss;

STATIC_FACTORY_BROKER_REGISTER(FreeSpace, Pathloss, "FreeSpace");

FreeSpace::FreeSpace(const wns::pyconfig::View& config)
    : Super(config, wns::Ratio::from_dB(32.44), 20, 20, 1.0 / 1000.0)
{
    setupRanges();
}

void FreeSpace::setupRanges()
{
    const wns::Frequency infFreq = std::numeric_limits<wns::Frequency>::infinity();
    addRange(FrequencyRange::From(-infFreq).To(0), false);
    addRange(FrequencyRange::Between(0).And(infFreq), true);
    addRange(FrequencyRange::From(infFreq).To(infFreq), false);

    const wns::Distance infDist = std::numeric_limits<wns::Distance>::infinity();
    addRange(DistanceRange::FromIncluding(0).ToExcluding(infDist), this);
}
