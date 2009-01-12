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

#ifndef _SCTRANSMITTER_HPP
#define _SCTRANSMITTER_HPP

#include <RISE/transceiver/transmitter.hpp>
#include <RISE/misc/pointer.hpp>
#include <RISE/misc/RISELogger.hpp>

#include <WNS/pyconfig/View.hpp>
#include <WNS/PowerRatio.hpp>

#include <sstream>

namespace rise
{
	class TransmissionObject;

	/**
	   @brief Allows a Station to transmit on a Single Carrier Transmitter
	* @ingroup TRANSCEIVER */
	class SCTransmitter :
		public Transmitter
	{
	public:
		//! Constructor that creates Transmitter with given Station and Antenna.
		SCTransmitter(const wns::pyconfig::View& config, Station* s, antenna::Antenna* a);

		//! Default destructor
		virtual ~SCTransmitter();

		//! Used to start a transmisson.
		/** startTransmitting() will fail if the Transmitter has no
		    PhysicalResource. A PhysicalResource may be assigned to the
		    Transmitter by using tune(frequency, bandwidth).
		    active is set true. If the Transmitter is already in state
		    active, the function will fail. */
		virtual void startTransmitting(TransmissionObjectPtr transmissionObject);

		//! Used to stop a transmisson.
		/** stopTransmitting() will fail if the Transmitter is not
		    active. */
		virtual void stopTransmitting(TransmissionObjectPtr transmissionObject);

        //! Tune the Transceiver to a PhysicalResource.
		virtual void tune(double f, double b);
		//! called if Transmitter moved
		virtual void mobilityUpdate();

		virtual double getFrequency();

		virtual bool isActive() const;

	private:
		bool active;
	};
}

#endif


