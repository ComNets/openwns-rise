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

#ifndef RISE_SCENARIO_MOBILITY_ROADMAP_HPP
#define RISE_SCENARIO_MOBILITY_ROADMAP_HPP

#include <RISE/scenario/mobility/Mobility.hpp>
#include <WNS/distribution/Distribution.hpp>

namespace rise { namespace scenario { namespace mobility {

	namespace roadmap {
		class MapUser;
		class Map;
	}

	/**
	 *@brief Mobility model from external RoadMap file.
	 *
	 * The file might contain street objects and crossing objects. Crossings are
	 * connected to streets, streets are connected to streets or crossings. Each
	 * street has a speed limit. All users are moving on the streets with
	 * randomly determined velocity (but always bounded by the speed limit).
	 */
	class Roadmap :
		public Mobility
	{
	  public:
        // pyconfig::Parser constructor
		explicit
		Roadmap(const wns::pyconfig::View& mobilityView);

        // Destructor
		virtual
		~Roadmap();

		/**
		 * @brief Implement MobilityInterface
		 */
		virtual void
		move();

		/**
		 * @brief Implement PeriodicTimeout
		 */
		virtual void
		periodically();

	  private:
		/**
		 * @brief map of all roadmaps
		 */
		typedef std::map<std::string, roadmap::Map*> RoadmapRegistry;

		/**
		 * @brief get map of all roadmaps
		 */
		static RoadmapRegistry& getRoadmapRegistry();

		/**
		 * @brief pointer to my roadmap
		 */
		roadmap::Map* map;

		/**
		 * @brief representation of the user
		 */
		roadmap::MapUser* user;

		/**
		 * @brief Normal distribution of velocity among mobile users
		 */
		wns::distribution::Distribution* velocityDistribution;
	}; // Roadmap

} // mobility
} // scenario
} // rise

#endif  // RISE_SCENARIO_MOBILITY_ROADMAP_HPP
