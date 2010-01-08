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
using namespace rise::scenario;

STATIC_FACTORY_BROKER_REGISTER(Objects, Shadowing, "Objects");

STATIC_FACTORY_REGISTER_WITH_CREATOR(Obstructing<wns::geometry::AxisParallelRectangle>,
                                     IObstruction,
                                     "rise.shadowing.obstruction.AxisParallelRectangle",
                                     IObstructionCreator);

STATIC_FACTORY_REGISTER_WITH_CREATOR(Obstructing<wns::geometry::LineSegment>,
                                     IObstruction,
                                     "rise.shadowing.obstruction.LineSegment",
                                     IObstructionCreator);

Objects::Objects(const wns::pyconfig::View& config) :
    obstructionList(Objects::fetchObstructionList(config))
{
}


rise::scenario::ObstructionList
Objects::fetchObstructionList(const wns::pyconfig::View& config)
{
    ObstructionList obstructions;

    for( int ii = 0; ii < config.len("obstructionList"); ++ii)
    {
        wns::pyconfig::View objectView = config.getView("obstructionList", ii);
        obstructions.push_front(ObstructionFactory::creator(objectView.get<std::string>("__plugin__"))->create(objectView));
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


wns::Ratio
Objects::getShadowing(const antenna::Antenna& source,
                      const antenna::Antenna& target) const
{
    const wns::Position& sourcePos = source.getPosition();
    const wns::Position& targetPos = target.getPosition();

    wns::geometry::LineSegment signalPath(sourcePos, targetPos);

    wns::Ratio shadowing;

    for (ObstructionList::const_iterator itr = obstructionList.begin();
         itr != obstructionList.end();
         ++itr)
    {
        shadowing += (*itr)->getAttenuation(signalPath);
    }

    return shadowing;
}
