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

#include <RISE/scenario/pathloss/ITUUMi.hpp>
#include <RISE/antenna/tests/AntennaDropIn.hpp>
#include <RISE/stations/tests/StationDropIn.hpp>

#include <WNS/TestFixture.hpp>

#include <cppunit/extensions/HelperMacros.h>

namespace rise { namespace scenario { namespace pathloss { namespace tests {

class ITUUMiTest:
    public wns::TestFixture
{
    CPPUNIT_TEST_SUITE( ITUUMiTest );
    CPPUNIT_TEST( testLosProbability );
    CPPUNIT_TEST( testShadowing );
    CPPUNIT_TEST( testLOSPathloss );
    CPPUNIT_TEST( testNLOSPathloss );
    CPPUNIT_TEST_SUITE_END();

public:

    void
    prepare();

    void
    cleanup();

    void
    testLosProbability();

    void
    testShadowing();

    void
    testLOSPathloss();

    void
    testNLOSPathloss();

private:
    ITUUMi* testee_;

    rise::tests::SystemManagerDropIn* systemManager_;
    rise::tests::StationDropIn* station1_;
    rise::tests::StationDropIn* station2_;
};

CPPUNIT_TEST_SUITE_REGISTRATION( ITUUMiTest );

void ITUUMiTest::prepare()
{
 wns::pyconfig::Parser config;
    config.loadString(
        "from rise.scenario.Pathloss import ITUUMi\n"
        "testee = ITUUMi()\n"
        "testee.outdoorProbability = 1.0\n"
        );

    wns::pyconfig::View configView(config, "testee");

    testee_ = new ITUUMi(configView);

    systemManager_ = new rise::tests::SystemManagerDropIn();

    station1_ = new rise::tests::StationDropIn(systemManager_);
    station2_ = new rise::tests::StationDropIn(systemManager_);

    station1_->moveTo(wns::Position(0.0, 0.0, 10.0));
    station2_->moveTo(wns::Position(0.0, 0.0, 1.5));
}

void
ITUUMiTest::cleanup()
{
    delete testee_;
    delete station1_;
    delete station2_;
    delete systemManager_;
}

void
ITUUMiTest::testLosProbability()
{
    // Everything below 18m is LOS
    double d1 = 2.0;
    double p1 = 1.0;

    // Calculated by "hand", formula + calculator
    double d2 = 100.34;
    double p2 = 0.22993313;

    // Calculated by "hand", formula + calculator
    double d3 = 27.024;
    double p3 = 0.82370467;

    CPPUNIT_ASSERT_DOUBLES_EQUAL(p1, testee_->getLOSProbability(d1), 1e-06);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(p2, testee_->getLOSProbability(d2), 1e-06);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(p3, testee_->getLOSProbability(d3), 3e-04);
}

void
ITUUMiTest::testShadowing()
{
    rise::tests::AntennaDropIn a1(station1_);
    station1_->moveTo(wns::Position(0.0, 0.0, 6.0));
    rise::tests::AntennaDropIn a2(station2_);
    station2_->moveTo(wns::Position(0.0, 2.0, 1.50));
    double f = 2000;

    // The standard deviation of the shadowing shall include
    // the standard deviation of the car penetration loss (Table 8.2)
    // The variance of the sum of two gaussian distributed variables is
    // the sum of the individual variances
    double LOSShadowing = 3;

    CPPUNIT_ASSERT_DOUBLES_EQUAL(LOSShadowing, testee_->getLOSShadowingStd(a1, a2, f, 20.0), 1e-05);

    double NLOSShadowing = 4;

    CPPUNIT_ASSERT_DOUBLES_EQUAL(NLOSShadowing, testee_->getNLOSShadowingStd(a1, a2, f, 20.0), 1e-05);
}

void
ITUUMiTest::testLOSPathloss()
{
    // At 2.5 GHz the breakpoint distance is 150m, so we stay below

    double f = 2500;
    double d1 = 100;
    double d1plane = sqrt(d1*d1 - 8.5*8.5);
    double pl1 = 79.95880017;

    rise::tests::AntennaDropIn a1(station1_);
    station1_->moveTo(wns::Position(0.0, 0.0, 10.0));
    rise::tests::AntennaDropIn a2(station2_);
    station2_->moveTo(wns::Position(0.0, d1plane, 1.50));

    CPPUNIT_ASSERT_DOUBLES_EQUAL(pl1, testee_->getLOSPathloss(a1,a2,f,d1).get_dB(), 1e-06);

    // Now above breakpoint distance
    double d2 = 400.0;
    double d2plane = sqrt(d2*d2 - 8.5*8.5);
    double pl2 = 100.920454422506;
    //double pl2 = 101.95715587;//MATLAB value?

    station1_->moveTo(wns::Position(0.0, 0.0, 10.0));
    station2_->moveTo(wns::Position(0.0, d2plane, 1.50));
    CPPUNIT_ASSERT_DOUBLES_EQUAL(pl2, testee_->getLOSPathloss(a1,a2,f,d2).get_dB(), 1e-06);
}

void
ITUUMiTest::testNLOSPathloss()
{
    double f = 2500;
    double d1 = 340;
    double d1plane = sqrt(d1*d1 - 8.5*8.5);
    double pl1 = 125.95171648;

    rise::tests::AntennaDropIn a1(station1_);
    station1_->moveTo(wns::Position(0.0, 0.0, 10.0));
    rise::tests::AntennaDropIn a2(station2_);
    station2_->moveTo(wns::Position(0.0, d1plane, 1.50));

    CPPUNIT_ASSERT_DOUBLES_EQUAL(pl1, testee_->getNLOSPathloss(a1,a2,f,d1).get_dB(), 1e-06);

}

} // tests
} // pathloss
} // scenario
} // rise
