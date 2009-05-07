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
#include <RISE/scenario/pathloss/Deny.hpp>
#include <RISE/scenario/pathloss/Pathloss.hpp>

#include <WNS/pyconfig/helper/Functions.hpp>
#include <WNS/Exception.hpp>
#include <WNS/CppUnit.hpp>

#include <cppunit/extensions/HelperMacros.h>

namespace rise { namespace scenario { namespace pathloss { namespace tests {

    class DenyTest :
	public PathlossTestsBase,
	public wns::TestFixture
    {
	CPPUNIT_TEST_SUITE( DenyTest );
      //CPPUNIT_TEST_EXCEPTION( testException, wns::Exception );
      //CPPUNIT_TEST( testCorrectException );
	CPPUNIT_TEST( testOutOfRange );
	CPPUNIT_TEST_SUITE_END();
    public:
	virtual void prepare();
	virtual void cleanup();
	void testException();
	void testCorrectException();
      void testOutOfRange();
    private:
	Pathloss* pathloss;
	bool deletePathloss;
    };

    CPPUNIT_TEST_SUITE_REGISTRATION( DenyTest );

    void DenyTest::prepare()
    {
	deletePathloss = false;
	PathlossTestsBase::prepare();
	pathloss = new Deny(wns::pyconfig::helper::createViewFromDropInConfig("rise.scenario.Pathloss",
									      "DenyTest"));
	deletePathloss = true;
    }

    void DenyTest::cleanup()
    {
	if (deletePathloss) delete pathloss;
	PathlossTestsBase::cleanup();
    }

    void DenyTest::testException()
    {
	pathloss->getPathloss(*antenna1, *antenna2, 5000.0);
    }

    void DenyTest::testCorrectException()
    {
#ifdef WNS_ASSURE_THROWS_EXCEPTION
	CPPUNIT_ASSERT_THROW( pathloss->getPathloss(*antenna1, *antenna2, -5000.0), wns::Assure::Exception );
	try {
		CPPUNIT_ASSERT_ASSERTION_FAIL(
			CPPUNIT_ASSERT_THROW(
				pathloss->getPathloss(*antenna1, *antenna2, -5000.0), Deny::Exception
				);
			);
	}
	catch (wns::Assure::Exception) {}
	catch (...) { throw; }

#endif
	CPPUNIT_ASSERT_THROW( pathloss->getPathloss(*antenna1, *antenna2, 5000.0), Deny::Exception );
#ifdef WNS_ASSURE_THROWS_EXCEPTION
	try {
		CPPUNIT_ASSERT_ASSERTION_FAIL(
			CPPUNIT_ASSERT_THROW(
				pathloss->getPathloss(*antenna1, *antenna2, 5000.0), wns::Assure::Exception
				)
			);
	}
	catch (Deny::Exception) {}
	catch (...) { throw; }
#endif
    }

      void DenyTest::testOutOfRange()
      {
	wns::Ratio pl = pathloss->getPathloss(*antenna1, *antenna2, 5000.0);
	WNS_ASSERT_MAX_REL_ERROR( rise::scenario::pathloss::OutOfRangePathloss.get_dB(), pl.get_dB(), 1E-5);
      }


}}}}
