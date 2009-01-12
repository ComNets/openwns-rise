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

#ifndef RISE_SCENARIO_MOBILITY_EVENTLIST_HPP
#define RISE_SCENARIO_MOBILITY_EVENTLIST_HPP

#include <RISE/scenario/mobility/Mobility.hpp>

namespace rise { namespace scenario { namespace mobility {

	class EventList :
		public Mobility
	{
	public:
		/**
		 * @brief pyconfig::Parser Constructor
		 */
		explicit
		EventList(const wns::pyconfig::View& mobilityView);

		/**
		 * @brief Destructor
		 */
		virtual
		~EventList();

		/**
		 * @brief Implement MobilityInterface
		 */
		virtual void
		move();

		/**
		 * @brief for testing and handling external events
		 */
		virtual void
		moveTo(const wns::Position& pos);

		/**
		 * @brief for testing and handling external events
		 */
		virtual void
		moveTo(const wns::PositionOffset& pos);

		/**
		 * @brief Implement PeriodicTimeout
		 */
		virtual void
		periodically();

		/**
		 * @brief add events for mobility updates
		 */
		void
		addEvent(const wns::pyconfig::View& eventView);

		template <typename POSITIONTYPE>
		class PositionUpdateEvent
		{
		public:
			PositionUpdateEvent(MobilityInterface* m, const POSITIONTYPE& p) :
				station(m),
				newPosition(p)
			{}

			void
			operator()()
			{
				station->moveTo(newPosition);
			}

		private:
			MobilityInterface* station;
			POSITIONTYPE newPosition;
		};
		typedef PositionUpdateEvent<wns::PositionOffset> RelativePositionUpdateEvent;
		typedef PositionUpdateEvent<wns::Position> AbsolutePositionUpdateEvent;

	}; // EventList

} // mobility
} // scenario
} // rise

#endif // RISE_SCENARIO_MOBILITY_EVENTLIST_HPP


