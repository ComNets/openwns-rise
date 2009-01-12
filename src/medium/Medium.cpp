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
#include <RISE/medium/PhysicalResource.hpp>
#include <RISE/RISE.hpp>

#include <WNS/Exception.hpp>

#include <list>

using namespace rise;
using namespace rise::medium;

Medium* Medium::getInstance()
{
	if(!myself) {
		myself = new Medium;
	}
	return myself;
}

void Medium::deleteInstance()
{
 	if(myself) delete myself;
 	myself = NULL;
}

void Medium::reset()
{
	PhysicalResourceIterator i = prs.begin();
	while( i != prs.end())
	{
		PhysicalResource* p = *i;
		prs.erase(i);

		MESSAGE_BEGIN(NORMAL, log, m, "Deleting PhysicalResource f: ");
		m << p->getFrequency() << " MHz, b: " << p->getBandwidth() << " MHz";
		MESSAGE_END();

		delete p;
		i = prs.begin();
	}
}

PhysicalResource* Medium::getPhysicalResource(double f, double b)
{
	PhysicalResourceIterator result;
	for(result = prs.begin(); result != prs.end(); ++result) {
		if((std::abs((*result)->getFrequency() - f)/f < 1.0e-6) &&
		   (std::abs((*result)->getBandwidth() - b)/b < 1.0e-6) )
			break;
	}
	if(result==prs.end()) {
		PhysicalResource* pr = new PhysicalResource(f, b);
		MESSAGE_BEGIN(NORMAL, log, m, "Created and delivered new PhysicalResource f: ");
		m << pr->getFrequency() << " MHz, b: " << pr->getBandwidth() << " MHz";
		MESSAGE_END();
		if(checkForOverlap(pr)) {
			delete pr;
			throw wns::Exception("Overlapping PhysicalResources!");
		}
		prs.push_back(pr);
		return pr;
	} else {
		MESSAGE_BEGIN(NORMAL, log, m, "Delivered new PhysicalResource f: ");
		m << (*result)->getFrequency() << " MHz, b: " << (*result)->getBandwidth() << " MHz";
		MESSAGE_END();
		return *result;
	}
}

Medium::Medium() :
	prs(),
	log("Medium")
{
	if(RISE::getPyConfigView().get<bool>("debug.medium")) {
		log.switchOn();
	} else {
		log.switchOff();
	}
}

Medium::~Medium()
{
	this->reset();
}

bool Medium::checkForOverlap(PhysicalResource* pr) const
{
	bool overlapping=false;
	PhysicalResourceContainer::const_iterator itr, itrEnd;
	itrEnd = prs.end();
	for(itr=prs.begin(); itr!=itrEnd; ++itr) {
		if( pr->getFrequencyRange().overlaps((*itr)->getFrequencyRange())) {
			overlapping = true;
			MESSAGE_BEGIN(NORMAL, log, m, "");
			m << "WARNING: THESE PHYSICAL RESOURCES ARE OVERLAPPING:"
			  << " New PR: Frequency=" << pr->getFrequency()
			  << "MHz Bandwidth="<< pr->getBandwidth()
			  << " Existing PR: Frequency=" << (*itr)->getFrequency()
			  << "MHz Bandwidth="<< (*itr)->getBandwidth()
			  << " INTERFRENCE BETWEEN THESE TWO PHYSICAL RESOURCES IS CURRENTLY"
			  << " NOT TAKEN INTO ACCOUNT!!!";
			MESSAGE_END();
		  }
	}
	return overlapping;
	/* Examples

	Non Overlap example:

    f1lb                 f1ub    f2lb                 f2ub
    --|--------------------|-------|--------------------|------> f
      |<-PhysicalResource->|       |<-PhysicalResource->|


	Overlap example:

    f1lb                f2lb     f1ub                 f2ub
    --|-------------------|--------|--------------------|------> f
      |<-PhysicalResource--------->|                    |
                          |<----------PhysicalResource->|
	*/
}

Medium* Medium::myself = NULL;


