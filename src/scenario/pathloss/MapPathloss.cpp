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

#include <RISE/scenario/pathloss/MapPathloss.hpp>

#include <RISE/scenario/scenerymap/SceneryMap.hpp>
#include <RISE/antenna/Antenna.hpp>
#include <RISE/stations/base/base.hpp>

#include <WNS/pyconfig/View.hpp>
#include <WNS/SmartPtr.hpp>
#include <WNS/PowerRatio.hpp>
#include <WNS/StaticFactoryBroker.hpp>
#include <WNS/isClass.hpp>

using namespace rise::scenario::pathloss;

STATIC_FACTORY_BROKER_REGISTER(Map, Pathloss, "Map");

Map::Map(const wns::pyconfig::View& config)
    : DistanceIndependent(config)
{}

Map::~Map()
{
}

wns::Ratio Map::calculatePathloss(const antenna::Antenna& source,
				  const antenna::Antenna& target,
				  const wns::Frequency&) const
{
    double x, y;

    const PathlossMap2D* pathlossMap;
    const Interpolation2D* interpolatedMap;
    if (!source.getStation()->hasPathlossMap()) { 
	pathlossMap = &target.getStation()->getPathlossMap();
	interpolatedMap = &target.getStation()->getInterpolatedPathlossMap();
	x = source.getPosition().getX() / pathlossMap->getResolution(scenerymap::Dim::X);
	y = source.getPosition().getY() / pathlossMap->getResolution(scenerymap::Dim::Y);
    }
    else {
	pathlossMap = &source.getStation()->getPathlossMap();
	interpolatedMap = &source.getStation()->getInterpolatedPathlossMap();
	x = target.getPosition().getX() / pathlossMap->getResolution(scenerymap::Dim::X);
	y = target.getPosition().getY() / pathlossMap->getResolution(scenerymap::Dim::Y);
    }

    return wns::Ratio::from_dB((*interpolatedMap)[x][y]);
}
