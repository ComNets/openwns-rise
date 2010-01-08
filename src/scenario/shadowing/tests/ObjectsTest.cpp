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


#include <RISE/scenario/shadowing/Objects.hpp>
#include <RISE/scenario/shadowing/Shadowing.hpp>
#include <RISE/antenna/tests/AntennaDropIn.hpp>
#include <RISE/stations/tests/StationDropIn.hpp>
#include <RISE/manager/tests/SystemManagerDropIn.hpp>

#include <WNS/CppUnit.hpp>
#include <WNS/pyconfig/helper/Functions.hpp>

#include <cppunit/extensions/HelperMacros.h>
#include <iostream>

namespace rise { namespace scenario { namespace shadowing { namespace tests {

    class ObjectsTest :
        public CppUnit::TestFixture
    {
        CPPUNIT_TEST_SUITE( ObjectsTest );
        CPPUNIT_TEST( testGetShadowing );
        CPPUNIT_TEST_SUITE_END();
    public:
        void setUp();
        void tearDown();
        void testGetShadowing();


    private:
        rise::tests::SystemManagerDropIn* systemManager;
        rise::tests::StationDropIn* station;
        rise::tests::AntennaDropIn* antenna1;
        rise::tests::AntennaDropIn* antenna2;
        rise::tests::AntennaDropIn* antenna3;
        rise::tests::AntennaDropIn* antenna4;
        rise::tests::AntennaDropIn* antenna5;
        rise::tests::AntennaDropIn* antenna6;
        rise::scenario::shadowing::Objects* objects;
    };

    CPPUNIT_TEST_SUITE_REGISTRATION( ObjectsTest );

    void ObjectsTest::setUp()
    {
        wns::pyconfig::View configView = wns::pyconfig::helper::
            createViewFromDropInConfig("rise.scenario.Shadowing",
                                       "ObjectsTest");
        systemManager = new rise::tests::SystemManagerDropIn(wns::pyconfig::
                                                             helper::createViewFromDropInConfig("rise.System",
                                                                                                "DropIn"),
                                                             (const std::string) "ObjectTest");
        station = new rise::tests::StationDropIn(systemManager);

        antenna1 = new rise::tests::AntennaDropIn(station, configView.getView("antennas", 0) );
        antenna2 = new rise::tests::AntennaDropIn(station, configView.getView("antennas", 1) );
        antenna3 = new rise::tests::AntennaDropIn(station, configView.getView("antennas", 2) );
        antenna4 = new rise::tests::AntennaDropIn(station, configView.getView("antennas", 3) );
        antenna5 = new rise::tests::AntennaDropIn(station, configView.getView("antennas", 4) );
        antenna6 = new rise::tests::AntennaDropIn(station, configView.getView("antennas", 5) );

        // inform all antennas about their position
        station->positionChanged();

        objects = new rise::scenario::shadowing::Objects(configView);
    }


    void ObjectsTest::tearDown()
    {
    }

    void ObjectsTest::testGetShadowing()
    {
        WNS_ASSERT_MAX_REL_ERROR(6.0, objects->getShadowing(*antenna1, *antenna2).get_dB(), 1E-6);
        WNS_ASSERT_MAX_REL_ERROR(6.0, objects->getShadowing(*antenna2, *antenna1).get_dB(), 1E-6);
        WNS_ASSERT_MAX_REL_ERROR(14.0, objects->getShadowing(*antenna3, *antenna4).get_dB(), 1E-6);
        WNS_ASSERT_MAX_REL_ERROR(14.0, objects->getShadowing(*antenna4, *antenna3).get_dB(), 1E-6);
        WNS_ASSERT_MAX_REL_ERROR(18.0, objects->getShadowing(*antenna5, *antenna6).get_dB(), 1E-6);
        WNS_ASSERT_MAX_REL_ERROR(18.0, objects->getShadowing(*antenna6, *antenna5).get_dB(), 1E-6);
    }
}}}}






