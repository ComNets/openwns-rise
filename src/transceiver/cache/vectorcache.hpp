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

#ifndef __VECTORCACHE_HPP
#define __VECTORCACHE_HPP

#include <list>
#include <vector>
#include <stdint.h>

#include <RISE/transceiver/cache/propagationcache.hpp>
#include <RISE/misc/pointerhashmap.hpp>

namespace rise {
	class PropCacheEntry;
	//! Interface of class VectorCache
    /** The VectorCache stores the loss between its receiver and all active 
	 *  transmitters in a vector sorted by an index which is set for 
	 *  each transmitter.
	*/
	class VectorCache : public PropagationCache {
	public:
		//! Default Constructor
		VectorCache(receiver::ReceiverInterface* r);
		//! Destructor
		virtual ~VectorCache();
        /**
		 * @brief Computes loss between the receiver and an antenna
		 * dependent on pathloss, shadowing and antennaGain.
		 */
		virtual wns::Ratio getLoss(Transmitter* txA, double freq);
		//! For the given transmitter update propagation conditions in cache
		virtual void invalidatePropagationEntries( Transmitter* t);
        //! For all transmitters update propagation conditions in cache
		virtual void invalidatePropagationEntries();
        //!Attaches given Transmitter to the list of active Transmitters
 		void attach(Transmitter* t);

	private:
		//! Struct to store a list of Transmitters 
		typedef std::list<Transmitter*> TransmitterContainer;
		//! Iterator
		typedef TransmitterContainer::iterator TransmitterIterator;
		//! Struct to store a list of Frequencies
		typedef std::list<double> FrequencyContainer;
		//! Iterator
		typedef FrequencyContainer::iterator FrequencyIterator;
		//! Vector that stores a list of PropCacheEntry object
		typedef std::vector<PropCacheEntry> CacheVector;
		//! Hash that contains a CacheVector for each used Frequency
		typedef FreqHash<CacheVector> FrequencyHash;
		//! Stores an individual index for each active transmitter
		PointerHashMap<Transmitter*, int32_t> transmitter2Index;
        //! Cache that stores the total loss towards all active Transmitters
		/** 
		 *  Cache stores PropCacheEntry objects, which contains pathloss, shadowing,
		 *  antennaGain and computes the total loss.
		 */
		FrequencyHash pathlossShadowGain;

        //! List of all active TransmissionObjects
		TransmitterContainer transmitters;
 		//! List of all used Frequencies
		FrequencyContainer frequencies;
		//! Returns an index for a transmitter
		/** If the transmitter is not active, it will be attached to the
		 *	cache and receive an individual index.
		 */  
		int getIndex(Transmitter* t);
		//! Expands the cache 
		void attachFrequency(double freq);
	};
}

#endif


