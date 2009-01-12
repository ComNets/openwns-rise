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

#include <RISE/manager/systemmanager.hpp>
#include <RISE/manager/metasystemmanager.hpp>
#include <RISE/scenario/Scenario.hpp>
#include <RISE/stations/station.hpp>
#include <RISE/RISE.hpp>
#include <WNS/Assure.hpp>

#include <string>
#include <sstream>
#include <vector>
#include <map>
#include <fstream>

using namespace std;
using namespace rise;

SystemManager::SystemManager(const string &aSystemName,
							 const wns::pyconfig::View& config)
	: systemName(aSystemName),
	  scenario(NULL),
	  stations(StationContainer()),
 	  pyConfigView(config),
	  stationId(0),
	  log(string("SystemManager (")+aSystemName+string(")"))
{
	const wns::pyconfig::View& riseConfig = RISE::getPyConfigView();
	MetaSystemManager::getInstance()->attach(this);
 	if (riseConfig.getView("debug").get<bool>("systemManager")) {
		log.switchOn();
	} else {
		log.switchOff();
	}
}

SystemManager::~SystemManager()
{
	MetaSystemManager::getInstance()->detach(this);
	if (scenario != NULL) delete scenario;
}

wns::pyconfig::View
SystemManager::getConfigFile() const
{
	return pyConfigView;
}

SystemManager::StationContainer SystemManager::getAllStations() const
{
	return stations;
}

string SystemManager::getSystemName() const
{
	return systemName;
}

scenario::Scenario* SystemManager::getScenario() const
{
	assure(scenario, "must be non-NULL");
	return scenario;
}

void SystemManager::addStation(Station* station)
{
	assert(find(stations.begin(), stations.end(), station)==stations.end());
	station->initialize();
	station->setStationId(stationId++);
	stations.push_back(station);
}

void SystemManager::createScenario()
{
	MESSAGE_BEGIN(NORMAL, log, m, "Creating Scenario");
	MESSAGE_END();
	scenario = new scenario::Scenario(pyConfigView.getView("Scenario"));
}


