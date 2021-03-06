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

#include <RISE/scenario/fastfading/NoFastFading.hpp>

#include <WNS/StaticFactoryBroker.hpp>
#include <WNS/pyconfig/View.hpp>
#include <WNS/PowerRatio.hpp>
using namespace rise::scenario::fastfading;

STATIC_FACTORY_BROKER_REGISTER(None, FastFading, "None");

None::None(const wns::pyconfig::View&)
    : FastFading()
{}

wns::Ratio None::getFastFading(const antenna::Antenna&,
			                 const antenna::Antenna&,
			                 const wns::Frequency&,
                             const antenna::Antenna&) const
{
    return getFastFading();
}

wns::Ratio None::getFastFading() const
{
    return wns::Ratio::from_dB(0.0);
}

