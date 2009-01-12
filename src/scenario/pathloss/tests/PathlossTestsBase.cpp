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

#include <RISE/scenario/pathloss/tests/PathlossTestsBase.hpp>

#include <WNS/CppUnit.hpp>
#include <WNS/Exception.hpp>

#include <cppunit/extensions/HelperMacros.h>

#include <limits>

using namespace rise::scenario::pathloss::tests;

const wns::Frequency PathlossTestsBase::BaseTestFrequency = 2000;
const wns::Frequency PathlossTestsBase::BaseTestDistance = 100;

PathlossTestsBase::~PathlossTestsBase()
{}

void PathlossTestsBase::testInput(const Pathloss *const pathloss)
{
    if (pathloss != NULL) {
	station1->moveTo(wns::Position(0, 0, 0));
	station2->moveTo(wns::Position(BaseTestDistance, 0, 0));

	// test valid frequency input
	CPPUNIT_ASSERT_NO_THROW( pathloss->getPathloss(*antenna1, *antenna2, BaseTestFrequency) );

#ifdef WNS_ASSURE_THROWS_EXCEPTION
	// test invalid frequency input
	WNS_ASSERT_ASSURE_EXCEPTION( pathloss->getPathloss(*antenna1, *antenna2, 0.0) );
	WNS_ASSERT_ASSURE_EXCEPTION( pathloss->getPathloss(*antenna1, *antenna2, -BaseTestFrequency) );
	const wns::Frequency infFreq = std::numeric_limits<wns::Frequency>::infinity();
	WNS_ASSERT_ASSURE_EXCEPTION( pathloss->getPathloss(*antenna1, *antenna2, infFreq) );
#endif
	// test zero distance
	station2->moveTo(wns::Position(0, 0, 0));
	CPPUNIT_ASSERT_NO_THROW( pathloss->getPathloss(*antenna1, *antenna2, BaseTestFrequency) );
    }
}

