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

#ifndef __IDVECTORCACHE_HPP
#define __IDVECTORCACHE_HPP

#include <RISE/transceiver/cache/propagationcache.hpp>

#include <WNS/container/FastList.hpp>
#include <WNS/PowerRatio.hpp>

#include <vector>
#include <stdint.h>

namespace rise {
	class PropCacheEntry;
	//! Interface of class IdVectorCache
    /** The IdVectorCache stores the loss between its receiver and all active
	 *  transmitters in a vector sorted by the ids of each transmitter.
	*/
	class IdVectorCache : public PropagationCache {
    public:
		//! Default Constructor
		IdVectorCache(receiver::ReceiverInterface* r);
		//! Destructor
		virtual ~IdVectorCache();

		/**
		 * @brief Returns the loss towards antenna 'txA' at a
		 * specific frequency.
		 */
		wns::Ratio getLoss(Transmitter* t, double frequency);

        /**
		 * @brief Invalidates the propagation entries for Transmitter "t"
		 * towards all Receivers
		 */
		void invalidatePropagationEntries(Transmitter* t);
		/**
		 * @brief Invalidates the propagation entries for Transmitters
		 */
		void invalidatePropagationEntries();

    private:
		typedef wns::container::FastList<Transmitter*> TransmitterContainer;
		typedef TransmitterContainer::iterator TransmitterIterator;

		//! Struct that stores a list of PropCacheEntry objects
		typedef std::vector<PropCacheEntry> CacheVector;
		//! Cache that stores the total loss towards all active Transmitters

		/**
		 *  Cache stores PropCacheEntry objects, which contains pathloss,
		 *  shadowing, antennaGain and computes the total loss.
		 */
		CacheVector pathlossShadowGain;
		//! List of all active transmitters
		TransmitterContainer transmitters;
		/**
		 * @brief Attaches transmitter to cache using its id
		 */
		void attach(Transmitter* t, uint32_t tId);
    };
}

#endif


