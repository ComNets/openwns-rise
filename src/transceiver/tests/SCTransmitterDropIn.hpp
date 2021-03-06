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

#ifndef _SCTRANSMITTERDROPIN_HPP
#define _SCTRANSMITTERDROPIN_HPP

#include <RISE/transceiver/SCTransmitter.hpp>
#include <RISE/antenna/Antenna.hpp>
#include <RISE/stations/station.hpp>

#include <WNS/pyconfig/helper/Functions.hpp>

namespace rise { namespace tests {
	class SCTransmitterDropIn :
		public SCTransmitter
	{
	public:
		SCTransmitterDropIn(Station* s, antenna::Antenna* a) :
			SCTransmitter(wns::pyconfig::helper::createViewFromDropInConfig("rise.Transmitter", "SCTransmitterDropIn"), s, a),
			station(s)
		{
		}

		virtual Station* getStation()
		{
			assure(station, "No valid station available");
			return station;
		}

		virtual ~SCTransmitterDropIn(){}

	private:
		Station* station;

	}; // class SCTransmitterDropIn
} // namespace tests
} // namespace rise

#endif // NOT defined _SCTRANSMITTERDROPIN_HPP


