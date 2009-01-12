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

#include <RISE/demodulator/tests/DemodulatorBPSKTest.hpp>

using namespace std;
using namespace rise;
using namespace wns;

CPPUNIT_TEST_SUITE_REGISTRATION( DemodulatorBPSKTest );

void DemodulatorBPSKTest::setUp()
{
		r = wns::Ratio();
		r2 = wns::Ratio();
		d = DemodulatorBPSK();
}

void DemodulatorBPSKTest::tearDown()
{}

void DemodulatorBPSKTest::testFunctions()
{
 		r.set_dB(4.3);
 		CPPUNIT_ASSERT_DOUBLES_EQUAL( 1E-2, d.getBER(r), 2E-4 );
  		r.set_dB(6.8);
 		CPPUNIT_ASSERT_DOUBLES_EQUAL( 1E-3, d.getBER(r), 2E-5 );
  		r.set_dB(8.4);
 		CPPUNIT_ASSERT_DOUBLES_EQUAL( 1E-4, d.getBER(r), 2E-6 );
  		r.set_dB(9.6);
 		CPPUNIT_ASSERT_DOUBLES_EQUAL( 1E-5, d.getBER(r), 3E-7 );
  		r.set_dB(10.5);
 		CPPUNIT_ASSERT_DOUBLES_EQUAL( 1E-6, d.getBER(r), 9E-8 );
 		r.set_dB(9.6);
 		r2.set_dB(10.5);
 		CPPUNIT_ASSERT ( d.getBER(r2) < d.getBER(r) );
}


