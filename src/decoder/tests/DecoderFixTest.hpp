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

#ifndef classDecoderfixTest
#define classDecoderfixTest

#include <RISE/decoder/DecoderFix.hpp>
#include <WNS/Exception.hpp>

#include <cppunit/extensions/HelperMacros.h>
#include <stdexcept>

namespace rise {
	class DecoderFixTest :
		public CppUnit::TestFixture
	{
		CPPUNIT_TEST_SUITE( DecoderFixTest );
		CPPUNIT_TEST( testFunctions );
#ifdef WNS_ASSURE_THROWS_EXCEPTION
		CPPUNIT_TEST_EXCEPTION( createDecoderFixTooBigBER, wns::Assure::Exception );
		CPPUNIT_TEST_EXCEPTION( createDecoderFixTooSmallBER, wns::Assure::Exception );
#endif // WNS_ASSURE_THROWS_EXCEPTION
		CPPUNIT_TEST_SUITE_END();
	private:
		DecoderFix d;
		DecoderFix d2;

	public:
		void setUp();
		void tearDown();
		void testFunctions();
		void createDecoderFixTooBigBER();
		void createDecoderFixTooSmallBER();
	};
}
#endif


