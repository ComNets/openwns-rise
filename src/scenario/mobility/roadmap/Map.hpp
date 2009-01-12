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

#ifndef _SCENARIO_MOBILITY_ROADMAP_MAP_HPP
#define _SCENARIO_MOBILITY_ROADMAP_MAP_HPP

#include <map>
#include <vector>

#include <RISE/misc/RISELogger.hpp>
#include <RISE/scenario/mobility/roadmap/MapObject.hpp>
#include <RISE/scenario/mobility/roadmap/MapUser.hpp>

#include <WNS/distribution/DiscreteUniform.hpp>
#include <WNS/distribution/Uniform.hpp>

class DUniDis;
class UniDis;

namespace rise { namespace scenario { namespace mobility { namespace roadmap {

	/**
	  @brief Class reading, storing and managing a RoadMap
	*/
	class Map
	{
		typedef std::vector<MapObject*> MapObjectContainer;
		typedef std::vector<MapObject::ID> StreetIDContainer;
	  public:

        /** @brief pyconfig::Parser constructor */
		Map(const wns::pyconfig::View& roadmapView);

        /** @brief Destructor */
		~Map();

		/** @brief This method returns a MapUser Object for a new User with
			a given velocity
		 */
		MapUser* createNewUser(double velocity);

		/** @brief This method returns an updated position for a mobile user
			based on its previous parameters. Contrary to the pospar concept
			used in the past, the responsibilty for the parameters is
			exclusively with the map. For the user it's like a sealed envelope.
		  */
		wns::Position getNextPosition(MapUser* user, simTimeType dt);
		/** @brief This method returns the current position of a mobile user
			based on its current parameters. */
		wns::Position getPosition(MapUser* user);
		/** @brief returns the MapObject Ptr for the object with the given id */
		MapObject* getObject(const MapObject::ID id);

	  private:
		/** @brief checks whether all streets and crossings are fully connected */
		void checkConsistency();
		MapObjectContainer mapObjectContainer;
		StreetIDContainer streetIDContainer;

		/** @brief the distributions required for the creation of new users */
		wns::distribution::DiscreteUniform* streetDist;
		wns::distribution::DiscreteUniform* headingDist;
		wns::distribution::StandardUniform* entryPointDist;

		RISELogger log;
	};


}}}}

#endif  // _SCENARIO_MOBILITY_ROADMAP_MAP_HPP
