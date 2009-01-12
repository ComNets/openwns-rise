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

#ifndef _SCRECEIVERTEST_HPP
#define _SCRECEIVERTEST_HPP

#include <RISE/manager/tests/SystemManagerDropIn.hpp>
#include <RISE/transceiver/tests/SCTransmitterDropIn.hpp>
#include <RISE/transceiver/tests/SCReceiverDropIn.hpp>
#include <RISE/stations/tests/StationDropIn.hpp>

#include <cppunit/extensions/HelperMacros.h>

namespace rise { namespace tests {
	class SCReceiverTest :
		public CppUnit::TestFixture
	{
		CPPUNIT_TEST_SUITE( SCReceiverTest );
		CPPUNIT_TEST( all );
		CPPUNIT_TEST_SUITE_END();
	public:
		void setUp();
		void tearDown();
		void all();
	private:
		SystemManagerDropIn* systemManagerDropIn;
		StationDropIn* station1;
		StationDropIn* station2;
		SCReceiverDropIn* rec1;
		SCReceiverDropIn* rec2;
		SCTransmitterDropIn* trans1;
		SCTransmitterDropIn* trans2;
	}; // class SCReceiverTest
} // namespace tests
} // namespace rise
#endif


