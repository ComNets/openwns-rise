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

#include <RISE/scenario/mobility/roadmap/Map.hpp>
#include <RISE/scenario/mobility/roadmap/Street.hpp>
#include <RISE/scenario/mobility/roadmap/Crossing.hpp>
#include <RISE/scenario/mobility/Mobility.hpp>

#include <WNS/module/Base.hpp>
#include <WNS/pyconfig/View.hpp>

using namespace rise::scenario::mobility::roadmap;
using namespace std;

//! pyconfig::Parser constructor
Map::Map(const wns::pyconfig::View& roadmapView) :
	mapObjectContainer(),
	streetIDContainer(),
	streetDist(),
	headingDist(),
	entryPointDist(),
	log(roadmapView.getView("logger"))
{
	mapObjectContainer.resize(roadmapView.len("crossings")+roadmapView.len("streets"));
	// 1.) read street and crossing Objects from pyconfig::Parser
	for (int s=0; s<roadmapView.len("streets"); ++s)
	{
		Street* aStreet = new Street(roadmapView.getView("streets",s), this);
		if (aStreet->getID() >= mapObjectContainer.size())
			mapObjectContainer.resize(aStreet->getID()+1);
		mapObjectContainer.at(aStreet->getID()) = aStreet;
		streetIDContainer.push_back(aStreet->getID());
	}
	for (int c=0; c<roadmapView.len("crossings"); ++c)
	{
		Crossing* aCrossing = new Crossing(roadmapView.getView("crossings",c), this);
		if (aCrossing->getID() >= mapObjectContainer.size())
			mapObjectContainer.resize(aCrossing->getID()+1);
		mapObjectContainer.at(aCrossing->getID()) = aCrossing;
	}
	// 2.) check Consistency of Map structure
	this->checkConsistency();
	// 3.) initialize random distributions
	wns::rng::RNGen* rng = wns::simulator::getRegistry()->find<wns::rng::RNGen*>("MOBILITY-RNG");
	streetDist = 
        new wns::distribution::DiscreteUniform(0, streetIDContainer.size()-1, rng);
	/** @brief discrete distribution of initial heading on a street */
	headingDist = 
        new wns::distribution::DiscreteUniform(0, 1, rng);
	/** @brief discrete distribution of entrypoint on a street */
	entryPointDist = new wns::distribution::StandardUniform(rng);

}

//! Destructor
Map::~Map()
{
	delete streetDist;
	delete headingDist;
	delete entryPointDist;
}


void
Map::checkConsistency()
{
	MESSAGE_BEGIN(NORMAL, log, m, "Starting Roadmap Integrity Check: ");
	MESSAGE_END();
	int mocs = mapObjectContainer.size();
	size_t streetCounter=0;
	int crossingCounter=0;

	for (int i=0; i<mocs; ++i){
		// visit and inspect all mapobjects
		if (mapObjectContainer[i]){
			Street* s = dynamic_cast<Street*>(mapObjectContainer[i]);
			if (s){
				// found a street
				++streetCounter;
				continue;
			}
			Crossing* c = dynamic_cast<Crossing*>(mapObjectContainer[i]);
			if (c){
				// found a crossing
				++crossingCounter;
				continue;
			}
			// get the objects connected to the currently inspected ones
			std::vector<MapObject::ID> deps = mapObjectContainer[i]->getDependencies();
			for (unsigned int j=0; j<deps.size(); ++j){
				// assure that all objects we are connected with exist in the container
				assure( mapObjectContainer.at(deps[i]), "object has undefined references!");
			}
		}
	}

	// another safety check to make sure we have all streets
	assure(streetCounter == streetIDContainer.size(), "Error after street counting!");

	MESSAGE_BEGIN(NORMAL, log, m, "Successfully passed Roadmap integrity check.");
	m << "nStreets: " << streetCounter << " nCrossings: " << crossingCounter;
	MESSAGE_END();
}

/*! This method assigns a user type, initial coordinates and an
  initial speed to a mobile user. Used Parameters: aMsId,
  aMobUser, msXCoord, msYCoord, msZCoord, velocity, direction */
MapUser*
Map::createNewUser(double velocity)
{
	// randomly select a street
	MapObject::ID aStreet = streetIDContainer.at(int((*streetDist)()));
	MapUser::Heading aHeading;
	// randomly select a heading, the distribution returns 0 or 1
	if ( int((*headingDist)()) ){
		aHeading = Headings::Forward();
	} else {
		aHeading = Headings::Reverse();
	}
	// randomly select an entryPoint into the street
	double aPercentage = (*entryPointDist)();

	MapUser* newUser = new MapUser(aStreet, aPercentage, aHeading, velocity);

	MESSAGE_BEGIN(NORMAL, log, m, "MapUser created at ");
	m << getPosition(newUser) << " with velocity "
	  << velocity << " m/s, heading "
	  << (aHeading==Headings::Forward() ? "forward, " : "reverse, ")
	  << int(100.0*aPercentage) << "% into street " << aStreet;
	MESSAGE_END();

	return newUser;
}



/*! This method performs a location update of a mobile user
  depending on his previous coordinates and his previous speed.
*/
wns::Position
Map::getNextPosition(MapUser* user, simTimeType dt)
{
	wns::Position newPosition = getObject(user->currentObject)->getNextPosition(user,dt);
	MESSAGE_BEGIN(NORMAL, log, m, "User moved to ");
	m << newPosition
	  << " with velocity " << user->velocity << " m/s, "
	  << int(100.0*user->percent) << "% into street " << user->currentObject;
	MESSAGE_END();
	return newPosition;
}

wns::Position
Map::getPosition(MapUser* user)
{
	return getObject(user->currentObject)->getPosition(user);
}

/** @brief returns the MapObject (Street or Crossing) with the given ID
 */
MapObject*
Map::getObject(const MapObject::ID id)
{
	assure(mapObjectContainer.at(id), "You searched for a non-existing MapObject!");
	return mapObjectContainer.at(id);
}





