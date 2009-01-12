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

#ifndef _RECEIVERDROPIN_HPP
#define _RECEIVERDROPIN_HPP

#include <RISE/transceiver/receiver.hpp>
#include <RISE/stations/station.hpp>
#include <RISE/antenna/Antenna.hpp>
#include <RISE/receiver/LossCalculation.hpp>
#include <RISE/manager/tests/SystemManagerDropIn.hpp>

#include <WNS/pyconfig/Parser.hpp>
#include <WNS/pyconfig/helper/Functions.hpp>

namespace rise { namespace tests {
	class ReceiverDropIn :
		public Receiver,
		public receiver::LossCalculation,
		virtual public receiver::NoTransmissionAveraging
	{
	public:
		ReceiverDropIn(Station* s,
					   antenna::Antenna* a,
					   const wns::pyconfig::View& config =
					   wns::pyconfig::helper::createViewFromDropInConfig("rise.Receiver",
																		 "ReceiverDropIn"))
			: Receiver(s,
					   a,
					   NULL,
					   NULL,
					   wns::Ratio::from_dB(0)),
			  receiver::LossCalculation(config),
			  station(s)
		{}

		virtual ~ReceiverDropIn()
		{}

		Station* getStation() const
		{
			assure(station, "No valid station available");
			return station;
		}

		virtual void notify(TransmissionObjectPtr)
		{}

	private:
		Station* station;
	};
} // namespace tests
} // namespace rise

#endif // _RECEIVERDROPIN_HPP


