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

#include <RISE/transceiver/tests/receiverTest.hpp>
#include <RISE/stations/station.hpp>
#include <RISE/stations/tests/StationDropIn.hpp>
#include <RISE/transmissionobjects/broadcasttransmissionobject.hpp>
#include <iostream>

using namespace rise;
using namespace rise::tests;

CPPUNIT_TEST_SUITE_REGISTRATION( ReceiverTest );

ReceiverTest::ReceiverTest() :
	CppUnit::TestFixture(),
	systemManagerDropIn(),
	station1(&systemManagerDropIn, 0),
	station2(&systemManagerDropIn, 1),
	rec1(&station1,
	     station1.getAntennaPtr(0)),
	rec2(&station2,
	     station2.getAntennaPtr(0)),
	trans1(&station1,
	       station1.getAntennaPtr(0)),
	trans2(&station2,
	       station2.getAntennaPtr(0))
{
	station1.set(&rec1);
	station1.set(&trans1);
	station2.set(&rec2);
	station2.set(&trans2);
	station2.setPosition(wns::Position(100, 100));
	systemManagerDropIn.addStation(&station1);
	systemManagerDropIn.addStation(&station2);
}

ReceiverTest::~ReceiverTest()
{
}

void ReceiverTest::setUp()
{
}

void ReceiverTest::tearDown()
{
}

void ReceiverTest::all()
{
	// This test works only if all parameters are set accordingly in
	// rise.ini
	CPPUNIT_ASSERT( &station1 == rec1.getStation() );
	CPPUNIT_ASSERT( &station1 == trans1.getStation() );
	CPPUNIT_ASSERT( &station2 == rec2.getStation() );
	CPPUNIT_ASSERT( &station2 == trans2.getStation() );

	CPPUNIT_ASSERT( !rec1.isActive() );
	CPPUNIT_ASSERT( !trans1.isActive() );
	CPPUNIT_ASSERT( !rec2.isActive() );
	CPPUNIT_ASSERT( !trans2.isActive() );

	rec1.tune(3000, 20, 1);
	rec2.tune(3000, 20, 1);
	trans1.tune(3000, 20, 1);
	trans2.tune(3000, 20, 1);

	CPPUNIT_ASSERT( !rec1.isActive() );
	CPPUNIT_ASSERT( !trans1.isActive() );
	CPPUNIT_ASSERT( !rec2.isActive() );
	CPPUNIT_ASSERT( !trans2.isActive() );

	CPPUNIT_ASSERT_DOUBLES_EQUAL( 3000, rec1.getFrequency(), 0.0001 );
	CPPUNIT_ASSERT_DOUBLES_EQUAL( 3000, rec2.getFrequency(), 0.0001 );
	CPPUNIT_ASSERT_DOUBLES_EQUAL( 3000, trans1.getFrequency(), 0.0001 );
	CPPUNIT_ASSERT_DOUBLES_EQUAL( 3000, trans2.getFrequency(), 0.0001 );

	trans1.setTxPower(wns::Power::from_dBm(10));
	trans2.setTxPower(wns::Power::from_dBm(20));

	CPPUNIT_ASSERT( !rec1.isActive() );
	CPPUNIT_ASSERT( !trans1.isActive() );
	CPPUNIT_ASSERT( !rec2.isActive() );
	CPPUNIT_ASSERT( !trans2.isActive() );

	rec1.startReceiving();
	rec2.startReceiving();

	CPPUNIT_ASSERT( rec1.isActive() );
	CPPUNIT_ASSERT( !trans1.isActive() );
	CPPUNIT_ASSERT( rec2.isActive() );
	CPPUNIT_ASSERT( !trans2.isActive() );

	CPPUNIT_ASSERT_DOUBLES_EQUAL( 42, rec1.getLoss(&trans1).get_dB(), 0.0001 );
	CPPUNIT_ASSERT_DOUBLES_EQUAL( 42, rec2.getLoss(&trans2).get_dB(), 0.0001 );

	// @todo (msg) check if pathloss is ok for freespace
	CPPUNIT_ASSERT_DOUBLES_EQUAL( 84.9927, rec2.getLoss(&trans1).get_dB(), 0.0001 );  // result: 42
	CPPUNIT_ASSERT_DOUBLES_EQUAL( 84.9927, rec1.getLoss(&trans2).get_dB(), 0.0001 );  // result 42 

	// Receivers must receive a signal equivalent to background noise
	CPPUNIT_ASSERT_DOUBLES_EQUAL( -97.9897, rec1.getTotalRxPower().get_dBm(), 0.0001 );
	CPPUNIT_ASSERT_DOUBLES_EQUAL( -97.9897, rec2.getTotalRxPower().get_dBm(), 0.0001 );

	BroadcastTransmissionObjectPtr b(new BroadcastTransmissionObject(&trans1, NULL, trans1.getTxPower(), 1));
	CPPUNIT_ASSERT(!rec1.contains(b));
	CPPUNIT_ASSERT(!rec2.contains(b));
	trans1.startTransmitting(b,0);
	CPPUNIT_ASSERT( rec1.isActive() );
	CPPUNIT_ASSERT( trans1.isActive() );
	CPPUNIT_ASSERT( rec2.isActive() );
	CPPUNIT_ASSERT( !trans2.isActive() );
	CPPUNIT_ASSERT(rec1.contains(b));
	CPPUNIT_ASSERT(rec2.contains(b));
	CPPUNIT_ASSERT_DOUBLES_EQUAL( -32, rec1.getTotalRxPower().get_dBm(), 0.0001 );
	CPPUNIT_ASSERT_DOUBLES_EQUAL( -32, rec1.getRxPower(b).get_dBm(), 0.0001 );
	CPPUNIT_ASSERT_DOUBLES_EQUAL( -97.9897, rec1.getInterference(b).get_dBm(), 0.0001 );
	CPPUNIT_ASSERT_DOUBLES_EQUAL( -74.971, rec2.getTotalRxPower().get_dBm(), 0.0001 ); //result -32
	CPPUNIT_ASSERT_DOUBLES_EQUAL( -74.9927, rec2.getRxPower(b).get_dBm(), 0.0001 );    //result -32
	CPPUNIT_ASSERT_DOUBLES_EQUAL( -97.9897, rec2.getInterference(b).get_dBm(), 0.0001 );

	// although cache is enabled should result in new values for
	// interference and rx power
	station1.moveTo(wns::Position(20, 20, 0));
	station2.moveTo(wns::Position(80, 80, 0));

	CPPUNIT_ASSERT( rec1.isActive() );
	CPPUNIT_ASSERT( trans1.isActive() );
	CPPUNIT_ASSERT( rec2.isActive() );
	CPPUNIT_ASSERT( !trans2.isActive() );
	CPPUNIT_ASSERT(rec1.contains(b));
	CPPUNIT_ASSERT(rec2.contains(b));
	CPPUNIT_ASSERT_DOUBLES_EQUAL( -32, rec1.getTotalRxPower().get_dBm(), 0.0001 );
	CPPUNIT_ASSERT_DOUBLES_EQUAL( -32, rec1.getRxPower(b).get_dBm(), 0.0001 );
	CPPUNIT_ASSERT_DOUBLES_EQUAL( -97.9897, rec1.getInterference(b).get_dBm(), 0.0001 );
	CPPUNIT_ASSERT_DOUBLES_EQUAL( -70.5479, rec2.getTotalRxPower().get_dBm(), 0.0001 );
	CPPUNIT_ASSERT_DOUBLES_EQUAL( -70.5558, rec2.getRxPower(b).get_dBm(), 0.0001 );
	CPPUNIT_ASSERT_DOUBLES_EQUAL( -97.9897, rec2.getInterference(b).get_dBm(), 0.0001 );

	// Move back (to old positons)
	station1.moveTo(wns::Position(0, 0, 0));
	station2.moveTo(wns::Position(100, 100, 0));

	BroadcastTransmissionObjectPtr b2(new BroadcastTransmissionObject(&trans2, NULL, trans2.getTxPower(), 1));
	CPPUNIT_ASSERT(!rec1.contains(b2));
	CPPUNIT_ASSERT(!rec2.contains(b2));
	trans2.startTransmitting(b2,0);
	CPPUNIT_ASSERT( rec1.isActive() );
	CPPUNIT_ASSERT( trans1.isActive() );
	CPPUNIT_ASSERT( rec2.isActive() );
	CPPUNIT_ASSERT( trans2.isActive() );
	CPPUNIT_ASSERT(rec1.contains(b));
	CPPUNIT_ASSERT(rec2.contains(b));
	CPPUNIT_ASSERT(rec1.contains(b2));
	CPPUNIT_ASSERT(rec2.contains(b2));
	CPPUNIT_ASSERT_DOUBLES_EQUAL( -31.9978, rec1.getTotalRxPower().get_dBm(), 0.0001 );
	CPPUNIT_ASSERT_DOUBLES_EQUAL( -32, rec1.getRxPower(b).get_dBm(), 0.0001 );
	CPPUNIT_ASSERT_DOUBLES_EQUAL( -64.9927, rec1.getRxPower(b2).get_dBm(), 0.0001 );
	CPPUNIT_ASSERT_DOUBLES_EQUAL( -64.9905, rec1.getInterference(b).get_dBm(), 0.0001 );
	CPPUNIT_ASSERT_DOUBLES_EQUAL( -32, rec1.getInterference(b2).get_dBm(), 0.0001 );

	CPPUNIT_ASSERT_DOUBLES_EQUAL( -22, rec2.getTotalRxPower().get_dBm(), 0.0001 );
	CPPUNIT_ASSERT_DOUBLES_EQUAL( -74.9927, rec2.getRxPower(b).get_dBm(), 0.0001 );
	CPPUNIT_ASSERT_DOUBLES_EQUAL( -22, rec2.getRxPower(b2).get_dBm(), 0.0001 );
	CPPUNIT_ASSERT_DOUBLES_EQUAL( -22, rec2.getInterference(b).get_dBm(), 0.0001 );
	CPPUNIT_ASSERT_DOUBLES_EQUAL( -74.971, rec2.getInterference(b2).get_dBm(), 0.0001 );

	trans1.stopTransmitting(b);
	CPPUNIT_ASSERT( rec1.isActive() );
	CPPUNIT_ASSERT( !trans1.isActive() );
	CPPUNIT_ASSERT( rec2.isActive() );
	CPPUNIT_ASSERT( trans2.isActive() );
	CPPUNIT_ASSERT(!rec1.contains(b));
	CPPUNIT_ASSERT(!rec2.contains(b));
	CPPUNIT_ASSERT(rec1.contains(b2));
	CPPUNIT_ASSERT(rec2.contains(b2));
	CPPUNIT_ASSERT_DOUBLES_EQUAL( -64.9905, rec1.getTotalRxPower().get_dBm(), 0.0001 );

	trans2.stopTransmitting(b2);
	CPPUNIT_ASSERT( rec1.isActive() );
	CPPUNIT_ASSERT( !trans1.isActive() );
	CPPUNIT_ASSERT( rec2.isActive() );
	CPPUNIT_ASSERT( !trans2.isActive() );
	CPPUNIT_ASSERT(!rec1.contains(b));
	CPPUNIT_ASSERT(!rec2.contains(b));
	CPPUNIT_ASSERT(!rec1.contains(b2));
	CPPUNIT_ASSERT(!rec2.contains(b2));

	// Receivers must receive a signal equivalent to background noise
	CPPUNIT_ASSERT_DOUBLES_EQUAL( -97.9897, rec1.getTotalRxPower().get_dBm(), 0.0001 );
	CPPUNIT_ASSERT_DOUBLES_EQUAL( -97.9897, rec2.getTotalRxPower().get_dBm(), 0.0001 );

	// Receivers must receive a signal equivalent to background noise
	CPPUNIT_ASSERT( rec1.getNoise() == rec1.getTotalRxPower() );
	CPPUNIT_ASSERT( rec2.getNoise() == rec2.getTotalRxPower() );

	rec1.stopReceiving();
	rec2.stopReceiving();

	CPPUNIT_ASSERT( !rec1.isActive() );
	CPPUNIT_ASSERT( !trans1.isActive() );
	CPPUNIT_ASSERT( !rec2.isActive() );
	CPPUNIT_ASSERT( !trans2.isActive() );
}


