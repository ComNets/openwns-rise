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

#include <RISE/plmapping/PhyMode.hpp>
#include <RISE/plmapping/CodeRates.hpp>

#include <WNS/pyconfig/Parser.hpp>
#include <WNS/pyconfig/helper/Functions.hpp>
#include <WNS/node/tests/Stub.hpp>
#include <WNS/TestFixture.hpp>
#include <cppunit/extensions/HelperMacros.h>

#include <iostream>
#include <iomanip>
#include <fstream>

namespace rise { namespace plmapping { namespace tests {

	class PhyModeTest
		: public wns::TestFixture
	{
 		CPPUNIT_TEST_SUITE( PhyModeTest );
		CPPUNIT_TEST( testConstructorDestructor );
		CPPUNIT_TEST( testPythonConstructor );
		CPPUNIT_TEST( testModulation );
      		CPPUNIT_TEST( testCoding );
		CPPUNIT_TEST( testModulationCoding );
		CPPUNIT_TEST( testSNR2MI );
		CPPUNIT_TEST( testMI2PER );
		CPPUNIT_TEST( testSNR2PER );
//		CPPUNIT_TEST( testXXX );
		CPPUNIT_TEST_SUITE_END();
	public:
		PhyModeTest();
		~PhyModeTest();
                void prepare();
                void cleanup();
		void testConstructorDestructor();
		void testPythonConstructor();
		void testModulation();
		void testCoding();
		void testModulationCoding();
 		void testSNR2MI();
 		void testMI2PER();
 		void testSNR2PER();
// 		void testInt();
//		void testInt2();
		bool useCout;

		PhyMode* testPhyMode;

		wns::service::phy::phymode::SNR2MIInterface* snr2miMapper; // Singleton
		wns::service::phy::phymode::CoderMappingInterface* coderMapper; // Singleton
		wns::service::phy::phymode::CoderFullMappingInterface* mi2perMapper; // Singleton
		Coding maxCodings;
	};

} // tests
} // plmapping
} // rise

using namespace rise;
using namespace rise::plmapping::tests;


/********************************* Test *****************************/

CPPUNIT_TEST_SUITE_REGISTRATION( PhyModeTest );

PhyModeTest::PhyModeTest() : /* 1. */
        wns::TestFixture(),
	useCout(false),
	testPhyMode(NULL),
	coderMapper(NULL), // Singleton
	maxCodings(0u)
{
}

PhyModeTest::~PhyModeTest() /* 5. */
{
}

void PhyModeTest::prepare() /* 2. */
{
	wns::pyconfig::Parser pyconfig;
	pyconfig.loadString(
		"import rise.CoderSpecification\n"
		"import rise.SNR2MI\n"
		"coderMapping = rise.CoderSpecification.default\n"
		"mi2perMapper = rise.CoderSpecification.defaultCoderMapping\n"
		//"snr2miMapping = rise.SNR2MI.default\n"
		"snr2miMapping = rise.SNR2MI.Formula()\n"
		//"\n"
		);
	wns::pyconfig::View snr2miConfig = pyconfig.getView("snr2miMapping");
	snr2miMapper = wns::service::phy::phymode::SNR2MIInterface::getSNR2MImapper(snr2miConfig);
	wns::pyconfig::View coderMappingConfig = pyconfig.getView("coderMapping");
	// small interface:
	coderMapper = wns::service::phy::phymode::CoderMappingInterface::getCoderMapping(coderMappingConfig);
	CPPUNIT_ASSERT(coderMapper != NULL);
	// big interface:
	mi2perMapper = wns::service::phy::phymode::CoderFullMappingInterface::getCoderFullMapping(coderMappingConfig);
	CPPUNIT_ASSERT(mi2perMapper != NULL);
	CPPUNIT_ASSERT(mi2perMapper == coderMapper);

	//testPhyMode = new PhyMode(UNDEFINED_PHYMODE);
	testPhyMode = new PhyMode();

} // prepare

void PhyModeTest::cleanup() /* 4. */
{
	delete testPhyMode;
	testPhyMode=NULL;

	coderMapper = NULL;
}

void PhyModeTest::testConstructorDestructor() /* 3. */
{
	CPPUNIT_ASSERT(testPhyMode != NULL);
	CPPUNIT_ASSERT_EQUAL(UNDEFINED_PHYMODE,testPhyMode->toInt());
	//CPPUNIT_ASSERT(bool);
	//CPPUNIT_ASSERT_EQUAL(soll,ist);
	//CPPUNIT_ASSERT_DOUBLES_EQUAL(soll,ist,errorTol)
	//CPPUNIT_ASSERT_MESSAGE("msg",bool)
	//CPPUNIT_ASSERT_THROW(foo(-1), wns::Assure::Exception)
	//WNS_ASSERT_MAX_REL_ERROR(soll,ist, 1E-3);
}

void PhyModeTest::testPythonConstructor() /* 3. */
{
	CPPUNIT_ASSERT(testPhyMode != NULL);
	delete testPhyMode; testPhyMode=NULL;
	wns::pyconfig::Parser pyconfig;
	pyconfig.loadString(
		"import rise.CoderSpecification\n"
		"import rise.SNR2MI\n"
		"coderMapping = rise.CoderSpecification.default\n"
		"mi2perMapper = rise.CoderSpecification.defaultCoderMapping\n"
		"snr2miMapping = rise.SNR2MI.default\n"
		"modulation = 'BPSK'\n"
		"coding = 'RCPC_BENQ-1/2'\n"
		);
	testPhyMode = new PhyMode(pyconfig);
	CPPUNIT_ASSERT_EQUAL(static_cast<unsigned int>(1), testPhyMode->getModulation());
	// look into PhyCodeRates.cpp for the mapping of strings:
	//CPPUNIT_ASSERT_EQUAL(/* should */ 14u, testPhyMode->getCoding());
}

/* this test checks for proper Modulation values (between 1 and 8) */
void PhyModeTest::testModulation() /* 3. */
{
    if (useCout)
	    std::cout << std::endl;
    for (unsigned int modulation=1; modulation <= MAX_MODULATIONS /* +1 */; modulation++) {
	    if (useCout)
		    std::cout << "modulation            = " << modulation << std::endl;
	    delete testPhyMode; testPhyMode=NULL;
	    if (modulation <= MAX_MODULATIONS) {
		    //testPhyMode = new PhyMode(modulation, UNDEFINED_CODING, coderMapper);
		    testPhyMode = new PhyMode(modulation, 1u, snr2miMapper, mi2perMapper);
		    if (useCout)
			    std::cout << "getModulation()       = " << testPhyMode->getModulation() << std::endl;
		    CPPUNIT_ASSERT_EQUAL(modulation, testPhyMode->getModulation());
		    if (useCout)
			    std::cout << "getCoding()           = " << testPhyMode->getCoding() << std::endl;
		    CPPUNIT_ASSERT_EQUAL(1u, testPhyMode->getCoding());
		    if (useCout) {
			    std::cout << "getCodeRate()         = " << testPhyMode->getCodeRate() << std::endl;
			    std::cout << "getModulationstd::string() = " << testPhyMode->getModulationString() << std::endl;
			    std::cout << "getCodingstd::string()     = " << testPhyMode->getCodingString() << std::endl;
			    std::cout << "getstd::string()           = " << testPhyMode->getString() << std::endl;
		    }
	    //std::cout << "" << std::endl;
	    } else {
		    //CPPUNIT_ASSERT_THROW( testPhyMode = new PhyMode(modulation, UNDEFINED_CODING, coderMapper), wns::Assure::Exception);
		    CPPUNIT_ASSERT_THROW( testPhyMode = new PhyMode(modulation, UNDEFINED_CODING, snr2miMapper, mi2perMapper), wns::Assure::Exception);
	    }
	//outputStream << std::endl;
    }
    //outputStream.close();
}


void PhyModeTest::testCoding() /* 3. */
{
    if (useCout)
	    std::cout << std::endl;

    for (Coding coding=1u; coding <= maxCodings; coding++) {
	    if (useCout)
		    std::cout << "coding            = " << coding << std::endl;
	    delete testPhyMode; testPhyMode=NULL;
	    if (coding <= maxCodings) {
		    //testPhyMode = new PhyMode(UNDEFINED_MODULATION, coding, coderMapper);
		    testPhyMode = new PhyMode(1u, coding, snr2miMapper, mi2perMapper);
		    if (useCout)
			    std::cout << "getModulation()       = " << testPhyMode->getModulation() << std::endl;
		    CPPUNIT_ASSERT_EQUAL(1u, testPhyMode->getModulation());
		    if (useCout)
			    std::cout << "getCoding()           = " << testPhyMode->getCoding() << std::endl;
		    CPPUNIT_ASSERT_EQUAL(coding, testPhyMode->getCoding());
		    if (useCout) {
			    std::cout << "getCodeRate()         = " << testPhyMode->getCodeRate() << std::endl;
			    std::cout << "getModulationstd::string() = " << testPhyMode->getModulationString() << std::endl;
			    std::cout << "getCodingstd::string()     = " << testPhyMode->getCodingString() << std::endl;
			    std::cout << "getstd::string()           = " << testPhyMode->getString() << std::endl;
		    }
	    //std::cout << "" << std::endl;
	    } else {
		    // this gives a SegFault:
		    //CPPUNIT_ASSERT_THROW( testPhyMode = new PhyMode(UNDEFINED_MODULATION, coding, coderMapper), wns::Assure::Exception);
		    CPPUNIT_ASSERT_THROW( testPhyMode = new PhyMode(1u, coding, snr2miMapper, mi2perMapper), wns::Assure::Exception);
	    }
	//outputStream << std::endl;
    }
    //outputStream.close();
}

void PhyModeTest::testModulationCoding() /* 3. */
{
    for (unsigned int modulation=1; modulation <= MAX_MODULATIONS /* +1 */; modulation++) {
	    for (Coding coding=1u; coding <= maxCodings; coding++) {
		    if (useCout)
			    std::cout << "modulation            = " << modulation<< ", coding            = " << coding << std::endl;
	            delete testPhyMode; testPhyMode=NULL;
		    if ((modulation <= MAX_MODULATIONS)&&(coding <= maxCodings)) {
			    //testPhyMode = new PhyMode(modulation, coding, coderMapper);
			    testPhyMode = new PhyMode(modulation, coding, snr2miMapper, mi2perMapper);
			    if (useCout)
				    std::cout << "getModulation()       = " << testPhyMode->getModulation() << std::endl;
			    CPPUNIT_ASSERT_EQUAL(modulation, testPhyMode->getModulation());
			    if (useCout)
				    std::cout << "getCoding()           = " << testPhyMode->getCoding() << std::endl;
			    CPPUNIT_ASSERT_EQUAL(coding, testPhyMode->getCoding());
			    if (useCout) {
				    std::cout << "getCodeRate()         = " << testPhyMode->getCodeRate() << std::endl;
				    std::cout << "getModulationstd::string() = " << testPhyMode->getModulationString() << std::endl;
				    std::cout << "getCodingstd::string()     = " << testPhyMode->getCodingString() << std::endl;
				    std::cout << "getstd::string()           = " << testPhyMode->getString() << std::endl;
			    }
		    } else {
			    // this gives a SegFault:
			    //CPPUNIT_ASSERT_THROW( testPhyMode = new PhyMode(modulation, coding, coderMapper), wns::Assure::Exception);
			    CPPUNIT_ASSERT_THROW( testPhyMode = new PhyMode(modulation, coding, snr2miMapper, mi2perMapper), wns::Assure::Exception);
		    }
	    }
		    //std::cout << "" << std::endl;
	//outputStream << std::endl;
    }
    //outputStream.close();
}

void PhyModeTest::testSNR2MI()
{
	// TODO: write out graph data files for Matlab|Gnuplot
	for (unsigned int modulation=1; modulation <= MAX_MODULATIONS; modulation++) {
		Coding coding=1u;
		//for (Coding coding=1u; coding < maxCodings; coding++) {
			if (useCout) std::cout << "modulation            = " << modulation<< ", coding            = " << coding << std::endl;
			delete testPhyMode; testPhyMode=NULL;
			testPhyMode = new PhyMode(modulation, coding, snr2miMapper, mi2perMapper);
			if (useCout) std::cout << "testSNR2MI(): PhyMode = " << testPhyMode->getString() << std::endl;
			for (double sinr=0; sinr<=20; sinr+=5) {
				wns::Ratio snr = wns::Ratio::from_dB(sinr);
				double mib = testPhyMode->getSINR2MIB(snr);
				if (useCout) std::cout << "  SNR="<<snr<<" => MIB="<<mib<< std::endl;
				CPPUNIT_ASSERT(mib>=0.0);
				CPPUNIT_ASSERT(mib<=1.0);
				// TODO: more tests/asserts
				//CPPUNIT_ASSERT();
			}
		 //}
	}
} // PhyModeTest::testSNR2MI()

void PhyModeTest::testMI2PER()
{
	// TODO: write out graph data files for Matlab|Gnuplot
	for (unsigned int modulation=1; modulation <= MAX_MODULATIONS; modulation++) {
		for (Coding coding=1u; coding <= maxCodings; coding++) {
			if (useCout) std::cout << "modulation            = " << modulation<< ", coding            = " << coding << std::endl;
			delete testPhyMode; testPhyMode=NULL;
			testPhyMode = new PhyMode(modulation, coding, snr2miMapper, mi2perMapper);
			if (useCout) std::cout << "testMI2PER(): PhyMode = " << testPhyMode->getString() << std::endl;
			unsigned int bits = 1000;
			for (double mib=0.5; mib<=1.0; mib+=0.05) {
				double per = testPhyMode->getMI2PER(mib,bits);
				if (useCout) std::cout << "  MIB="<<mib<<" => PER="<<per<< std::endl;
				CPPUNIT_ASSERT(per<=1.0);
				// TODO: more tests/asserts
				//CPPUNIT_ASSERT();
			}
		}
	}
}

void PhyModeTest::testSNR2PER()
{
	// TODO: write out graph data files for Matlab|Gnuplot
	for (unsigned int modulation=1; modulation <= MAX_MODULATIONS; modulation++) {
		for (Coding coding=1u; coding < maxCodings; coding++) {
			if (useCout) std::cout << "modulation            = " << modulation<< ", coding            = " << coding << std::endl;
			delete testPhyMode; testPhyMode=NULL;
			testPhyMode = new PhyMode(modulation, coding, snr2miMapper, mi2perMapper);
			if (useCout) std::cout << "testSNR2MI(): PhyMode = " << testPhyMode->getString() << std::endl;
			unsigned int bits = 1000;
			for (double sinr=0; sinr<=20; sinr+=5) {
				wns::Ratio snr = wns::Ratio::from_dB(sinr);
				//double mib = testPhyMode->getSINR2MIB(snr);
				double per = testPhyMode->getSINR2PER(snr,bits);
				if (useCout) std::cout << "  SNR="<<snr<<" => PER="<<per<< std::endl;
				CPPUNIT_ASSERT(per<=1.0);
				// TODO: more tests/asserts
				//CPPUNIT_ASSERT();
			}
		 }
	}
} // PhyModeTest::testSNR2PER()



