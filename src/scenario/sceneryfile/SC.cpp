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

#include <fstream>
#include <cassert>

#include <RISE/scenario/sceneryfile/SC.hpp>
#include <RISE/scenario/Scenario.hpp>
#include <RISE/scenario/scenerymap/SceneryMap.hpp>

using namespace rise::scenario::sceneryfile;

SC::SC() :
	SceneryFile(),
	initialized(false),
	morphologyMap(),
	pathlossMaps(),
	shadowingMap()
{
}

#ifndef WNS_DEBUG
SC::SC(const std::string filename, const scenario::Scenario& scenario) :
#else
SC::SC(const std::string filename, const scenario::Scenario& /*scenario*/) :
#endif
	SceneryFile(),
	initialized(false),
	SCfile(filename.c_str()),
	morphologyMap(),
	pathlossMaps(),
	shadowingMap()
{
	std::ifstream SCfile(filename.c_str());

    if (!SCfile.is_open()) throw wns::Exception(("SC: Could not open " + filename).c_str());

    // check if we really have a SC file
    char fileMagic[20];
    long int fileVersion = 0;

    SCfile.getline(fileMagic, 20);
    if (std::string(fileMagic) == "SceneryMap,") fileVersion = 1;
    if (std::string(fileMagic) == "SceneryMap2") fileVersion = 2;
    if (std::string(fileMagic) == "SceneryMap3") fileVersion = 3;

    if (fileVersion == 0)
		throw wns::Exception(("SC: " + filename + " not a SC map file").c_str());

    // read in x and y size
    long int xSize, ySize;
    if (fileVersion < 3)
		{
			SCfile >> xSize;
			ySize = xSize;
		}

    if (fileVersion >= 3)
		{
			SCfile >> xSize >> ySize;
		}

    if ((xSize <= 0) || (ySize <= 0))
		throw wns::Exception("SC: illegal scenario size");

    // read in sqrt of number of shadowing tiles per pathloss/morphology tile
    unsigned long int shadowingSize;
    SCfile >> shadowingSize;

    // read in number of base stations
    long int maxBS;
    SCfile >> maxBS;

    if (maxBS < 1)
		throw wns::Exception("SC: too few base stations");

    // read in resolution
    unsigned long int resolution;

    if (fileVersion >=2)
		{
			SCfile >> resolution;
		}
    else
		{
			resolution = 10;
		}

    // create maps
    scenerymap::resolution resolutions[2] = {resolution, resolution};
    std::size_t sizes[2] = {xSize, ySize};
    scenerymap::resolution shadowingResolutions[2] = {resolution / shadowingSize, resolution / shadowingSize};
    std::size_t shadowingSizes[2] = {xSize * shadowingSize, ySize * shadowingSize};

    morphologyMap = scenerymap::MorphologyMap2D(sizes, resolutions);
    pathlossMaps = scenerymap::PathlossMaps2D(maxBS, scenerymap::PathlossMap2D(sizes, resolutions));
    shadowingMap = scenerymap::ShadowingMap2D(shadowingSizes, shadowingResolutions);

    // fill maps with values
    char aChar;
    SCfile.get(aChar);
    for (long int y = 0; y < ySize; ++y)
		{
			for (long int x = 0; x < xSize; ++x)
				{
					// morphology
					SCfile.get(aChar);
					morphologyMap[x][y] = (unsigned char)aChar;

					// shadowing
					for (unsigned long int sy = 0; sy < shadowingSize; ++sy)
						{
							for (unsigned long int sx = 0; sx < shadowingSize; ++sx)
								{
									SCfile.get(aChar);
									shadowingMap[x*shadowingSize + sx][y*shadowingSize + sy] = aChar;
								}
						}

					// pathloss
					for (long int bs = 0; bs < maxBS; ++bs)
						{
							SCfile.get(aChar);
							pathlossMaps[bs][x][y] = (unsigned char)aChar;
						}

				}
		}

    initialized = true;
}


