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

#ifndef _SCRECEIVERDROPIN_HPP
#define _SCRECEIVERDROPIN_HPP

#include <RISE/transceiver/SCReceiver.hpp>

#include <WNS/pyconfig/Parser.hpp>
#include <WNS/pyconfig/helper/Functions.hpp>

namespace rise { namespace tests {
	class SCReceiverDropIn :
		public SCReceiver,
		virtual public receiver::NoTransmissionAveraging
	{
	public:
		SCReceiverDropIn(Station* s,
						 antenna::Antenna* a,
						 const wns::pyconfig::View& config =
						 wns::pyconfig::helper::createViewFromDropInConfig("rise.Receiver",
																		   "SCReceiverDropIn")) :
			SCReceiver(config,
					   s,
					   a,
					   NULL,
					   NULL,
					   wns::Ratio::from_dB(3)),
			station(s)
		{}

		virtual ~SCReceiverDropIn()
		{}

		Station* getStation() const
		{
			assure(station, "No valid station available");
			return station;
		}
	private:
		Station* station;
	};
} // namespace tests
} // namespace rise

#endif // _SCRECEIVERDROPIN_HPP


