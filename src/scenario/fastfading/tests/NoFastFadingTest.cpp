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
#include <RISE/scenario/fastfading/NoFastFading.hpp>
#include <RISE/scenario/fastfading/FastFading.hpp>
#include <RISE/scenario/pathloss/Deny.hpp>

#include <WNS/Types.hpp>
#include <WNS/CppUnit.hpp>
#include <WNS/pyconfig/helper/Functions.hpp>

#include <cppunit/extensions/HelperMacros.h>

namespace rise { namespace scenario { namespace fastfading { namespace tests {

    class NoneTest :
	public rise::scenario::tests::ScenarioTestsBase,
	public wns::TestFixture
    {
	CPPUNIT_TEST_SUITE( NoneTest );
	CPPUNIT_TEST( testOutput );
	CPPUNIT_TEST_SUITE_END();
    public:
	virtual void prepare();
	virtual void cleanup();
	void testOutput();
    private:
	None* fastFading;
	bool deleteFastFading;
    };

    CPPUNIT_TEST_SUITE_REGISTRATION( NoneTest );

    void NoneTest::prepare()
    {
	deleteFastFading = false;
	ScenarioTestsBase::prepare();
	fastFading = new None(wns::pyconfig::helper::createViewFromDropInConfig("rise.scenario.FastFading",
										"No"));
	deleteFastFading = true;
    }

    void NoneTest::cleanup()
    {
	if (deleteFastFading) delete fastFading;
	ScenarioTestsBase::cleanup();
    }

    void NoneTest::testOutput()
    {
	WNS_ASSERT_MAX_REL_ERROR( 0.0, fastFading->getFastFading().get_dB(), 1E-5 );
    }

}}}}
