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

#ifndef __RECEIVER_HPP
#define __RECEIVER_HPP

#include <RISE/misc/pointer.hpp>
#include <RISE/misc/RISELogger.hpp>
#include <RISE/receiver/ReceiverInterface.hpp>

#include <WNS/PowerRatio.hpp>
#include <WNS/DoubleDispatcher.hpp>
#include <WNS/TypeInfo.hpp>
#include <WNS/TimeWeightedAverage.hpp>
#include <WNS/Cache.hpp>
#include <WNS/logger/Logger.hpp>
#include <WNS/pyconfig/View.hpp>

#include <sstream>
#include <list>

namespace rise { namespace antenna {
	class Antenna;
}}

namespace rise { namespace medium {
	class PhysicalResource;
}}

namespace rise
{
	class Transmitter;
	class Station;
	class Demodulator;
	class Decoder;
	class PropagationCache;
	class PropCacheEntry;

	/**
	 * @brief Receives TransmissionObjects and calculates Carrier and
	 * Interference
	 *
	 * This is a general Multi-Carrier Receiver. It is able to listen to
	 * multiple frequency bands. A frequency band corresponds to a
	 * PhysicalResource. Hence, the Receiver registers on a PhysicalResource and
	 * gets notified on Transmission start and stop. The Receiver is either in
	 * state receiving or not ;). In case a Transmitter will start transmitting
	 * (Transmitter::startTransmitting()) on a PhysicalResource the Receiver
	 * will be notified by the PhysicalResource, that is Receiver::notify() is
	 * called by the PhysicalResource.
	 *
	 * @ingroup TRANSCEIVER
	 */
	class ReceiverBase :
		virtual public receiver::ReceiverInterface
	{
		friend class PropagationCache;
	public:
		/**
		 * @brief Constructor
		 */
		ReceiverBase(Station* s,
					 Demodulator *demodulator,
					 Decoder *decoder,
					 wns::Ratio rnf);

		/**
		 * @brief Destructor
		 */
		virtual ~ReceiverBase();

		//! listen to a specific PhysicalResource
		/** @brief
		   Sets receiving to true. The Receiver is attached to a
		   PhysicalResource (via PhysicalResource::attachReceiver()). If the
		   Receiver has no PhysicalResource, the function will fail. If the
		   Receiver is already listening on a PhysicalResource the function
		   will do nothing.
		*/
		virtual void startReceiving();

		//! stop listening to a specific PhysicalResource
		/** @brief
		   Sets receiving to false. The Receiver is detached from a
		   PhysicalResource (via PhysicalResource::detachReceiver()). If the
		   Receiver has no PhysicalResource, the function will fail. If the
		   Receiver was not listening on a PhysicalResource the function will
		   do nothing.
		*/
		virtual void stopReceiving();

		//! Returns the total received signal power of the PhysicalResource the
		//  Receiver is listening to.
		/** NOTE: This is a very simple implementation. It assumes your receiver
			is working on the whole bandwidth of the PhysicalResource. If you
			need something sophisticated please go ahead and implement something
			on your own (that's why the method is virtual).*/
		virtual wns::Power getTotalRxPower();

        //! Returns the interference for this TransmissionObject.
		/** The interference is here defined as: the total received signal power
			- the carrier (getTotalRxPower()-getRxPower(TransmissionObject)) */
		virtual wns::Power getInterference(const TransmissionObjectPtr& t);

		//! Returns the received signal power of this transmission
		virtual wns::Power getRxPower(const TransmissionObjectPtr& t);

		//! return receiver noise power
		wns::Power getNoise();

		/** @brief listening on these frequencies */
		virtual void tune(double f, double b, long int numberOfSubCarriers);

		//! Called by the PhysicalResource, if a Transmitter moves.
		/** PhysicalResource will call this function of every Receiver
			attached to itsself, if a Transmitter moves. The Receiver
			will update its Cache for this Transmitter.		*/
		virtual void mobilityUpdate(Transmitter* transmitter);

        //! Returns true if TransmissionObject is active.
		/**
		   Function checks if the list 'TransmissionObjects' contains
		   the given TransmissionObject.
		*/
		virtual bool contains(TransmissionObjectPtr);

		bool isActive() const
		{
			return active;
		}

		//! Called, if Receiver moved
		/** Called, if Receiver moved. The Receiver will update its Cache.
		 */
		virtual void positionChanged();

		//! Called, before Receiver moves.
		virtual void positionWillChange();

		virtual antenna::Antenna* getAntenna() const = 0;
	protected:

        //! Updates the given cache entry.
		/** Sets the current values for pathloss, shadowing and
			antennaGain in the given cache entry.
		**/
		virtual void writeCacheEntry(PropCacheEntry& cacheEntry,
									 Transmitter* t,
									 double freq);

		//! The C/I of the current connection
		wns::Ratio CIR;

		//! The Id of the receiver (for messages only)
		unsigned long int receiverId;

		//! holds the next id of a receiver
		static unsigned long int nextid;

		//! The demodulator used
		Demodulator *demodulator;

		//! The convolutional decoder used
		Decoder *decoder;

		//! The total received signal power
		wns::Cache<wns::Power> totalRxPower;

		//! List of all active TransmissionObjects
		std::list<TransmissionObjectPtr> transmissionObjects;

		//! stores losses between receiver and all its transmitters
		PropagationCache* propCache;

		wns::Ratio receiverNoiseFigure;
		bool pd_debugFlag;

		typedef std::vector<medium::PhysicalResource*> PhysicalResourceContainer;
		typedef PhysicalResourceContainer::iterator PhysicalResourceIterator;

		/**
		 * @brief Contains all PhysicalResources the Transceiver is tuned to
		 */
		PhysicalResourceContainer prc;

		/**
		 * @brief State of the Transceiver
		 */
		bool active;

		/**
		 * @brief Mid frequency of the whole frequency band
		 */
		double midFrequency;
		//! Logs messages to a main logger.
		RISELogger log;
		// TODO: ^ replace by this:
		//wns::logger::Logger logger;

	private:

		void configureLogger();
		//! Initiate the receivers cache which stores total losses.
		void initiatePropCache();
	};

	class Receiver : public ReceiverBase
	{
	public:
		/**
		 * @brief Constructor
		 */
		Receiver(Station* s,
				 antenna::Antenna* a,
				 Demodulator *demodulator,
				 Decoder *decoder,
				 wns::Ratio rnf);
		/**
		 * @brief Destructor
		 */
		virtual ~Receiver();
		virtual antenna::Antenna* getAntenna() const
			{
				return antenna;
			}
		//! Returns the total loss between the Receiver and a given Transmitter
		virtual wns::Ratio getLoss( Transmitter* t);

	protected:
		/**
		 * @brief The antenna of the Transceiver
		 */
		antenna::Antenna* antenna;
	private:
		/**
		 * @brief Copy constructor is disabled
		 */
		Receiver(const Receiver&);
		/**
		 * @brief Assignment operator is disabled
		 */
		ReceiverBase& operator=(const ReceiverBase&);

	};

}
#endif


