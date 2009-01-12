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

#ifndef _OMNIDIRECTIONAL9_HPP
#define _OMNIDIRECTIONAL9_HPP

#include <RISE/antenna/pattern/kernel/TwoDimensional.hpp>

namespace rise { namespace antenna { namespace pattern { namespace twodimensional {
	class OmniDirectional9 :
		public kernel::TwoDimensional
	{
	public:
		/**
		 * @brief Omni directional antenna pattern
		 *
		 * Omni directional antenna pattern has the same gain in all azimuth
		 * angles. Over elevation angle it changes, though: the highest gain is
		 * in the horizontal plane, and the lowest in vertical. Omni directional
		 * antenna has a gain of ~5dB for elevation angle 90�.
		 * This is in contrast to isotropic antenna which has the gain of zero
		 * in all the directions.
		 *
		 */
		inline static  OmniDirectional9* getInstance() {
			if(!myself)
				myself = new OmniDirectional9();
			return myself;
		};
		inline static void deleteInstance() {
			delete(myself);
		};
	protected:
		OmniDirectional9();
		~OmniDirectional9();
		virtual void pd_startUp();
	private:
		static OmniDirectional9 *myself;
	};
}}}}
#endif // _OMNIDIRECTIONAL9_HPP


