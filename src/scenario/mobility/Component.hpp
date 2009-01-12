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

#ifndef RISE_MOBILITY_COMPONENT_HPP
#define RISE_MOBILITY_COMPONENT_HPP

#include <WNS/node/component/Component.hpp>
#include <WNS/PositionObserver.hpp>
#include <WNS/logger/Logger.hpp>
#include <WNS/Observer.hpp>
#include <WNS/probe/bus/ContextCollector.hpp>

namespace rise { namespace scenario { namespace mobility {

	class MobilityInterface;

	/**
	 * @brief Can be added to a node in order to make it move
	 *
	 * The component is additionally derived from PositionObserver to
	 * observe the mobility object itself. The component registers the
	 * following IDProviders:
	 * - rise.scenario.mobility.x
	 * - rise.scenario.mobility.y
	 * - rise.scenario.mobility.z
	 */
	class Component :
		public wns::node::component::Component,
		public wns::Observer<wns::PositionObserver>
	{
	public:
		Component(
			wns::node::Interface* node,
			const wns::pyconfig::View& pyConfigView);

		virtual
		~Component();

		// Component interface
		virtual void
		onNodeCreated();

		virtual void
		onWorldCreated();

		virtual void
		onShutdown();

		// PositionObserver interface
		virtual void
		positionWillChange();

		virtual void
		positionChanged();

		int
		getX();

		int
		getY();

		int
		getZ();

	private:
		virtual void
		doStartup();

		/**
		 * @brief The actual object realizing the mobility
		 */
		MobilityInterface* mobility;

		/**
		 * @brief Logger of mobility component
		 */
		wns::logger::Logger logger;

        /**
         * @brief Position ContextCollectorX
         */
        wns::probe::bus::ContextCollectorPtr positionContextCollectorX;

        /**
         * @brief Position ContextCollectorY
         */
        wns::probe::bus::ContextCollectorPtr positionContextCollectorY;

        /**
         * @brief Position ContextCollectorZ
         */
        wns::probe::bus::ContextCollectorPtr positionContextCollectorZ;
	};
} // mobility
} // scenario
} // rise

#endif // NOT defined RISE_MOBILITY_COMPONENT_HPP


