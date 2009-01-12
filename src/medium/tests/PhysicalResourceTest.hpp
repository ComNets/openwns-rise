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

#ifndef _PHYSICALRESOURCETEST_HPP
#define _PHYSICALRESOURCETEST_HPP

#include <RISE/medium/PhysicalResource.hpp>
#include <RISE/receiver/ReceiverInterface.hpp>

#include <vector>

#include <cppunit/extensions/HelperMacros.h>

namespace rise { namespace medium { namespace tests {
	class PhysicalResourceTest :
		public CppUnit::TestFixture
	{
		CPPUNIT_TEST_SUITE( PhysicalResourceTest );
		CPPUNIT_TEST( testAttachReceiver );
		CPPUNIT_TEST( testDetachReceiver );
		CPPUNIT_TEST( testStartTransmission );
		CPPUNIT_TEST( testStopTransmission );
		CPPUNIT_TEST( testMobilityUpdate );
		CPPUNIT_TEST( testComparePhysicalResources );
		CPPUNIT_TEST( testIterator );
		CPPUNIT_TEST( testContains );
		CPPUNIT_TEST( testGetter );
		CPPUNIT_TEST_SUITE_END();

		class TestObserver :
			public PhysicalResourceObserver
		{
		public:
			// PhysicalResourceObserver interface
			virtual void notify(TransmissionObjectPtr t)
			{
				if(t->getIsStart()) {
					transmissions.push_back(t);
				} else {
					transmissions.erase(std::find(transmissions.begin(),
								      transmissions.end(),
								      t));
				}
			}

			virtual void mobilityUpdate(Transmitter* t)
			{
				movedTransmitters.push_back(t);
			}

			std::vector<TransmissionObjectPtr> transmissions;
			std::vector<Transmitter*> movedTransmitters;
		};
	public:
                PhysicalResourceTest();
                ~PhysicalResourceTest();
		void setUp();
		void tearDown();
		void testAttachReceiver();
		void testDetachReceiver();
		void testStartTransmission();
		void testStopTransmission();
		void testMobilityUpdate();
		void testComparePhysicalResources();
		void testIterator();
		void testContains();
		void testGetter();

	private:
	};
}}}
#endif


