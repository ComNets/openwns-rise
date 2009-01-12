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

#ifndef _STATIONDROPIN_HPP
#define _STATIONDROPIN_HPP

#include <RISE/stations/station.hpp>
#include <RISE/manager/tests/SystemManagerDropIn.hpp>
#include <RISE/transceiver/receiver.hpp>
#include <RISE/transceiver/transmitter.hpp>
#include <RISE/scenario/mobility/None.hpp>
#include <WNS/pyconfig/Parser.hpp>
#include <WNS/pyconfig/View.hpp>
#include <WNS/pyconfig/helper/Functions.hpp>

namespace rise { namespace tests {
	class SystemManagerDropIn;
	class StationDropIn :
		public Station
	{
	public:
		StationDropIn(SystemManagerDropIn* _sm,
					  const wns::pyconfig::View& config =
					  wns::pyconfig::helper::createViewFromDropInConfig("rise.Station",
																		"StationDropIn")) :
			Station(config),
			sm(_sm)
		{
			wns::pyconfig::View mobilityView = wns::pyconfig::helper::createViewFromDropInConfig("rise.Mobility",
																								 "MobilityDropin");
			setMobility( new rise::scenario::mobility::None(mobilityView) );
		}

		virtual ~StationDropIn() {
			//delete mobility;
		}

		virtual void move()
		{}

		void set(receiver::ReceiverInterface* r)
		{
			receiver = r;
		}

		void set(Transmitter* t)
		{
			transmitter = t;
		}

		SystemManagerDropIn* getSystemManager() const
		{
			return sm;
		}

	private:
		SystemManagerDropIn* sm;
		Transmitter* transmitter;
		receiver::ReceiverInterface* receiver;

	};
}
}


#endif // _STATIONDROPIN_HPP



