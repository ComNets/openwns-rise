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

#ifndef _POINTERHASHSET_HPP
#define _POINTERHASHSET_HPP

#include <functional>
#ifndef WNS_USE_STLPORT
#include <ext/hash_set>
#else
#include <hash_set>
#endif
#include <WNS/SmartPtr.hpp>

namespace rise {
	//! The hash function for pointers
	/** @ingroup MISC */
	template<class Key>
	class PointerHashSetFunctor :
#ifndef WNS_USE_STLPORT
		public __gnu_cxx::hash<Key const>
#else
		public std::hash<Key const>
#endif
	{
	public:
		ptrdiff_t operator()(Key const k) const
		{
			return ((ptrdiff_t)(k));
		};
	};

	//! specialization for SmartPtr
	template<class Key>
	class PointerHashSetFunctor<wns::SmartPtr<Key> > :
#ifndef WNS_USE_STLPORT
		public __gnu_cxx::hash<Key* const>
#else
		public std::hash<Key* const>
#endif
	{
	public:
		ptrdiff_t operator()(wns::SmartPtr<Key> const k) const
		{
			return ((ptrdiff_t)(k.get()));
		};
	};

	//! A hash_set that takes pointers/shared_pointers a key and any value
	template<class Key>
	class PointerHashSet :
#ifndef WNS_USE_STLPORT
		public __gnu_cxx::hash_set<Key,
#else
		public std::hash_set<Key,
#endif
								   PointerHashSetFunctor<Key>,
								   std::equal_to<Key> > {
	public:
		PointerHashSet() :
#ifndef WNS_USE_STLPORT
			__gnu_cxx::hash_set<Key,
#else
			std::hash_set<Key,
#endif
								PointerHashSetFunctor<Key>,
								std::equal_to<Key> >() {};

		virtual ~PointerHashSet() {};

		virtual bool containsKey(Key k) const
		{
			return this->find(k)!=this->end();
		};
	};

}
#endif


