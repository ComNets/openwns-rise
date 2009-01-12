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

#include <RISE/antenna/pattern/twodimensional/OmniDirectional7.hpp>

using namespace rise::antenna::pattern::twodimensional;

OmniDirectional7::OmniDirectional7()
{
	pd_startUp();
}

OmniDirectional7::~OmniDirectional7()
{}

OmniDirectional7* OmniDirectional7::myself = 0;

void OmniDirectional7::pd_startUp()
{
	type = "Internal";
	pattern = "Omnidirectional7";
	pd_gain = 7;
	pd_elevationVector = Double64Vector (2);
	pd_azimuthVector = Double64Vector (2);

	pd_elevationVector[0] = 0;
	pd_azimuthVector[0] = 0;
	pd_elevationVector[1] = 0;
	pd_azimuthVector[1] = 0;
	TwoDimensional::pd_startUp();
}


