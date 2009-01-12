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

#ifndef __ANTENNASTATIC_HPP
#define __ANTENNASTATIC_HPP

#include <RISE/antenna/Antenna.hpp>
#include <WNS/Direction.hpp>

namespace rise { namespace antenna { namespace pattern { namespace kernel {
	class Kernel;
}}}}

namespace rise { namespace antenna {
	class Static :
		public Antenna
	{
	public:
		Static(const wns::pyconfig::View& pyConfigView,
			   Station* const station);

		~Static();

		virtual wns::Ratio getGain(const wns::Position& pos,
								   const PatternPtr) const;

		virtual wns::Ratio getGain(const wns::Position& pos) const;

	private:
		wns::Direction direction;
		pattern::kernel::Kernel* kernel;

	};
}}
#endif //__ANTENNASTATIC_HPP

