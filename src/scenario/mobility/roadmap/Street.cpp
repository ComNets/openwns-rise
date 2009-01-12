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

#include <sstream>

#include <RISE/scenario/mobility/roadmap/Street.hpp>

#include <WNS/Position.hpp>
#include <WNS/Assure.hpp>


using namespace rise::scenario::mobility::roadmap;
using namespace std;

Street::Street(const wns::pyconfig::View& streetView, Map* _map) :
	MapObject(streetView.get<MapObject::ID>("ID"), _map),
	endPoints(),
	connections(),
	vMax(0.0),
	streetVector()
{
	int nPoints = streetView.getSequence("endPoints").size();
	assure(nPoints==2, "Street object should defined by exactly 2 Points!");
	for (int i=0; i<nPoints; ++i)
	{
		std::stringstream index;
		index << i;
		endPoints.push_back(wns::Position(streetView.getView("endPoints["+index.str()+"]")));
	}
	int nConnections = streetView.getSequence("connections").size();
	assure(nConnections==2, "Street always has to be connected to 2 other MapObjects!");
	for (int i=0; i<nConnections; ++i)
		connections.push_back(streetView.getSequence("connections").at<MapObject::ID>(i));

	// in m/s
	vMax = streetView.get<double>("vMax");

	streetVector = endPoints.at(1)-endPoints.at(0);
}

wns::Position
Street::getNextPosition(MapUser* user, simTimeType dt) const
{
	// in case we just entered this street, we have to determine the
	// user's heading. If we come from the crossing at endPoint[0], we
	// call it "Forward", otherwise we'll call it "Reverse"
	if (user->currentObject!=this->id)
	{
		MapObject::ID from = user->currentObject;
		user->heading = (  ( from==connections.at(0))
						   ? int(Headings::Forward())
						   : int(Headings::Reverse()) );
		user->currentObject=this->id;
	}

	// how much of this road remains?
	double remaining = (1-user->percent)*streetVector.abs();

	// is user velocity on this street bounded by vMax?
	double currentVelocity = ( user->velocity < vMax ? user->velocity : vMax );

	if (currentVelocity*dt < remaining)
	{
		// we stay on this road and only advance by a certain
		// percentage (given through velocity and timestep)
		user->percent += (currentVelocity*dt)/streetVector.abs();

		MESSAGE_BEGIN(NORMAL, log, m, "MapUser advancing on street ");
		m << id << ", completed " << int(100.0*user->percent) 
		  << "%, velocity " << currentVelocity << "(desired: " << user->velocity << ") m/s";
		MESSAGE_END();

		return this->getPosition(user);
	}
	else
	{
		// we reach the end of this street, how long does that take?
		double untilCrossing = remaining/currentVelocity;
		// determine the object we'll reach next
		MapObject* nextObject = map->getObject(( user->heading==Headings::Forward()
												 ? connections.at(1)
												 : connections.at(0) ));
		// and delegate the decision about the new final position
		return nextObject->getNextPosition(user,dt-untilCrossing);
	}
}

wns::Position
Street::getPosition(const MapUser* user) const
{
	assure(user->currentObject==this->id, "User/Street mismatch.");

	// where did we enter the road?
	wns::Position entryPoint =
		( user->heading == Headings::Forward() ? endPoints.at(0) : endPoints.at(1) );

	// what distance have we traveled on the road
	wns::PositionOffset userTravelled = streetVector *
		user->percent * user->heading;

	// this results in our current position
	wns::Position userPosition = entryPoint + userTravelled;

	return userPosition;

}

std::vector<MapObject::ID>
Street::getDependencies() const
{
	return connections;
}
