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
#include <RISE/medium/tests/PhysicalResourceTest.hpp>
#include <RISE/transmissionobjects/broadcasttransmissionobject.hpp>

using namespace rise::medium;
using namespace rise::medium::tests;

CPPUNIT_TEST_SUITE_REGISTRATION( PhysicalResourceTest );

PhysicalResourceTest::PhysicalResourceTest()
{
}

PhysicalResourceTest::~PhysicalResourceTest()
{
}

void PhysicalResourceTest::setUp()
{
	Medium::getInstance()->reset();
}

void PhysicalResourceTest::tearDown()
{
}

void PhysicalResourceTest::testAttachReceiver()
{
	// a PhysicalResource for testing
	PhysicalResource* pr = Medium::getInstance()->getPhysicalResource(5000, 20);

	// create and attach first observer
	TestObserver observer;
	pr->attach(&observer);

	// no transmissions
	CPPUNIT_ASSERT_EQUAL( static_cast<size_t>(0), observer.transmissions.size() );

	// start a transmission
	BroadcastTransmissionObjectPtr bto =
		BroadcastTransmissionObjectPtr(new BroadcastTransmissionObject(NULL, wns::osi::PDUPtr(), wns::Power()));
	pr->startTransmission(bto);

	// received 1 transmission
	CPPUNIT_ASSERT_EQUAL( static_cast<size_t>(1), observer.transmissions.size() );

	// create and attach 2nd observer
	TestObserver observer2;
	pr->attach(&observer2);

	// no transmissions so far
	CPPUNIT_ASSERT_EQUAL( static_cast<size_t>(0), observer2.transmissions.size() );

	BroadcastTransmissionObjectPtr bto2 =
		BroadcastTransmissionObjectPtr(new BroadcastTransmissionObject(NULL, wns::osi::PDUPtr(), wns::Power()));
	pr->startTransmission(bto2);

	// 2 and 1 transmission(s)
	CPPUNIT_ASSERT_EQUAL( static_cast<size_t>(2), observer.transmissions.size() );
	CPPUNIT_ASSERT_EQUAL( static_cast<size_t>(1), observer2.transmissions.size() );
}


void PhysicalResourceTest::testDetachReceiver()
{
	// a PhysicalResource for testing
	PhysicalResource* pr = Medium::getInstance()->getPhysicalResource(5000, 20);

	// create and attach two observer
	TestObserver observer;
	TestObserver observer2;
	pr->attach(&observer);
	pr->attach(&observer2);

	// no transmissions should be in list at startup
	CPPUNIT_ASSERT_EQUAL( static_cast<size_t>(0), observer.transmissions.size() );
	CPPUNIT_ASSERT_EQUAL( static_cast<size_t>(0), observer2.transmissions.size() );

	pr->detach(&observer);

	BroadcastTransmissionObjectPtr bto =
		BroadcastTransmissionObjectPtr(new BroadcastTransmissionObject(NULL, wns::osi::PDUPtr(), wns::Power()));
	pr->startTransmission(bto);

	// check that no transmissions are received (we're not attached any longer)
	CPPUNIT_ASSERT_EQUAL( static_cast<size_t>(0), observer.transmissions.size() );
	// observer2 was attached so it should have one transmission
	CPPUNIT_ASSERT_EQUAL( static_cast<size_t>(1), observer2.transmissions.size() );
}


void PhysicalResourceTest::testStartTransmission()
{
	TestObserver observer;
	PhysicalResource* pr = Medium::getInstance()->getPhysicalResource(5000, 20);
	PhysicalResource* pr2 = Medium::getInstance()->getPhysicalResource(2000, 5);
	pr->attach(&observer);
	pr2->attach(&observer);

	// no transmissions should be in list at startup
	CPPUNIT_ASSERT_EQUAL( static_cast<size_t>(0), observer.transmissions.size() );

	BroadcastTransmissionObjectPtr bto =
		BroadcastTransmissionObjectPtr(new BroadcastTransmissionObject(NULL, wns::osi::PDUPtr(), wns::Power()));
	pr->startTransmission(bto);

	// one transmission was received via pr ...
	CPPUNIT_ASSERT_EQUAL( static_cast<size_t>(1), observer.transmissions.size() );

	BroadcastTransmissionObjectPtr bto2 =
		BroadcastTransmissionObjectPtr(new BroadcastTransmissionObject(NULL, wns::osi::PDUPtr(), wns::Power()));
	pr2->startTransmission(bto2);

	// ... and another one via pr2
	CPPUNIT_ASSERT_EQUAL( static_cast<size_t>(2), observer.transmissions.size() );

	CPPUNIT_ASSERT( *(observer.transmissions.at(0)->getPhysicalResource()) == *pr );
	CPPUNIT_ASSERT( *(observer.transmissions.at(1)->getPhysicalResource()) == *pr2 );
}


void PhysicalResourceTest::testStopTransmission()
{
	TestObserver observer;
	PhysicalResource* pr = Medium::getInstance()->getPhysicalResource(5000, 20);
	PhysicalResource* pr2 = Medium::getInstance()->getPhysicalResource(2000, 5);
	pr->attach(&observer);
	pr2->attach(&observer);

	// no transmissions should be in list at startup
	CPPUNIT_ASSERT_EQUAL( static_cast<size_t>(0), observer.transmissions.size() );

	BroadcastTransmissionObjectPtr bto =
		BroadcastTransmissionObjectPtr(new BroadcastTransmissionObject(NULL, wns::osi::PDUPtr(), wns::Power()));
	pr->startTransmission(bto);

	BroadcastTransmissionObjectPtr bto2 =
		BroadcastTransmissionObjectPtr(new BroadcastTransmissionObject(NULL, wns::osi::PDUPtr(), wns::Power()));
	pr2->startTransmission(bto2);


	pr->stopTransmission(bto);
	// one still in list
	CPPUNIT_ASSERT_EQUAL( static_cast<size_t>(1), observer.transmissions.size() );

	pr2->stopTransmission(bto2);
	// all removed
	CPPUNIT_ASSERT_EQUAL( static_cast<size_t>(0), observer.transmissions.size() );
}


void PhysicalResourceTest::testMobilityUpdate()
{
	TestObserver observer;
	PhysicalResource* pr = Medium::getInstance()->getPhysicalResource(5000, 20);
	PhysicalResource* pr2 = Medium::getInstance()->getPhysicalResource(2000, 5);
	pr->attach(&observer);
	pr2->attach(&observer);

	// no mobility update
	CPPUNIT_ASSERT_EQUAL( static_cast<size_t>(0), observer.movedTransmitters.size() );

	pr->mobilityUpdate(NULL);
	// one mobility update
	CPPUNIT_ASSERT_EQUAL( static_cast<size_t>(1), observer.movedTransmitters.size() );

	pr2->mobilityUpdate(NULL);
	// two mobility update
	CPPUNIT_ASSERT_EQUAL( static_cast<size_t>(2), observer.movedTransmitters.size() );
}


void PhysicalResourceTest::testComparePhysicalResources()
{
	PhysicalResource* p = Medium::getInstance()->getPhysicalResource(5000, 20);
	PhysicalResource* p2 = Medium::getInstance()->getPhysicalResource(5000, 20);
	PhysicalResource* p3 = Medium::getInstance()->getPhysicalResource(2000, 5);
	PhysicalResource* p4 = Medium::getInstance()->getPhysicalResource(2000, 5);

	CPPUNIT_ASSERT( *p == *p2 );
	CPPUNIT_ASSERT( *p3 == *p4 );

	CPPUNIT_ASSERT( *p != *p4 );
}


void PhysicalResourceTest::testIterator()
{
	PhysicalResource* pr = Medium::getInstance()->getPhysicalResource(5000, 20);

	BroadcastTransmissionObjectPtr bto =
		BroadcastTransmissionObjectPtr(new BroadcastTransmissionObject(NULL, wns::osi::PDUPtr(), wns::Power()));
	BroadcastTransmissionObjectPtr bto2 =
		BroadcastTransmissionObjectPtr(new BroadcastTransmissionObject(NULL, wns::osi::PDUPtr(), wns::Power()));

	// No transmissions active
	CPPUNIT_ASSERT( pr->getTOBegin() == pr->getTOEnd() );

	pr->startTransmission(bto);

	// one transmissions active
	CPPUNIT_ASSERT( pr->getTOBegin() != pr->getTOEnd() );
	CPPUNIT_ASSERT( ++(pr->getTOBegin()) == pr->getTOEnd() );
	CPPUNIT_ASSERT( (*(*(pr->getTOBegin()))->getPhysicalResource()) == *pr );

	pr->startTransmission(bto2);
	// two transmissions active
	CPPUNIT_ASSERT( pr->getTOBegin() != pr->getTOEnd() );
	CPPUNIT_ASSERT( ++(++(pr->getTOBegin())) == pr->getTOEnd() );
	CPPUNIT_ASSERT( (*(*(++(pr->getTOBegin())))->getPhysicalResource()) == *pr );
}


void PhysicalResourceTest::testContains()
{
	PhysicalResource* pr = Medium::getInstance()->getPhysicalResource(5000, 20);

	BroadcastTransmissionObjectPtr bto =
		BroadcastTransmissionObjectPtr(new BroadcastTransmissionObject(NULL, wns::osi::PDUPtr(), wns::Power()));
	BroadcastTransmissionObjectPtr bto2 =
		BroadcastTransmissionObjectPtr(new BroadcastTransmissionObject(NULL, wns::osi::PDUPtr(), wns::Power()));

	CPPUNIT_ASSERT( !pr->contains(bto) );
	CPPUNIT_ASSERT( !pr->contains(bto2) );

	pr->startTransmission(bto);
	CPPUNIT_ASSERT( pr->contains(bto) );
	CPPUNIT_ASSERT( !pr->contains(bto2) );

	pr->startTransmission(bto2);
	CPPUNIT_ASSERT( pr->contains(bto) );
	CPPUNIT_ASSERT( pr->contains(bto2) );

	pr->stopTransmission(bto);
	CPPUNIT_ASSERT( !pr->contains(bto) );
	CPPUNIT_ASSERT( pr->contains(bto2) );

	pr->stopTransmission(bto2);
	CPPUNIT_ASSERT( !pr->contains(bto) );
	CPPUNIT_ASSERT( !pr->contains(bto2) );
}


void PhysicalResourceTest::testGetter()
{
	PhysicalResource* p = Medium::getInstance()->getPhysicalResource(5000, 20);
	PhysicalResource* p2 = Medium::getInstance()->getPhysicalResource(2000, 5);

	CPPUNIT_ASSERT_DOUBLES_EQUAL( 5000, p->getFrequency(), 0 );
	CPPUNIT_ASSERT_DOUBLES_EQUAL( 2000, p2->getFrequency(), 0 );

	CPPUNIT_ASSERT_DOUBLES_EQUAL( 20, p->getBandwidth(), 0 );
	CPPUNIT_ASSERT_DOUBLES_EQUAL( 5, p2->getBandwidth(), 0 );

 	CPPUNIT_ASSERT( wns::Interval<double>::Between(4990).And(5010) == p->getFrequencyRange() );
 	CPPUNIT_ASSERT( wns::Interval<double>::Between(1997.5).And(2002.5) == p2->getFrequencyRange() );

}


