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

#ifndef __NOCACHE_HPP
#define __NOCACHE_HPP

#include <RISE/transceiver/cache/propagationcache.hpp>
#include <RISE/transceiver/cache/propcacheentry.hpp>
namespace rise {
	/**
	 * @brief Provides no caching at all
	 *
	 * Useful if you think, caching will lead to erroneous results (or to check
	 * if the other caches work correct ;))
	 */
//	class PropCacheEntry;
	class NoCache : public PropagationCache {
    public:
		/**
		 * @brief Default Constructor
		 */
		NoCache(receiver::ReceiverInterface* r);

		/**
		 * @brief Default Destructor
		 */
		virtual ~NoCache();

		/**
		 * @brief Computes the loss values on the fly
		 */
		virtual wns::Ratio getLoss(Transmitter* a2, double freq);

		/**
		 * @brief Empty
		 */
		virtual void invalidatePropagationEntries(Transmitter* t);

		/**
		 * @brief Empty
		 */
		virtual void invalidatePropagationEntries();
	private:
		PropCacheEntry propObject;
	};
}

#endif


