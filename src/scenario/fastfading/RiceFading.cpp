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


#include <RISE/scenario/fastfading/RiceFading.hpp>
#include <RISE/scenario/fastfading/FastFading.hpp>

#include <WNS/pyconfig/View.hpp>
#include <WNS/PowerRatio.hpp>
#include <WNS/StaticFactoryBroker.hpp>

using namespace rise::scenario::fastfading;

STATIC_FACTORY_BROKER_REGISTER(Rice, FastFading, "Rice"); // __plugin__ in class FTFading

//! Default constructor
Rice::Rice(const wns::pyconfig::View& config)
	: FastFading(),
	  riceDistRNG(0.0, config.get<double>("variance"), wns::simulator::getRNG())
{
}


//! Returns short term fading
wns::Ratio Rice::getFastFading() const
{
    return wns::Ratio::from_dB(riceDistRNG());
}
