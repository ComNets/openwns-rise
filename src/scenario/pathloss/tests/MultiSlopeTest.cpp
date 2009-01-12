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

#include <RISE/scenario/pathloss/tests/PathlossTestsBase.hpp>
#include <RISE/scenario/pathloss/MultiSlope.hpp>
#include <RISE/scenario/pathloss/Pathloss.hpp>

#include <WNS/Types.hpp>
#include <WNS/CppUnit.hpp>
#include <WNS/pyconfig/helper/Functions.hpp>

#include <cppunit/extensions/HelperMacros.h>

namespace rise { namespace scenario { namespace pathloss { namespace tests {

    class MultiSlopeTest :
	public PathlossTestsBase,
	public wns::TestFixture
    {
	CPPUNIT_TEST_SUITE( MultiSlopeTest );
	CPPUNIT_TEST( testInput );
	CPPUNIT_TEST( testOutput );
	CPPUNIT_TEST_SUITE_END();
	using PathlossTestsBase::testInput;
    public:
	virtual void prepare();
	virtual void cleanup();
	void testInput();
	void testOutput();
    private:
	Pathloss* pathlossWINNERC1LOS;
	bool deletePathlossWINNERC1LOS;
    };

    CPPUNIT_TEST_SUITE_REGISTRATION( MultiSlopeTest );

    void MultiSlopeTest::prepare()
    {
	deletePathlossWINNERC1LOS = false;
	PathlossTestsBase::prepare();
	pathlossWINNERC1LOS = new MultiSlope(wns::pyconfig::helper::createViewFromDropInConfig("rise.scenario.Pathloss",
											  "WINNERC1LOSTest"));
	deletePathlossWINNERC1LOS = true;
    }

    void MultiSlopeTest::cleanup()
    {
	if (deletePathlossWINNERC1LOS) delete pathlossWINNERC1LOS;
	PathlossTestsBase::cleanup();
    }

    void MultiSlopeTest::testInput()
    {
	testInput(pathlossWINNERC1LOS);
    }

    void MultiSlopeTest::testOutput()
    {
	const wns::Frequency frequency = 3950;
	station1->moveTo(wns::Position(0, 0, 0));
	// Distance 25 m --> should be freespace
	station2->moveTo(wns::Position(0, 25, 0));
	WNS_ASSERT_MAX_REL_ERROR( 72.33074,
				  pathlossWINNERC1LOS->getPathloss(*antenna1, *antenna2, frequency).get_dB(),
				  1E-5 );

	// Distance 500 m --> should be slope 1 (LOS)
	station2->moveTo(wns::Position(0, 500, 0));
	WNS_ASSERT_MAX_REL_ERROR( 103.38803,
				  pathlossWINNERC1LOS->getPathloss(*antenna1, *antenna2, frequency).get_dB(),
				  1E-5 );

 	// Breakpoint at 1976.36726 m, left of breakpoint
	station2->moveTo(wns::Position(0, 1976.367, 0));
	WNS_ASSERT_MAX_REL_ERROR( 117.5942,
				  pathlossWINNERC1LOS->getPathloss(*antenna1, *antenna2, frequency).get_dB(),
				  1E-4 );
 	// right of breakpoint
	station2->moveTo(wns::Position(0, 1976.368, 0));
	WNS_ASSERT_MAX_REL_ERROR( 117.5942,
				  pathlossWINNERC1LOS->getPathloss(*antenna1, *antenna2, frequency).get_dB(),
				  1E-4 );

	// Distance Breakpoint 2500 m --> should be slope 2 (NLOS)
	station2->moveTo(wns::Position(0, 2500, 0));
	WNS_ASSERT_MAX_REL_ERROR( 121.6793,
				  pathlossWINNERC1LOS->getPathloss(*antenna1, *antenna2, frequency).get_dB(),
				  1E-4 );

    }

}}}}
