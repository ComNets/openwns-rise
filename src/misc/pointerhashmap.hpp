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

#ifndef _POINTERHASHMAP_HPP
#define _POINTERHASHMAP_HPP
#include <WNS/SmartPtr.hpp>
#include <functional>
#include <boost/unordered_map.hpp>

namespace rise {
	//! The hash function for pointers
	/** @ingroup MISC */
	template<class Key>
	class PointerHashMapFunctor :
		public boost::hash<Key const>
	{
	public:
		ptrdiff_t operator()(Key const k) const {
			return ((ptrdiff_t)(k));
		};
	};

	//! specialization for SmartPtr
	/** @ingroup MISC */
	template<class Key>
	class PointerHashMapFunctor<wns::SmartPtr<Key> > :
		public boost::hash<Key* const>
	{
	public:
		ptrdiff_t operator()(wns::SmartPtr<Key> const k) const {
			return ((ptrdiff_t)(k.get()));
		};
	};

	//! A hash_map that takes pointers/shared_pointers a key and any value
	/** @ingroup MISC */
	template<class Key, class Value>
	class PointerHashMap :
		public boost::unordered_map<Key const,
								   Value,
								   PointerHashMapFunctor<Key>,
								   std::equal_to<Key> > {
	public:
		PointerHashMap() :
			boost::unordered_map<Key const,
								Value,
								PointerHashMapFunctor<Key>,
								std::equal_to<Key> >() {};

		virtual ~PointerHashMap() {};

		virtual bool contains(Key k) const {
			return find(k)!=this->end();
		};
};

}
#endif


