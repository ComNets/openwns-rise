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

#ifndef __RISE_MEDIUMTEST
#define __RISE_MEDIUMTEST

#include <RISE/medium/PhysicalResource.hpp>
#include <RISE/medium/Medium.hpp>

#include <WNS/Exception.hpp>

#include <cppunit/extensions/HelperMacros.h>

namespace rise { namespace medium { namespace tests {

    class MediumTest :
		public CppUnit::TestFixture  {

		CPPUNIT_TEST_SUITE( MediumTest );
		CPPUNIT_TEST( testGetPhysicalResource );
		CPPUNIT_TEST( testOverlapForDiffFreqNoOverlap );
		CPPUNIT_TEST( testOverlapForF1ubEqualF2lb );
		CPPUNIT_TEST_EXCEPTION( testOverlapForEqualFreqDiffBandwidths, wns::Exception );
		CPPUNIT_TEST_EXCEPTION( testOverlapForDiffFreqWithOverlap, wns::Exception );
		CPPUNIT_TEST_EXCEPTION( testOverlapForManyFreqOneWithOverlap, wns::Exception );
		CPPUNIT_TEST_SUITE_END();

	public:
		void setUp();
		void tearDown();
		void testGetPhysicalResource();
		void testOverlapForDiffFreqNoOverlap();
		void testOverlapForEqualFreqDiffBandwidths();
		void testOverlapForF1ubEqualF2lb();
		void testOverlapForDiffFreqWithOverlap();
		void testOverlapForManyFreqOneWithOverlap();
	};
}}}
#endif //classMediumTest


