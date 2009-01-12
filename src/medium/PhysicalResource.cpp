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

#include <RISE/medium/PhysicalResource.hpp>
#include <RISE/RISE.hpp>

#include <WNS/pyconfig/View.hpp>

#include <sstream>

using namespace rise;
using namespace rise::medium;

PhysicalResource::PhysicalResource(double f, double b) :
	frequency(f),
	bandwidth(b),
	frequencyRange(Range::Between(f - (b/2)).And(f + (b/2))),
	receivers(),
	transmissionObjects(),
	log("")
{
 	std::stringstream location;
 	location << "PhysicalResource f: " << frequency << " b: " << bandwidth;
 	log=RISELogger(location.str());
 	if(RISE::getPyConfigView().get<bool>("debug.physicalResource")) {
		log.switchOn();
	} else {
 		log.switchOff();
 	}
}

PhysicalResource::~PhysicalResource()
{
	receivers.clear();
	transmissionObjects.clear();
}

void PhysicalResource::attach(PhysicalResourceObserver* r)
{
	assure(std::find(receivers.begin(), receivers.end(), r) == receivers.end(),
		   "PhysicalResourceObserver already in list!");

	MESSAGE_BEGIN(NORMAL, log, m, "Attaching PhysicalResourceObserver!");
	MESSAGE_END();


	receivers.push_front(r);
}

void PhysicalResource::detach(PhysicalResourceObserver* r)
{
	assure(std::find(receivers.begin(), receivers.end(), r) != receivers.end(),
		   "PhysicalResourceObserver is not in list!");

	MESSAGE_BEGIN(NORMAL, log, m, "Detaching PhysicalResourceObserver!");
	MESSAGE_END();

	receivers.remove(r);
}


