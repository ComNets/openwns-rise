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

#include <RISE/scenario/shadowing/Objects.hpp>
#include <RISE/scenario/sceneryfile/SceneryFile.hpp>
#include <RISE/scenario/sceneryfile/Walls.hpp>

#include <WNS/StaticFactoryBroker.hpp>
#include <WNS/pyconfig/View.hpp>
#include <WNS/Position.hpp>

#include <string>

using namespace rise::scenario::shadowing;

STATIC_FACTORY_BROKER_REGISTER(Objects, Shadowing, "Objects");

Objects::Objects(const wns::pyconfig::View& config) :
 	xGridBlocks(1),
 	yGridBlocks(1),
	xGridBlockSize(0.0),
	yGridBlockSize(0.0),
	obstructionList(Objects::fetchObstructionList(config)),
	blockObstructionLists()
{
    xGridBlockSize = (max.getX() - min.getX()) / xGridBlocks;
    yGridBlockSize = (max.getY() - min.getY()) / yGridBlocks;

	const size_t sizes[2] = {this->xGridBlocks, this->yGridBlocks};
	this->blockObstructionLists = BlockObstructionLists(sizes);
	this->createBlockObstructionLists();
}


rise::scenario::ObstructionList
Objects::fetchObstructionList(const wns::pyconfig::View& config)
{
	ObstructionList obstructions;

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

        if (a.getX() < min.getX()) min.setX(a.getX());
        if (a.getY() < min.getX()) min.setY(a.getY());
        if (a.getX() > max.getX()) max.setX(a.getX());
        if (a.getY() > max.getY()) max.setY(a.getY());

		obstructions.push_front(
			ObjectPtr(
				new Object(
					wns::geometry::AxisParallelRectangle(a,b),
					objectView.get<wns::Ratio>("attenuation"))));
	}

	return obstructions;
}


rise::scenario::ObstructionList
Objects::fetchObstructionList(Scenario& scenario,
			      const wns::pyconfig::View& config)
{
	std::string fileName = config.get<std::string>("obstructionFileName");

	scenario.openSceneryFile(fileName, "WallsFile");

	const sceneryfile::Walls& walls =
		dynamic_cast<const sceneryfile::Walls&>(scenario.getSceneryFile(fileName));

	return walls.getObstructionList();
}


void
Objects::createBlockObstructionLists()
{
	for (ObstructionList::iterator obstruction = this->obstructionList.begin();
	     obstruction != this->obstructionList.end();
	     ++obstruction)
	{
		for (unsigned int x = 0; x < this->xGridBlocks; ++x)
		{
			for (unsigned int y = 0; y < this->yGridBlocks; ++y)
			{
				wns::geometry::Point a(x * this->xGridBlockSize, y * this->yGridBlockSize, 0);
				wns::geometry::Point b((x+1) * this->xGridBlockSize, (y+1) * this->yGridBlockSize, 0);

				wns::geometry::AxisParallelRectangle currentBlock(a, b);

				wns::geometry::Shape2D& element = dynamic_cast<wns::geometry::Shape2D&>(*(*obstruction));
				if (currentBlock.intersectsBoundingBoxOf(element))
				{
					this->blockObstructionLists[x][y].push_front(*obstruction);
				}
			}
		}
	}
}

wns::Ratio
Objects::getShadowing(
	const antenna::Antenna& source,
	const antenna::Antenna& target) const
{
	const wns::Position& sourcePos = source.getPosition();
	const wns::Position& targetPos = target.getPosition();

	wns::geometry::LineSegment signalPath(sourcePos, targetPos);

	wns::Ratio shadowing;

	for (uint32_t x = (uint32_t)std::floor(signalPath.getBoundingBox().getMinX() / xGridBlockSize);
	     x < std::ceil(signalPath.getBoundingBox().getMaxX() / xGridBlockSize);
	     ++x)
	{
		for (uint32_t y = (uint32_t)std::floor(signalPath.getBoundingBox().getMinY() / yGridBlockSize);
		     y < std::ceil(signalPath.getBoundingBox().getMaxY() / yGridBlockSize);
		     ++y)
		{
			const ObstructionList& theList = this->blockObstructionLists[x][y];
			for (ObstructionList::const_iterator obstruction = theList.begin();
			     obstruction != theList.end();
			     ++obstruction)
			{
				shadowing += (*obstruction)->getAttenuation(signalPath);
			}
		}
	}

	return shadowing;
}
