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

#include <RISE/scenario/mobility/roadmap/Crossing.hpp>
#include <RISE/scenario/mobility/roadmap/Map.hpp>
#include <RISE/scenario/mobility/Mobility.hpp>

#include <WNS/pyconfig/Sequence.hpp>
#include <WNS/distribution/Uniform.hpp>
#include <WNS/Assure.hpp>

using namespace rise::scenario::mobility::roadmap;
using namespace std;

matrix rise::scenario::mobility::roadmap::getMatrix(const wns::pyconfig::View& aView, const std::string& aName)
{
	matrix result;
	for (int i=0; i<aView.len(aName); ++i)
	{
		std::stringstream index;
		index << i;

		std::string subviewName = aName +
			std::string("[") + index.str() + std::string("]");

		std::vector<double> subVector;
		for (int j=0; j<aView.len(subviewName); ++j){
			subVector.push_back(aView.get<double>(subviewName,j));
		}
		result.push_back(subVector);
	}
	return result;
}

Crossing::Crossing(const wns::pyconfig::View& crossingView, Map* _map) :
	MapObject(crossingView.get<MapObject::ID>("ID"), _map),
	position(crossingView.getView("position")),
	connections(),
	probMatrix(getMatrix(crossingView,std::string("probMatrix"))),
	uniDis(NULL)
{
	// int connection list
	for (int i=0; i<crossingView.len("connections"); ++i)
		connections.push_back(crossingView.get<MapObject::ID>("connections",i));

	// check consistency of probability Matrix
	assure(probMatrix.size()==connections.size(), "Mismatch in connections and probability Matrix.");
	for (unsigned int i=0; i<probMatrix.size(); ++i)
		assure(probMatrix[i].size()==connections.size(), "Mismatch in connections and probability Matrix.");

	// init the distribution function to determine where to turn
    wns::rng::RNGen* rng = wns::simulator::getRegistry()->find<wns::rng::RNGen*>("MOBILITY-RNG");
    uniDis = new wns::distribution::StandardUniform(rng);
}

/** @brief when asked by another object for a new position */
wns::Position Crossing::getNextPosition(MapUser* user, simTimeType dt) const
{
	// where do we come from?
	MapObject::ID from = user->currentObject;
	unsigned int index;
	// determine index of object we come from
	for (index=0 ; index<connections.size(); ++index)
		if (connections.at(index)==from) break;
	// store relevant probability vector handy
	std::vector<double> probVector = probMatrix.at(index);
	// Choose next connected Object
	double randomNumber = (*uniDis)();
	unsigned int j = 0;
	double aWeight = probVector.at(0);
	for ( ; j < probVector.size()-1; ++j)
	{
		if (randomNumber<aWeight)
			break;
		aWeight += probVector.at(j+1);
	}
	// now j should be the index of the next object.
	MapObject* next = this->map->getObject(connections.at(j));
	user->currentObject = this->id; // my own ID
	user->percent = 0.0; // entering a new street
	user->heading = Headings::Forward(); // the next street will re-set this
	// velocity remains unchanged

	MESSAGE_BEGIN(NORMAL, log, m, "MapUser arrived");
	m << " from street " << from 
	  << " at Crossing " << id 
	  << " continuing on street " << connections.at(j);
	MESSAGE_END();

	// delegate the final decision to the next street.
	return next->getNextPosition(user, dt);
}

/** @brief returns current position of user */
#ifndef WNS_NDEBUG
wns::Position Crossing::getPosition(const MapUser* user) const
#else
wns::Position Crossing::getPosition(const MapUser* /*user*/) const
#endif // NOT defined WNS_NDEBUG
{
	assure(user->currentObject==this->id, "User/Crossing mismatch.");
	return position;
}

/** @brief used for consistency checking */
std::vector<MapObject::ID> Crossing::getDependencies() const
{
	return connections;
}
