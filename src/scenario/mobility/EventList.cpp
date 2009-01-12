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

#include <RISE/scenario/mobility/EventList.hpp>

using namespace rise::scenario::mobility;

STATIC_FACTORY_REGISTER_WITH_CREATOR(
	EventList,
	MobilityInterface,
	"rise.mobility.EventList",
	wns::PyConfigViewCreator);

EventList::EventList(const wns::pyconfig::View& mobilityView) :
	Mobility(mobilityView)
{
	for (int ii=0; ii < mobilityView.len("events"); ++ii)
	{
		addEvent(mobilityView.get<wns::pyconfig::View>("events",ii));
	}
}

EventList::~EventList()
{
}

void
EventList::move()
{
}

void
EventList::moveTo(const wns::Position& pos)
{
	MESSAGE_BEGIN(NORMAL, log, m, "Setting new position to ");
	m << pos;
	MESSAGE_END();
	setPosition(pos);
}

void
EventList::moveTo(const wns::PositionOffset& pos)
{
	setPosition(getPosition()+pos);
}

void
EventList::periodically()
{
}

void
EventList::addEvent(const wns::pyconfig::View& eventView)
{
	wns::Position newPosition(eventView.get<wns::pyconfig::View>("position"));
	AbsolutePositionUpdateEvent e (this, newPosition);
	wns::simulator::getEventScheduler()->schedule(e, eventView.get<simTimeType>("time"));
	MESSAGE_BEGIN(NORMAL, log, m, "");
	m << eventView.get<simTimeType>("time");
	m << " " << newPosition;
	MESSAGE_END();
}



