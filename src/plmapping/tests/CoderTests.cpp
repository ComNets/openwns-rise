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

	class CoderTests :
		public wns::TestFixture
	{
		CPPUNIT_TEST_SUITE( CoderTests );
		CPPUNIT_TEST( testConstructorDestructor );
		CPPUNIT_TEST( testCoderSpecification );
		CPPUNIT_TEST( testCoderMapping );
		//CPPUNIT_TEST(  );
		//CPPUNIT_TEST(  );
		CPPUNIT_TEST_SUITE_END();
	public:
		CoderTests();
		virtual ~CoderTests();
		void prepare();
		void cleanup();
		void testConstructorDestructor();
		void testCoderSpecification();
		void testCoderMapping();

		CoderMapping* coderMapper; // Singleton
	};


	CPPUNIT_TEST_SUITE_REGISTRATION( CoderTests );

	CoderTests::CoderTests()
		: wns::TestFixture(),
		  coderMapper(NULL)
	{}

	CoderTests::~CoderTests()
	{}

	void CoderTests::prepare()
	{
		wns::pyconfig::Parser pyconfig;
		pyconfig.loadString(
			"import rise.CoderSpecification\n"
			"coderMapping = rise.CoderSpecification.default\n"
			//"\n"
			);
		wns::pyconfig::View coderMappingConfig = pyconfig.getView("coderMapping");
		coderMapper = dynamic_cast<CoderMapping*>(wns::service::phy::phymode::CoderMappingInterface::getCoderMapping(coderMappingConfig));
		CPPUNIT_ASSERT(coderMapper != NULL);
	}

	void CoderTests::cleanup()
	{

	}

	void CoderTests::testConstructorDestructor()
	{
		CPPUNIT_ASSERT(coderMapper != NULL); // global Singleton
		CPPUNIT_ASSERT(coderMapper->getMaxCodeIndex() > 0u);
		//CPPUNIT_ASSERT(mi2perMapper != NULL);
		//CPPUNIT_ASSERT_EQUAL(0u, mi2perMapper->getMaxCodeIndex());
		//CPPUNIT_ASSERT();
	}

	void CoderTests::testCoderSpecification()
	{
		Coding coding = 5u; /* arbitrary */
		double rate = 1.0/3.0;
		std::string name = std::string("TestCoder");
		CoderSpecification* coderSpecification = new CoderSpecification(coding, rate, name);
		CPPUNIT_ASSERT(coderSpecification != NULL);
		CPPUNIT_ASSERT_EQUAL(coding,coderSpecification->getCoding());
		CPPUNIT_ASSERT_EQUAL(rate,coderSpecification->getRate());
		CPPUNIT_ASSERT_EQUAL(name,coderSpecification->getName());
		delete coderSpecification;
	}

	void CoderTests::testCoderMapping()
	{
		// CoderMapping* coderMapper; exists as Singleton pointer
		CPPUNIT_ASSERT(coderMapper->getMaxCodeIndex() > 0);
		unsigned int maxCodeIndex = coderMapper->getMaxCodeIndex();
		for(Coding coding=0u; coding<=maxCodeIndex; coding++) {
			const CoderSpecification coderSpecification = coderMapper->getCoderSpecification(coding);
			CPPUNIT_ASSERT(&coderSpecification != NULL);
			CPPUNIT_ASSERT_EQUAL(coding,coderSpecification.getCoding()); // index must match
			CPPUNIT_ASSERT_EQUAL(coderSpecification.getRate(),coderMapper->getRate(coding));
			double rate = coderMapper->getRate(coding);
			CPPUNIT_ASSERT(rate <= 1.0);
			CPPUNIT_ASSERT(rate > 0.0);
			CPPUNIT_ASSERT_EQUAL(coderSpecification.getName(),coderMapper->getString(coding));
			std::string name     = std::string(coderMapper->getString(coding)); // copy
			const std::string &nameRef = coderMapper->getString(coding); // pointer copy
			CPPUNIT_ASSERT_EQUAL(name,nameRef);
			CPPUNIT_ASSERT_EQUAL(name,coderMapper->getString(coding));
			CPPUNIT_ASSERT_EQUAL(coding,coderMapper->fromString(name));
		}
	}

} // tests
} // plmapping
} // rise


