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
#include <string>
#include <iostream>
#include <iomanip>


using namespace rise::scenario::pathloss;

STATIC_FACTORY_BROKER_REGISTER(PyFunction, Pathloss, "PyFunction");

PyFunction::PyFunction(const wns::pyconfig::View& _config)
    : DistanceDependent(_config),
      config(_config)
{

}

wns::Ratio
PyFunction::calculatePathloss(const antenna::Antenna& source,
                              const antenna::Antenna& target,
                              const wns::Frequency& frequency,
                              const wns::Distance& /*distance*/) const
{
    bool sourceIsBase = wns::isClass<Base>(*(source.getStation()));
    const antenna::Antenna& baseAntenna(sourceIsBase ? source : target);
    wns::Distance baseHeight = baseAntenna.getPosition().getZ();

    std::string s = "calculatePathloss("
        +preciseDtos(source.getPosition().getX())+ ","
        +preciseDtos(source.getPosition().getY())+ ","
        +preciseDtos(target.getPosition().getX())+ ","
        +preciseDtos(target.getPosition().getY())+ ","
        +preciseDtos(frequency)+ ","
        +preciseDtos(baseHeight)+ ")";
    wns::Ratio pl = config.get<wns::Ratio>(s);

    return(pl);
}

std::string
PyFunction::preciseDtos(double value) const
{
    std::ostringstream temp;
    temp << std::setiosflags(std::ios::fixed) << std::setprecision(3) << value;
    return temp.str();
}
