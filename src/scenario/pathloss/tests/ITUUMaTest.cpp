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
#include <WNS/evaluation/statistics/pdf.hpp>

#include <cppunit/extensions/HelperMacros.h>

namespace rise { namespace scenario { namespace pathloss { namespace tests {

    class ITUUMaTester:
        public ITUUMa
        {
            public:

            enum LOSBEHAVIOUR {
                NORMALLOS,
                FIXEDLOS,
                FIXEDNLOS
            };

            enum SHADOWINGBEHAVIOUR {
                NORMALSHADOWING,
                OFF
            };

            enum CARPENETRATIONBEHAVIOUR {
                NORMALCARPEN,
                NOCARPEN
            };

            ITUUMaTester(const wns::pyconfig::View& config):
                ITUUMa(config),
                losBehaviour(NORMALLOS),
                shadowingBehaviour(NORMALSHADOWING),
                carpenetrationBehaviour(NORMALCARPEN)
            {
            };

            void
            setLOSBehaviour(LOSBEHAVIOUR b)
            {
                this->losBehaviour = b;
            };

            void
            setShadowingBehaviour(SHADOWINGBEHAVIOUR b)
            {
                this->shadowingBehaviour = b;
            }

            void
            setCarPenetrationBehaviour(CARPENETRATIONBEHAVIOUR b)
            {
                this->carpenetrationBehaviour = b;
            }

            virtual double
            getLOSProbability(double distance) const
            {
                if(this->losBehaviour == FIXEDLOS)
                {
                    return 1.0;
                }
                if(this->losBehaviour == FIXEDNLOS)
                {
                    return 0.0;
                }
                return ITUUMa::getLOSProbability(distance);
            }

            virtual double
            getLOSShadowingStd(const rise::antenna::Antenna& source,
                               const rise::antenna::Antenna& target,
                               const wns::Frequency& frequency,
                               const wns::Distance& distance) const
            {
                if (shadowingBehaviour == OFF)
                {
                    return 0.0;
                }
                return ITUUMa::getLOSShadowingStd(source, target, frequency, distance);
            }

            virtual double
            getNLOSShadowingStd(const rise::antenna::Antenna& source,
                                const rise::antenna::Antenna& target,
                                const wns::Frequency& frequency,
                                const wns::Distance& distance) const
            {
                if (shadowingBehaviour == OFF)
                {
                    return 0.0;
                }
                return ITUUMa::getNLOSShadowingStd(source, target, frequency, distance);
            }

            virtual double
            getCarPenetrationStd() const
            {
                if (carpenetrationBehaviour == NOCARPEN)
                {
                    return 0.0;
                }
                return ITUUMa::getCarPenetrationStd();
            }

            virtual double
            getCarPenetrationMean() const
            {
                if(carpenetrationBehaviour == NOCARPEN)
                {
                    return 0.0;
                }
                return ITUUMa::getCarPenetrationMean();
            }

            private:
                LOSBEHAVIOUR losBehaviour;
                SHADOWINGBEHAVIOUR shadowingBehaviour;
                CARPENETRATIONBEHAVIOUR carpenetrationBehaviour;
        };

class ITUUMaTest:
    public wns::TestFixture
{
    CPPUNIT_TEST_SUITE( ITUUMaTest );
    CPPUNIT_TEST( testLosProbability );
    CPPUNIT_TEST( testShadowing );
    CPPUNIT_TEST( testLOSPathloss );
    CPPUNIT_TEST( testNLOSPathloss );
    CPPUNIT_TEST( testCarPenetration );
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
    testCarPenetration();

    void
    testPlotLosProbability();

    void
    testPlotLosPathloss();

    void
    testPlotNLosPathloss();

private:
    ITUUMaTester* testee_;

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

    testee_ = new ITUUMaTester(configView);

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

    double LOSShadowing = 4.0;

    CPPUNIT_ASSERT_DOUBLES_EQUAL(LOSShadowing, testee_->getLOSShadowingStd(a1, a2, f, 20.0), 1e-05);

    double NLOSShadowing = 6.0;

    CPPUNIT_ASSERT_DOUBLES_EQUAL(NLOSShadowing, testee_->getNLOSShadowingStd(a1, a2, f, 20.0), 1e-05);

    double carPenetrationStd = 5.0;

    CPPUNIT_ASSERT_DOUBLES_EQUAL(carPenetrationStd, testee_->getCarPenetrationStd(), 1e-05);

    double carPenetrationMean = 9.0;
    
    CPPUNIT_ASSERT_DOUBLES_EQUAL(carPenetrationMean, testee_->getCarPenetrationMean(), 1e-05);
}

void
ITUUMaTest::testLOSPathloss()
{
    // At 2 GHz the breakpoint distance is 320m, so we stay below

    double f = 2000;
    double d1 = 100;
    double d1plane = sqrt(d1*d1 + 23.5*23.5);
    double pl1 = 78.020599913;

    rise::tests::AntennaDropIn a1(station1_);
    station1_->moveTo(wns::Position(0.0, 0.0, 25.0));
    rise::tests::AntennaDropIn a2(station2_);
    station2_->moveTo(wns::Position(0.0, d1plane, 1.50));

    CPPUNIT_ASSERT_DOUBLES_EQUAL(pl1, testee_->getLOSPathloss(a1,a2,f,d1).get_dB(), 1e-06);

    // Now above breakpoint distance
    double d2 = 400;
    double d2plane = sqrt(d2*d2 + 23.5*23.5);
    double pl2 = 93.059197216;

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
    double pl1 = 118.511659003;

    rise::tests::AntennaDropIn a1(station1_);
    station1_->moveTo(wns::Position(0.0, 0.0, 25.0));
    rise::tests::AntennaDropIn a2(station2_);
    station2_->moveTo(wns::Position(0.0, d1plane, 1.50));

    CPPUNIT_ASSERT_DOUBLES_EQUAL(pl1, testee_->getNLOSPathloss(a1,a2,f,d1).get_dB(), 1e-06);

}

void
ITUUMaTest::testCarPenetration()
{
    // Turn off shadowing and fix LOS
    // Only random thing is car penetration which depends only on UT position
    testee_->setShadowingBehaviour(ITUUMaTester::OFF);
    testee_->setLOSBehaviour(ITUUMaTester::FIXEDLOS);

    double f = 2000;
    double d1 = 340;
    double d1plane = sqrt(d1*d1 + 23.5*23.5);
    
    rise::tests::AntennaDropIn a1(station1_);
    station1_->moveTo(wns::Position(0.0, 0.0, 25.0));
    rise::tests::AntennaDropIn a2(station2_);
    station2_->moveTo(wns::Position(0.0, d1plane, 1.50));

    double pl = testee_->calculatePathloss(a1, a2, f, d1).get_dB();

    // Now if we move station1_ keeping the distance constant
    // PL should remain the same, since user is the same,
    // thus the _car_ is the same
    station1_->moveTo(wns::Position(0.0, 2*d1plane, 25.0));
    double pl2 = testee_->calculatePathloss(a1, a2, f, d1).get_dB();

    CPPUNIT_ASSERT_DOUBLES_EQUAL(pl, pl2, 1e-06);

    // Now if station2_ is moved the car penetration should change
    // We now measure stddev and mean
    using namespace wns::evaluation::statistics;
    PDF carPenPDF(-20.0, 40.0, 1000, PDF::linear, StatEval::fixed, "rise.pathloss.ITUUMa.carPenetration", "Test CarPenetration");

    for (int ii = 0; ii < 50000; ++ii)
    {
        station2_->moveTo(wns::Position(0.0, 0.0 + (d1plane/50000.0 * ii), 1.50));
        testee_->setCarPenetrationBehaviour(ITUUMaTester::NORMALCARPEN);
        pl = testee_->calculatePathloss(a1, a2, f, d1).get_dB();
        testee_->setCarPenetrationBehaviour(ITUUMaTester::NOCARPEN);
        pl2 = testee_->calculatePathloss(a1, a2, f, d1).get_dB();
        carPenPDF.put(pl-pl2);
    }

    std::ofstream myfile;
    myfile.open("rise.pathloss.detail.ITUUMa.carPenetration_PDF.dat");
    carPenPDF.print(myfile);
    myfile.close();

    CPPUNIT_ASSERT_DOUBLES_EQUAL(9.0, carPenPDF.mean() , 0.1);
    // Converges to below 0.1 bound for 150000 trials, this runs to long
    // so relaxing the bound to 0.5 (2%)
    CPPUNIT_ASSERT_DOUBLES_EQUAL(25.0, carPenPDF.variance() , 0.5);
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
