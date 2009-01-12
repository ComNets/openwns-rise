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

#ifndef _ANTENNABFTEST_HPP
#define _ANTENNABFTEST_HPP

#include <RISE/antenna/Beamforming.hpp>
#include <RISE/manager/tests/SystemManagerDropIn.hpp>
#include <WNS/TestFixture.hpp>

namespace rise { namespace tests {
	class BeamformingTest
		: public wns::TestFixture
	{
		CPPUNIT_TEST_SUITE( BeamformingTest );
		CPPUNIT_TEST( simple );
		CPPUNIT_TEST( testAlgorithm );
		CPPUNIT_TEST( testPositionError );
		CPPUNIT_TEST( testCircularLayout );
		CPPUNIT_TEST_SUITE_END();
	public:
                BeamformingTest();
                ~BeamformingTest();
		void prepare();
		void cleanup();
		void simple();
		void testAlgorithm();
		void testPositionError();
		void testCircularLayout();
	private:
		tests::SystemManagerDropIn* manager;
	};
}}
#endif


