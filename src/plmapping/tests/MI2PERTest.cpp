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

#include <WNS/pyconfig/helper/Functions.hpp>
#include <WNS/CppUnit.hpp>
#include <cppunit/extensions/HelperMacros.h>
#include <stdexcept>

#include <RISE/plmapping/PhyMode.hpp>
#include <RISE/plmapping/CodeRates.hpp>
#include <RISE/plmapping/MI2PER.hpp>

namespace rise { namespace plmapping { namespace tests {

	class MI2PERTest :
		public wns::TestFixture
	{
		CPPUNIT_TEST_SUITE( MI2PERTest );
		CPPUNIT_TEST( TestTableEntries );
		CPPUNIT_TEST( Fixed );
		CPPUNIT_TEST( TestMI2PER );
		CPPUNIT_TEST( TestPER2MIB );
		CPPUNIT_TEST_SUITE_END();
	public:
		MI2PERTest();
		virtual ~MI2PERTest();
		void prepare();
		void cleanup();
		void TestTableEntries();
		void Fixed();
		void TestMI2PER();
		void TestPER2MIB();

		rise::plmapping::CoderMapping* coderMapper; // Singleton
		rise::plmapping::CoderFullMapping* mi2perMapper; // Singleton

		wns::service::phy::phymode::MI2PERMappingInterface* testee1;
		wns::service::phy::phymode::MI2PERMappingInterface* testee2;


	};


	CPPUNIT_TEST_SUITE_REGISTRATION( MI2PERTest );

	MI2PERTest::MI2PERTest()
		: wns::TestFixture(),
		  coderMapper(NULL),
		  mi2perMapper(NULL),
		  testee1(NULL),
		  testee2(NULL)
	{}

	MI2PERTest::~MI2PERTest()
	{}

	void MI2PERTest::prepare()
	{
		wns::pyconfig::Parser pyconfig;
		pyconfig.loadString(
			"import rise.CoderSpecification\n"
			"coderMapping = rise.CoderSpecification.default\n"
			//"\n"
			);
		wns::pyconfig::View coderMappingConfig = pyconfig.getView("coderMapping");
		coderMapper  = dynamic_cast<CoderMapping*>(wns::service::phy::phymode::CoderMappingInterface::getCoderMapping(coderMappingConfig));
		mi2perMapper = dynamic_cast<CoderFullMapping*>(wns::service::phy::phymode::CoderFullMappingInterface::getCoderFullMapping(coderMappingConfig));
		CPPUNIT_ASSERT(coderMapper != NULL);
		CPPUNIT_ASSERT(mi2perMapper != NULL);

		wns::pyconfig::Parser M2Ppyconfig;
		M2Ppyconfig.loadString(
			"import rise.CoderSpecification\n"
			"mi2per = rise.CoderSpecification.default\n"
			"PER = 0.05\n"
			);
		testee1 = new WINNERM2P(M2Ppyconfig);
		testee2 = new FixedM2P(M2Ppyconfig);
	}

	void MI2PERTest::cleanup()
	{
		coderMapper = NULL;
		mi2perMapper = NULL;
		delete testee1;
		delete testee2;
	}


	void MI2PERTest::TestTableEntries()
	{
		//We have to take into account that here we are introducing the payload,
		//so the CWL used will be the payload multiplied by the rate of the CoderSpecification
		WNS_ASSERT_MAX_REL_ERROR( 0.9746288,
						  testee1->mapMI2PER(0.921, 400,coderMapper->fromString("No")),
							  1E-6 );
		// 300 * 3 = CWL 840 -> the CWL 600 will be used.
		WNS_ASSERT_MAX_REL_ERROR( 0.9932241,
							  testee1->mapMI2PER(0.35, 280,coderMapper->fromString("Turbo_UMTS-1/3")),
							  1E-6 );

		WNS_ASSERT_MAX_REL_ERROR( 0.6567467,
							  testee1->mapMI2PER(0.340, 100,coderMapper->fromString("RCPC_BENQ-1/3")),
					          1E-6 );
		// (450 * 3) / 2 = 675 -> the CWL 576 will be used
		WNS_ASSERT_MAX_REL_ERROR( 0.00502388,
							  testee1->mapMI2PER(0.799, 450,coderMapper->fromString("LDPC_SEUK-2/3")),
							  1E-6 );

	}

	void MI2PERTest::Fixed()
	{
		WNS_ASSERT_MAX_REL_ERROR( 0.05,
								  testee2->mapMI2PER(1.0,   // Mutual Information
													 42,    // any block length
													 1234), // the coding ID has
															// no effect
								  1E-6 );
		// another combination should have the same effect
		WNS_ASSERT_MAX_REL_ERROR( 0.05,
								  testee2->mapMI2PER(0.5,   // Mutual Information
													 43,    // any block length
													 123),  // the coding ID has
															// no effect
								  1E-6 );
	}

	void  MI2PERTest::TestMI2PER()
	{
		const Coding &coding = coderMapper->fromString("RCPC_BENQ-1/3");
		const TableCoder* coderFullSpec =
			dynamic_cast<const TableCoder*>(&(mi2perMapper->getCoderFullSpecification(coding)));
		CPPUNIT_ASSERT( coderFullSpec != NULL );
		const CodeWordLengths& codeWordLengths = coderFullSpec->getCWLvector();
		unsigned int bl = (int) (coderFullSpec->getRate() * (double) codeWordLengths[0] + 0.5); // smallest CWL
		//std::cout << "TestMI2PER(): "<<coderFullSpec.getName()<<": bl="<<bl<<std::endl;
		for (double mib=0.0; mib<=1.0; mib+=0.1) {
			// these two ways should be equivalent:
			double per1 = testee1->mapMI2PER(mib, bl, coding);
			double per2 = coderFullSpec->mapMI2PER(mib, bl); // faster access method
			WNS_ASSERT_MAX_REL_ERROR( per1, per2, 1e-6 );
		}
	}

	void  MI2PERTest::TestPER2MIB()
	{
		const Coding &coding = coderMapper->fromString("RCPC_BENQ-1/3");
		const TableCoder* coderFullSpec =
			dynamic_cast<const TableCoder*>(&(mi2perMapper->getCoderFullSpecification(coding)));
		CPPUNIT_ASSERT( coderFullSpec != NULL );
		const CodeWordLengths& codeWordLengths = coderFullSpec->getCWLvector();
		unsigned int bl = (int) (coderFullSpec->getRate() * (double) codeWordLengths[0] + 0.5); // smallest CWL
		//std::cout << "TestPER2MIB(): "<<coderFullSpec.getName()<<": bl="<<bl<<std::endl;
		for (double mib=0.0; mib<=1.0; mib+=0.05) {
			// these two ways should be equivalent:
			double per1 = testee1->mapMI2PER(mib, bl, coding);
			double per2 = coderFullSpec->mapMI2PER(mib, bl); // faster access method
			//std::cout << "MIB="<<mib<<" => PER="<<per1<<std::endl;
			WNS_ASSERT_MAX_REL_ERROR( per1, per2, 1e-6 );
			// inverse function:
			double mib2 = coderFullSpec->PER2MIB(per1, bl);
			//std::cout << "          PER="<<per1<<" => MIB="<<mib2<<std::endl;
			WNS_ASSERT_MAX_REL_ERROR( mib, mib2, 0.02 );
		}
	}


} // tests
} // plmapping
} // rise


