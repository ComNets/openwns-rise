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
#include <RISE/scenario/pathloss/RangeChecked.hpp>
#include <RISE/scenario/pathloss/Constant.hpp>
#include <RISE/scenario/pathloss/Pathloss.hpp>
#include <RISE/manager/systemmanager.hpp>

#include <WNS/Types.hpp>
#include <WNS/CppUnit.hpp>
#include <WNS/pyconfig/helper/Functions.hpp>

#include <cppunit/extensions/HelperMacros.h>

namespace rise { namespace scenario { namespace pathloss { namespace tests {

    class RangeCheckedTest :
	public PathlossTestsBase,
	public wns::TestFixture
    {
	CPPUNIT_TEST_SUITE( RangeCheckedTest );
	CPPUNIT_TEST( testInput );
	CPPUNIT_TEST( testDispatching );
	CPPUNIT_TEST_SUITE_END();
	using PathlossTestsBase::testInput;
    public:
	virtual void prepare();
	virtual void cleanup();
	void testInput();
	void testDispatching();
    private:
	class TestPathloss : public RangeChecked
	{
	public:
	    TestPathloss()
		: RangeChecked(wns::pyconfig::helper::createViewFromDropInConfig("rise.scenario.Pathloss",
										 "RangeCheckedTest")),
		  functionCalled(false),
		  constant(wns::pyconfig::helper::createViewFromDropInConfig("rise.scenario.Pathloss",
									     "RangeCheckedTest").getView("constant"))
	    {
		addRange(DistanceRange::FromIncluding(0).ToExcluding(1), this);
		addRange(DistanceRange::FromIncluding(1).ToIncluding(2), &constant);
		addRange(DistanceRange::FromExcluding(2).ToExcluding(3), this);
		if (!knowsDistance(BaseTestDistance)) 
		    addRange(DistanceRange::From(BaseTestDistance).To(BaseTestDistance), this);

		addRange(FrequencyRange::FromExcluding(0).ToExcluding(1000), false);
		addRange(FrequencyRange::FromIncluding(1000).ToIncluding(2000), true);
		addRange(FrequencyRange::FromExcluding(2000).ToIncluding(3000), false);
		if (!knowsFrequency(BaseTestFrequency))
		    addRange(FrequencyRange::From(BaseTestFrequency).To(BaseTestFrequency), true);
	    }

	    virtual wns::Ratio calculatePathloss(const antenna::Antenna&,
						 const antenna::Antenna&,
						 const wns::Frequency&,
						 const wns::Distance&) const
	    {
		functionCalled = true;
		return wns::Ratio::from_dB(1.0);
	    };


	    mutable bool functionCalled;
	    const Constant constant;
	};

	Pathloss* pathloss;
	bool deletePathloss;
    };

    CPPUNIT_TEST_SUITE_REGISTRATION( RangeCheckedTest );

    void RangeCheckedTest::prepare()
    {
	deletePathloss = false;
	PathlossTestsBase::prepare();
	pathloss = new TestPathloss();
	deletePathloss = true;
    }

    void RangeCheckedTest::cleanup()
    {
	if (deletePathloss) delete pathloss;
	PathlossTestsBase::cleanup();
    }

    void RangeCheckedTest::testInput()
    {
	testInput(pathloss);
    }

    void RangeCheckedTest::testDispatching()
    {
	static_cast<TestPathloss*>(pathloss)->functionCalled = false;
	pathloss->getPathloss(*antenna1, *antenna2, 1000);
	CPPUNIT_ASSERT( static_cast<TestPathloss*>(pathloss)->functionCalled );

	station1->moveTo(wns::Position(0, 0, 0));
	station2->moveTo(wns::Position(BaseTestDistance, 0, 0));
	/*
	  // There are no more exceptions here, because we need simulations for big scenarios also.
	WNS_ASSERT_ASSURE_EXCEPTION( pathloss->getPathloss(*antenna1, *antenna2, 0) );
	WNS_ASSERT_ASSURE_EXCEPTION( pathloss->getPathloss(*antenna1, *antenna2, 999.99) );
	CPPUNIT_ASSERT_NO_THROW( pathloss->getPathloss(*antenna1, *antenna2, 1000) );
	CPPUNIT_ASSERT_NO_THROW( pathloss->getPathloss(*antenna1, *antenna2, 2000) );
	WNS_ASSERT_ASSURE_EXCEPTION( pathloss->getPathloss(*antenna1, *antenna2, 2000.01) );
	*/
	station2->moveTo(wns::Position(0, 0, 0));
	WNS_ASSERT_MAX_REL_ERROR( 1.0, pathloss->getPathloss(*antenna1, *antenna2, BaseTestFrequency).get_dB(), 1E-5 );
	station2->moveTo(wns::Position(0, 0.99, 0));
	WNS_ASSERT_MAX_REL_ERROR( 1.0, pathloss->getPathloss(*antenna1, *antenna2, BaseTestFrequency).get_dB(), 1E-5 );
	station2->moveTo(wns::Position(0, 1, 0));
	WNS_ASSERT_MAX_REL_ERROR( 2.0, pathloss->getPathloss(*antenna1, *antenna2, BaseTestFrequency).get_dB(), 1E-5 );
	station2->moveTo(wns::Position(2, 0, 0));
	WNS_ASSERT_MAX_REL_ERROR( 2.0, pathloss->getPathloss(*antenna1, *antenna2, BaseTestFrequency).get_dB(), 1E-5 );
	station2->moveTo(wns::Position(2.01, 0, 0));
	WNS_ASSERT_MAX_REL_ERROR( 1.0, pathloss->getPathloss(*antenna1, *antenna2, BaseTestFrequency).get_dB(), 1E-5 );

    }

}}}}
