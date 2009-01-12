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

#ifndef __PHYSICALRESOURCEOBSERVER_HPP
#define __PHYSICALRESOURCEOBSERVER_HPP

#include <RISE/misc/pointer.hpp>
#include <RISE/transmissionobjects/transmissionobject.hpp>
#include <RISE/transceiver/transmitter.hpp>

#include <WNS/PowerRatio.hpp>
#include <WNS/PositionObserver.hpp>

namespace rise {
	/**
	 * @brief Interface that is used by PhysicalResource
	 *
	 * The PhysicalResource uses this interface in order to notify a
	 * PhysicalResourceObserver of important changes (like change of interference
	 * (via notify) and movement of an Transmitter (results in changed
	 * interference, too)
	 */
	class PhysicalResourceObserver
	{
	public:
		virtual ~PhysicalResourceObserver()
		{}

		/**
		 * @brief If a Transmission starts or stops this method is called by the
		 * PhysicalResource
		 */
		virtual void notify(TransmissionObjectPtr t) = 0;

		/**
		 * @brief If a Transmitter moves this method is called by the
		 * PhysicalResource.
		 *
		 * @param t The Transmitter, that moved (necessary for caching)
		 * @param p The PhysicalResource on which the Transmitter is transmitting
		 */
		virtual void mobilityUpdate(Transmitter* t) = 0;
	};
}


#endif // NOT defined PHYSICALRESOURCEOBSERVER_HPP




