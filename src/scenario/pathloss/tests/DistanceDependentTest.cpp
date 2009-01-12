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
#include <RISE/scenario/pathloss/DistanceDependent.hpp>
#include <RISE/scenario/pathloss/Pathloss.hpp>
#include <RISE/antenna/Antenna.hpp>

#include <WNS/PowerRatio.hpp>
#include <WNS/Position.hpp>
#include <WNS/CppUnit.hpp>
#include <WNS/pyconfig/helper/Functions.hpp>
#include <WNS/Types.hpp>

#include <cppunit/extensions/HelperMacros.h>

namespace rise { namespace scenario { namespace pathloss { namespace tests {

    class DistanceDependentTest :
	public PathlossTestsBase,
	public wns::TestFixture
    {
	CPPUNIT_TEST_SUITE( DistanceDependentTest );
	CPPUNIT_TEST( testInput );
	CPPUNIT_TEST( testDispatching );
	CPPUNIT_TEST( testDistanceCalculation );
	CPPUNIT_TEST( testMinPathloss );
	CPPUNIT_TEST_SUITE_END();
	using PathlossTestsBase::testInput;
    public:
	virtual void prepare();
	virtual void cleanup();
	void testInput();
	void testDispatching();
	void testDistanceCalculation();
	void testMinPathloss();
    private:
	class TestPathloss : public DistanceDependent
	{
	public:
	    TestPathloss()
		: DistanceDependent(wns::pyconfig::helper::createViewFromDropInConfig("rise.scenario.Pathloss",
										      "DistanceDependentTest")),
		  functionCalled(false)
	    {}

	    wns::Ratio calculatePathloss(const antenna::Antenna&,
					 const antenna::Antenna&,
					 const wns::Frequency&,
					 const wns::Distance& distance) const
	    {
		functionCalled = true;
		// return distance as pathloss - kind of a hack
		return wns::Ratio::from_factor(distance);
	    }

	    mutable bool functionCalled;

	};

	Pathloss* pathloss;
	bool deletePathloss;
    };

    CPPUNIT_TEST_SUITE_REGISTRATION( DistanceDependentTest );

    void DistanceDependentTest::prepare()
    {
	deletePathloss = false;
	PathlossTestsBase::prepare();
	pathloss = new TestPathloss();
	deletePathloss = true;
    }

    void DistanceDependentTest::cleanup()
    {
	if (deletePathloss) delete pathloss;
	PathlossTestsBase::cleanup();
    }

    void DistanceDependentTest::testInput()
    {
	testInput(pathloss);
    }

    void DistanceDependentTest::testDispatching()
    {
	static_cast<TestPathloss*>(pathloss)->functionCalled = false;
	pathloss->getPathloss(*antenna1, *antenna2, 1000);
	CPPUNIT_ASSERT( static_cast<TestPathloss*>(pathloss)->functionCalled );
    }

    void DistanceDependentTest::testDistanceCalculation()
    {
	station1->moveTo(wns::Position(0, 0, 0));
	station2->moveTo(wns::Position(0, 100, 0));
	WNS_ASSERT_MAX_REL_ERROR( 100.0, pathloss->getPathloss(*antenna1, *antenna2, 1000).get_factor(), 1E-5 );
	station2->moveTo(wns::Position(100, 100, 0));
	WNS_ASSERT_MAX_REL_ERROR( sqrt(100*100 + 100*100), pathloss->getPathloss(*antenna1, *antenna2, 2000).get_factor(), 1E-5 );
    }

    void DistanceDependentTest::testMinPathloss()
    {
	station1->moveTo(wns::Position(0, 0, 0));
	station2->moveTo(wns::Position(0, 0, 0));
	WNS_ASSERT_MAX_REL_ERROR( 1.0, pathloss->getPathloss(*antenna1, *antenna2, 1000).get_dB(), 1E-5);
    }

}}}}
