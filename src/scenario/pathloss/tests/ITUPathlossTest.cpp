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
#include <WNS/TestFixture.hpp>

#include <RISE/scenario/pathloss/ITUUMa.hpp>
#include <RISE/antenna/tests/AntennaDropIn.hpp>
#include <RISE/stations/tests/StationDropIn.hpp>

#include <WNS/evaluation/statistics/pdf.hpp>

namespace rise { namespace scenario { namespace pathloss { namespace tests {


class ITUPathlossTest:
    public wns::TestFixture
{
    CPPUNIT_TEST_SUITE( ITUPathlossTest );
    CPPUNIT_TEST( testRNG );
    CPPUNIT_TEST( testSwap );
    CPPUNIT_TEST( testPerformance );
    CPPUNIT_TEST_SUITE_END();

public:

    void
    prepare();

    void
    cleanup();

    void
    testRNG();

    void
    testSwap();

    void
    testPerformance();

private:
    rise::scenario::pathloss::ITUUMa* testee_;
    rise::tests::SystemManagerDropIn* systemManager_;
    rise::tests::StationDropIn* station1_;
    rise::tests::StationDropIn* station2_;
};

} // tests
} // pathloss
} // scenario
} // rise

using namespace rise::scenario::pathloss::tests;

CPPUNIT_TEST_SUITE_REGISTRATION( ITUPathlossTest );

void ITUPathlossTest::prepare()
{
    wns::pyconfig::Parser config;
    config.loadString(
        "from rise.scenario.Pathloss import ITUUMa\n"
        "testee = ITUUMa()\n"
    );

    wns::pyconfig::View configView(config, "testee");

    testee_ = new rise::scenario::pathloss::ITUUMa(configView);

    systemManager_ = new rise::tests::SystemManagerDropIn();

    station1_ = new rise::tests::StationDropIn(systemManager_);
    station2_ = new rise::tests::StationDropIn(systemManager_);

    station1_->moveTo(wns::Position(0.0, 0.0, 25.0));
    station2_->moveTo(wns::Position(0.0, 0.0, 1.5));
}

void
ITUPathlossTest::cleanup()
{
    delete testee_;
    delete systemManager_;
    delete station1_;
    delete station2_;
}

void
ITUPathlossTest::testSwap()
{
    rise::tests::AntennaDropIn a1(station1_);
    station1_->moveTo(wns::Position(132.0, 4.155, 7.60));
    wns::Position p1 = a1.getPosition();
    rise::tests::AntennaDropIn a2(station2_);
    station2_->moveTo(wns::Position(1.0, 3.155, 1.60));

    CPPUNIT_ASSERT_DOUBLES_EQUAL(testee_->getPathloss(a1, a2, 2000.0).get_factor(),
                                 testee_->getPathloss(a2, a1, 2000.0).get_factor(), 1e-6);

    station1_->moveTo(wns::Position(11.0, 23.0, 15.0));
    station2_->moveTo(wns::Position(112.3, 34.2, 1.5));
    CPPUNIT_ASSERT_DOUBLES_EQUAL(testee_->getPathloss(a1, a2, 2000.0).get_factor(),
                                 testee_->getPathloss(a2, a1, 2000.0).get_factor(), 1e-6);
}

void
ITUPathlossTest::testRNG()
{
    double last = 0.0;

    using namespace wns::evaluation::statistics;
    PDF e(-10.0, 10.0, 1000, PDF::linear, StatEval::fixed, "rise::pathloss::detail::HashRNG", "Test randomness of HashRNG");

    // Decrease step size and use e.print
    // to validate externally
    double stepsize = 0.1;
    for (double xx = 0.0; xx < 5.0; xx=xx + stepsize)
    {
        for (double yy = 0.0; yy < 5.0; yy=yy+ stepsize)
        {
            for (double zz = 0.0; zz < 5.0; zz=zz+stepsize)
            {
                wns::Position pos1(xx, yy, zz);
                wns::Position pos2(xx+34, yy +2.2, zz);
                detail::HashRNG hrng(0, pos1, pos2, (pos1-pos2).abs());
                boost::normal_distribution<double> dis(1.0, 2.0);
                e.put(dis(hrng));
            }
        }
    }

    // Check if the resulting distribution is really uniform
    // e.print();

    // Only used as guard to ensure changes don't break randomness
    // This is no test of correctness for the stochastical behaviour
    CPPUNIT_ASSERT_DOUBLES_EQUAL(1.0, e.mean(), 0.03);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(4.0, e.variance(), 0.05);
}

void
ITUPathlossTest::testPerformance()
{
    rise::tests::AntennaDropIn a1(station1_);
    station1_->moveTo(wns::Position(132.0, 4.155, 7.60));
    wns::Position p1 = a1.getPosition();
    rise::tests::AntennaDropIn a2(station2_);
    station2_->moveTo(wns::Position(1.0, 3.155, 1.60));

    for (int ii=0; ii < 650000; ++ii)
    {
        wns::Ratio r = testee_->getPathloss(a1, a2, 2000.0);
    } 
}
