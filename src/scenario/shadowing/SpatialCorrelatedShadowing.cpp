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

#include <RISE/scenario/shadowing/SpatialCorrelatedShadowing.hpp>
#include <RISE/stations/station.hpp>

#include <WNS/StaticFactoryBroker.hpp>
#include <WNS/pyconfig/View.hpp>
#include <WNS/container/UntypedRegistry.hpp>
#include <WNS/Ttos.hpp>

#include <string>
#include <math.h>


using namespace rise::scenario::shadowing;

STATIC_FACTORY_BROKER_REGISTER(SpatialCorrelated, Shadowing, "SpatialCorrelated");


SpatialCorrelated::SpatialCorrelated(const wns::pyconfig::View& _config)
    : Shadowing(),
	  config(_config)
{

}

wns::Ratio SpatialCorrelated::getShadowing(const antenna::Antenna& source, const antenna::Antenna& target) const
{
	const double x = source.getPosition().getX();
	const double y = source.getPosition().getY();
	const double u = target.getPosition().getX();
	const double v = target.getPosition().getY();

	std::string s = "getShadowing(" + wns::Ttos(x) + "," + wns::Ttos(y) + "," + wns::Ttos(u) + "," + wns::Ttos(v) + ")";
	return(config.get<wns::Ratio>(s));
}
