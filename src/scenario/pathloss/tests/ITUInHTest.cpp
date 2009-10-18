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

#include <RISE/scenario/pathloss/ITUInH.hpp>
#include <RISE/antenna/tests/AntennaDropIn.hpp>
#include <RISE/stations/tests/StationDropIn.hpp>

#include <WNS/TestFixture.hpp>

#include <cppunit/extensions/HelperMacros.h>

namespace rise { namespace scenario { namespace pathloss { namespace tests {

class ITUInHTest:
    public wns::TestFixture
{
    CPPUNIT_TEST_SUITE( ITUInHTest );
    CPPUNIT_TEST( testLosProbability );
    CPPUNIT_TEST( testShadowing );
    CPPUNIT_TEST( testLOSPathloss );
    CPPUNIT_TEST( testNLOSPathloss );
    CPPUNIT_TEST( testPlotLosProbability );
    CPPUNIT_TEST( testPlotLosPathloss );
    CPPUNIT_TEST( testPlotNLosPathloss );
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

    void
    testPlotLosProbability();

    void
    testPlotLosPathloss();

    void
    testPlotNLosPathloss();

private:
    ITUInH* testee_;

    rise::tests::SystemManagerDropIn* systemManager_;
    rise::tests::StationDropIn* station1_;
    rise::tests::StationDropIn* station2_;
};

CPPUNIT_TEST_SUITE_REGISTRATION( ITUInHTest );

void ITUInHTest::prepare()
{
    wns::pyconfig::Parser config;
    config.loadString(
                       "from rise.scenario.Pathloss import ITUInH\n"
                       "testee = ITUInH()\n"
                     );

    wns::pyconfig::View configView(config, "testee");

    testee_ = new ITUInH(configView);

    systemManager_ = new rise::tests::SystemManagerDropIn();

    station1_ = new rise::tests::StationDropIn(systemManager_);
    station2_ = new rise::tests::StationDropIn(systemManager_);

    station1_->moveTo(wns::Position(0.0, 0.0, 6.0));
    station2_->moveTo(wns::Position(0.0, 0.0, 1.5));
}

void
ITUInHTest::cleanup()
{
    delete testee_;
    delete station1_;
    delete station2_;
    delete systemManager_;
}

void
ITUInHTest::testLosProbability()
{
    // Everything below 18m is LOS
    double d1 = 2.0;
    double p1 = 1.0;

    // Calculated by "hand", formula + calculator
    double d2 = 99.34;
    double p2 = 0.5;

    // Calculated by "hand", formula + calculator
    double d3 = 27.024;
    double p3 = 0.715894677;

    CPPUNIT_ASSERT_DOUBLES_EQUAL(p1, testee_->getLOSProbability(d1), 1e-06);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(p2, testee_->getLOSProbability(d2), 1e-06);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(p3, testee_->getLOSProbability(d3), 1e-06);
}

void
ITUInHTest::testShadowing()
{
    rise::tests::AntennaDropIn a1(station1_);
    station1_->moveTo(wns::Position(0.0, 0.0, 6.0));
    rise::tests::AntennaDropIn a2(station2_);
    station2_->moveTo(wns::Position(0.0, 2.0, 1.50));
    double f = 3400;

    double LOSShadowing = 3.0;

    CPPUNIT_ASSERT_DOUBLES_EQUAL(LOSShadowing, testee_->getLOSShadowingStd(a1, a2, f, 20.0), 1e-05);

    double NLOSShadowing = 4.0;

    CPPUNIT_ASSERT_DOUBLES_EQUAL(NLOSShadowing, testee_->getNLOSShadowingStd(a1, a2, f, 20.0), 1e-05);
}

void
ITUInHTest::testLOSPathloss()
{
    // At 2 GHz the breakpoint distance is 320m, so we stay below
    // Pathloss includes car penetration loss of 9dB

    double f = 3400;
    double d1 = 97.32;
    double d1plane = sqrt(d1*d1 + 4.5*4.5);
    double pl1 = 77.030193835;

    rise::tests::AntennaDropIn a1(station1_);
    station1_->moveTo(wns::Position(0.0, 0.0, 6.0));
    rise::tests::AntennaDropIn a2(station2_);
    station2_->moveTo(wns::Position(0.0, d1plane, 1.50));

    CPPUNIT_ASSERT_DOUBLES_EQUAL(pl1, testee_->getLOSPathloss(a1,a2,f,d1).get_dB(), 1e-06);

    double d2 = 12.3;
    double d2plane = sqrt(d2*d2 + 4.5*4.5);
    double pl2 = 61.848974724;

    station1_->moveTo(wns::Position(0.0, 0.0, 6.0));
    station2_->moveTo(wns::Position(0.0, d2plane, 1.50));

    CPPUNIT_ASSERT_DOUBLES_EQUAL(pl2, testee_->getLOSPathloss(a1,a2,f,d2).get_dB(), 1e-06);
}

void
ITUInHTest::testNLOSPathloss()
{
    double f = 3400;
    double d1 = 95.6;
    double d1plane = sqrt(d1*d1 + 4.5*4.5);
    double pl1 = 107.883405076;

    rise::tests::AntennaDropIn a1(station1_);
    station1_->moveTo(wns::Position(0.0, 0.0, 6.0));
    rise::tests::AntennaDropIn a2(station2_);
    station2_->moveTo(wns::Position(0.0, d1plane, 1.50));

    CPPUNIT_ASSERT_DOUBLES_EQUAL(pl1, testee_->getNLOSPathloss(a1,a2,f,d1).get_dB(), 1e-06);
}

void
ITUInHTest::testPlotLosProbability()
{
    std::ofstream myfile;
    myfile.open("rise.pathloss.ITUInHTest.losProbability.py");
    myfile << "from pylab import *\n" << "x = []\n" << "y = []\n";

    for (int ii=1; ii < 149; ++ii)
    {
        myfile << "x.append(" << ii << ")\n";
        myfile << "y.append(" << testee_->getLOSProbability(ii) << ")\n";
    }
    myfile << "x = array(x)\n";
    myfile << "y = array(y)\n";
    myfile << "plot(x,y)\n";
    myfile << "title(\"LOS Probability for InH (openWNS)\")\n";
    myfile << "grid()\n";
    myfile << "savefig(\"rise.pathloss.ITUInHTest.losProbability.png\")\n";
    myfile.close();
}

void
ITUInHTest::testPlotLosPathloss()
{
    double f = 3400;

    rise::tests::AntennaDropIn a1(station1_);
    station1_->moveTo(wns::Position(0.0, 0.0, 6.0));

    std::ofstream myfile;
    myfile.open("rise.pathloss.ITUInHTest.losPathloss.py");
    myfile << "from pylab import *\n" << "x = []\n" << "y = []\n";

    for (int ii=11; ii < 99; ++ii)
    {
        double dplane = sqrt( abs(ii*ii - 4.5*4.5) );
        rise::tests::AntennaDropIn a2(station2_);
        station2_->moveTo(wns::Position(0.0, dplane, 1.50));

        double pl = testee_->getLOSPathloss(a1, a2, f, ii).get_dB();
        myfile << "x.append(" << ii << ")\n";
        myfile << "y.append(" << pl << ")\n";
    }

    myfile << "x = array(x)\n";
    myfile << "y = array(y)\n";
    myfile << "plot(x,y)\n";
    myfile << "title(\"LOS Pathloss @ 3.4GHz for InH (openWNS)\")\n";
    myfile << "grid()\n";
    myfile << "savefig(\"rise.pathloss.ITUInHTest.losPathloss.png\")\n";
    myfile.close();
}

void
ITUInHTest::testPlotNLosPathloss()
{
    double f = 3400;

    rise::tests::AntennaDropIn a1(station1_);
    station1_->moveTo(wns::Position(0.0, 0.0, 6.0));

    std::ofstream myfile;
    myfile.open("rise.pathloss.ITUInHTest.nlosPathloss.py");
    myfile << "from pylab import *\n" << "x = []\n" << "y = []\n";

    for (int ii=11; ii < 149; ++ii)
    {
        double dplane = sqrt( abs(ii*ii - 4.5*4.5) );
        rise::tests::AntennaDropIn a2(station2_);
        station2_->moveTo(wns::Position(0.0, dplane, 1.50));

        double pl = testee_->getNLOSPathloss(a1, a2, f, ii).get_dB();
        myfile << "x.append(" << ii << ")\n";
        myfile << "y.append(" << pl << ")\n";
    }

    myfile << "x = array(x)\n";
    myfile << "y = array(y)\n";
    myfile << "plot(x,y)\n";
    myfile << "title(\"NLOS Pathloss @ 3.4GHz for InH (openWNS)\")\n";
    myfile << "grid()\n";
    myfile << "savefig(\"rise.pathloss.ITUInHTest.nlosPathloss.png\")\n";
    myfile.close();
}

} // tests
} // pathloss
} // scenario
} // rise
