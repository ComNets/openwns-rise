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

#ifndef _DIRECTED_90_HPP
#define _DIRECTED_90_HPP

#include <RISE/antenna/pattern/kernel/TwoDimensional.hpp>

namespace rise { namespace antenna { namespace pattern { namespace twodimensional {
	class Directed90 :
		public kernel::TwoDimensional
	{
	public:
		inline static  Directed90* getInstance() {
			if(!myself)
				myself = new Directed90();
			return myself;
		}
		inline static void deleteInstance() {
			delete(myself);
		}
	protected:
		Directed90();
		~Directed90();
		virtual void pd_startUp();
	private:
		static Directed90 *myself;
	};
}}}}
#endif


