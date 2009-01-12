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

#ifndef _ANTENNATEST_HPP
#define _ANTENNATEST_HPP

#include <RISE/antenna/Antenna.hpp>
#include <RISE/manager/tests/SystemManagerDropIn.hpp>
#include <WNS/TestFixture.hpp>

namespace rise { namespace antenna {
	class AntennaTest
		: public wns::TestFixture
	{
		CPPUNIT_TEST_SUITE( AntennaTest );
		CPPUNIT_TEST( testConfig );
		CPPUNIT_TEST( testGetPosition );
		CPPUNIT_TEST( testIsotropic );
		CPPUNIT_TEST( testOmniDirectional );
		CPPUNIT_TEST( testLambdaHalf );
		CPPUNIT_TEST( testAzimuth );
		CPPUNIT_TEST_SUITE_END();
	public:
                AntennaTest();
                ~AntennaTest();
		void prepare();
		void cleanup();
		void testConfig();
		void testGetPosition();
		void testIsotropic();
		void testOmniDirectional();
		void testDirected13();
		void testDirected17();
		void testDirected30();
		void testDirected66();
		void testDirected90();
		void testDirected120();
		void testLambdaHalf();
		void testAzimuth();

	private:
		wns::Position calcPosition(const Antenna& a, double azimuth, double evelvation);
		std::vector<double> lambdaHalf;
		tests::SystemManagerDropIn* systemManager;
	};
}
}

#endif


