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

#include <RISE/transceiver/tests/SCReceiverTest.hpp>

using namespace rise;
using namespace rise::tests;
using namespace rise::antenna;

CPPUNIT_TEST_SUITE_REGISTRATION( SCReceiverTest );

void SCReceiverTest::setUp()
{
	systemManagerDropIn = new SystemManagerDropIn();
	station1 = new StationDropIn(systemManagerDropIn/*, 0*/);
	station1->setStationId(0);
	station2 = new StationDropIn(systemManagerDropIn/*, 1*/);
	station2->setStationId(1);
	rec1 = new SCReceiverDropIn(station1, station1->getAntenna());
	rec2 = new SCReceiverDropIn(station2, station2->getAntenna());
	trans1 = new SCTransmitterDropIn(station1, station1->getAntenna());
	trans2 = new SCTransmitterDropIn(station2, station2->getAntenna());
	station1->set(rec1);
	station1->set(trans1);
	station2->set(rec2);
	station2->set(trans2);
	station2->moveTo(wns::Position(100, 100, 0));
	systemManagerDropIn->addStation(station1);
	systemManagerDropIn->addStation(station2);
}

void SCReceiverTest::tearDown()
{
	delete trans1;
	delete trans2;
	delete rec1;
	delete rec2;
	delete station1;
	delete station2;
	delete systemManagerDropIn;
}

void SCReceiverTest::all()
{
	rec1->tune(3000, 20);
	rec2->tune(3000, 20);

	trans1->setTxPower(wns::Power::from_dBm(10));
	trans2->setTxPower(wns::Power::from_dBm(20));
	trans1->tune(3000, 20);
	trans2->tune(3000, 20);

	rec1->startReceiving();
	rec2->startReceiving();

	CPPUNIT_ASSERT_DOUBLES_EQUAL( 42, rec1->getLoss(trans1).get_dB(), 0.0001 );
	CPPUNIT_ASSERT_DOUBLES_EQUAL( 141.4213, station1->getDistance(station2), 0.0001 );
	CPPUNIT_ASSERT_DOUBLES_EQUAL( 84.9927, rec2->getLoss(trans1).get_dB(), 0.0001 );
	CPPUNIT_ASSERT_DOUBLES_EQUAL( 84.9927, rec1->getLoss(trans2).get_dB(), 0.0001 );
	CPPUNIT_ASSERT_DOUBLES_EQUAL( 42, rec2->getLoss(trans2).get_dB(), 0.0001 );

	// Receivers must receive a signal equivalent to background noise
	CPPUNIT_ASSERT_DOUBLES_EQUAL( -97.9897, rec1->getTotalRxPower().get_dBm(), 0.0001 );
	CPPUNIT_ASSERT_DOUBLES_EQUAL( -97.9897, rec2->getTotalRxPower().get_dBm(), 0.0001 );

	BroadcastTransmissionObjectPtr b(new BroadcastTransmissionObject(trans1, wns::osi::PDUPtr(), trans1->getTxPower(), 1));
	CPPUNIT_ASSERT(!rec1->contains(b));
	CPPUNIT_ASSERT(!rec2->contains(b));
	trans1->startTransmitting(b);
	CPPUNIT_ASSERT(rec1->contains(b));
	CPPUNIT_ASSERT(rec2->contains(b));
	CPPUNIT_ASSERT_DOUBLES_EQUAL( -32, rec1->getTotalRxPower().get_dBm(), 0.0001 );
	CPPUNIT_ASSERT_DOUBLES_EQUAL( -32, rec1->getRxPower(b).get_dBm(), 0.0001 );
	CPPUNIT_ASSERT_DOUBLES_EQUAL( -97.9897, rec1->getInterference(b).get_dBm(), 0.0001 );
	CPPUNIT_ASSERT_DOUBLES_EQUAL( -74.971, rec2->getTotalRxPower().get_dBm(), 0.0001 );
	CPPUNIT_ASSERT_DOUBLES_EQUAL( -74.9927, rec2->getRxPower(b).get_dBm(), 0.0001 );
	CPPUNIT_ASSERT_DOUBLES_EQUAL( -97.9897, rec2->getInterference(b).get_dBm(), 0.0001 );

	BroadcastTransmissionObjectPtr b2(new BroadcastTransmissionObject(trans2, wns::osi::PDUPtr(), trans2->getTxPower(), 1));
	CPPUNIT_ASSERT(!rec1->contains(b2));
	CPPUNIT_ASSERT(!rec2->contains(b2));
	trans2->startTransmitting(b2);
	CPPUNIT_ASSERT(rec1->contains(b));
	CPPUNIT_ASSERT(rec2->contains(b));
	CPPUNIT_ASSERT(rec1->contains(b2));
	CPPUNIT_ASSERT(rec2->contains(b2));
	CPPUNIT_ASSERT_DOUBLES_EQUAL( -31.9978, rec1->getTotalRxPower().get_dBm(), 0.0001 );
	CPPUNIT_ASSERT_DOUBLES_EQUAL( -32, rec1->getRxPower(b).get_dBm(), 0.0001 );
	CPPUNIT_ASSERT_DOUBLES_EQUAL( -64.9927, rec1->getRxPower(b2).get_dBm(), 0.0001 );
	CPPUNIT_ASSERT_DOUBLES_EQUAL( -64.9905, rec1->getInterference(b).get_dBm(), 0.0001 );
	CPPUNIT_ASSERT_DOUBLES_EQUAL( -32, rec1->getInterference(b2).get_dBm(), 0.0001 );

	CPPUNIT_ASSERT_DOUBLES_EQUAL( -22, rec2->getTotalRxPower().get_dBm(), 0.0001 );
	CPPUNIT_ASSERT_DOUBLES_EQUAL( -74.9927, rec2->getRxPower(b).get_dBm(), 0.0001 );
	CPPUNIT_ASSERT_DOUBLES_EQUAL( -22, rec2->getRxPower(b2).get_dBm(), 0.0001 );
	CPPUNIT_ASSERT_DOUBLES_EQUAL( -22, rec2->getInterference(b).get_dBm(), 0.0001 );
	CPPUNIT_ASSERT_DOUBLES_EQUAL( -74.971, rec2->getInterference(b2).get_dBm(), 0.0001 );

	trans1->stopTransmitting(b);
	CPPUNIT_ASSERT(!rec1->contains(b));
	CPPUNIT_ASSERT(!rec2->contains(b));
	CPPUNIT_ASSERT(rec1->contains(b2));
	CPPUNIT_ASSERT(rec2->contains(b2));
	CPPUNIT_ASSERT_DOUBLES_EQUAL( -64.9905, rec1->getTotalRxPower().get_dBm(), 0.0001 );

	trans2->stopTransmitting(b2);
	CPPUNIT_ASSERT(!rec1->contains(b));
	CPPUNIT_ASSERT(!rec2->contains(b));
	CPPUNIT_ASSERT(!rec1->contains(b2));
	CPPUNIT_ASSERT(!rec2->contains(b2));

	// Receivers must receive a signal equivalent to background noise
	CPPUNIT_ASSERT_DOUBLES_EQUAL( -97.9897, rec1->getTotalRxPower().get_dBm(), 0.0001 );
	CPPUNIT_ASSERT_DOUBLES_EQUAL( -97.9897, rec2->getTotalRxPower().get_dBm(), 0.0001 );

	// Receivers must receive a signal equivalent to background noise
	CPPUNIT_ASSERT( rec1->getNoise() == rec1->getTotalRxPower() );
	CPPUNIT_ASSERT( rec2->getNoise() == rec2->getTotalRxPower() );
}


