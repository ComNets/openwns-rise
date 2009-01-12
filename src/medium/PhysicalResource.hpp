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

#ifndef _PHYSICALRESOURCE_HPP
#define _PHYSICALRESOURCE_HPP

#include <RISE/medium/PhysicalResourceObserver.hpp>
#include <RISE/transmissionobjects/transmissionobject.hpp>
#include <RISE/misc/pointer.hpp>
#include <RISE/misc/RISELogger.hpp>

#include <WNS/Functor.hpp>
#include <WNS/container/FastList.hpp>
#include <WNS/Interval.hpp>
#include <WNS/PowerRatio.hpp>

#ifndef WNS_USE_STLPORT
#include <ext/slist>
#else
#include <slist>
#endif

namespace rise {
	class PhysicalResourceObserver;
}

namespace rise { namespace medium {
	/**
	 * @brief Distributes TransmissionObjects send by Transmitters to the
	 * attached Receivers
	 * @ingroup MEDIUM
	 *
	 * A PhysicalResource(Subject) may have a number of
	 * Receiver(Observer). This PhysicalResource holds all Receiver in a
	 * list. An Receiver(Observer) may attach and detach to a
	 * PhysicalResource(Subject) with attach(Receiver *r) and detach(Receiver
	 * *r). By calling notifyObservers(), the Physicalresource will call
	 * Receiver::notify() of every Receiver.
	 *
	 * All transmitter and receiver communicate via this class. DO NOT create
	 * the PhysicalResource yourself, ask Medium to create one for you with
	 * Medium::getInstance()->getPhysicalResource().
	 *
	 * The PhysicalResource is a Subject as in the Observer pattern as proposed
	 * by "Gamma".
	 */
	class PhysicalResource {
		friend class Gather80211StationData;
		// Medium manages PhysicalResources
		friend class Medium;

		/**
		 * @brief Container for Receiver
		 */
#ifndef WNS_USE_STLPORT
		typedef __gnu_cxx::slist<PhysicalResourceObserver*> ReceiverContainer;
#else
		typedef std::slist<PhysicalResourceObserver*> ReceiverContainer;
#endif
		/**
		 * @brief Iterator to container for Receiver
		 */
		typedef ReceiverContainer::const_iterator ReceiverIterator;

		/**
		 * @brief Container for pointer to TransmissionObjects
		 */
		typedef wns::container::FastList<TransmissionObjectPtr> TransmissionObjectContainer;

		/**
		 * @brief Interval of the frequency range
		 */
		typedef wns::Interval<double> Range;

	public:
		/**
		 * @brief Iterator to container for pointer to TransmissionObjects
		 *
		 * Needs to be public since the iterator can be acces from outside this
		 * class thru the methods getTOBegin() and getTOEnd()
		 */
		typedef wns::container::FastList<TransmissionObjectPtr>::iterator TransmissionObjectIterator;

		/**
		 * @brief Start the transmission of a TransmissionObject on this
		 * PhysicalResource and notify all attached Receivers
		 */
		void startTransmission(TransmissionObjectPtr transmissionObject)
		{
			transmissionObject->setPhysicalResource(this);
			transmissionObject->setIsStart(true);
			notifyReceivers(transmissionObject);
			transmissionObjects.push_front(transmissionObject);
		};

		/**
		 * @brief Stop the transmission of a TransmissionObject on this
		 * PhysicalResource and notify all attached Receivers
		 */
		void stopTransmission(TransmissionObjectPtr transmissionObject)
		{
			transmissionObject->setIsStart(false);
			notifyReceivers(transmissionObject);
			transmissionObjects.remove(transmissionObject);
		};

		/**
		 * @brief Add receiver (observer) to the PhysicalResource
		 *
		 * This will add the Receiver to a list of observers (that will be
		 * notified on changes on the PhysicalResource) and trigger stuff in the
		 * PropagationEngine to ensure this Receiver is available in the
		 * PropagataionCache
		 */
		void attach(PhysicalResourceObserver *r);

		/**
		 * @brief Remove receiver (observer) from the PhysicalResource
		 *
		 * This will add the Receiver to a list of observers (that will be
		 * notified on changes on the PhysicalResource) and trigger stuff in the
		 * PropagationEngine to ensure this Receiver is available in the
		 * PropagataionCache
		 */
		void detach(PhysicalResourceObserver *r);

		/**
		 * @brief Compare two PhysicalResources
		 *
		 * Two PhysicalResource are the same if the mid-frequency and the
		 * bandwidth is the same (mainly, needed for Medium)
		 */
		bool operator ==(const PhysicalResource &p) const
		{
			return (frequencyRange == p.frequencyRange);
		}

		bool operator !=(const PhysicalResource &p) const
		{
			return !(*this==p);
		}

		/**
		 * @brief get an iterator to the first TransmissionObject
		 */
 		TransmissionObjectIterator getTOBegin() const
		{
 			return transmissionObjects.begin();
 		}

		/**
		 * @brief get an iterator to the last+1 TransmissionObject (sentinel)
		 */
 		TransmissionObjectIterator getTOEnd() const
		{
 			return transmissionObjects.end();
 		}

		/**
		 * @brief Returns true if the TransmissionObject is currently being
		 * transmitted on the PhysicalResource, false otherwise.
		 */
  		bool contains(TransmissionObjectPtr t)
		{
 			return transmissionObjects.contains(t);
 		}

		/**
		 * @brief Returns the center=mid-frequency
		 */
		double getFrequency() const
		{
			return frequency;
		}

		/**
		 * @brief Returns the bandwidth
		 */
		double getBandwidth() const
		{
			return bandwidth;
		}

		/**
		 * @brief Returns interval of the frequency range
		 */
		Range getFrequencyRange()
		{
			return frequencyRange;
		}

        /**
         * @brief Calls the receivers to update their cache for a Transmitter
         */
		void mobilityUpdate(Transmitter* transmitter)
		{
			MESSAGE_BEGIN(NORMAL, log, m, "Calling Mobility update for ");
			m << receivers.size() << " receivers on PhysicalResource";
			MESSAGE_END();
			std::for_each(
				receivers.begin(),
				receivers.end(),
				std::bind2nd(
					std::mem_fun(
						&PhysicalResourceObserver::mobilityUpdate),
					transmitter));
 		};

	private:
		/**
		 * @brief Default constructor
		 *
		 * May only be called by Medium
		 */
		PhysicalResource(double f, double b);

		/**
		 * @brief Default destructor
		 *
		 * May only be called by Medium
		 */
		virtual ~PhysicalResource();

		/**
		 * @brief disallow default constructor
		 */
		PhysicalResource();

  		/**
		 * @brief notify the receivers of started/stopped transmission
		 */
 		void notifyReceivers(const TransmissionObjectPtr& transmissionObject) const
		{
 			MESSAGE_BEGIN(NORMAL, log, m, "NotifyReceivers (start=");
 			m << transmissionObject->getIsStart()<< ")";
 			MESSAGE_END();
			std::for_each(
				receivers.begin(),
				receivers.end(),
				std::bind2nd(
					std::mem_fun(
						&PhysicalResourceObserver::notify),
					transmissionObject));
			// this calls Receiver::notify(TransmissionObjectPtr t)
			// in class Receiver derived from PhysicalResourceObserver
 		};

		/**
		 * @brief (Center) Frequency of this PhysicalResource
		 */
		double frequency;

		/**
		 * @brief Bandwidth of this PhysicalResource
		 */
		double bandwidth;

        /**
		 * @brief Interval of the frequency range
		 */
		Range frequencyRange;

		/**
		 * @brief List of Receivers (observers) attached to this
		 * PhysicalResource (subject)
		 */
		ReceiverContainer receivers;

		/**
		 * @brief List of all active TransmissionObjects
		 */
		TransmissionObjectContainer transmissionObjects;

		/**
		 * @brief Sends message to the main logger
		 */
		RISELogger log;

		/**
		 * @brief Disallow Copy-Constructor
		 */
		PhysicalResource(const PhysicalResource&);

		/**
		 * @brief Disallow assignment operator
		 */
		PhysicalResource& operator=(const PhysicalResource&);

	};
}}
#endif


