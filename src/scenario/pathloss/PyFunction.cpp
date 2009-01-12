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

#include <RISE/scenario/pathloss/PyFunction.hpp>

#include <WNS/Ttos.hpp>
#include <WNS/StaticFactoryBroker.hpp>

#include <string>

using namespace rise::scenario::pathloss;

STATIC_FACTORY_BROKER_REGISTER(PyFunction, Pathloss, "PyFunction");

PyFunction::PyFunction(const wns::pyconfig::View& _config)
    : SingleSlope(_config),
	  config(_config)
{

}

wns::Ratio
PyFunction::calculatePathloss(	const antenna::Antenna& source,
                                const antenna::Antenna& target,
                                const wns::Frequency& frequency,
                                const wns::Distance& /*distance*/) const
{
	bool sourceIsBase = wns::isClass<Base>(*(source.getStation()));
	const antenna::Antenna& baseAntenna(sourceIsBase ? source : target);
	wns::Distance baseHeight = baseAntenna.getPosition().getZ();

	std::string s = "calculatePathloss(" +wns::Ttos(source.getPosition().getX())+ ","
					     +wns::Ttos(source.getPosition().getY())+ ","
					     +wns::Ttos(target.getPosition().getX())+ ","
					     +wns::Ttos(target.getPosition().getY())+ ","
					     +wns::Ttos(frequency)+ "," 
					     +wns::Ttos(baseHeight)+ ")";
	wns::Ratio pl = config.get<wns::Ratio>(s);

	return(pl);
}
