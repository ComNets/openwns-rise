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
#include <RISE/scenario/pathloss/Constant.hpp>
#include <RISE/scenario/pathloss/Pathloss.hpp>

#include <WNS/CppUnit.hpp>
#include <WNS/pyconfig/helper/Functions.hpp>

#include <cppunit/extensions/HelperMacros.h>

namespace rise { namespace scenario { namespace pathloss { namespace tests {

    class ConstantTest :
	public PathlossTestsBase,
	public wns::TestFixture
    {
	CPPUNIT_TEST_SUITE( ConstantTest );
	CPPUNIT_TEST( testInput );
	CPPUNIT_TEST( testOutput );
	CPPUNIT_TEST_SUITE_END();
	using PathlossTestsBase::testInput;
    public:
	virtual void prepare();
	virtual void cleanup();
	void testOutput();
	void testInput();
    private:
	Pathloss* pathlossPos;
	Pathloss* pathlossNeg;
	bool deletePathlossPos;
	bool deletePathlossNeg;
    };

    CPPUNIT_TEST_SUITE_REGISTRATION( ConstantTest );

    void ConstantTest::prepare()
    {
	deletePathlossPos = false;
	deletePathlossNeg = false;
	PathlossTestsBase::prepare();
	pathlossPos = new Constant(wns::pyconfig::helper::createViewFromDropInConfig("rise.scenario.Pathloss",
										     "ConstantTestPos"));
	deletePathlossPos = true;

	pathlossNeg = new Constant(wns::pyconfig::helper::createViewFromDropInConfig("rise.scenario.Pathloss",
										     "ConstantTestNeg"));
	deletePathlossNeg = true;
    }

    void ConstantTest::cleanup()
    {
	if (deletePathlossPos) delete pathlossPos;
	if (deletePathlossNeg) delete pathlossNeg;
	PathlossTestsBase::cleanup();
    }

    void ConstantTest::testInput()
    {
	testInput(pathlossPos);
	testInput(pathlossNeg);
    }

    void ConstantTest::testOutput()
    {
	WNS_ASSERT_MAX_REL_ERROR( 23.11, pathlossPos->getPathloss(*antenna1, *antenna2, 5000.0).get_dB(), 1E-5);
	WNS_ASSERT_MAX_REL_ERROR( 23.11, pathlossPos->getPathloss(*antenna1, *antenna2, 5.0).get_dB(), 1E-5);

	WNS_ASSERT_MAX_REL_ERROR( 0.0, pathlossNeg->getPathloss(*antenna1, *antenna2, 5000.0).get_dB(), 1E-5);
	WNS_ASSERT_MAX_REL_ERROR( 0.0, pathlossNeg->getPathloss(*antenna1, *antenna2, 5.0).get_dB(), 1E-5);
    }

}}}}
