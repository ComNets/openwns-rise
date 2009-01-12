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
#include <WNS/isClass.hpp>

#include <cppunit/extensions/HelperMacros.h>
#include <stdexcept>
#include <cmath>
#include <iostream>
#include <iomanip>
#include <fstream>

#include <RISE/plmapping/PhyMode.hpp>
#include <RISE/plmapping/CodeRates.hpp>
#include <RISE/plmapping/MI2PER.hpp>
#include <RISE/plmapping/SNR2MI.hpp>

namespace rise { namespace plmapping { namespace tests {

	/**
	 * @brief Test of the table of all coders with full MI2PER info
	 * @author Rainer Schoenen (rs@comnets.rwth-aachen.de)
	 */
	class CoderFullTests :
		public wns::TestFixture
	{
		CPPUNIT_TEST_SUITE( CoderFullTests );
		CPPUNIT_TEST( testConstructorDestructor );
		CPPUNIT_TEST( testOneCoderFullSpecification );
		CPPUNIT_TEST( testCoderMapping );
		CPPUNIT_TEST( testCoderFullMapping );
		CPPUNIT_TEST( testCoderLimits );
		CPPUNIT_TEST( testPhyModeLimits );
		//CPPUNIT_TEST(  );
		//CPPUNIT_TEST(  );
		CPPUNIT_TEST_SUITE_END();
	public:
		CoderFullTests();
		virtual ~CoderFullTests();
		void prepare();
		void cleanup();
		void testConstructorDestructor();
		void testOneCoderFullSpecification();
		void testCoderMapping();
		void testCoderFullMapping();
		void testCoderLimits();
		void testPhyModeLimits();
		bool useCout; // verbose output
		bool writeTables; // write tables of the coding performance
	private:
		//CoderMapping* coderMapper; // Singleton
		CoderFullMapping* mi2perMapper; // Singleton
		SNR2MI* SNR2MImapper;
		void writeFile(Coding coding); // subfunction
	};

	CPPUNIT_TEST_SUITE_REGISTRATION( CoderFullTests );

	CoderFullTests::CoderFullTests()
		: wns::TestFixture(),
		  useCout(false), // switch true|false here to see an output to stdout.
		  writeTables(false),
		  mi2perMapper(NULL),
		  SNR2MImapper(NULL)
	{}

	CoderFullTests::~CoderFullTests()
	{}

	void CoderFullTests::prepare()
	{
		/** @note by importing the default coderMapping config, this test
		 * implicitly assumes that the coderMapping has the TableCoder type */
		wns::pyconfig::Parser pyconfig;
		pyconfig.loadString(
			"import rise.CoderSpecification\n"
			"coderMapping = rise.CoderSpecification.default\n"
			//"\n"
			);
		wns::pyconfig::View coderMappingConfig = pyconfig.getView("coderMapping");
		mi2perMapper = dynamic_cast<CoderFullMapping*>(wns::service::phy::phymode::CoderFullMappingInterface::getCoderFullMapping(coderMappingConfig));
		CPPUNIT_ASSERT(mi2perMapper != NULL);

		wns::pyconfig::View pyco = wns::pyconfig::Parser::fromString("nameInSNR2MIFactory = 'rise.SNR2MI.Table'");
		SNR2MImapper = dynamic_cast<SNR2MI*>( wns::service::phy::phymode::SNR2MIInterface::getSNR2MImapper(pyco));
		CPPUNIT_ASSERT(SNR2MImapper != NULL);
	}

	void CoderFullTests::cleanup()
	{
		SNR2MImapper = NULL;
	}

	void CoderFullTests::testConstructorDestructor()
	{
		//CPPUNIT_ASSERT(coderMapper != NULL); // global Singleton
		//CPPUNIT_ASSERT(coderMapper->getMaxCodeIndex() > 0u);
		CPPUNIT_ASSERT(mi2perMapper != NULL);
		CPPUNIT_ASSERT(mi2perMapper->getMaxCodeIndex() > 0u);
		//CPPUNIT_ASSERT();
	}

	void CoderFullTests::testOneCoderFullSpecification()
	{
		// configure() must be done with a complicated Python class...
		// this is tedious to test here. So it is omitted.
		// instead we test the Singleton object here,
		// which is already configured!
		// CoderMapping* coderMapper; exists as Singleton pointer
		CoderFullMapping* coderFullMapper = mi2perMapper;
		CPPUNIT_ASSERT(coderFullMapper->getMaxCodeIndex() > 0);
		CPPUNIT_ASSERT(coderFullMapper->getMaxCodeIndex() > 10); // there should be more than 10 different coders
		unsigned int maxCodeIndex = coderFullMapper->getMaxCodeIndex();
		unsigned int myCodeIndex = maxCodeIndex / 2; // choose one out of the middle
		const TableCoder* coderFullSpecification =
			dynamic_cast<const TableCoder*>(&coderFullMapper->getCoderFullSpecification(myCodeIndex));
		// ^ copied, not const
		CPPUNIT_ASSERT(coderFullSpecification != NULL);
		CPPUNIT_ASSERT(coderFullSpecification->suitableCWL(300) > 0);
		unsigned int blockLength = coderFullSpecification->suitableCWL(300);
		unsigned int pduSize = (unsigned int)(blockLength * coderFullSpecification->getRate() + 0.5); // round
		for (double mi = 0.0; mi <= 1.0; mi += 0.1) {
			double PER = coderFullSpecification->mapMI2PER(mi,pduSize);
			//if (useCout) std::cout << "CoderFullTests::testOneCoderFullSpecification(): mapMI2PER("<<mi<<","<<pduSize<<")="<<PER<< std::endl;
			CPPUNIT_ASSERT(PER <= 1.0);
			CPPUNIT_ASSERT(PER >= 0.0);
			// here more can be tested...
		}
		//CPPUNIT_ASSERT_EQUAL();
	}

	void CoderFullTests::testCoderMapping()
	{
		// CoderMapping* coderMapper; exists as Singleton pointer
		CoderMapping* coderMapper = mi2perMapper; // base class functionality
		CPPUNIT_ASSERT(coderMapper->getMaxCodeIndex() > 0);
		CPPUNIT_ASSERT(coderMapper->getMaxCodeIndex() > 10); // there should be more than 10 different coders
		unsigned int maxCodeIndex = coderMapper->getMaxCodeIndex();
		for(Coding coding=0u; coding<=maxCodeIndex; coding++) {
			const CoderSpecification* coderSpecification = &coderMapper->getCoderSpecification(coding);
			CPPUNIT_ASSERT(&coderSpecification != NULL);
			CPPUNIT_ASSERT_EQUAL(coding,coderSpecification->getCoding()); // index must match
			CPPUNIT_ASSERT_EQUAL(coderSpecification->getRate(),coderMapper->getRate(coding));
			double rate = coderMapper->getRate(coding);
			CPPUNIT_ASSERT(rate <= 1.0);
			CPPUNIT_ASSERT(rate > 0.0);
			CPPUNIT_ASSERT_EQUAL(coderSpecification->getName(),coderMapper->getString(coding));
			std::string name     = std::string(coderMapper->getString(coding)); // copy
			const std::string &nameRef = coderMapper->getString(coding); // pointer copy
			CPPUNIT_ASSERT_EQUAL(name,nameRef);
			CPPUNIT_ASSERT_EQUAL(name,coderMapper->getString(coding));
			CPPUNIT_ASSERT_EQUAL(coding,coderMapper->fromString(name));
		}
	}

	void CoderFullTests::testCoderFullMapping()
	{
		CoderFullMapping* coderFullMapper = mi2perMapper; // exists as Singleton pointer
		CPPUNIT_ASSERT(coderFullMapper->getMaxCodeIndex() > 0);
		CPPUNIT_ASSERT(coderFullMapper->getMaxCodeIndex() > 10); // there should be more than 10 different coders
		unsigned int maxCodeIndex = coderFullMapper->getMaxCodeIndex();
		for(Coding coding=0u; coding<=maxCodeIndex; coding++) {
			const TableCoder& coderFullSpecification = 
				*dynamic_cast<const TableCoder*>(&coderFullMapper->getCoderFullSpecification(coding));
			CPPUNIT_ASSERT(&coderFullSpecification != NULL);
			const CodeWordLengths& codeWordLengths = coderFullSpecification.getCWLvector();
			for(unsigned int CWLindex=0; CWLindex<codeWordLengths.size(); ++CWLindex) {
				unsigned int cwl = codeWordLengths[CWLindex];
				// blockLength is smaller (netto, usable bits only)
				unsigned int bl = (int) (coderFullSpecification.getRate() * (double)cwl  + 0.5);
				//if (useCout) std::cout << "testCoderFullMapping(): "<<coderFullSpecification.getName()<<": bl="<<bl<<std::endl;
				double lastPER = 1.0;
				for (double mib = 0.0; mib <= 1.0; mib += 0.01) {
					double per1 = coderFullMapper->mapMI2PER(mib, bl, coding);
					double per2 = coderFullSpecification.mapMI2PER(mib,bl);
					//if (useCout) std::cout << "MIB="<<mib<<" => PER1="<<per1<<", PER2="<<per2<<std::endl;
					WNS_ASSERT_MAX_REL_ERROR( per1, per2, 1e-6 );
					CPPUNIT_ASSERT( per1<=lastPER ); // test of monotonousity
					lastPER=per1;
					// inverse function:
					double mib2 = coderFullSpecification.PER2MIB(per1, bl);
					//if (useCout) std::cout << "          PER="<<per1<<" => MIB="<<mib2<<std::endl;
					if ((per1<0.5) && (per1>1e-5)) { // the relevant section
						WNS_ASSERT_MAX_REL_ERROR( mib, mib2, 0.01 ); // allow 1%
					}
				} // for all mi values
			} // for all blocklengths
			if (writeTables) writeFile(coding);
		} // for all codings
	}

	void CoderFullTests::writeFile(Coding coding)
	{
		if (coding==UNDEFINED_CODING) return;
		CoderFullMapping* coderFullMapper = mi2perMapper; // exists as Singleton pointer
		const TableCoder& coderFullSpecification =
			*dynamic_cast<const TableCoder*>(&coderFullMapper->getCoderFullSpecification(coding));
		CPPUNIT_ASSERT(&coderFullSpecification != NULL);
		const CodeWordLengths& codeWordLengths = coderFullSpecification.getCWLvector();
		std::ofstream outputStream;
		std::string coderName = coderFullSpecification.getName();
		std::string::size_type loc = coderName.find( "/", 0 );
		if (loc != std::string::npos) coderName.replace( loc, 1, "_" );
		while ((loc = coderName.find( "-", 0 )) != std::string::npos) {
			coderName.replace( loc, 1, "_" );
		}
		std::string fileName = "/tmp/mi2per_"+coderName+".m";
		outputStream.open(fileName.c_str(), std::ios::out);
		//CPPUNIT_ASSERT(outputStream);
		assure(outputStream, "cannot write file "<<fileName);
		outputStream << "%%% "<<coderName<<" %%%"<< std::endl;
		outputStream << "% cwl=\t";
		for(unsigned int CWLindex=0; CWLindex<codeWordLengths.size(); ++CWLindex) {
			unsigned int cwl = codeWordLengths[CWLindex];
			outputStream << cwl << "\t";
		}
		outputStream << std::endl;
		outputStream << "% bl=\t";
		for(unsigned int CWLindex=0; CWLindex<codeWordLengths.size(); ++CWLindex) {
			unsigned int cwl = codeWordLengths[CWLindex];
			unsigned int bl = (int) (coderFullSpecification.getRate() * (double)cwl  + 0.5);
			outputStream << bl << "\t";
		}
		outputStream << std::endl;
		outputStream << "% MI";
		outputStream << std::setprecision( 6 ) << std::endl;
		//outputStream << "MI2PER(:,:,"<<coding<<")=["<< std::endl;
		outputStream << "MI2PER_"<<coderName<<"(:,:)=["<< std::endl;
		for (double mib = 0.0; mib <= 1.0001; mib += 0.001) {
			outputStream << mib << "\t";
			for(unsigned int CWLindex=0; CWLindex<codeWordLengths.size(); ++CWLindex) {
				unsigned int cwl = codeWordLengths[CWLindex];
				// blockLength is smaller (netto, usable bits only)
				unsigned int bl = (int) (coderFullSpecification.getRate() * (double)cwl  + 0.5);
				double per = coderFullSpecification.mapMI2PER(mib,bl);
				outputStream << per << "\t";
			} // for all blocklengths
			outputStream << std::endl;
		} // for all mi values
		outputStream << "];"<< std::endl;
	    outputStream.close();
	}

	void CoderFullTests::testCoderLimits()
	{
		CoderFullMapping* coderFullMapper = mi2perMapper; // exists as Singleton pointer
		CPPUNIT_ASSERT(coderFullMapper->getMaxCodeIndex() > 0);
		CPPUNIT_ASSERT(coderFullMapper->getMaxCodeIndex() > 10); // there should be more than 10 different coders
		unsigned int maxCodeIndex = coderFullMapper->getMaxCodeIndex();
		for(Coding coding=0u; coding<=maxCodeIndex; coding++) {
			const TableCoder& coderFullSpecification = 
				*dynamic_cast<const TableCoder*>(&coderFullMapper->getCoderFullSpecification(coding));
			CPPUNIT_ASSERT(&coderFullSpecification != NULL);
			const CodeWordLengths& codeWordLengths = coderFullSpecification.getCWLvector();
			for(unsigned int CWLindex=0; CWLindex<codeWordLengths.size(); ++CWLindex) {
				unsigned int cwl = codeWordLengths[CWLindex];
				// blockLength is smaller (netto, usable bits only)
				unsigned int bl = (int) (coderFullSpecification.getRate() * (double)cwl  + 0.5);
				const double PERtarget = 1e-2;
				// inverse function:
				double mib = coderFullSpecification.PER2MIB(PERtarget, bl);
				//if (useCout) std::cout << "testCoderLimits("<<coderFullSpecification.getName()<<",bl="<<bl<<",PER="<<PERtarget<<"):\trequires MIB="<<mib<<std::endl;
				// any assert possible?
				CPPUNIT_ASSERT(mib < 1.0);
			} // for all blocklengths
		} // for all codings
	}

	void CoderFullTests::testPhyModeLimits()
	{
		double errorSum = 0.0;
		double errorSqSum = 0.0;
		unsigned int errorCount = 0;
		wns::pyconfig::View pyco = wns::pyconfig::Parser::fromString("nameInSNR2MIFactory = 'rise.SNR2MI.Formula'");
		SNR2MI* mySNR2MImapper = dynamic_cast<SNR2MI*>( wns::service::phy::phymode::SNR2MIInterface::getSNR2MImapper(pyco));
		CPPUNIT_ASSERT( wns::isClass<SNR2MIFormula>( *mySNR2MImapper ) );
		CoderFullMapping* coderFullMapper = mi2perMapper; // exists as Singleton pointer
		CPPUNIT_ASSERT(coderFullMapper->getMaxCodeIndex() > 0);
		CPPUNIT_ASSERT(coderFullMapper->getMaxCodeIndex() > 10); // there should be more than 10 different coders
		unsigned int maxCodeIndex = coderFullMapper->getMaxCodeIndex();
		for(Modulation modulation=1; modulation<=MAX_MODULATIONS; ++modulation) {
			for(Coding coding=0u; coding<=maxCodeIndex; coding++) {
				//PhyMode phyMode(modulation,coding);
				const TableCoder& coderFullSpecification =
					*dynamic_cast<const TableCoder*>(&coderFullMapper->getCoderFullSpecification(coding));
				CPPUNIT_ASSERT(&coderFullSpecification != NULL);
				if (coderFullSpecification.getRate() == 1.0) { // means NO coding
					CPPUNIT_ASSERT(coderFullSpecification.hasNoCoding() == true);
					if (!useCout) { // if verbose, print out all results, even when error bound is exceeded
						continue; // do not use this PhyMode for further tests
					}
				}
				const CodeWordLengths& codeWordLengths = coderFullSpecification.getCWLvector();
				for(unsigned int CWLindex=0; CWLindex<codeWordLengths.size(); ++CWLindex) {
					unsigned int cwl = codeWordLengths[CWLindex];
					// blockLength is smaller (netto, usable bits only)
					unsigned int bl = (int) (coderFullSpecification.getRate() * (double)cwl  + 0.5);
					const double PERtarget = 1e-2;
					// inverse function:
					double mib = coderFullSpecification.PER2MIB(PERtarget, bl); // inversion
					double sinr = mySNR2MImapper->MIB2SNR(mib,modulation); // inversion
					// forward translation:
					wns::Ratio snr = wns::Ratio::from_dB(sinr);
					double mib2 = mySNR2MImapper->convertSNR2MIB(snr,modulation);
					CPPUNIT_ASSERT_DOUBLES_EQUAL(mib,mib2,1e-6); // tight
					double per2 = coderFullSpecification.mapMI2PER(mib2, bl);
					double error = (per2-PERtarget)/PERtarget;
					if (coderFullSpecification.getRate() != 1.0) { // ignore "NO" coding, which has very bad error results (max 36%)
						errorSum   += error;
						errorSqSum += error*error;
						errorCount++;
					}
					//if (useCout) {
					if (useCout && (fabs(error)>0.01)) { // show only if more than 1% error
						//std::cout << "testPhyModeLimits("<<phyMode.getString()<<", bl="<<bl<<",PER="<<PERtarget<<"):"<<std::endl;
						std::cout << "testPhyModeLimits("<<rise::plmapping::Modulations::toString(modulation)<<"-"<<coderFullSpecification.getName()<<", cwl="<<cwl<<", bl="<<bl<<", PER="<<PERtarget<<"):"<<std::endl;
						std::cout << "\t\t\t\trequired MIB="<<mib<<", SINR="<<sinr<<std::endl;
						//std::cout << "\t\t\t\tMIB2="<<mib2<<"\t=> PER="<<per2<<"\terror="<<error*100.0<<"%"<<std::endl;
						std::cout << "\t\t\t\t\t=> PER="<<per2<<"\terror="<<error*100.0<<"%"<<std::endl;
					}
				} // for all blocklengths
			} // for all codings
		} // forall modulations
		double errorMean = errorSum/errorCount;
		double errorVar  = errorSqSum/errorCount - errorMean*errorMean;
		if (useCout) std::cout << "statistical error: mean="<<errorMean<<", var="<<errorVar<<", sd="<<sqrt(errorVar)<<std::endl;
		// statistical error: mean=2.12377e-06, var=5.84767e-12, sd=2.4182e-06
		CPPUNIT_ASSERT( sqrt(errorVar) < 1e-4 );
	}


} // tests
} // plmapping
} // rise


