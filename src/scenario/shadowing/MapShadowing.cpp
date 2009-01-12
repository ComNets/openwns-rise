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

#include <RISE/scenario/shadowing/MapShadowing.hpp>
#include <RISE/scenario/scenerymap/SceneryMap.hpp>
#include <RISE/stations/station.hpp>

#include <WNS/StaticFactoryBroker.hpp>
#include <WNS/pyconfig/View.hpp>

#include <string>

using namespace rise::scenario::shadowing;

STATIC_FACTORY_BROKER_REGISTER(Map, Shadowing, "Map");

Map::Map(const wns::pyconfig::View&)
    : Shadowing()
{
}

wns::Ratio Map::getShadowing(const antenna::Antenna& source, const antenna::Antenna& target) const
{
    const ShadowingMap2D* shadowingMap;
    const Interpolation2D* interpolatedMap;
    // let's call the station, that has the map, well, umm... base! ;-)
    const antenna::Antenna* baseAntenna;
    const antenna::Antenna* mobileAntenna;

    if (!source.getStation()->hasPathlossMap()) {
	baseAntenna = &source;
	mobileAntenna = &target;
    }
    else {
	baseAntenna = &target;
	mobileAntenna = &source;
    }

    shadowingMap = &baseAntenna->getStation()->getShadowingMap();
    interpolatedMap = &baseAntenna->getStation()->getInterpolatedShadowingMap();

    // Calculate the point on the line "source - target" with a distance of 1 from source.
    const double dx = baseAntenna->getPosition().getX() - mobileAntenna->getPosition().getX();
    const double dy = baseAntenna->getPosition().getY() - mobileAntenna->getPosition().getY();

    const double len = sqrt(dx*dx + dy*dy);

    double x = (mobileAntenna->getPosition().getX() / shadowingMap->getResolution(scenerymap::Dim::X)) + dx/len;
    double y = (mobileAntenna->getPosition().getY() / shadowingMap->getResolution(scenerymap::Dim::Y)) + dy/len;

    if (x > shadowingMap->dimSize(scenerymap::Dim::X) - 1) x = shadowingMap->dimSize(scenerymap::Dim::X) - 1;
    if (y > shadowingMap->dimSize(scenerymap::Dim::Y) - 1) y = shadowingMap->dimSize(scenerymap::Dim::Y) - 1;

    return wns::Ratio::from_dB((*interpolatedMap)[x][y]);
}
