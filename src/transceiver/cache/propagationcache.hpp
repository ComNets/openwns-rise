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

#ifndef __PROPCACHE_HPP
#define __PROPCACHE_HPP

#include <RISE/misc/RISELogger.hpp>
#include <WNS/PowerRatio.hpp>
#include <WNS/Position.hpp>
#include <string>
#include <algorithm>

#include <boost/version.hpp>

// From Boost 1.38 on the unordered_map container is present
// at the same time hash_map becomes deprecated in GNU C++
#if BOOST_VERSION < 103800
    #define WNS_USE_OLD_HASH_MAP
    #include <ext/hash_map>
#else
    #include <boost/unordered_map.hpp>
#endif

namespace rise {
    class PropCacheEntry;
    class Transmitter;
	namespace receiver {
		class ReceiverInterface;
	}

    /**
     * @brief Functor as hash function to the FreqHash
     */
    class FreqHashFunctor :
#ifdef WNS_USE_OLD_HASH_MAP
		public __gnu_cxx::hash<double const>
#else
		public boost::hash<double const>
#endif
    {
    public:
		size_t operator()(double const k) const {
			return ((size_t)(k));
		}
    };

    /**
     * @brief Hash to store objects with a frequency (double) as key
     */
    template<class Value>
    class FreqHash :
#ifdef WNS_USE_OLD_HASH_MAP
		public __gnu_cxx::hash_map<double const,
#else
		public boost::unordered_map<double const,
#endif
							 Value,
							 FreqHashFunctor,
							 std::equal_to<double> >
	{};

    /**
     * @brief Abstract class (interface) for caches to be use by
     * each Receiver.
     *
     * There are various strategies for caching available. 
	 * Consequently this abstract class provides an interface that all
     * derived caches have to implement.
     *
     * Three functions have to be reimplemented:
     * - getLoss(Antenna* a1,  double freq)
     * - invalidatePropEntries(Transmitter* t)
     * - invalidatePropEntries()
     * For the purpose of this functions please read the detailed description of
     * the functions below.
     */
    class PropagationCache {
    public:
		/**
		 * @brief Default constructor
		 */
		PropagationCache(receiver::ReceiverInterface* r, const std::string& name);

		/**
		 * @brief Default destructor
		 */
		virtual ~PropagationCache();

		/**
		 * @brief Computes the loss from the receivers antenna towards 
		 * antenna "txA" at a specific frequency
		 *
		 * This method is called by the Receiver and computes the loss
		 * towards antenna "txA" at a specific frequency.
		 *
		 * Different ways to implement this method are possible:
		 * @li Just return the value. Updating and adding new values is done by
		 * invalidatePropagationEntries()
		 * @li Check if a PropagationEntry is valid any longer, if not recompute
		 * and store the value. Invalidating the cache is done by
		 * invalidatePropagationEntries
		 */
		virtual wns::Ratio getLoss(Transmitter* txA, double freq)=0;

		/**
		 * @brief Invalidates the propagation entries for Transmitter "t"
		 */
		virtual void invalidatePropagationEntries(Transmitter* t)=0;

		/**
		 * @brief Invalidates the propagation entries for
		 * all Transmitters
		 */
		virtual void invalidatePropagationEntries()=0;
	protected:
		/**
		 * @brief Computes the loss and stores it in the given PSG object.
		 * In the given PSG object the attributes pathloss, shadowing and
		 * antennaGain will be updated.
		 */
		virtual void updatePropEntry(PropCacheEntry& cacheEntry,
									 Transmitter* t,
									 double freq);
		/**
		 * @brief Receiver the cache belongs to.
		 */
		receiver::ReceiverInterface* receiver;
		/**
		 * @brief Logs messages to a main logger.
		 */
		RISELogger log;
	};
}

#endif // __PROPCACHE_HPP


