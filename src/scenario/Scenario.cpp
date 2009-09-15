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

#include <RISE/scenario/Scenario.hpp>
#include <RISE/scenario/pathloss/Pathloss.hpp>
#include <RISE/scenario/shadowing/Shadowing.hpp>
#include <RISE/scenario/fastfading/FastFading.hpp>
#include <RISE/scenario/sceneryfile/SceneryFile.hpp>
#include <RISE/scenario/sceneryfile/SC.hpp>

#include <WNS/pyconfig/View.hpp>
#include <WNS/Exception.hpp>

#include <string>
#include <map>

using namespace rise::scenario;

Scenario::Scenario(const wns::pyconfig::View& config) :
	sceneryFiles()
{}

Scenario::~Scenario()
{
	for (SceneryFilesMap::iterator sfIter = sceneryFiles.begin();
	     sfIter != sceneryFiles.end();
	     ++sfIter)
	{
		delete (*sfIter).second;
	}
}

void
Scenario::openSceneryFile(const std::string& filename, const std::string& filetype)
{
    if (sceneryFiles.count(filename) == 0)
    {
	if (filetype == "ScenMapSC")
	{
	    sceneryFiles[filename] = new sceneryfile::SC(filename, *this);
	}
	else
	{
		throw wns::Exception("scenery file type " + filetype + " is not supported!");
	}
    }
}

const sceneryfile::SceneryFile&
Scenario::getSceneryFile(const std::string& filename) const
{
    SceneryFilesMap::const_iterator entryIter = sceneryFiles.find(filename);
    assure(entryIter != sceneryFiles.end(), "File " + filename + " not open!");
    return *(entryIter->second);
}

