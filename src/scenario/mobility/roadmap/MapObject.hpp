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

#ifndef _SCENARIO_MOBILITY_ROADMAP_MAPOBJECT_HPP
#define _SCENARIO_MOBILITY_ROADMAP_MAPOBJECT_HPP

#include <WNS/Position.hpp>
#include <vector>

#include <RISE/misc/RISELogger.hpp>

namespace rise { namespace scenario { namespace mobility { namespace roadmap {

	class MapUser;
	class Map;

	/** @brief abstract base class for Street and Crossing Objects*/
	class MapObject
	{
	public:
		typedef uint32_t ID;

		/** @brief constructor */
		MapObject(ID _id, Map* _map);
		virtual ~MapObject();
		/** @brief returns ID */
		ID getID() const;
		/** @brief when asked by another object for a new position */
		virtual wns::Position getNextPosition(MapUser* user, simTimeType dt) const = 0;
		/** @brief when asked by another object for the current position */
		virtual wns::Position getPosition(const MapUser* user) const = 0;
		/** @brief used for consistency checking */
		virtual std::vector<ID> getDependencies() const = 0;
	protected:
		/** @brief unique identifier for this object */
		ID id;
		/** @brief pointer to my map */
		Map* map;
		/** @brief logger for the mapObjects */
		RISELogger log;
	};

}}}}

#endif  // _SCENARIO_MOBILITY_ROADMAP_MAPOBJECT_HPP
