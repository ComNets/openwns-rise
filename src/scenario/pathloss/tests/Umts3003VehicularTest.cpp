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
#include <RISE/scenario/pathloss/Umts3003Vehicular.hpp>
#include <RISE/scenario/pathloss/Pathloss.hpp>

#include <WNS/Types.hpp>
#include <WNS/CppUnit.hpp>
#include <WNS/pyconfig/helper/Functions.hpp>

#include <cppunit/extensions/HelperMacros.h>

namespace rise { namespace scenario { namespace pathloss { namespace tests {

    class Umts3003VehicularTest :
	public PathlossTestsBase,
	public wns::TestFixture
    {
	CPPUNIT_TEST_SUITE( Umts3003VehicularTest );
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
	Pathloss* pathloss;
	bool deletePathloss;
    };

    CPPUNIT_TEST_SUITE_REGISTRATION( Umts3003VehicularTest );

    void Umts3003VehicularTest::prepare()
    {
	deletePathloss = false;
	PathlossTestsBase::prepare();
	pathloss = new Umts3003Vehicular(wns::pyconfig::helper::createViewFromDropInConfig("rise.scenario.Pathloss",
											   "Umts3003VehicularTest"));
	deletePathloss = true;
    }

    void Umts3003VehicularTest::cleanup()
    {
	if (deletePathloss) delete pathloss;
	PathlossTestsBase::cleanup();
    }

    void Umts3003VehicularTest::testInput()
    {
	testInput(pathloss);
    }

    void Umts3003VehicularTest::testOutput()
    {
	wns::Frequency frequency = 2000;
	station1->moveTo(wns::Position(0, 0, 10));
	station2->moveTo(wns::Position(100, 0, 10));
	WNS_ASSERT_MAX_REL_ERROR( 92.9216,
				  pathloss->getPathloss(*antenna1, *antenna2, frequency).get_dB(),
				  1E-5 );
	WNS_ASSERT_MAX_REL_ERROR( 92.4538,
				  pathloss->getPathloss(*antenna1, *antenna2, 1900).get_dB(),
				  1E-5 );
	station2->moveTo(wns::Position(0, 50, 10));
	WNS_ASSERT_MAX_REL_ERROR( 81.3621,
				  pathloss->getPathloss(*antenna1, *antenna2, frequency).get_dB(),
				  1E-5 );
	station2->moveTo(wns::Position(0, 0, 10));
	WNS_ASSERT_MAX_REL_ERROR( 42.0,
				  pathloss->getPathloss(*antenna1, *antenna2, frequency).get_dB(),
				  1E-5 );
    }

}}}}
