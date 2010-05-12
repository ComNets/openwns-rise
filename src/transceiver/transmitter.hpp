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

#ifndef _TRANSMITTER_HPP
#define _TRANSMITTER_HPP

#include <RISE/misc/pointer.hpp>
#include <RISE/medium/PhysicalResource.hpp>
#include <RISE/scenario/Scenario.hpp>
#include <RISE/scenario/PropagationCharacteristic.hpp>
#include <RISE/transmissionobjects/transmissionobject.hpp>

#include <WNS/PositionObserver.hpp>
#include <WNS/container/FastListEnabler.hpp>
#include <WNS/logger/Logger.hpp>
#include <WNS/pyconfig/View.hpp>
#include <WNS/PowerRatio.hpp>
#include <WNS/Observer.hpp>

#include <sstream>

namespace rise { namespace antenna {
	class Antenna;
}}

namespace rise
{
	class TransmissionObject;
	class Station;

	/**
	   @brief Allows a Station to transmit on one PhysicalResource.
	   This class holds the respective information to allow a Station to
	    transmit on one PhysicalResource. One Transmitter may have exactly one
	    PhysicalResource and an appropriate wns::Power (txPwr). In case a Station
	    should transmit on more than one PhysicalResource in parallel, it must
	    have more than one Transmitter!  The Transmitter is either in state
	    transmitting or not ;).  ATTENTION: A transmission here means emitting
	    radio waves. So in GSM a Transmitter would start and stop his
	    transmission every radio frame ... */
	/** @ingroup TRANSCEIVER */
	class Transmitter :
		public wns::container::FastListEnabler<Transmitter*>,
		public wns::Observer<wns::PositionObserver>,
		public scenario::PropagationCharacteristic
	{
	public:
		//! Constructor that creates Transmitter with given Station and Antenna.
		Transmitter(const wns::pyconfig::View& config, Station* s, antenna::Antenna* a);

		//! Default destructor
		virtual ~Transmitter();

		//! Used to start a transmisson.
		/** startTransmitting() will fail if the Transmitter has no
		    PhysicalResource. A PhysicalResource may be assigned to the
		    Transmitter by using tune(PhysicalResource *resource).
		    Transmitting is set true. If the Transmitter is already in state
		    transmitting, the function will do nothing. */
		virtual void startTransmitting(TransmissionObjectPtr transmissionObject, long int Sub_band_Id);

		//! Used to stop a transmisson.
		/** stopTransmitting() will fail if the Transmitter has no
		    PhysicalResource. A PhysicalResource may be assigned to the
		    Transmitter by using tune(PhysicalResource *resource).
		    Transmitting is set to false. If the Transmitter is not in state
		    transmitting, the function will do nothing. */
		virtual void stopTransmitting(TransmissionObjectPtr transmissionObject);

		//! Returns the the ID of this transmitter.
		virtual long int getTransmitterId() const
		{
			return transmitterId;
		}
        //! Tune the Transceiver to a PhysicalResource.
		virtual void tune(double f, double b, long int numberOfSubCarriers);
		//! called if Transmitter moved
		virtual void positionChanged();

		//! called if Transmitter will move
		virtual void positionWillChange();

		//! Returns the current signal power of the Transmitter.
		virtual wns::Power getTxPower() const
		{
			return txPwr;
		}
		//! Sets the current signal power of this Transmitter
		virtual void setTxPower(wns::Power p)
		{
			txPwr = p;
		}
		//! Returns the max. signal power of this Transmitter
		virtual wns::Power getMaxTxPower() const
		{
			return maxTxPwr;
		}
		//! Sets the max. signal power of this Transmitter 
		virtual void setMaxTxPower(wns::Power p) {
			maxTxPwr=p;
		}

		antenna::Antenna* getAntenna() const
		{
			return antenna;
		}

		virtual std::string getDebugInfo();

		virtual Station* getStation()=0; // pure virtual! Derive and implement!

	protected:
		//! The wns::Power the Transmitter is currently adjusted to
		wns::Power txPwr;
		//! The Id of the transmitter (for messages only)
		long int transmitterId;
		//! The wns::Power the Transmitter is currently adjusted to
		wns::Power maxTxPwr;
		bool pd_debugFlag;

		typedef std::vector<medium::PhysicalResource*> PhysicalResourceContainer;
		typedef PhysicalResourceContainer::iterator PhysicalResourceIterator;

		/**
		 * @brief Contains all PhysicalResource the Transceiver is tuned to
		 */
		PhysicalResourceContainer prc;

		/**
		 * @brief The antenna of the Transceiver
		 */
		antenna::Antenna* antenna;

		/**
		 * @brief Mid frequency of the whole frequency band
		 */
		double midFrequency;

		//! Logs messages to a main logger.
		//RISELogger log;
		wns::logger::Logger log;

	private:

		static long int nextid;

		bool active;

		void configureLogger();
	};
}

#endif


