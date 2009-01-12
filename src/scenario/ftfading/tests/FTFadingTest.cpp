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

#include <RISE/scenario/ftfading/FTFading.hpp>
#include <RISE/scenario/ftfading/JakesFadingGenerator.hpp>

#include <WNS/CppUnit.hpp>
#include <WNS/pyconfig/helper/Functions.hpp>
#include <WNS/pyconfig/View.hpp>
#include <WNS/events/NoOp.hpp>

#include <stdexcept>
#include <iostream>
#include <fstream>

#include <cppunit/extensions/HelperMacros.h>

namespace rise { namespace scenario { namespace ftfading { namespace tests {


	class FTFadingTest
		: public wns::TestFixture
	{

 		CPPUNIT_TEST_SUITE(FTFadingTest);
		CPPUNIT_TEST(test_FTFadingOff);
		CPPUNIT_TEST(test_FTFadingFflat);
		CPPUNIT_TEST(test_FTFadingFuncorrelated);
		CPPUNIT_TEST(test_FTFadingFneighbourCorrelation);
		CPPUNIT_TEST_SUITE_END();
	public:
		FTFadingTest();
		~FTFadingTest();
		void prepare();
		void cleanup();
		void test_FTFadingOff();
		void test_FTFadingFflat();
		void test_FTFadingFuncorrelated();
		void test_FTFadingFneighbourCorrelation();
		bool useCout;
		bool useFout;

		FTFadingOff* test1;
		FTFadingFflat* test2;
		FTFadingFuncorrelated* test3;
		FTFadingFneighbourCorrelation* test4;



	};
}//tests
}//ftfading
}//scenario
}//rise

using namespace rise;
using namespace rise::scenario::ftfading;
using namespace rise::scenario::ftfading::tests;


/*********************************** FTFadingTest *************************************/

CPPUNIT_TEST_SUITE_REGISTRATION( FTFadingTest );

FTFadingTest::FTFadingTest() :
        wns::TestFixture(),
	useCout(false),
	useFout(false),
	test1(NULL),
	test2(NULL),
	test3(NULL),
	test4(NULL)
{
}

FTFadingTest::~FTFadingTest()
{
}

void FTFadingTest::prepare()
{
	wns::pyconfig::Parser FT_FadingOffpyconfig;
	FT_FadingOffpyconfig.loadString(
		"from rise.scenario.FTFading import * \n"
		"import os \n"
		"ftfading = FTFadingOff()\n"
		"outputDir = 'FTFadingOutput.junk' \n"
		"if (not os.access(outputDir, os.F_OK)): \n"
		"    os.mkdir(outputDir)\n"
		);

	wns::pyconfig::View FTFadingOffpyconfig = FT_FadingOffpyconfig.getView("ftfading");

	wns::pyconfig::Parser FTJakespyconfig;
	FTJakespyconfig.loadString(
		"from rise.scenario.FTFading import * \n"
		"ftfading = FTFadingJakes(samplingTime = '0.0005',dopFreq = '100',numWaves = '100',numSubCarriers = '100')\n"
                 );

	wns::pyconfig::View FTFadingJakespyconfig = FTJakespyconfig.getView("ftfading");

	wns::pyconfig::Parser FT_neigh_pyconfig;
	FT_neigh_pyconfig.loadString(
		"from rise.scenario.FTFading import * \n"
		"ftfading_Fneigh = FTFadingFneighbourCorrelation(samplingTime = '0.0005',neighbourCorrelationFactor = '0.99',dopFreq = '10',numWaves = '100',numSubCarriers = '100')\n"

		);

	wns::pyconfig::View FTFadingNeigh_pyconfig = FT_neigh_pyconfig.getView("ftfading_Fneigh");

	wns::simulator::getEventScheduler()->reset();

	test1 = new FTFadingOff(FTFadingOffpyconfig);
	test2 = new FTFadingFflat(FTFadingJakespyconfig);
	test3 = new FTFadingFuncorrelated(FTFadingJakespyconfig);
	test4 = new FTFadingFneighbourCorrelation(FTFadingNeigh_pyconfig);
}

void FTFadingTest::cleanup()
{
	delete test1;
	delete test2;
	delete test3;
	delete test4;
}


void FTFadingTest::test_FTFadingOff()
{
	/** This test only checks that FTFadingOff strategy doesn't apply any Fading. The
	 * plot of the FTFading provided by this strategy is also created (when activating useFout) **/

	std::ofstream fout1;
	fout1.open("FTFadingOutput.junk/test_FTFadingOff_fading.dat",std::ofstream::out);
	std::ofstream fout2;
	fout2.open("FTFadingOutput.junk/test_FTFadingOff_singleTimeShot.dat",std::ofstream::out);

	wns::Ratio OneFadingTest = wns::Ratio::from_factor(1);

	int samples_test = 500;
	int numSubCarriers_test = 100; // In this case, not configurable from the python file.

	for (int samp = 0; samp < samples_test; ++samp){
		wns::simulator::getEventScheduler()->scheduleDelay(wns::events::NoOp(), 0.0005);
		wns::simulator::getEventScheduler()->processOneEvent();
		for (int subc = 0; subc < numSubCarriers_test; ++subc){
			CPPUNIT_ASSERT_EQUAL(OneFadingTest, test1->getFTFading(subc));
			if (useFout) {
				fout1 << test1->getFTFading(subc).get_dB() << " " << samp << " " << subc << std::endl;
				/* We choose any sample to have a single shot in time */
				if (samp == 100)
					fout2 << test1->getFTFading(subc).get_dB() << " " << subc << std::endl;
			}
			if (useCout)
				std::cout << "Fading provided for the FTFadingOff strategy: " << test1->getFTFading(subc) << std::endl;
		}
	}
	fout1.close();
	fout2.close();
}



/** This test checks that FTFadingFflat strategy aplies the same fading to all the
 * subchannels.The plot of the FTFading provided by this strategy is also
 * created (when activating useFout) **/
void FTFadingTest::test_FTFadingFflat()
{
	std::ofstream fout1;
	fout1.open("FTFadingOutput.junk/test_FTFadingFflat_fading.dat",std::ofstream::out);
	std::ofstream fout2;
	fout2.open("FTFadingOutput.junk/test_FTFadingFflat_singleTimeShot.dat",std::ofstream::out);

	int samples_test = 500;
	int numSubCarriers_test = test2->numberOfSubChannels;

	for (int samp = 0; samp < samples_test; ++samp){
		wns::simulator::getEventScheduler()->scheduleDelay(wns::events::NoOp(), 0.0005);
		wns::simulator::getEventScheduler()->processOneEvent();
		for (int subc = 0; subc < numSubCarriers_test; ++subc){
			if (useFout) {
				fout1 << test2->getFTFading(subc).get_dB() << " " << samp << " " << subc << std::endl;
				/* We choose any sample to have a single shot in time */
				if (samp == 100)
					fout2 << test2->getFTFading(subc).get_dB() << " " << subc << std::endl;
			}
		}
	}

	for (int samp = 0; samp < samples_test; ++samp){
		wns::simulator::getEventScheduler()->scheduleDelay(wns::events::NoOp(), 0.0005);
		wns::simulator::getEventScheduler()->processOneEvent();
		for (int subc_1 = 0; subc_1 < numSubCarriers_test; ++subc_1){
			for (int subc_2 = 0; subc_2 < numSubCarriers_test; ++subc_2){
					CPPUNIT_ASSERT_EQUAL(test2->getFTFading(subc_1), test2->getFTFading(subc_2)); // All frequencies must have the same Fading
			}
		 	if ((useCout)&&(subc_1 == 0)){
		       		std::cout << "Fading provided for the FTFadingFflat Strategy for the SubCarrier: " << subc_1<< " and the sample: " << samp << ": " << test2->getFTFading(subc_1) << std::endl;
	      		}
		 	if ((useCout)&&(subc_1 == 4)){
		       		std::cout << "Fading provided for the FTFadingFflat Strategy for the SubCarrier: " << subc_1<< " and the sample: " << samp << ": " << test2->getFTFading(subc_1) << std::endl;
	      		}
	       	}
	}
	fout1.close();
	fout2.close();
}



/** This test checks that FTFadingFuncorrelated strategy applies a different Fading in
 * every subchannel. The plot of the FTFading provided by this strategy is also
 * created (when activating useFout) **/
void FTFadingTest::test_FTFadingFuncorrelated()
{
	std::ofstream fout1;
	fout1.open("FTFadingOutput.junk/test_FTFadingFuncorrelated_fading.dat",std::ofstream::out);
	std::ofstream fout2;
	fout2.open("FTFadingOutput.junk/test_FTFadingFuncorrelated_singleTimeShot.dat",std::ofstream::out);

	int samples_test = 500;
	int numSubCarriers_test = test3->numberOfSubChannels;

      	for (int samp = 0; samp < samples_test; ++samp) {
		wns::simulator::getEventScheduler()->scheduleDelay(wns::events::NoOp(), 0.0005);
		wns::simulator::getEventScheduler()->processOneEvent();
		for (int subc_1 = 0; subc_1 < numSubCarriers_test; ++subc_1) {
			if (useFout) {
				fout1 << test3->getFTFading(subc_1).get_dB() << " " << samp << " " << subc_1 << std::endl;

				/* We choose any sample to have a single shot in time */
				if (samp == 100)
					fout2 << test3->getFTFading(subc_1).get_dB() << " " << subc_1 << std::endl;
			}
			if (useCout) {
				if (subc_1 % 4 != 0) continue;
				for (int subc_2 = 0; subc_2 < numSubCarriers_test; ++subc_2){
					if (subc_2 % 4 != 0) continue;
					if (subc_1 != subc_2) {
						// All frequencies must have a different and uncorrelated Fading:
						CPPUNIT_ASSERT(test3->getFTFading(subc_1) != test3->getFTFading(subc_2));
					}
				}
				if ((useCout)&&(subc_1 == 1)) {
					std::cout << "Fading provided for the FTFadingFuncorrelated Strategy for the SubCarrier: " << subc_1<< " and the sample: " << samp << ": " << test3->getFTFading(subc_1) << std::endl;
				}
				if ((useCout)&&(subc_1 == 5)) {
					std::cout << "Fading provided for the FTFadingFuncorrelated Strategy for the SubCarrier: " << subc_1<< " and the sample: " << samp << ": " << test3->getFTFading(subc_1) << std::endl;
				}
			}
		}
	}
	fout1.close();
	fout2.close();
}

/** This test checks that FTFadingFneighbourCorrelation strategy applies a specific
 * correlation between neighbour subchannels creating its plot **/
void FTFadingTest::test_FTFadingFneighbourCorrelation()
{
	std::ofstream fout1;
	fout1.open("FTFadingOutput.junk/test_FTFadingFneighbourCorrelation_fading.dat",std::ofstream::out);
	std::ofstream fout2;
	fout2.open("FTFadingOutput.junk/test_FTFadingFneighbourCorrelation_singleTimeShot.dat",std::ofstream::out);

	int samples_test = 500;
	int numSubCarriers_test = test4->numberOfSubChannels;

	for (int samp = 0; samp < samples_test; ++samp) {
		wns::simulator::getEventScheduler()->scheduleDelay(wns::events::NoOp(), 0.0005);
		wns::simulator::getEventScheduler()->processOneEvent();
		for (int subc = 0; subc < numSubCarriers_test; ++subc){
			if (useFout) {
				fout1 << test4->getFTFading(subc).get_dB() << " " << samp << " " << subc << std::endl;
				/* We choose any sample to have a single shot in time */
				if (samp == 100)
					fout2 << test4->getFTFading(subc).get_dB() << " " << subc << std::endl;
			}
		}
	}
	fout1.close();
	fout2.close();
}
