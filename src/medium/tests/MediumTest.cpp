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

#include <RISE/medium/tests/MediumTest.hpp>

using namespace rise;
using namespace rise::medium;
using namespace rise::medium::tests;

CPPUNIT_TEST_SUITE_REGISTRATION( MediumTest );

void MediumTest::setUp()
{
}

void MediumTest::tearDown()
{
	Medium::deleteInstance();
}

void MediumTest::testGetPhysicalResource()
{
 	Medium* testMed = Medium::getInstance();
 	double frequency1 = 20;
 	double frequency2 = 50;
 	double frequency3 = 80;
 	double bandwidth1 =  5;
 	double bandwidth2 = 10;

 	PhysicalResource* pr    = testMed->getPhysicalResource(frequency1,bandwidth1);
 	PhysicalResource* pr1_1 = testMed->getPhysicalResource(frequency1,bandwidth1);
 	PhysicalResource* pr2_1 = testMed->getPhysicalResource(frequency2,bandwidth1);
 	PhysicalResource* pr3_2 = testMed->getPhysicalResource(frequency3,bandwidth2);

 	// case same Frequency; same Bandwidth --> same PhysicalResource
 	CPPUNIT_ASSERT(pr == pr1_1 );

     // case diff. Frequencies; same Bandwidth --> diff. PhysicalResources
 	CPPUNIT_ASSERT(pr != pr2_1 );

 	// case diff Frequencies; diff. Bandwidths --> diff. PhysicalResources
	CPPUNIT_ASSERT(pr != pr3_2 );
}



void MediumTest::testOverlapForEqualFreqDiffBandwidths()
{
	Medium* testMed = Medium::getInstance();
    double frequency  = 100;
    double bandwidth1 =  10;
    double bandwidth2 =   5;

    // PhysicalResources with same Frequency and diff. Bandwidth
    testMed->getPhysicalResource(frequency, bandwidth1);
    testMed->getPhysicalResource(frequency, bandwidth2);
}

void MediumTest::testOverlapForDiffFreqNoOverlap()
{
	Medium* testMed = Medium::getInstance();
    double frequency1 = 100;
    double frequency2 = 120;
    double frequency3 = 140;
	double bandwidth  =  10;
	double bandwidth2 =   5;

    // not overlapping PhysicalResources
	testMed->getPhysicalResource(frequency1,bandwidth);
    testMed->getPhysicalResource(frequency2,bandwidth);
	testMed->getPhysicalResource(frequency3,bandwidth2);
}

void MediumTest::testOverlapForF1ubEqualF2lb()
{
	Medium* testMed = Medium::getInstance();
    double frequency1 = 100;
    double frequency2 = 120;
    double bandwidth  =  20;

    //PhysicalResoureces with f1ub = f2lb
    testMed->getPhysicalResource(frequency1, bandwidth);
    testMed->getPhysicalResource(frequency2, bandwidth);
}

void MediumTest::testOverlapForDiffFreqWithOverlap()
{
	Medium* testMed = Medium::getInstance();
    double frequency1 = 100;
    double frequency2 = 120;
	double bandwidth1 =  25;
	double bandwidth2 =  20;

    //overlapping PhysicalResources
	testMed->getPhysicalResource(frequency1,bandwidth1);
    testMed->getPhysicalResource(frequency2,bandwidth2);
}
void MediumTest::testOverlapForManyFreqOneWithOverlap()
{
	Medium* testMed = Medium::getInstance();
	double frequency1 = 100;
    double frequency2 = 120;
    double frequency3 = 140;
	double frequency4 = 115;
	double bandwidth1 =   5;
	double bandwidth2 =  10;

    //not overlapping PhysicalResources
	testMed->getPhysicalResource(frequency1,bandwidth1);
    testMed->getPhysicalResource(frequency2,bandwidth1);
	testMed->getPhysicalResource(frequency3,bandwidth2);

    //overlapping with PhysicalResource pr2
	testMed->getPhysicalResource(frequency4,bandwidth2);
}



