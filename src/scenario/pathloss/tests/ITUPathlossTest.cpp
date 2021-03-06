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

#include <RISE/scenario/pathloss/ITUPathloss.hpp>
#include <RISE/antenna/tests/AntennaDropIn.hpp>
#include <RISE/stations/tests/StationDropIn.hpp>

#include <WNS/TestFixture.hpp>

#include <WNS/evaluation/statistics/pdf.hpp>

namespace rise { namespace scenario { namespace pathloss { namespace tests {


class ITUPathlossStub:
    public ITUPathloss
{
public:
    ITUPathlossStub(double losProbability, double losPL, double nlosPL, double losSh, double nlosSh, wns::pyconfig::View pyco):
        ITUPathloss(pyco)
    {
        losProbability_ = losProbability;
        losPL_ = losPL;
        nlosPL_ = nlosPL;
        losSh_ = losSh;
        nlosSh_ = nlosSh;
    };

    virtual ~ITUPathlossStub() {};

    virtual double
    getLOSProbability(double distance) const
    {
        return losProbability_;
    }

    virtual wns::Ratio
    getLOSPathloss(const rise::antenna::Antenna& source,
                   const rise::antenna::Antenna& target,
                   const wns::Frequency& frequency,
                   const wns::Distance& distance) const
    {
        return wns::Ratio::from_dB(losPL_);
    }

    virtual wns::Ratio
    getNLOSPathloss(const rise::antenna::Antenna& source,
                    const rise::antenna::Antenna& target,
                    const wns::Frequency& frequency,
                    const wns::Distance& distance) const
    {
        return wns::Ratio::from_dB(nlosPL_);
    }

    virtual double
    getLOSShadowingStd(const rise::antenna::Antenna& source,
                       const rise::antenna::Antenna& target,
                       const wns::Frequency& frequency,
                       const wns::Distance& distance) const
    {
        return losSh_;
    }

    virtual double
    getNLOSShadowingStd(const rise::antenna::Antenna& source,
                        const rise::antenna::Antenna& target,
                        const wns::Frequency& frequency,
                        const wns::Distance& distance) const
    {
        return nlosSh_;
    }

    virtual double
    getCarPenetrationStd() const
    {
        return 0.0;
    }

    virtual double
    getCarPenetrationMean() const
    {
        return 0.0;
    }

private:
    double losProbability_;
    double losPL_;
    double nlosPL_;
    double losSh_;
    double nlosSh_;
};

class ITUPathlossTest:
    public wns::TestFixture
{
    CPPUNIT_TEST_SUITE( ITUPathlossTest );
    CPPUNIT_TEST( testBaseRNG );
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
    testBaseRNG();

    void
    testRNG();

    void
    testSwap();

    void
    testPerformance();

private:
    rise::scenario::pathloss::tests::ITUPathlossStub* testee_;
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
    "class A:\n"
    "   pass\n"
    "\n"
    "testee = A()\n"
    "testee.minPathloss = \"0.0 dB\"\n"
    "testee.useShadowing = True\n"
    "testee.useCarPenetration = True\n"
    );

    wns::pyconfig::View configView(config, "testee");

    testee_ = new rise::scenario::pathloss::tests::ITUPathlossStub(0.38, 1.0, 0.0, 0.0, 0.0, configView);

    systemManager_ = new rise::tests::SystemManagerDropIn();

    station1_ = new rise::tests::StationDropIn(systemManager_);
    station2_ = new rise::tests::StationDropIn(systemManager_);

    station1_->moveTo(wns::Position(0.0, 0.0, 25.0));
    station2_->moveTo(wns::Position(0.0, 0.0, 1.5));
    station1_->setStationId(12);
    station2_->setStationId(2);
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
    station1_->moveTo(wns::Position(132.0, 4.155, 25.0));
    wns::Position p1 = a1.getPosition();
    rise::tests::AntennaDropIn a2(station2_);
    station2_->moveTo(wns::Position(1.0, 3.155, 1.50));

    CPPUNIT_ASSERT_DOUBLES_EQUAL(testee_->getPathloss(a1, a2, 2000.0).get_factor(),
                                 testee_->getPathloss(a2, a1, 2000.0).get_factor(), 1e-6);

    station1_->moveTo(wns::Position(11.0, 23.0, 25.0));
    station2_->moveTo(wns::Position(112.3, 34.2, 1.5));
    CPPUNIT_ASSERT_DOUBLES_EQUAL(testee_->getPathloss(a1, a2, 2000.0).get_factor(),
                                 testee_->getPathloss(a2, a1, 2000.0).get_factor(), 1e-6);
}

void
ITUPathlossTest::testBaseRNG()
{
    std::ofstream tracefile;
    tracefile.open("rise.pathloss.detail.HashRNG.acorr.py");
    tracefile << "from pylab import *" << std::endl;
    tracefile << "x = [" << std::endl;

    std::ofstream tracefile2;
    tracefile2.open("rise.pathloss.detail.RNG.acorr.py");
    tracefile2 << "from pylab import *" << std::endl;
    tracefile2 << "x = [" << std::endl;
    
    wns::Position pos1(12.34, 23.2, 15.0);
    wns::Position pos2(100, 30, 30.0);

    detail::HashRNG hrngBase(0,
                             pos1,
                             pos2,
                             true, true);

    for (int ii=0; ii < 1000; ++ii)
    {
        double b = hrngBase();
        tracefile << b << "," << std::endl;
        tracefile2 << (*wns::simulator::getRNG())() << "," << std::endl;
    }

    tracefile << "]" << std::endl;
    tracefile << "acorr(x, normed=True, detrend=mlab.detrend_mean, maxlags=10)" << std::endl;
    tracefile << "show()" << std::endl;
    tracefile.close();
    
    tracefile2 << "]" << std::endl;
    tracefile2 << "acorr(x, detrend=mlab.detrend_mean, maxlags=10)" << std::endl;
    tracefile2 << "show()" << std::endl;
    tracefile2.close();
}

void
ITUPathlossTest::testRNG()
{
    double last = 0.0;

    std::ofstream tracefile;
    tracefile.open("rise.pathloss.detail.HashRNG.position.acorr.py");
    tracefile << "from pylab import *" << std::endl;
    tracefile << "x = [" << std::endl;
    
    using namespace wns::evaluation::statistics;
    PDF eNorm(-10.0, 10.0, 1000, PDF::linear, StatEval::fixed, "rise.pathloss.detail.HashRNG.Norm", "Test randomness of HashRNG");
    PDF eBase(0.0, 1.0, 1000, PDF::linear, StatEval::fixed, "rise.pathloss.detail.HashRNG.Base", "Test randomness of HashRNG");

    // Decrease step size and use e.print
    // to validate externally
    double stepsize = 0.1;
    for (double xx = 0.0; xx < 5.0; xx=xx + stepsize)
    {
        for (double yy = 0.0; yy < 5.0; yy=yy+ stepsize)
        {
            for (double zz = 25.0; zz < 30.0; zz=zz+stepsize)
            {
                wns::Position pos1(xx, yy, zz);
                wns::Position pos2(xx+100, yy +30, zz);
                detail::HashRNG hrngNorm(0, pos1, pos2, true, true);
                detail::HashRNG hrngBase(0, pos1, pos2, true, true);
                boost::normal_distribution<double> dis(1.0, 2.0);
                eNorm.put(dis(hrngNorm));
                double b = hrngBase();
                tracefile << b << "," << std::endl;
                eBase.put(b);
            }
        }
    }

    tracefile << "]" << std::endl;
    tracefile << "acorr(x, normed=True, detrend=mlab.detrend_mean, maxlags=10)" << std::endl;
    tracefile << "show()" << std::endl;
    tracefile.close();

    // Check if the resulting distribution is really uniform
    // e.print();
    std::ofstream myfile;
    myfile.open("rise.pathloss.detail.HashRNG.Norm_PDF.dat");
    eNorm.print(myfile);
    myfile.close();

    myfile.open("rise.pathloss.detail.HashRNG.Base_PDF.dat");
    eBase.print(myfile);
    myfile.close();

    // Only used as guard to ensure changes don't break randomness
    // This is no test of correctness for the stochastical behaviour
    CPPUNIT_ASSERT_DOUBLES_EQUAL(1.0, eNorm.mean(), 0.03);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(4.0, eNorm.variance(), 0.05);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.5, eBase.mean(), 0.03);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(1.0/12.0, eBase.variance(), 0.005);
}

void
ITUPathlossTest::testPerformance()
{
    rise::tests::AntennaDropIn a1(station1_);
    station1_->moveTo(wns::Position(132.0, 4.155, 25.0));
    wns::Position p1 = a1.getPosition();
    rise::tests::AntennaDropIn a2(station2_);
    station2_->moveTo(wns::Position(1.0, 3.155, 1.50));

    double sum = 0;
    for (int ii=0; ii < 65000; ++ii)
    {
        // Move to make the outcome of the experiment random
        // The stubs PL model is distance independent
        station2_->moveTo(wns::Position(1.0, 3.155 + ii, 1.50));
        wns::Ratio r = testee_->getPathloss(a1, a2, 2000.0);
        sum += r.get_dB();
    }
    sum = sum / 65000;
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0.38, sum, 1e-02);
}
