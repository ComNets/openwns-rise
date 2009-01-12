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

#include <RISE/scenario/pathloss/Deny.hpp>
#include <RISE/scenario/pathloss/Pathloss.hpp>

#include <WNS/PyConfigViewCreator.hpp>
#include <WNS/PowerRatio.hpp>
#include <WNS/StaticFactoryBroker.hpp>
#include <WNS/pyconfig/Parser.hpp>
#include <WNS/pyconfig/View.hpp>
#include <WNS/Types.hpp>
#include <WNS/Exception.hpp>

using namespace rise::scenario::pathloss;

STATIC_FACTORY_BROKER_REGISTER(Deny, Pathloss, "Deny");

Deny::Exception::Exception(const std::string& s)
    : wns::Exception(s)
{}

Deny::Deny()
    : DistanceIndependent(wns::pyconfig::Parser::fromString("minPathloss = \"0 dB\""))
{}

Deny::Deny(const wns::pyconfig::View& config)
    : DistanceIndependent(config)
{}

Deny::~Deny()
{}


wns::Ratio Deny::calculatePathloss(const antenna::Antenna&,
				   const antenna::Antenna&,
				   const wns::Frequency&) const
{
    throw Exception("Pathloss: distance out of range.");
}
