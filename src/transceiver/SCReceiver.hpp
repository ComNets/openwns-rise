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

#ifndef __SCRECEIVER_HPP
#define __SCRECEIVER_HPP

#include <RISE/transceiver/transmitter.hpp>
#include <RISE/misc/pointer.hpp>
#include <RISE/transceiver/receiver.hpp>
#include <RISE/transmissionobjects/broadcasttransmissionobject.hpp>
#include <RISE/misc/RISELogger.hpp>
#include <RISE/receiver/LossCalculation.hpp>

#include <WNS/PowerRatio.hpp>
#include <WNS/DoubleDispatcher.hpp>
#include <WNS/TypeInfo.hpp>
#include <WNS/TimeWeightedAverage.hpp>
#include <WNS/Cache.hpp>
#include <WNS/pyconfig/View.hpp>

#include <sstream>
#include <list>

namespace rise
{
	class Transmitter;
	class Station;
	class Demodulator;
	class Decoder;
	class PropagationCache;
	class PropCacheEntry;
	class Scenario;

	/**
	 * @brief Allows a Station to listen to one PhysicalResource
	 * @ingroup TRANSCEIVER
	 */
	class SCReceiver :
		public Receiver,
		protected receiver::LossCalculation
	{

	public:
		//! Constructor
		/** Initializes the following attributes of the SCReceiver with the
		 *	given parameters:
		 *     - Station* station
		 *     - Antenna* antenna
		 *     - Demodulator* demodulator
		 *     - Decoder* decoder
		 *     - wns::Ratio receiverNoiseFigure
		 **/
		SCReceiver(const wns::pyconfig::View& config,
				   Station* s,
				   antenna::Antenna* a,
				   Demodulator *demodulator,
				   Decoder *decoder,
				   wns::Ratio rnf);

		//! Default destructor
		virtual ~SCReceiver();
		virtual void startReceiving();

		//! stop listening to a specific PhysicalResource
		/**
		   Sets receiving to false. The Receiver is detached from a
		   PhysicalResource (via PhysicalResource::detachReceiver()). If the
		   Receiver has no PhysicalResource, the function will fail. If the
		   Receiver was not listening on a PhysicalResource the function will
		   do nothing.
		*/
		virtual void stopReceiving();

		//! Called by the PhysicalResource to notify the Receiver of a change
		/**
		   This method is part of publish-subscriber design pattern. The
		   PhysicalResource will call this function of every Receiver attached
		   to itsself,if a change on the PhysicalResource has happend (e.g, a
		   Transmitter started a Transmission).
		   The Receiver may then decide what to do.
		*/
		virtual void notify(TransmissionObjectPtr transmissionObject);

		//! Returns the total received signal power of the PhysicalResource the
		//  Receiver is listening to.
		/** NOTE: This is a very simple implementation. It assumes your receiver
			is working on the whole bandwidth of the PhysicalResource. If you
			need something sophisticated please go ahead and implement something
			on your own (that's why the method is virtual).*/
		virtual wns::Power getTotalRxPower();

		/**
		 * @brief Returns the interference for this TransmissionObject.
		 *
		 * The interference is here defined as: the total received signal power
		 * - the carrier (getTotalRxPower()-getRxPower(TransmissionObject))
		 */
		virtual wns::Power getInterference(const TransmissionObjectPtr& t);

		//! Returns the received signal power of this transmission
		virtual wns::Power getRxPower(const TransmissionObjectPtr& t);

		//! return receiver noise power
		virtual wns::Power getNoise();

		virtual wns::Ratio getLoss(Transmitter* t);

		virtual void tune(double f, double b);

		/**
		 * @brief Returns true if TransmissionObject is active.
		 *
		 * Function checks if the list 'TransmissionObjects' contains
		 * the given TransmissionObject.
		 */
		virtual bool contains(TransmissionObjectPtr);

		virtual double getFrequency();
	};


	template<typename DerivedReceiver>
	class ReceiverWithDispatcher : public SCReceiver {
		typedef wns::DoubleDispatcher<DerivedReceiver, const TransmissionObjectPtr&> D;
	public:
		ReceiverWithDispatcher(const wns::pyconfig::View& config,
							   Station* s,
							   antenna::Antenna* a,
							   Demodulator *demodulator,
							   Decoder *decoder,
							   wns::Ratio rnf)
			: SCReceiver(config, s, a, demodulator, decoder, rnf)
			{
				if(!instantiated) {
					DerivedReceiver::initDispatcher();
					instantiated = true;
				}
			}
	protected:
		static D* getDispatcher() {
			static D d;
			return &d;
		}

		static bool instantiated;
	};
	template <typename DerivedReceiver> bool ReceiverWithDispatcher<DerivedReceiver>::instantiated = false;

}
#endif


