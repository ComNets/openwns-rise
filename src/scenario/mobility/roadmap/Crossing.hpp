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

#ifndef _ROADMAPCROSSING_HPP
#define _ROADMAPCROSSING_HPP

#include <vector>
#include <WNS/distribution/Uniform.hpp>
#include <WNS/pyconfig/View.hpp>

#include <RISE/scenario/mobility/roadmap/MapUser.hpp>
#include <RISE/scenario/mobility/roadmap/MapObject.hpp>

namespace rise { namespace scenario { namespace mobility { namespace roadmap {

	typedef std::vector< std::vector<double> > matrix;
	matrix getMatrix(const wns::pyconfig::View& aView, const std::string& aName);

	/**
	 * @brief Crossing object for RoadMap mobility model.
	 */
	class Crossing :
		public MapObject
	{
		wns::Position position;
		std::vector<MapObject::ID> connections;
		std::vector< std::vector<double> > probMatrix;
		wns::distribution::StandardUniform* uniDis;
	  public:
		/** @brief pyconfig::Parser Constructor */
		Crossing(const wns::pyconfig::View& crossingView, Map* _map);
		virtual ~Crossing(){delete uniDis;};
		/** @brief when asked by another object for a new position */
		virtual wns::Position getNextPosition(MapUser* user, simTimeType dt) const;
		/** @brief when asked for a user's position */
		virtual wns::Position getPosition(const MapUser* user) const;
		/** @brief used for consistency checking */
		virtual std::vector<MapObject::ID> getDependencies() const;
	};

}}}}

#endif  // _ROADMAPCROSSING_HPP


