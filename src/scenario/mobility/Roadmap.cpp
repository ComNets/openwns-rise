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


#include <RISE/scenario/mobility/Roadmap.hpp>
#include <RISE/scenario/mobility/roadmap/Map.hpp>

using namespace rise;
using namespace rise::scenario::mobility;

STATIC_FACTORY_REGISTER_WITH_CREATOR(
	Roadmap,
	MobilityInterface,
	"rise.mobility.Roadmap",
	wns::PyConfigViewCreator);


Roadmap::Roadmap(const wns::pyconfig::View& mobilityView) :
	Mobility(mobilityView),
	map(NULL),
	user(NULL)
{
	RoadmapRegistry& rmr = getRoadmapRegistry();

	std::string mapName = mobilityView.get<wns::pyconfig::View>("roadMap").get<std::string>("name");

	if (rmr.find(mapName) == rmr.end() )
		rmr[mapName] = new roadmap::Map(mobilityView.get<wns::pyconfig::View>("roadMap"));

	map = rmr[mapName];

    wns::rng::RNGen* rng = 
        wns::simulator::getRegistry()->find<wns::rng::RNGen*>("MOBILITY-RNG");

	wns::pyconfig::View velocityConfig = mobilityView.get<wns::pyconfig::View>("userVelocityDist");

	std::string name = velocityConfig.get<std::string>("__plugin__");
	velocityDistribution =
		wns::distribution::RNGDistributionFactory::creator(name)->create(rng, velocityConfig);

	user = map->createNewUser((*velocityDistribution)());

	this->setPosition(map->getPosition(user));

	startPeriodicTimeout(moveTimeStep, moveTimeStep);
}

Roadmap::~Roadmap()
{
	// Don't delete others might be using this ...
    // delete map;
	delete user;
	delete velocityDistribution;
}

void
Roadmap::periodically()
{
	this->move();
}

void
Roadmap::move()
{
	wns::Position newPosition = map->getNextPosition(user, moveTimeStep);
	this->setPosition(newPosition);
}

Roadmap::RoadmapRegistry&
Roadmap::getRoadmapRegistry()
{
	static RoadmapRegistry map;
	return map;
}
