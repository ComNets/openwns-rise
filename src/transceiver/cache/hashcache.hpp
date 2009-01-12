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

#ifndef __HASHCACHE_HPP
#define __HASHCACHE_HPP

#include <RISE/transceiver/cache/propagationcache.hpp>
#include <RISE/misc/pointerhashmap.hpp>
#include <RISE/transceiver/cache/propcacheentry.hpp>

#include <WNS/Position.hpp>

namespace rise {
	class Station;
	class Antenna;
	class Scenario;
	class Transmitter;
	class ReceiverBase;

    //! Interface of class HashCache
    /** The HashCache stores the loss between its receiver and all 
	 *  active transmitters for different frequency in a 2-dim hash.
	 */
	class HashCache : public PropagationCache{
	public:
		//! Default Constructor
		explicit HashCache(ReceiverBase* r);
		//! Destructor
		virtual ~HashCache();
        /**
		 * Returns loss dependent on pathloss, shadowing and
		 * antennaGain twoards antenna txA. Update propagation
		 * condition for this antenna in cache.
		 */
		wns::Ratio getLoss(Transmitter* t, double freq);
		/**
		 * Set propagation conditions in cache for 't' invalid.
		 */
		void invalidatePropagationEntries(Transmitter* t);
		/**
		 * For all Transmitter set propagation conditions in cache invalid.
		 */
		void invalidatePropagationEntries();


	private:
		typedef FreqHash<PropCacheEntry> Level2Hash;
		typedef PointerHashMap<Transmitter*, Level2Hash> Level1Hash;
		typedef Level2Hash::iterator FrequencyIterator;
		typedef Level1Hash::iterator TransmitterIterator;

		/**
		 * 3-dim hash that stores for each triple of Transmitter, Receiver and
		 * Frequency the values of their pathloss, shadowing and antennaGain
		 */
		Level1Hash propHash;
	};
}

#endif //__HASHCACHE_HPP


