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

#include <RISE/plmapping/PhyModeMapper.hpp>

#include <WNS/pyconfig/Parser.hpp>
#include <WNS/pyconfig/helper/Functions.hpp>
#include <WNS/node/tests/Stub.hpp>
#include <WNS/TestFixture.hpp>

#include <iostream>
#include <iomanip>
#include <fstream>

namespace rise { namespace plmapping { namespace tests {

	/**
	 * @brief Test of the PhyModeMapper (PhyMode-to-SINR table)
	 * @author Rainer Schoenen (rs@comnets.rwth-aachen.de)
	 */
	class PhyModeMapperTest
		: public wns::TestFixture
	{
 		CPPUNIT_TEST_SUITE( PhyModeMapperTest );
		CPPUNIT_TEST( testConstructorDestructor );
		//CPPUNIT_TEST( testPythonConstructor );
		CPPUNIT_TEST( testGetListofPhyModes );
		CPPUNIT_TEST( testGetPhyModeForIndex );
		CPPUNIT_TEST( testGetLowestPhyMode );
		CPPUNIT_TEST( testGetHighestPhyMode );
		CPPUNIT_TEST( testGetBestPhyMode );
		CPPUNIT_TEST( testGetMinSINRRatio );
		CPPUNIT_TEST( testGetMinSINR );
		CPPUNIT_TEST( testGetSINRRange );
		CPPUNIT_TEST( testMinimumSINR );
		CPPUNIT_TEST( testCalculateSINRRanges );
		CPPUNIT_TEST( testCalculateLTESINRRanges );
		//CPPUNIT_TEST(  );
		//CPPUNIT_TEST(  );
		CPPUNIT_TEST_SUITE_END();
	public:
		PhyModeMapperTest();
		~PhyModeMapperTest();
                void prepare();
                void cleanup();
		void testConstructorDestructor();
		void testGetListofPhyModes();
		void testGetPhyModeForIndex();
		void testGetLowestPhyMode();
		void testGetHighestPhyMode();
		void testGetBestPhyMode();
		void testGetMinSINRRatio();
		void testGetMinSINR();
		void testGetSINRRange();
		void testMinimumSINR();
		void testCalculateSINRRanges();
		void testCalculateLTESINRRanges();
		bool useCout;

		PhyModeMapper* testPhyModeMapper;

		wns::service::phy::phymode::CoderMappingInterface* coderMapper;
		wns::service::phy::phymode::CoderFullMappingInterface* mi2perMapper;
	};

} // tests
} // plmapping
} // rise

using namespace rise;
using namespace rise::plmapping::tests;

/********************************* Test *****************************/

CPPUNIT_TEST_SUITE_REGISTRATION( PhyModeMapperTest );

PhyModeMapperTest::PhyModeMapperTest() : /* 1. */
        wns::TestFixture(),
	useCout(false), // switch true|false here to see an output to stdout.
	testPhyModeMapper(NULL),
	coderMapper(NULL), // Singleton
	mi2perMapper(NULL) // Singleton
{
}

PhyModeMapperTest::~PhyModeMapperTest() /* 5. */
{
}

void PhyModeMapperTest::prepare() /* 2. */
{
	wns::pyconfig::Parser pyconfig;
	pyconfig.loadString(
		"import rise.CoderSpecification\n"
		"coderMapping = rise.CoderSpecification.default\n" // Table
		//"coderMapping = rise.CoderSpecification.CoderMapping(mapping = \"Formula\")\n"
		//"\n"
		);
	wns::pyconfig::View coderMappingConfig = pyconfig.getView("coderMapping");
	coderMapper  = wns::service::phy::phymode::CoderMappingInterface::getCoderMapping(coderMappingConfig);
	mi2perMapper = wns::service::phy::phymode::CoderFullMappingInterface::getCoderFullMapping(coderMappingConfig);
	CPPUNIT_ASSERT(coderMapper != NULL);
	CPPUNIT_ASSERT(mi2perMapper != NULL);
	CPPUNIT_ASSERT(coderMapper == dynamic_cast<wns::service::phy::phymode::CoderMappingInterface*>(mi2perMapper));

	// testPhyModeMapper
	CPPUNIT_ASSERT(testPhyModeMapper == NULL);

	pyconfig.loadString(
		"from rise.PhyMode import PhyMode,PhyModeMapper\n"
		"from rise.SNR2MI import default,Table,Formula\n"
		"from rise.CoderSpecification import defaultCoderMapping,TableCoder,FormulaCoder\n"
		"from openwns.interval import Interval\n\n"
		"bpsk  = PhyMode(modulation = \"BPSK\",  coding = \"LDPC_SEUK-1/2\")\n"
		"qpsk  = PhyMode(modulation = \"QPSK\",  coding = \"LDPC_SEUK-1/2\")\n"
		"qam16 = PhyMode(modulation = \"QAM16\", coding = \"LDPC_SEUK-1/2\")\n"
		"qam64 = PhyMode(modulation = \"QAM64\", coding = \"LDPC_SEUK-1/2\")\n"
		"snr2miMapper = rise.SNR2MI.default\n"
		//"snr2miMapper = rise.SNR2MI.Formula()\n"
		"mi2perMapper = rise.CoderSpecification.defaultCoderMapping\n"
		//"mi2perMapper = rise.CoderSpecification.CoderMapping(mapping = "Formula")\n"
		"class Baseline(PhyModeMapper):\n"
		"    def __init__(self):\n"
		"        symbolDuration = 1e-6\n"
		"        subCarriersPerSubChannel = 12\n"
		"        # configure base class rise.PhyMode.PhyModeMapper():\n"
		"        super(Baseline, self).__init__(symbolDuration, subCarriersPerSubChannel, snr2miMapper=snr2miMapper, mi2perMapper=mi2perMapper)\n"
		"        self.setMinimumSINR(-1.0);\n"
 		"        self.addPhyMode(Interval(-200.0,   1.0, \"(]\"), bpsk)\n"
		"        self.addPhyMode(Interval(   1.0,   6.5, \"(]\"), qpsk)\n"
		"        self.addPhyMode(Interval(   6.5,  14.0, \"(]\"), qam16)\n"
		"        self.addPhyMode(Interval(  14.0, 200.0, \"(]\"), qam64)\n"
		"phyModeMapper = Baseline()\n"
		);
	testPhyModeMapper = new PhyModeMapper(pyconfig.getView("phyModeMapper"));
} // prepare

void PhyModeMapperTest::cleanup() /* 4. */
{
	delete testPhyModeMapper;
	testPhyModeMapper=NULL;
}

void PhyModeMapperTest::testConstructorDestructor() /* 3. */
{
	CPPUNIT_ASSERT(coderMapper != NULL); // global Singleton
	CPPUNIT_ASSERT(coderMapper->getMaxCodeIndex() > 0u);
	CPPUNIT_ASSERT(testPhyModeMapper != NULL);
}

void PhyModeMapperTest::testGetListofPhyModes()
{
	PhyModeMapper::PhyModeVector phyModeVector =
		testPhyModeMapper->getListofPhyModes();
	//const std::vector<wns::SmartPtr<const wns::service::phy::phymode::PhyModeInterface> > phyModePtrVector =
	const std::vector< wns::service::phy::phymode::PhyModeInterfacePtr > phyModePtrVector =
		testPhyModeMapper->getListofPhyModePtr();
	int listSize = (int)phyModeVector.size();
	if (useCout) std::cout << "testGetListofPhyModes(): "<<listSize<<" elements" << std::endl;
	CPPUNIT_ASSERT(listSize > 0);
	CPPUNIT_ASSERT_EQUAL(4, listSize); // according to Python spec above
	CPPUNIT_ASSERT_EQUAL(listSize, testPhyModeMapper->getPhyModeCount());
	for (unsigned int phymodeIndex=0; (int)phymodeIndex<listSize; ++phymodeIndex){
		if (useCout) std::cout << "phyModeVector["<<phymodeIndex<<"]: valid="<<phyModeVector[phymodeIndex]->isValid() << std::endl;
		CPPUNIT_ASSERT(phyModeVector[phymodeIndex]->isValid());
		CPPUNIT_ASSERT(phyModePtrVector[phymodeIndex]->isValid());
		//CPPUNIT_ASSERT_EQUAL(&phyModeVector[phymodeIndex], phyModePtrVector[phymodeIndex]);
		//CPPUNIT_ASSERT_EQUAL(phyModeVector[phymodeIndex], *phyModePtrVector[phymodeIndex]);
		const PhyMode phyMode = *phyModeVector[phymodeIndex];
		const PhyMode* phyModePtr = static_cast<const rise::plmapping::PhyMode*>(phyModePtrVector[phymodeIndex].getPtr());
		CPPUNIT_ASSERT_EQUAL(phyMode,*phyModePtr);
		CPPUNIT_ASSERT(phyMode.isValid());
		CPPUNIT_ASSERT(phyModePtr->isValid());
		CPPUNIT_ASSERT(phyMode.dataRateIsValid());
		CPPUNIT_ASSERT(phyModePtr->dataRateIsValid());
		CPPUNIT_ASSERT(phyMode.toInt() > 0);
		CPPUNIT_ASSERT(phyModePtr->toInt() > 0);
		CPPUNIT_ASSERT_EQUAL(phyMode.toInt(), phyModePtr->toInt());
		double bitsPerSymbol = phyMode.getBitsPerSymbol();
		CPPUNIT_ASSERT(bitsPerSymbol > 0.0);
		double dataRate = phyMode.getDataRate();
		CPPUNIT_ASSERT(dataRate > 0.0);
		CPPUNIT_ASSERT_EQUAL(bitsPerSymbol,phyMode.getDataRate(1.0,1));
		if (phymodeIndex>0) // compare order
			CPPUNIT_ASSERT(PhyModeIsBetter(phyMode,*phyModeVector[phymodeIndex-1]));
		//CPPUNIT_ASSERT();
		//if (useCout) std::cout << ""<< << std::endl;
	}
}

void PhyModeMapperTest::testGetPhyModeForIndex()
{
	unsigned int phyModeCount = testPhyModeMapper->getPhyModeCount();
	unsigned int phymodeIndex = phyModeCount/2; // middle
	//const PhyMode& phyMode = testPhyModeMapper->getPhyModeForIndex(phymodeIndex);
	//wns::SmartPtr<const wns::service::phy::phymode::PhyModeInterface> phyMode = testPhyModeMapper->getPhyModeForIndex(phymodeIndex);
	wns::service::phy::phymode::PhyModeInterfacePtr phyModePtr = testPhyModeMapper->getPhyModeForIndex(phymodeIndex);
	CPPUNIT_ASSERT(phyModePtr->isValid());
	unsigned int phymodeIndex2 = testPhyModeMapper->getIndexForPhyMode(*phyModePtr);
	CPPUNIT_ASSERT_EQUAL(phymodeIndex,phymodeIndex2);
}

void PhyModeMapperTest::testGetLowestPhyMode()
{
	//const PhyMode& phyMode = testPhyModeMapper->getLowestPhyMode();
	//wns::SmartPtr<const wns::service::phy::phymode::PhyModeInterface> phyMode = testPhyModeMapper->getLowestPhyMode();
	wns::service::phy::phymode::PhyModeInterfacePtr phyModePtr = testPhyModeMapper->getLowestPhyMode();
	CPPUNIT_ASSERT(phyModePtr->isValid());
}

void PhyModeMapperTest::testGetHighestPhyMode()
{
	//const PhyMode& phyMode = testPhyModeMapper->getHighestPhyMode();
	//wns::SmartPtr<const wns::service::phy::phymode::PhyModeInterface> phyMode = testPhyModeMapper->getHighestPhyMode();
	wns::service::phy::phymode::PhyModeInterfacePtr phyModePtr = testPhyModeMapper->getHighestPhyMode();
	CPPUNIT_ASSERT(phyModePtr->isValid());
}

void PhyModeMapperTest::testGetBestPhyMode()
{
	for (double sinr=-2.0; sinr<20.0; sinr+=1.0) {
		//wns::SmartPtr<const wns::service::phy::phymode::PhyModeInterface> phyModePtr =
		wns::service::phy::phymode::PhyModeInterfacePtr phyModePtr =
			testPhyModeMapper->getBestPhyMode(sinr);
		CPPUNIT_ASSERT(phyModePtr->isValid());
		unsigned int bestPhyModeIndex = testPhyModeMapper->getBestPhyModeIndex(sinr);
		//wns::SmartPtr<const wns::service::phy::phymode::PhyModeInterface> phyModePtr2 =
		wns::service::phy::phymode::PhyModeInterfacePtr phyModePtr2 =
			testPhyModeMapper->getPhyModeForIndex(bestPhyModeIndex);
		CPPUNIT_ASSERT(*phyModePtr == *phyModePtr2);
	}
}

void PhyModeMapperTest::testGetMinSINRRatio()
{
	//wns::SmartPtr<const wns::service::phy::phymode::PhyModeInterface> phyMode =
	wns::service::phy::phymode::PhyModeInterfacePtr phyModePtr =
		testPhyModeMapper->getHighestPhyMode();
	CPPUNIT_ASSERT(phyModePtr->isValid());
	wns::Ratio ratio = testPhyModeMapper->getMinSINRRatio(*phyModePtr);
	if (useCout) std::cout << "testGetMinSINRRatio: "<<ratio << std::endl;
}

void PhyModeMapperTest::testGetMinSINR()
{
	//const PhyMode& phyMode = testPhyModeMapper->getHighestPhyMode();
	//wns::SmartPtr<const wns::service::phy::phymode::PhyModeInterface> phyMode =
	wns::service::phy::phymode::PhyModeInterfacePtr phyModePtr =
		testPhyModeMapper->getHighestPhyMode();
	CPPUNIT_ASSERT(phyModePtr->isValid());
	double minSINR = testPhyModeMapper->getMinSINR(*phyModePtr);
	CPPUNIT_ASSERT(minSINR > 0.0);
}

void PhyModeMapperTest::testGetSINRRange()
{
	std::vector< wns::SmartPtr<PhyMode> > phyModeVector =
		testPhyModeMapper->getListofPhyModes();
	unsigned int listSize = phyModeVector.size();
	CPPUNIT_ASSERT(listSize > 0);
	CPPUNIT_ASSERT_EQUAL(4u, listSize); // according to Python spec above
	unsigned int phyModeCount = testPhyModeMapper->getPhyModeCount();
	CPPUNIT_ASSERT_EQUAL(listSize, phyModeCount);
	unsigned int phymodeIndex = listSize/2; // middle
	const PhyMode phyMode = *phyModeVector[phymodeIndex];
	CPPUNIT_ASSERT(phyMode.isValid());
	double min = testPhyModeMapper->getSINRRange(phyMode).min();
	double max = testPhyModeMapper->getSINRRange(phyMode).max();
	// if (useCout) std::cout << "testGetSINRRange: " <<min<<"..."<<max << std::endl;
	CPPUNIT_ASSERT(max > min);
	double test_sinr = (max+min)/2.0; // within the interval
	//wns::SmartPtr<const wns::service::phy::phymode::PhyModeInterface> phyMode2i =
	wns::service::phy::phymode::PhyModeInterfacePtr phyModePtr2i =
		testPhyModeMapper->getBestPhyMode(test_sinr);
	wns::SmartPtr<const PhyMode> phyModePtr2 =
		wns::staticCast<const rise::plmapping::PhyMode>(phyModePtr2i);
	CPPUNIT_ASSERT(*phyModePtr2 == phyMode);
}

void PhyModeMapperTest::testMinimumSINR()
{
	double minimumSINR = testPhyModeMapper->getMinimumSINR(); // -1.0
	WNS_ASSERT_MAX_REL_ERROR( -1.0, minimumSINR, 1e-6 );
	CPPUNIT_ASSERT(testPhyModeMapper->sinrIsAboveLimit(+10.0) == true);
	CPPUNIT_ASSERT(testPhyModeMapper->sinrIsAboveLimit(-10.0) == false);
	wns::Ratio snr = wns::Ratio::from_dB(10.0);
	CPPUNIT_ASSERT(testPhyModeMapper->sinrIsAboveLimit(snr) == true);
	snr.set_dB(-10.0);
	CPPUNIT_ASSERT(testPhyModeMapper->sinrIsAboveLimit(snr) == false);
}

void PhyModeMapperTest::testCalculateSINRRanges()
{
	if (useCout) std::cout<<std::endl;
	unsigned int bl  = 8; // smallest
	for(int b=1; b<=10; ++b) { // iterate through several payload lengths
		//bl = bl<<1; // *2 <<=
		bl <<= 1;
		if (useCout) std::cout<<"calculateSINRRanges(bl="<<bl<<"):"<<std::endl;
		double targetPER = 1.0;
		for(int i=1; i<=4; ++i) {
			targetPER /= 10.0; // 10^-i
			testPhyModeMapper->calculateSINRRanges(targetPER, bl /* smaller number (payload) */);
			double minimumSINR = testPhyModeMapper->getMinimumSINR();
			CPPUNIT_ASSERT(minimumSINR != -1.0);
			// now the tables have changed. Test new:
			testGetSINRRange();
			testGetBestPhyMode();
			if (useCout) {
				std::cout<<"  calculateSINRRanges(targetPER="<<targetPER<<", bl="<<bl<<"):"<<std::endl;
				std::cout<<"    minimumSINR = "<<minimumSINR<<std::endl;
				unsigned int phyModeCount = testPhyModeMapper->getPhyModeCount();
				for(unsigned int phymodeIndex=0; phymodeIndex<phyModeCount; phymodeIndex++) {
					//wns::SmartPtr<const wns::service::phy::phymode::PhyModeInterface> phyMode =
					wns::service::phy::phymode::PhyModeInterfacePtr phyModePtr =
						testPhyModeMapper->getPhyModeForIndex(phymodeIndex);
					double min = testPhyModeMapper->getSINRRange(*phyModePtr).min();
					double max = testPhyModeMapper->getSINRRange(*phyModePtr).max();
					std::cout<<"    SINRRange("<<*phyModePtr<<") = "<<min<<"..."<<max<<std::endl;
				}
			}
		}
	}
}

void PhyModeMapperTest::testCalculateLTESINRRanges()
{
	delete testPhyModeMapper;
	wns::pyconfig::Parser pyconfig;
	pyconfig.loadString(
		"from rise.PhyMode import PhyMode,PhyModeMapper\n"
		"from rise.SNR2MI import default,Table,Formula\n"
		"from rise.CoderSpecification import defaultCoderMapping,TableCoder,FormulaCoder,CoderMapping\n"
		"from openwns.interval import Interval\n\n"
		//"snr2miMapper = rise.SNR2MI.default\n"
		"snr2miMapper = Table()\n" // rise.SNR2MI.Table()\n"
		//"snr2miMapper = rise.SNR2MI.Formula()\n"
		//"mi2perMapper = rise.CoderSpecification.defaultCoderMapping\n"
		"mi2perMapper = CoderMapping(mapping = \"Table\")\n" // rise.CoderSpecification.CoderMapping(mapping = \"Table\")\n"
		//"mi2perMapper = rise.CoderSpecification.CoderMapping(mapping = \"Formula\")\n"
		"class LTEMapper(PhyModeMapper):\n"
		"    def __init__(self):\n"
		"        symbolDuration = 1e-6\n"
		"        subCarriersPerSubChannel = 12\n"
		"        # configure base class rise.PhyMode.PhyModeMapper():\n"
		"        super(LTEMapper, self).__init__(symbolDuration, subCarriersPerSubChannel, snr2miMapper=snr2miMapper, mi2perMapper=mi2perMapper)\n"
		"        self.setMinimumSINR(-1.0);\n"
		"        self.addPhyMode(Interval(-200.0,   2.8, \"(]\"), PhyMode(\"QPSK-Turbo_UMTS-1/3\"))\n"
		"        self.addPhyMode(Interval(   2.8,   4.9, \"(]\"), PhyMode(\"QPSK-Turbo_UMTS-1/2\"))\n"
		"        self.addPhyMode(Interval(   4.9,   7.6, \"(]\"), PhyMode(\"QPSK-Turbo_UMTS-2/3\"))\n"
		"        self.addPhyMode(Interval(   7.6,  10.1, \"(]\"), PhyMode(\"QAM16-Turbo_UMTS-1/2\"))\n"
		"        self.addPhyMode(Interval(  10.1,  13.0, \"(]\"), PhyMode(\"QAM16-Turbo_UMTS-2/3\"))\n"
		"        self.addPhyMode(Interval(  13.0,  14.9, \"(]\"), PhyMode(\"QAM16-Turbo_UMTS-5/6\"))\n"
		"        self.addPhyMode(Interval(  14.9,  18.2, \"(]\"), PhyMode(\"QAM64-Turbo_UMTS-2/3\"))\n"
		"        self.addPhyMode(Interval(  18.2, 200.0, \"(]\"), PhyMode(\"QAM64-Turbo_UMTS-5/6\"))\n"
		"phyModeMapper = LTEMapper()\n"
		);

	testPhyModeMapper = new PhyModeMapper(pyconfig.getView("phyModeMapper"));
	if (useCout) std::cout<<std::endl;
	unsigned int bl  = 8; // smallest
	for(int b=1; b<=10; ++b) { // iterate through several payload lengths
		bl <<= 1; // *=2
		//if (useCout) std::cout<<"calculateSINRRanges(bl="<<bl<<"):"<<std::endl;
		double targetPER = 1.0;
		for(int i=1; i<=4; ++i) {
			targetPER /= 10.0; // 10^-i
			testPhyModeMapper->calculateSINRRanges(targetPER, bl /* smaller number (payload) */);
			double minimumSINR = testPhyModeMapper->getMinimumSINR();
			CPPUNIT_ASSERT(minimumSINR != -1.0);
			// now the tables have changed. Test new:
			//testGetSINRRange();
			//testGetBestPhyMode();
			if (useCout) {
				std::cout<<"#calculateLTESINRRanges(targetPER="<<targetPER<<", bl="<<bl<<"):"<<std::endl;
				std::cout<<"\tself.setMinimumSINR("<<minimumSINR<<")"<<std::endl;
				unsigned int phyModeCount = testPhyModeMapper->getPhyModeCount();
				for(unsigned int phymodeIndex=0; phymodeIndex<phyModeCount; phymodeIndex++) {
					//wns::SmartPtr<const wns::service::phy::phymode::PhyModeInterface> phyMode =
					wns::service::phy::phymode::PhyModeInterfacePtr phyModePtr =
						testPhyModeMapper->getPhyModeForIndex(phymodeIndex);
					double min = testPhyModeMapper->getSINRRange(*phyModePtr).min();
					double max = testPhyModeMapper->getSINRRange(*phyModePtr).max();
					//std::cout<<"    SINRRange("<<*phyModePtr<<") = "<<min<<"..."<<max<<std::endl;
					std::cout<<"\tself.addPhyMode(Interval("<<min<<","<<max<<",\"(]\"),PhyMode(\""<<*phyModePtr<<"\"))"<<std::endl;
				}
				// format for Matlab: modeSwitchSNRs = [  0.9    2.1    3.8    7.7    9.8   12.6   15.0   18.2  23.0 ];
				std::cout<<"modeSwitchSNRs = "<<testPhyModeMapper->printSwitchingPoints()<<";"
					 <<" % targetPER="<<targetPER<<", bl="<<bl<<std::endl;
			}
		}
	}
}



