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

#include <RISE/scenario/pathloss/ITUUMa.hpp>
#include <RISE/antenna/tests/AntennaDropIn.hpp>
#include <RISE/stations/tests/StationDropIn.hpp>

#include <WNS/TestFixture.hpp>

#include <cppunit/extensions/HelperMacros.h>

namespace rise { namespace scenario { namespace pathloss { namespace tests {

class ITUUMaTest:
    public wns::TestFixture
{
    CPPUNIT_TEST_SUITE( ITUUMaTest );
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
    ITUUMa* testee_;

    rise::tests::SystemManagerDropIn* systemManager_;
    rise::tests::StationDropIn* station1_;
    rise::tests::StationDropIn* station2_;
};

CPPUNIT_TEST_SUITE_REGISTRATION( ITUUMaTest );

void ITUUMaTest::prepare()
{
    wns::pyconfig::Parser config;
    config.loadString(
        "from rise.scenario.Pathloss import ITUUMa\n"
        "testee = ITUUMa()\n"
        );

    wns::pyconfig::View configView(config, "testee");

    testee_ = new ITUUMa(configView);

    systemManager_ = new rise::tests::SystemManagerDropIn();

    station1_ = new rise::tests::StationDropIn(systemManager_);
    station2_ = new rise::tests::StationDropIn(systemManager_);

    station1_->moveTo(wns::Position(0.0, 0.0, 25.0));
    station2_->moveTo(wns::Position(0.0, 0.0, 1.5));
}

void
ITUUMaTest::cleanup()
{
    delete testee_;
    delete station1_;
    delete station2_;
    delete systemManager_;
}

void
ITUUMaTest::testLosProbability()
{
    // Everything below 18m is LOS
    double d1 = 2.0;
    double p1 = 1.0;

    // Calculated by "hand", formula + calculator
    double d2 = 100.34;
    double p2 = 0.346282502;

    // Calculated by "hand", formula + calculator
    double d3 = 27.024;
    double p3 = 0.883744095;

    CPPUNIT_ASSERT_DOUBLES_EQUAL(p1, testee_->getLOSProbability(d1), 1e-06);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(p2, testee_->getLOSProbability(d2), 1e-06);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(p3, testee_->getLOSProbability(d3), 3e-04);
}

void
ITUUMaTest::testShadowing()
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
    double LOSShadowing = sqrt(4.0*4.0 + 5.0*5.0);

    CPPUNIT_ASSERT_DOUBLES_EQUAL(LOSShadowing, testee_->getLOSShadowingStd(a1, a2, f, 20.0), 1e-05);

    double NLOSShadowing = sqrt(6.0*6.0 + 5.0*5.0);

    CPPUNIT_ASSERT_DOUBLES_EQUAL(NLOSShadowing, testee_->getNLOSShadowingStd(a1, a2, f, 20.0), 1e-05);
}

void
ITUUMaTest::testLOSPathloss()
{
    // At 2 GHz the breakpoint distance is 320m, so we stay below
    // Pathloss includes car penetration loss of 9dB

    double f = 2000;
    double d1 = 100;
    double d1plane = sqrt(d1*d1 + 23.5*23.5);
    double pl1 = 78.020599913 + 9.0;

    rise::tests::AntennaDropIn a1(station1_);
    station1_->moveTo(wns::Position(0.0, 0.0, 25.0));
    rise::tests::AntennaDropIn a2(station2_);
    station2_->moveTo(wns::Position(0.0, d1plane, 1.50));

    CPPUNIT_ASSERT_DOUBLES_EQUAL(pl1, testee_->getLOSPathloss(a1,a2,f,d1).get_dB(), 1e-06);

    // Now above breakpoint distance
    double d2 = 400;
    double d2plane = sqrt(d2*d2 + 23.5*23.5);
    double pl2 = 93.059197216 + 9.0;

    station1_->moveTo(wns::Position(0.0, 0.0, 25.0));
    station2_->moveTo(wns::Position(0.0, d2plane, 1.50));

    CPPUNIT_ASSERT_DOUBLES_EQUAL(pl2, testee_->getLOSPathloss(a1,a2,f,d2).get_dB(), 1e-06);
}

void
ITUUMaTest::testNLOSPathloss()
{

    // Pathloss includes car penetration loss of 9dB

    double f = 2000;
    double d1 = 340;
    double d1plane = sqrt(d1*d1 + 23.5*23.5);
    double pl1 = 118.511659003 + 9.0;

    rise::tests::AntennaDropIn a1(station1_);
    station1_->moveTo(wns::Position(0.0, 0.0, 25.0));
    rise::tests::AntennaDropIn a2(station2_);
    station2_->moveTo(wns::Position(0.0, d1plane, 1.50));

    CPPUNIT_ASSERT_DOUBLES_EQUAL(pl1, testee_->getNLOSPathloss(a1,a2,f,d1).get_dB(), 1e-06);

}

void
ITUUMaTest::testPlotLosProbability()
{
    std::ofstream myfile;
    myfile.open("rise.pathloss.ITUUMaTest.losProbability.py");
    myfile << "from pylab import *\n" << "x = []\n" << "y = []\n";

    for (int ii=1; ii < 1000; ++ii)
    {
        myfile << "x.append(" << ii << ")\n";
        myfile << "y.append(" << testee_->getLOSProbability(ii) << ")\n";
    }
    myfile << "x = array(x)\n";
    myfile << "y = array(y)\n";
    myfile << "plot(x,y)\n";
    myfile << "title(\"LOS Probability for UMa (openWNS)\")\n";
    myfile << "grid()\n";
    myfile << "savefig(\"rise.pathloss.ITUUMaTest.losProbability.png\")\n";
    myfile.close();
}

void
ITUUMaTest::testPlotLosPathloss()
{
    double f = 2000;

    rise::tests::AntennaDropIn a1(station1_);
    station1_->moveTo(wns::Position(0.0, 0.0, 25.0));

    std::ofstream myfile;
    myfile.open("rise.pathloss.ITUUMaTest.losPathloss.py");
    myfile << "from pylab import *\n" << "x = []\n" << "y = []\n";

    for (int ii=11; ii < 1000; ++ii)
    {
        double dplane = sqrt( abs(ii*ii - 23.5*23.5) );
        rise::tests::AntennaDropIn a2(station2_);
        station2_->moveTo(wns::Position(0.0, dplane, 1.50));

        double pl = testee_->getLOSPathloss(a1, a2, f, ii).get_dB();
        myfile << "x.append(" << ii << ")\n";
        myfile << "y.append(" << pl << ")\n";
    }

    myfile << "x = array(x)\n";
    myfile << "y = array(y)\n";
    myfile << "plot(x,y)\n";
    myfile << "title(\"LOS Pathloss @ 2GHz for UMa (openWNS)\")\n";
    myfile << "grid()\n";
    myfile << "savefig(\"rise.pathloss.ITUUMaTest.losPathloss.png\")\n";
    myfile.close();
}

void
ITUUMaTest::testPlotNLosPathloss()
{
    double f = 2000;

    rise::tests::AntennaDropIn a1(station1_);
    station1_->moveTo(wns::Position(0.0, 0.0, 25.0));

    std::ofstream myfile;
    myfile.open("rise.pathloss.ITUUMaTest.nlosPathloss.py");
    myfile << "from pylab import *\n" << "x = []\n" << "y = []\n";

    for (int ii=11; ii < 1000; ++ii)
    {
        double dplane = sqrt( abs(ii*ii - 23.5*23.5) );
        rise::tests::AntennaDropIn a2(station2_);
        station2_->moveTo(wns::Position(0.0, dplane, 1.50));

        double pl = testee_->getNLOSPathloss(a1, a2, f, ii).get_dB();
        myfile << "x.append(" << ii << ")\n";
        myfile << "y.append(" << pl << ")\n";
    }

    myfile << "x = array(x)\n";
    myfile << "y = array(y)\n";
    myfile << "plot(x,y)\n";
    myfile << "title(\"NLOS Pathloss @ 2GHz for UMa (openWNS)\")\n";
    myfile << "grid()\n";
    myfile << "savefig(\"rise.pathloss.ITUUMaTest.nlosPathloss.png\")\n";
    myfile.close();
}

} // tests
} // pathloss
} // scenario
} // rise
