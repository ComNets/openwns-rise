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

#include <RISE/scenario/mobility/Brown.hpp>
#include <WNS/module/Base.hpp>
#include <WNS/distribution/Distribution.hpp>
#include <fstream>
#include <iostream>

using namespace rise;
using namespace rise::scenario::mobility;
using namespace std;

STATIC_FACTORY_REGISTER_WITH_CREATOR(
	BrownianRect,
	MobilityInterface,
	"rise.mobility.BrownianRect",
	wns::PyConfigViewCreator);

STATIC_FACTORY_REGISTER_WITH_CREATOR(
	BrownianCirc,
	MobilityInterface,
	"rise.mobility.BrownianCirc",
	wns::PyConfigViewCreator);

STATIC_FACTORY_REGISTER_WITH_CREATOR(
	BrownianEquiangularPolygon,
	MobilityInterface,
	"rise.mobility.BrownianEquiangularPolygon",
	wns::PyConfigViewCreator);

BrownianBase::BrownianBase(const wns::pyconfig::View& mobilityView) :
	Mobility(mobilityView),
	velocityVector()
{
	initRNGs();

    wns::rng::RNGen* rng = 
        wns::simulator::getRegistry()->find<wns::rng::RNGen*>("MOBILITY-RNG");

	wns::pyconfig::View velocityConfig(mobilityView, "userVelocityDist");

	std::string name = velocityConfig.get<std::string>("__plugin__");
	velocityDistribution =
		wns::distribution::RNGDistributionFactory::creator(name)->create(rng, velocityConfig);

	/** calculate velocity Vector from distributions of velocity and
	 * direction. Velocity is assumed to be specified in m/s.
	 * We only have 2D-mobility for now, therefore we set theta to
	 * 90.0 Deg (Pi/2) always
	 */
	velocityVector.setPolar((*velocityDistribution)() * moveTimeStep,
				(*angleDistribution)(),
				M_PI/2.0); 
	velocityVector.setDeltaZ(0.0);

	fetchObstructionList(mobilityView);

	MESSAGE_BEGIN(NORMAL, log, m, "Mobility Object created at ");
	m << getPosition() << " with velocity "
	  << velocityVector.getR()/moveTimeStep
	  << " m/s and direction " << velocityVector.getAzimuth()
	  << " considering " << obstructionList.size() << " obstructions.";
	MESSAGE_END();

	// trigger the periodic mobility updates
	startPeriodicTimeout(moveTimeStep, moveTimeStep);
}

void
BrownianBase::initRNGs()
{
    wns::rng::RNGen* rng = wns::simulator::getRegistry()->find<wns::rng::RNGen*>("MOBILITY-RNG");

	angleDistribution = new wns::distribution::Uniform(0.0, 2.0 * M_PI, rng);
	updateProbability = new wns::distribution::StandardUniform(rng);
	
}

BrownianBase::~BrownianBase()
{
    delete velocityDistribution;
    delete angleDistribution;
    delete updateProbability;
}

void
BrownianBase::newVelocityVector()
{
	// some abbreviations
	double myVelocity = velocityVector.getR()/moveTimeStep;
	double aVelocity = 0.0;

	// draw new direction
	velocityVector.setAzimuth((*angleDistribution)());

	// draw new velocity
	do
	{
		aVelocity = (myVelocity + (*velocityDistribution)()) / 2;
	} while (aVelocity <= 0.0);

	velocityVector.setR(aVelocity*moveTimeStep);

	MESSAGE_BEGIN(NORMAL, log, m, "Changed Velocity and Direction: ");
	m << " Velocity: " << velocityVector.getR()/moveTimeStep
		<< " Direction: " << velocityVector.getAzimuth();
	MESSAGE_END();
}

void
BrownianBase::move()
{
	// velocity-dependent random experiment for direction update
	if ((*updateProbability)()*moveTimeStep<0.0004*velocityVector.getR())
		newVelocityVector();

	// update position
	wns::Position newPosition = getPosition() + velocityVector;
	
	MESSAGE_BEGIN(NORMAL, log, m, "New Position?: ");
	m << "Old Position: " << getPosition()
	<< "New Position: " << newPosition;
	MESSAGE_END();
	
	while(!checkBounds(newPosition))
	{
		if ((*updateProbability)()*moveTimeStep<0.0004*velocityVector.getR())
			newVelocityVector();
		
		newPosition = getPosition() + velocityVector;
		
		MESSAGE_BEGIN(NORMAL, log, m, "New Position?: ");
		m << "Old Position: " << getPosition()
		<< "New Position: " << newPosition;
		MESSAGE_END();
	}
	
	// check if bounds are exceeded
// 	if ( !checkBounds(newPosition) ){
// 		// turn 180 degrees
// 		velocityVector.setAzimuth( velocityVector.getAzimuth() + M_PI );
// 		newPosition = getPosition() + velocityVector;
// 		assure( checkBounds(newPosition), "Oops. Can't go back where I came from!");
// 		//continue;
// 	}
	
	// bounds check ok, set new position
	setPosition(newPosition);
	
	fstream fileout;
	fileout.open("mobilityStationPositions", ios::out | ios::app);
	fileout << getPosition() << endl;
	fileout.close();

	MESSAGE_BEGIN(NORMAL, log, m, "Mobility update: ");
	m << "Position: " << getPosition()
	  << " Velocity: " << velocityVector.getR()/moveTimeStep
	  << " Distance: " << velocityVector.getR()
	  << " Direction: " << velocityVector.getAzimuth();
	MESSAGE_END();
}

void
BrownianBase::periodically()
{
	move();
}

void
BrownianBase::fetchObstructionList(const wns::pyconfig::View& config)
{
	obstructionList.clear();

	for( int ii = 0; ii < config.len("obstructionList"); ++ii)
	{
		wns::pyconfig::View objectView = config.getView("obstructionList", ii);

		assert(objectView.len("pointA") == 3);
		assert(objectView.len("pointB") == 3);

		wns::geometry::Point a(
			objectView.get<double>("pointA",0),
			objectView.get<double>("pointA",1),
			objectView.get<double>("pointA",2));

		wns::geometry::Point b(
			objectView.get<double>("pointB",0),
			objectView.get<double>("pointB",1),
			objectView.get<double>("pointB",2));

		obstructionList.push_front(wns::geometry::AxisParallelRectangle(a,b));
	}
}

/** checks whether the given point is within at least one of the known obstructions. */
bool
BrownianBase::isObstructed(const wns::Position& pos) const
{
	for (std::list<wns::geometry::AxisParallelRectangle>::const_iterator iter = obstructionList.begin();
	     iter != obstructionList.end();
	     ++iter)
	{
		if ((*iter).contains(pos)){
			// found one match --> the point is obstructed
			return true;
		}
	}
	// no match occured
	return false;
}

BrownianRect::BrownianRect(const wns::pyconfig::View& mobilityView)
	: BrownianBase(mobilityView),
	  xmin(mobilityView.get<double>("xmin")),
	  ymin(mobilityView.get<double>("ymin")),
	  xmax(mobilityView.get<double>("xmax")),
	  ymax(mobilityView.get<double>("ymax"))
{}

bool
BrownianRect::checkBounds(const wns::Position& pos) const
{
	// range check
	if ( (pos.getX() <= xmin) || (pos.getX() >= xmax) ) return false;
	if ( (pos.getY() <= ymin) || (pos.getY() >= ymax) ) return false;
	return !BrownianBase::isObstructed(pos);
}

BrownianCirc::BrownianCirc(const wns::pyconfig::View& mobilityView) :
	BrownianBase(mobilityView),
	center(mobilityView.getView("center")),
	maxDistance(mobilityView.get<double>("maxDistance")),
	nSectors(mobilityView.get<int>("nSectors"))
{}

bool
BrownianCirc::checkBounds(const wns::Position& pos) const
{
	// range check
	MESSAGE_BEGIN(NORMAL, log, m, "Check Bounds: ");
	m << "Angle: " << (pos-center).getAzimuth()
	<< " Center: " << center
	<< " SS position: " << pos 
	<< " Distance: " << (center-pos).getR();
	MESSAGE_END();
	
	wns::Position posWithZ;
	
	posWithZ.setX(pos.getX());
	posWithZ.setY(pos.getY());
	posWithZ.setZ(center.getZ());

	MESSAGE_BEGIN(NORMAL, log, m, "Check Bounds: ");
	m << "Angle: " << (posWithZ-center).getAzimuth()
			<< " Center: " << center
			<< " SS position: " << posWithZ 
			<< " Distance: " << (center-posWithZ).getR();
	MESSAGE_END();
	
	if(nSectors > 1)
	{
		if ( (center-posWithZ).getR() >= maxDistance || (posWithZ-center).getAzimuth() > M_PI/2 || (posWithZ-center).getAzimuth() < ((M_PI/2) - (2*M_PI/nSectors)) ) 
		{
			return false;
		}
	}
	else
	{
		if ( (center-posWithZ).getR() >= maxDistance ) 
		{
			return false;
		}
	}
	
	return !BrownianBase::isObstructed(pos);
}


BrownianEquiangularPolygon::BrownianEquiangularPolygon(const wns::pyconfig::View& mobilityView) :
	BrownianBase(mobilityView),
	center(mobilityView.getView("center")),
	maxDistance(mobilityView.get<double>("maxDistance")),
	corners(mobilityView.get<int>("corners")),
	angle(mobilityView.get<double>("angle"))
{}

bool
BrownianEquiangularPolygon::checkBounds(const wns::Position& pos) const
{
	const wns::Position& c = center;
	const wns::Position& d = pos;
	double ao = angle*2*M_PI/360;
	for (int ii=0;ii<corners;++ii)
	{
		wns::Position a = c+wns::PositionOffset(maxDistance*cos(ao+ ii   *M_PI*2/corners),maxDistance*sin(ao+ ii   *M_PI*2/corners),0);
		wns::Position b = c+wns::PositionOffset(maxDistance*cos(ao+(ii+1)*M_PI*2/corners),maxDistance*sin(ao+(ii+1)*M_PI*2/corners),0);
		// are lines parallel?
		double rd = (b.getX()-a.getX())*(d.getY()-c.getY()) - (b.getY()-a.getY())*(d.getX()-c.getX()); 
		if (rd==0) continue;
		// find intersection
		double rn = (a.getY()-c.getY())*(d.getX()-c.getX()) - (a.getX()-c.getX())*(d.getY()-c.getY());
		double sn = (a.getY()-c.getY())*(b.getX()-a.getX()) - (a.getX()-c.getX())*(b.getY()-a.getY());
		double intersection_ab = rn / rd;
		double intersection_cd = sn / rd;
		if (!(intersection_ab<0 or intersection_ab>1 or intersection_cd<0 or intersection_cd>1)) return false;
	}
	return !BrownianBase::isObstructed(pos);
}



