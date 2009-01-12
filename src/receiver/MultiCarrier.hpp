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

#ifndef __RISE_MULTICARRIER_HPP
#define __RISE_MULTICARRIER_HPP

#include <RISE/receiver/ReceiverInterface.hpp>
#include <WNS/PowerRatio.hpp>

namespace rise {
	class Transmitter;
}

namespace rise { namespace receiver {
	/**
	 * @brief Enables a Receiver to have more than one Carrier
	 *
	 * @note We don't have a start- and stopReceiving method here because it
	 * turned out that it is not needed that often. We can add it later, if we
	 * need. The Receiver will connect to the PhysicalResource after tuning.
	 */
	class MultiCarrierAspect
	{
	public:
		virtual ~MultiCarrierAspect() {}
		virtual void tune(double f, double b, int numberOfCarriers) = 0;
// 		virtual wns::Power getTotalRxPower(int subCarrier) = 0;
// 		virtual wns::Power getNoise(int carrier) = 0;
	};

	/**
	 * @brief Base of every MuliCarrierReceiver
	 */
	class MultiCarrier :
		virtual public ReceiverInterface,
		virtual public MultiCarrierAspect
	{
	};
}}

#endif // not defined __RISE_MULTICARRIER_HPP



