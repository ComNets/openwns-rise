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

#include <RISE/stations/tests/stationTest.hpp>
#include <RISE/stations/tests/StationDropIn.hpp>
#include <RISE/manager/tests/SystemManagerDropIn.hpp>
using namespace std;
using namespace rise::tests;

CPPUNIT_TEST_SUITE_REGISTRATION( StationTest );

StationTest::StationTest() :
	sm()
{
}

StationTest::~StationTest()
{
}

void StationTest::setUp()
{
	sm = new SystemManagerDropIn();
}

void StationTest::tearDown()
{
	delete sm;
}

void StationTest::constructor()
{
	StationDropIn s(sm);
#ifdef WNS_ASSURE_THROWS_EXCEPTION
	// you are not allowed to ask for the stationId until
	// it has explicitly been set
	bool thrown_up = false;
	try {
		s.getStationId();
	}
	catch(...) {
		thrown_up = true;
	}
	CPPUNIT_ASSERT(thrown_up);
#endif // WNS_ASSURE_THROWS_EXCEPTION
	CPPUNIT_ASSERT( wns::Position(0,0,0) == s.getPosition() );
	CPPUNIT_ASSERT( s.getSystemManager() );
}


void StationTest::stationId()
{
	StationDropIn s1(sm);
	StationDropIn s2(sm);
	s1.setStationId(1);
	s2.setStationId(2);
	CPPUNIT_ASSERT_EQUAL( 1, s1.getStationId() );
	CPPUNIT_ASSERT_EQUAL( 2, s2.getStationId() );
}

void StationTest::distance()
{
	StationDropIn s1(sm);
	StationDropIn s2(sm);
	s2.moveTo(wns::Position(3,4,0));
	CPPUNIT_ASSERT(s2.getPosition() == wns::Position(3,4,0));
	CPPUNIT_ASSERT_DOUBLES_EQUAL( 5, s1.getDistance(&s2), 0.000001 );
}


