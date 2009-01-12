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

#include <RISE/scenario/mobility/None.hpp>

using namespace rise::scenario::mobility;

STATIC_FACTORY_REGISTER_WITH_CREATOR(
	None,
	MobilityInterface,
	"rise.mobility.None",
	wns::PyConfigViewCreator);


None::None(const wns::pyconfig::View& mobilityView) :
	Mobility(mobilityView)
{
}


void
None::move()
{
}


void
None::moveTo(const wns::Position& pos)
{
	setPosition(pos);
}


void
None::moveTo(const wns::PositionOffset& pos)
{
	setPosition(getPosition()+pos);
}


void
None::periodically()
{
}



