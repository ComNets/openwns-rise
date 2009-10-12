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

#include <RISE/antenna/ITUAntenna.hpp>
#include <RISE/stations/tests/StationDropIn.hpp>
#include <RISE/manager/tests/SystemManagerDropIn.hpp>

#include <WNS/TestFixture.hpp>

#include <cppunit/extensions/HelperMacros.h>

namespace rise { namespace antenna { namespace tests {

class ITUAntennaTest:
    public wns::TestFixture
{
    CPPUNIT_TEST_SUITE( ITUAntennaTest );
    CPPUNIT_TEST( testMethod );
    CPPUNIT_TEST_SUITE_END();

public:

    void
    prepare();

    void
    cleanup();

    void
    testMethod();

private:
    rise::tests::SystemManagerDropIn* systemManager_;
    rise::tests::StationDropIn* station_;
    ITUAntenna* testee_;
};

} // tests
} // antenna
} // rise

using namespace rise::antenna::tests;

CPPUNIT_TEST_SUITE_REGISTRATION( ITUAntennaTest );

void ITUAntennaTest::prepare()
{
    systemManager_ = new rise::tests::SystemManagerDropIn();
    station_ = new rise::tests::StationDropIn(systemManager_);

    wns::pyconfig::Parser config;
    config.loadString(
    "from rise.Antenna import ITU\n"
    "downtilt = 12.0/360.0 * 2 * 3.14159265\n"
    "testee = ITU(\"0.0 dB\", [0.0, 0.0, 23.5], 0.0, downtilt)\n");

    wns::pyconfig::View configView(config, "testee");

    testee_ = new ITUAntenna(configView, station_);

    station_->moveTo(wns::Position(1000.0, 1000.0, 1.5));
    station_->setStationId(1);
}

void
ITUAntennaTest::cleanup()
{
    delete testee_;
    delete station_;
    delete systemManager_;
}

void
ITUAntennaTest::testMethod()
{
    std::cout << std::endl;
    for (int ii =359; ii >= 0; --ii)
    {
        // Far away elevation ~= 0 degree
        double x = 200 * cos(ii/360.0 * 2 * 3.14);
        double y = 200 * sin(ii/360.0 * 2 * 3.14);
        wns::Ratio gain = testee_->getGain(wns::Position(1000.0 + x, 1000.0 + y, 0.0), PatternPtr());
        std::cout << ii << "\t" << x << "\t" << y << "\t" << gain.get_dB() << std::endl;
    }
}
