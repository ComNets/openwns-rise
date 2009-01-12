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

#include <RISE/receiver/tests/TimeWeightedTransmissionAveragingTest.hpp>
#include <RISE/transmissionobjects/broadcasttransmissionobject.hpp>
#include <WNS/events/NoOp.hpp>

using namespace rise;
using namespace rise::receiver;
using namespace rise::receiver::tests;

CPPUNIT_TEST_SUITE_REGISTRATION( TimeWeightedTransmissionAveragingTest );

TimeWeightedTransmissionAveragingTest::TimeWeightedTransmissionAveragingTest()
{
}

TimeWeightedTransmissionAveragingTest::~TimeWeightedTransmissionAveragingTest()
{
}

void TimeWeightedTransmissionAveragingTest::setUp()
{
	// fresh TestAverager
	t = TestAverager();
	t.rxPower = wns::Power::from_mW(200);
	t.interference = wns::Power::from_mW(1);
	wns::simulator::getEventScheduler()->reset();

	// configure transmission
	bto = BroadcastTransmissionObjectPtr(new BroadcastTransmissionObject(NULL, wns::osi::PDUPtr(), wns::Power()));
	bto->setIsStart(true);
	// Add the Transmission to the averager
	t.add(bto);
}

void TimeWeightedTransmissionAveragingTest::tearDown()
{
	// Remove the Transmission from the averager
	t.remove(bto);

	// There should be no transmission left, but this should do no harm
	t.removeAll();
}

void TimeWeightedTransmissionAveragingTest::testConstantSignalLevel()
{
	// Check if the TestAverager is working
	CPPUNIT_ASSERT( t.getRxPower(bto) == t.rxPower );
	CPPUNIT_ASSERT( t.getInterference(bto) == t.interference );

	// If we would ask for the averaged values right after we added them to
	// the averager this would result in an assertion. You may only ask
	// after the time has advanced.
	// advance in time
	wns::simulator::getEventScheduler()->scheduleDelay(wns::events::NoOp(), 0.5);
	wns::simulator::getEventScheduler()->processOneEvent();
	CPPUNIT_ASSERT_DOUBLES_EQUAL(0.5, wns::simulator::getEventScheduler()->getTime(), 0);

	CPPUNIT_ASSERT( t.getAveragedRxPower(bto) == t.rxPower );
	CPPUNIT_ASSERT( t.getAveragedInterference(bto) == t.interference );
	CPPUNIT_ASSERT( t.getAveragedCIR(bto) == t.rxPower/t.interference );

	// advance in time
	wns::simulator::getEventScheduler()->scheduleDelay(wns::events::NoOp(), 0.5);
	wns::simulator::getEventScheduler()->processOneEvent();
	CPPUNIT_ASSERT_DOUBLES_EQUAL(1.0, wns::simulator::getEventScheduler()->getTime(), 0);

	CPPUNIT_ASSERT( t.getAveragedRxPower(bto) == t.rxPower );
	CPPUNIT_ASSERT( t.getAveragedInterference(bto) == t.interference );
	CPPUNIT_ASSERT( t.getAveragedCIR(bto) == t.rxPower/t.interference );
}

void TimeWeightedTransmissionAveragingTest::testSignalLevelChanges()
{
	wns::simulator::getEventScheduler()->scheduleDelay(wns::events::NoOp(), 0.5);
	wns::simulator::getEventScheduler()->processOneEvent();
	CPPUNIT_ASSERT_DOUBLES_EQUAL(0.5, wns::simulator::getEventScheduler()->getTime(), 0);

	CPPUNIT_ASSERT( t.getAveragedRxPower(bto) == t.rxPower );
	CPPUNIT_ASSERT( t.getAveragedInterference(bto) == t.interference );
	CPPUNIT_ASSERT( t.getAveragedCIR(bto) == t.rxPower/t.interference );

	// change values
	t.rxPower = wns::Power::from_mW(800);
	t.interference = wns::Power::from_mW(2);
	t.signalLevelsChange();

	// advance in time
	wns::simulator::getEventScheduler()->scheduleDelay(wns::events::NoOp(), 0.5);
	wns::simulator::getEventScheduler()->processOneEvent();
	CPPUNIT_ASSERT_DOUBLES_EQUAL(1.0, wns::simulator::getEventScheduler()->getTime(), 0);

	CPPUNIT_ASSERT( t.getAveragedRxPower(bto) == wns::Power::from_mW(500) );
	CPPUNIT_ASSERT( t.getAveragedInterference(bto) == wns::Power::from_mW(1.5) );
	CPPUNIT_ASSERT( t.getAveragedCIR(bto) == wns::Power::from_mW(500)/wns::Power::from_mW(1.5) );
}

void TimeWeightedTransmissionAveragingTest::testSignalLevelChangesAfterEndOfTransmission()
{
	wns::simulator::getEventScheduler()->scheduleDelay(wns::events::NoOp(), 0.5);
	wns::simulator::getEventScheduler()->processOneEvent();
	CPPUNIT_ASSERT_DOUBLES_EQUAL(0.5, wns::simulator::getEventScheduler()->getTime(), 0);

	CPPUNIT_ASSERT( t.getAveragedRxPower(bto) == t.rxPower );
	CPPUNIT_ASSERT( t.getAveragedInterference(bto) == t.interference );
	CPPUNIT_ASSERT( t.getAveragedCIR(bto) == t.rxPower/t.interference );

	// change values
	bto->setIsStart(false);
	t.endOfTransmission(bto);
	t.rxPower = wns::Power::from_mW(800);
	t.interference = wns::Power::from_mW(2);
	t.signalLevelsChange();

	// advance in time
	wns::simulator::getEventScheduler()->scheduleDelay(wns::events::NoOp(), 0.5);
	wns::simulator::getEventScheduler()->processOneEvent();
	CPPUNIT_ASSERT_DOUBLES_EQUAL(1.0, wns::simulator::getEventScheduler()->getTime(), 0);

	CPPUNIT_ASSERT( t.getAveragedRxPower(bto) == wns::Power::from_mW(200) );
	CPPUNIT_ASSERT( t.getAveragedInterference(bto) == wns::Power::from_mW(1) );
	CPPUNIT_ASSERT( t.getAveragedCIR(bto) == wns::Power::from_mW(200)/wns::Power::from_mW(1) );
}


