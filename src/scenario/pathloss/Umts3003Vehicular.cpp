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

#include <RISE/scenario/pathloss/Umts3003Vehicular.hpp>

#include <WNS/StaticFactoryBroker.hpp>
#include <WNS/pyconfig/View.hpp>
#include <WNS/PowerRatio.hpp>

using namespace rise::scenario::pathloss;

STATIC_FACTORY_BROKER_REGISTER(Umts3003Vehicular, Pathloss, "Umts3003Vehicular");

Umts3003Vehicular::Umts3003Vehicular(const wns::pyconfig::View& config)
    : Super(config, wns::Ratio::from_dB(80), 21, 40, 1.0/1000.0, 18, 0.16),
      deny()
{
    const wns::Frequency infFreq = std::numeric_limits<wns::Frequency>::infinity();
    const wns::Distance infDist = std::numeric_limits<wns::Distance>::infinity();
    addRange(FrequencyRange::From(150).To(2000));
    addRange(FrequencyRange::FromIncluding(0).ToExcluding(150), false);
    addRange(FrequencyRange::Between(2000).And(infFreq), false);
    addRange(DistanceRange::From(0).To(20000), this);
    addRange(DistanceRange::Between(20000).And(infDist), &deny);
}

