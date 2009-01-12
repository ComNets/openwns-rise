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

#include <RISE/manager/metasystemmanager.hpp>
#include <RISE/manager/systemmanager.hpp>
#include <RISE/stations/base/base.hpp>

#include <vector>
#include <string>
#include <functional>

using namespace rise;

MetaSystemManager::MetaSystemManager()
	: systemManagers(),
	  log("MetaSystemManager")
{
	log.switchOn();
}

MetaSystemManager::~MetaSystemManager()
{
}

void MetaSystemManager::attach(SystemManager* sysmgr)
{
	MESSAGE_BEGIN(NORMAL, log, m, "Attaching SystemManager: ");
	m << sysmgr->getSystemName();
	MESSAGE_END();
	systemManagers.insert(sysmgr->getSystemName(), sysmgr);
}

void MetaSystemManager::detach(SystemManager* sysmgr)
{
	MESSAGE_BEGIN(NORMAL, log, m, "Detaching SystemManager: ");
	m << sysmgr->getSystemName();
	MESSAGE_END();
	systemManagers.erase(sysmgr->getSystemName());
}

SystemManager* MetaSystemManager::getSystemManagerBySystemName(const std::string& aSystemName)
{
	return systemManagers.find(aSystemName);
}

void MetaSystemManager::shutDownSystemManagers()
{
	while(!systemManagers.empty()) {
		delete systemManagers.begin()->second;
	}
}

MetaSystemManager* MetaSystemManager::myself = NULL;


