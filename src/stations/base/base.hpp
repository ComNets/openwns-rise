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

#ifndef _BASE_HPP
#define _BASE_HPP

#include <RISE/stations/station.hpp>

namespace rise {
	class SystemManager;
	//! Generic base station object.*/
	/** \ingroup STATION */
	class Base : public Station
	{
	public:
		//! Default constructor
		Base(wns::pyconfig::View _pyConfigView);

		//! Destructor
		virtual ~Base()
		{
		};

		//! Id of this base station
 		virtual uint32_t getBaseId() = 0;

	};
}

#endif  // _BASE_HPP


