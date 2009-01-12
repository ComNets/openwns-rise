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
#include <RISE/scenario/pathloss/SingleSlope.hpp>
#include <RISE/scenario/pathloss/Pathloss.hpp>

#include <WNS/Types.hpp>
#include <WNS/CppUnit.hpp>
#include <WNS/pyconfig/helper/Functions.hpp>

#include <cppunit/extensions/HelperMacros.h>

namespace rise { namespace scenario { namespace pathloss { namespace tests {

    class SingleSlopeTest :
	public PathlossTestsBase,
	public wns::TestFixture
    {
	CPPUNIT_TEST_SUITE( SingleSlopeTest );
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
	Pathloss* pathlossFreeSpace;
	Pathloss* pathlossUmts3003Outdoor;
	bool deletePathlossFreeSpace;
	bool deletePathlossUmts3003Outdoor;
    };

    CPPUNIT_TEST_SUITE_REGISTRATION( SingleSlopeTest );

    void SingleSlopeTest::prepare()
    {
	deletePathlossFreeSpace = false;
	deletePathlossUmts3003Outdoor = false;
	PathlossTestsBase::prepare();
	pathlossFreeSpace = new SingleSlope(wns::pyconfig::helper::createViewFromDropInConfig("rise.scenario.Pathloss",
											  "SingleSlopeTestFreeSpace"));
	deletePathlossFreeSpace = true;
	pathlossUmts3003Outdoor = new SingleSlope(wns::pyconfig::helper::createViewFromDropInConfig("rise.scenario.Pathloss",
												"Umts3003Outdoor"));
	deletePathlossUmts3003Outdoor = true;
    }

    void SingleSlopeTest::cleanup()
    {
	if (deletePathlossUmts3003Outdoor) delete pathlossUmts3003Outdoor;
	if (deletePathlossFreeSpace) delete pathlossFreeSpace;
	PathlossTestsBase::cleanup();
    }

    void SingleSlopeTest::testInput()
    {
	testInput(pathlossFreeSpace);
	testInput(pathlossUmts3003Outdoor);
    }

    void SingleSlopeTest::testOutput()
    {
	const wns::Frequency frequency = 2000;
	station1->moveTo(wns::Position(0, 0, 0));
	station2->moveTo(wns::Position(100, 100, 0));
	WNS_ASSERT_MAX_REL_ERROR( 81.4709,
				  pathlossFreeSpace->getPathloss(*antenna1, *antenna2, frequency).get_dB(),
				  1E-5 );
	WNS_ASSERT_MAX_REL_ERROR( 114.0515,
				  pathlossUmts3003Outdoor->getPathloss(*antenna1, *antenna2, frequency).get_dB(),
				  1E-5 );
	WNS_ASSERT_MAX_REL_ERROR( 75.4503,
				  pathlossFreeSpace->getPathloss(*antenna1, *antenna2, 1000).get_dB(),
				  1E-5 );
	WNS_ASSERT_MAX_REL_ERROR( 113.3832,
				  pathlossUmts3003Outdoor->getPathloss(*antenna1, *antenna2, 1900).get_dB(),
				  1E-5 );

	station2->moveTo(wns::Position(20000, 0, 0));
	WNS_ASSERT_MAX_REL_ERROR( 124.4812,
				  pathlossFreeSpace->getPathloss(*antenna1, *antenna2, frequency).get_dB(),
				  1E-5 );
	WNS_ASSERT_MAX_REL_ERROR( 200.0721,
				  pathlossUmts3003Outdoor->getPathloss(*antenna1, *antenna2, frequency).get_dB(),
				  1E-5 );

	station2->moveTo(wns::Position(0, 0, 0));
	WNS_ASSERT_MAX_REL_ERROR( 42.0,
				  pathlossFreeSpace->getPathloss(*antenna1, *antenna2, frequency).get_dB(),
				  1E-5 );
	WNS_ASSERT_MAX_REL_ERROR( 0.0,
				  pathlossUmts3003Outdoor->getPathloss(*antenna1, *antenna2, frequency).get_dB(),
				  1E-5 );
    }

}}}}
