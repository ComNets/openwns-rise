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

#include <RISE/scenario/sceneryfile/Walls.hpp>

#include <cctype>
#include <fstream>

#include <WNS/Assure.hpp>
#include <WNS/PowerRatio.hpp>

using namespace rise::scenario::sceneryfile;

Walls::Walls()
    : initialized(false),
      obstructionList()
{
}

Walls::Walls(const std::string filename)
    : initialized(false),
      obstructionList()
{
    std::ifstream file(filename.c_str());
    assure(file.is_open(), "could not open " + filename);

    std::string inputLine = "";
    while (inputLine.find("numWalls1") == std::string::npos)
    {
        file >> inputLine;
    }

    unsigned int numWalls = atoi(inputLine.substr(inputLine.find("=") + 1).c_str());
    for (unsigned int wallI = 0; wallI < numWalls; ++wallI)
    {
        file >> inputLine;
        if (inputLine[0] == 'w') {
            obstructionList.push_front(createWall(inputLine.substr(2)));
        }
    }

    file.close();
    initialized = true;
}

Walls::Wall*
Walls::createWall(std::string input)
{
    input = input.substr(input.find_first_not_of(" \t"));
    double x1 = strtod(input.substr(0, input.find_first_of(" \t")).c_str(), NULL);
    input = input.substr(input.find_first_of(" \t"));

    input = input.substr(input.find_first_not_of(" \t"));
    double y1 = strtod(input.substr(0, input.find_first_of(" \t")).c_str(), NULL);
    input = input.substr(input.find_first_of(" \t"));

    input = input.substr(input.find_first_not_of(" \t"));
    double x2 = strtod(input.substr(0, input.find_first_of(" \t")).c_str(), NULL);
    input = input.substr(input.find_first_of(" \t"));

    input = input.substr(input.find_first_not_of(" \t"));
    double y2 = strtod(input.substr(0, input.find_first_of(" \t")).c_str(), NULL);
    input = input.substr(input.find_first_of(" \t"));

    input = input.substr(input.find_first_not_of(" \t"));
    unsigned int floor = atoi(input.substr(0, input.find_first_of(" \t")).c_str());
    input = input.substr(input.find_first_of(" \t"));
    floor = floor;

    input = input.substr(input.find_first_not_of(" \t"));
    double attenuation = strtod(input.substr(0, input.find_first_of(" \t")).c_str(), NULL);

    wns::geometry::Point a(x1, y1, 0);
    wns::geometry::Point b(x2, y2, 0);

    return (new Wall(wns::geometry::LineSegment(a, b), wns::Ratio::from_dB(attenuation)));
}
