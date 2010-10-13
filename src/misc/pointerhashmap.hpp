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
	//! The hash function for pointers
	/** @ingroup MISC */
	template<class Key>
	class PointerHashMapFunctor :
#ifdef WNS_USE_OLD_HASH_MAP
		public __gnu_cxx::hash<Key const>
#else
		public boost::hash<Key const>
#endif
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
#ifdef WNS_USE_OLD_HASH_MAP
		public __gnu_cxx::hash<Key* const>
#else
		public boost::hash<Key* const>
#endif
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
#ifdef WNS_USE_OLD_HASH_MAP
        public __gnu_cxx::hash_map<Key const,
#else
		public boost::unordered_map<Key const,
#endif
								   Value,
								   PointerHashMapFunctor<Key>,
								   std::equal_to<Key> > {
	public:
		PointerHashMap() :
#ifdef WNS_USE_OLD_HASH_MAP
            __gnu_cxx::hash_map<Key const,
#else
    		boost::unordered_map<Key const,
#endif
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


