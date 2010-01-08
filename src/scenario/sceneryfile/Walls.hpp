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

#ifndef _WALLS_HPP
#define _WALLS_HPP

#include <cassert>

#include <WNS/SmartPtr.hpp>
#include <WNS/geometry/LineSegment.hpp>

#include <RISE/scenario/sceneryfile/SceneryFile.hpp>
#include <RISE/scenario/Obstruction.hpp>

namespace rise { namespace scenario { namespace sceneryfile	{
        class Walls : public SceneryFile
        {
        public:
            typedef Obstructing<wns::geometry::LineSegment> Wall;

            Walls();
            Walls(const std::string filename);
            virtual ~Walls() {}

            virtual bool hasMorphologyMap() const { return false; }
            virtual bool hasPathlossMap() const { return false; }
            virtual bool hasShadowingMap() const { return false; }
            virtual bool hasObstructionList() const { return true; }

            virtual const ObstructionList& getObstructionList() const
                {
                    assert(initialized);
                    return obstructionList;
                }

        private:
            bool initialized;

            ObstructionList obstructionList;
            static Wall* createWall(std::string input);
        };
}}}

#endif // _WALLS_HPP
