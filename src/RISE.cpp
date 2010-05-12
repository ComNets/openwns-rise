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

#include <RISE/medium/Medium.hpp>
#include <RISE/RISE.hpp>
#include <RISE/manager/metasystemmanager.hpp>

#include <WNS/logger/Master.hpp>
#include <WNS/pyconfig/View.hpp>

#include <WNS/rng/RNGen.hpp>
#include <WNS/simulator/Simulator.hpp>

#include <WNS/pyconfig/Parser.hpp>
using namespace std;
using namespace rise;

STATIC_FACTORY_REGISTER_WITH_CREATOR(RISE, wns::module::Base, "rise", wns::PyConfigViewCreator);

RISE::RISE(const wns::pyconfig::View& _pyConfigView) :
	wns::module::Module<RISE>(_pyConfigView),
	mobilityRNG(NULL)
{
}

RISE::~RISE()
{
	MetaSystemManager::deleteInstance();
	rise::medium::Medium::deleteInstance();
}

void RISE::configure()
{
	wns::pyconfig::View config(getPyConfigView());
 	if(!config.isNone("ownMobilityRNG"))
	{
		wns::simulator::getMasterLogger()->write("RISE",
												 "configure()",
												 "WARNING: You're about to Instantiate an additional RNG for the RISE Mobility!");
		mobilityRNG = new wns::rng::RNGen();
		wns::pyconfig::View rngConfig = config.get("ownMobilityRNG");
		mobilityRNG->seed(rngConfig.get<unsigned long int>("seed"));
	} else {
 		mobilityRNG = wns::simulator::getRNG();
	}
    wns::simulator::getRegistry()->insert("MOBILITY-RNG", mobilityRNG);
}

void RISE::startUp()
{
}

void RISE::shutDown()
{
	try
	{
		MetaSystemManager::getInstance()->shutDownSystemManagers();
	}
	catch(wns::Exception& anException)
	{
		anException << "\nThis exception occured during RISE::shutDown ...\n";
		throw;
	}
}

wns::rng::RNGen*
RISE::getMobilityRNG()
{
	return mobilityRNG;
}



