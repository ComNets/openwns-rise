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

#include <RISE/decoder/tests/DecoderFixTest.hpp>

using namespace rise;

CPPUNIT_TEST_SUITE_REGISTRATION( DecoderFixTest );

void DecoderFixTest::setUp()
{
		d = DecoderFix();
		d2 = DecoderFix(0.2);
}

void DecoderFixTest::tearDown()
{}

void DecoderFixTest::testFunctions()
{
		double r=0.1;
		CPPUNIT_ASSERT( d.getBER(r) == 0.1 );
		CPPUNIT_ASSERT( d.getBER(r,2.0) == 0.2 );
		CPPUNIT_ASSERT( d2.getBER(r) == 0.2 );
		CPPUNIT_ASSERT( d2.getBER(r,2.0) == 0.4 );
		CPPUNIT_ASSERT( d2.getBER(r,3.0) == 0.5 );
		CPPUNIT_ASSERT( d2.getBER(r) > d.getBER(r) );
		r=0.2;
		CPPUNIT_ASSERT( d.getBER(r) == 0.1 );
		CPPUNIT_ASSERT( d.getBER(r,2.0) == 0.2 );
		CPPUNIT_ASSERT( d2.getBER(r) == 0.2 );
		CPPUNIT_ASSERT( d2.getBER(r,2.0) == 0.4 );
		CPPUNIT_ASSERT( d2.getBER(r,3.0) == 0.5 );
		CPPUNIT_ASSERT( d2.getBER(r) > d.getBER(r) );
}

void DecoderFixTest::createDecoderFixTooBigBER()
{
		d=DecoderFix(0.6);
}
void DecoderFixTest::createDecoderFixTooSmallBER()
{
		d=DecoderFix(-0.1);
}


