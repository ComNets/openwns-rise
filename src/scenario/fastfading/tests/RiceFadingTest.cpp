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

#include <RISE/scenario/tests/ScenarioTestsBase.hpp>
#include <RISE/scenario/fastfading/RiceFading.hpp>
#include <RISE/scenario/fastfading/FastFading.hpp>

#include <WNS/Types.hpp>
#include <WNS/CppUnit.hpp>
#include <WNS/pyconfig/helper/Functions.hpp>

#include <RISE/scenario/pathloss/Deny.hpp>

#include <cppunit/extensions/HelperMacros.h>

namespace rise { namespace scenario { namespace fastfading { namespace tests {

    class RiceFadingTest :
	public rise::scenario::tests::ScenarioTestsBase,
	public wns::TestFixture
    {
	CPPUNIT_TEST_SUITE( RiceFadingTest );
	CPPUNIT_TEST( testOutput );
	CPPUNIT_TEST_SUITE_END();
    public:
	virtual void prepare();
	virtual void cleanup();
	void testOutput();
    private:
	Rice* fastFading;
	bool deleteFastFading;
    };

    CPPUNIT_TEST_SUITE_REGISTRATION( RiceFadingTest );

    void RiceFadingTest::prepare()
    {
	deleteFastFading = false;
	ScenarioTestsBase::prepare();
	fastFading = new Rice(wns::pyconfig::helper::createViewFromDropInConfig("rise.scenario.FastFading",
										"Rice"));
	deleteFastFading = true;
    }

    void RiceFadingTest::cleanup()
    {
	if (deleteFastFading) delete fastFading;
	ScenarioTestsBase::cleanup();
    }

    void RiceFadingTest::testOutput()
    {
	CPPUNIT_ASSERT( fastFading->getFastFading().get_dB() >= 0);
	CPPUNIT_ASSERT( fastFading->getFastFading().get_dB() >= 0);
	CPPUNIT_ASSERT( fastFading->getFastFading().get_dB() >= 0);
    }

}}}}
