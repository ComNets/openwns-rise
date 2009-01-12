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

#ifndef _RECEIVERTEST_HPP
#define _RECEIVERTEST_HPP

#include <RISE/transceiver/tests/TransmitterDropIn.hpp>
#include <RISE/transceiver/tests/ReceiverDropIn.hpp>
#include <RISE/manager/tests/SystemManagerDropIn.hpp>
#include <RISE/stations/tests/StationDropIn.hpp>
#include <WNS/Module.hpp>

#include <cppunit/extensions/HelperMacros.h>

namespace rise { namespace tests {
	class ReceiverTest :
		public CppUnit::TestFixture
	{
		CPPUNIT_TEST_SUITE( ReceiverTest );
		CPPUNIT_TEST( all );
		CPPUNIT_TEST_SUITE_END();
	public:
		ReceiverTest();
		~ReceiverTest();
		void setUp();
		void tearDown();
		void all();
	private:
		SystemManagerDropIn systemManagerDropIn;
		StationDropIn station1;
		StationDropIn station2;
		ReceiverDropIn rec1;
		ReceiverDropIn rec2;
		TransmitterDropIn trans1;
		TransmitterDropIn trans2;
	}; // class ReceiverTest
} // namespace tests
} // namespace rise
#endif


