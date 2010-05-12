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

#ifndef __H2CACHE_HPP
#define __H2CACHE_HPP

#include <WNS/container/FastList.hpp>
#include <RISE/transceiver/cache/propagationcache.hpp>
#include <WNS/PowerRatio.hpp>
#include <RISE/transceiver/transmitter.hpp>

namespace rise {
    //! Interface of class H2Cache
    /** The H2Cache stores the loss between its receiver and the
	 *  transmitters in a vector sorted by the ids of the transmitters.
	 */
	class H2Cache : public PropagationCache {
    public:
		//! Default Constructor
		H2Cache(receiver::ReceiverInterface* r);
		//! Destructor
		virtual ~H2Cache();

		/**
		 * @brief Retruns the momentary loss towards antenna txA
		 * at a specific frequency
		 */
		wns::Ratio getLoss(Transmitter* txA, double frequency);

        /**
		 * @brief Invalidates the propagation entries for Transmitter "t"
		 *
		 */
		void invalidatePropagationEntries(Transmitter* t);
		/**
		 * @brief Invalidates the propagation entries for all Transmitters
		 */
		void invalidatePropagationEntries();

		//! Attachs transmitter "t" to the cache
		void attach(Transmitter* t, unsigned long int tId);

    private:
		typedef wns::container::FastList<Transmitter*> TransmitterContainer;
		typedef TransmitterContainer::iterator TransmitterIterator;

		typedef std::vector<PropCacheEntry> CacheVector;

        //! Cache that stores the total loss to all active Transmitters
		/**
			Cache stores PropCacheEntry objects, which contains pathloss,
			shadowing, antennaGain and computes the total loss.
		 */
		CacheVector pathlossShadowGain;
        //! List of all active transmitters
		TransmitterContainer transmitters;
	};
}

#endif


