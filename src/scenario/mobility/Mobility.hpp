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

#ifndef RISE_SCENARIO_MOBILITY_MOBILITY_HPP
#define RISE_SCENARIO_MOBILITY_MOBILITY_HPP

#include <WNS/logger/Logger.hpp>
#include <WNS/Positionable.hpp>
#include <WNS/events/PeriodicTimeout.hpp>
#include <WNS/StaticFactory.hpp>
#include <WNS/PyConfigViewCreator.hpp>
#include <WNS/service/Service.hpp>

#include <WNS/rng/RNGen.hpp>

namespace rise { namespace scenario { namespace mobility {

	class MobilityInterface :
		virtual public wns::service::Service,
		virtual public wns::PositionableInterface
	{
	public:
		/**
		 * @brief abstract Interface to the outside world
		 */
		virtual void
		move() = 0;

		/**
		 * @brief for testing and handling external events
		 */
		virtual void
		moveTo(const wns::Position& pos) = 0;

		/**
		 * @brief for testing and handling external events
		 */
		virtual void
		moveTo(const wns::PositionOffset& pos) = 0;
	}; // MobilityInterface

	class Mobility :
		virtual public MobilityInterface,
		public wns::Positionable,
		public wns::events::PeriodicTimeout
	{
	public:
		/**
		 * @brief pyconfig::Parser Constructor
		 */
		explicit
		Mobility(const wns::pyconfig::View& mobilityView);

		/**
		 * @brief Destructor
		 */
		virtual
		~Mobility();

		/**
		 * @brief for testing and handling external events
		 */
		void
		moveTo(const wns::Position& pos);

		/**
		 * @brief for testing and handling external events
		 */
		void
		moveTo(const wns::PositionOffset& pos);

	protected:
		/**
		 * @brief time resolution of mobility model
		 */
		simTimeType moveTimeStep;
		wns::logger::Logger log;
	}; // Mobility

	/**
	 * @brief Special Creator for MobilityInterface (takes a pyconfig::View)
	 */
	typedef wns::PyConfigViewCreator<MobilityInterface, MobilityInterface> MobilityCreator;
	/**
	 * @brief Provide a StaticFactory for MobilityInterfaces
	 */
	typedef wns::StaticFactory<MobilityCreator> MobilityFactory;

}
}
}

#endif // RISE_SCENARIO_MOBILITY_MOBILITY_HPP


