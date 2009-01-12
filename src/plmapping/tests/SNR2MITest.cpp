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

#include <RISE/plmapping/SNR2MI.hpp>
#include <RISE/plmapping/PhyMode.hpp>
#include <WNS/PowerRatio.hpp>

#include <WNS/pyconfig/Parser.hpp>
#include <WNS/pyconfig/helper/Functions.hpp>
#include <WNS/node/tests/Stub.hpp>
#include <WNS/TestFixture.hpp>
#include <cppunit/extensions/HelperMacros.h>

#include <iostream>
#include <iomanip>
#include <fstream>

// call me with   ./wns-core -T "rise::plmapping::tests::SNR2MITest"

namespace rise { namespace plmapping { namespace tests {

	/**
	 * @brief Test of the SNR2MI translation functions
	 * @author Rainer Schoenen (rs@comnets.rwth-aachen.de)
	 */
	class SNR2MITest
		: public wns::TestFixture
	{
 		CPPUNIT_TEST_SUITE( SNR2MITest );
		CPPUNIT_TEST( testConstructorDestructor );
		CPPUNIT_TEST( testTables );
		CPPUNIT_TEST( testInterpolation );
		CPPUNIT_TEST( testFormulaSNR2MI );
		// TODO: CPPUNIT_TEST( testFormulaVsTable );
		CPPUNIT_TEST( testMIB2SNR );
		CPPUNIT_TEST( testBERfunctions );
		CPPUNIT_TEST_SUITE_END();
	public:
		SNR2MITest();
		~SNR2MITest();
                void prepare();
                void cleanup();
		void testConstructorDestructor();
		void testTables();
		void testInterpolation();
		void testFormulaSNR2MI();
		void testMIB2SNR();
		void testBERfunctions();
		bool useCout; // verbose output
	private:
		SNR2MI* testSNR2MIformula;
		SNR2MI* testSNR2MItable;
	};

} // tests
} // plmapping
} // rise

using namespace rise;
using namespace rise::plmapping::tests;
using namespace rise::plmapping::Modulations;


/********************************* SNR2MITest *****************************/

CPPUNIT_TEST_SUITE_REGISTRATION( SNR2MITest );

SNR2MITest::SNR2MITest() :
        wns::TestFixture(),
	useCout(false),
	testSNR2MIformula(NULL),
	testSNR2MItable(NULL)
{
}

SNR2MITest::~SNR2MITest()
{
}

void SNR2MITest::prepare()
{
	wns::pyconfig::Parser pyco;
	pyco.loadString("import rise.SNR2MI\n"
			"table = rise.SNR2MI.Table()\n"
			"formula = rise.SNR2MI.Formula()\n");

	testSNR2MIformula = dynamic_cast<SNR2MI*>(wns::service::phy::phymode::SNR2MIInterface::getSNR2MImapper(pyco.get("formula")));
	testSNR2MItable   = dynamic_cast<SNR2MI*>(wns::service::phy::phymode::SNR2MIInterface::getSNR2MImapper(pyco.get("table")));
} // prepare

void SNR2MITest::cleanup()
{
	testSNR2MIformula = NULL;
	testSNR2MItable = NULL;
}

void SNR2MITest::testConstructorDestructor()
{
	CPPUNIT_ASSERT(testSNR2MIformula != NULL);
	CPPUNIT_ASSERT(testSNR2MItable != NULL);
}

/* this test checks for values monotonous in SINR and lower PHY-mode */
void SNR2MITest::testTables()
{
    bool writeOutputFile = false;
    std::ofstream outputStream;
    if (writeOutputFile) {
	    outputStream.open("/tmp/snr2mi.tab", std::ios::out);
	    CPPUNIT_ASSERT(outputStream);
	    outputStream << "# dB\t";
	    for (Modulation modulation=1; modulation<7; ++modulation) {
		    outputStream << Modulations::toString(modulation) << "\t";
	    }
	    outputStream << std::setprecision( 6 ) << std::endl;
    }
    double lastmi[8] = { 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0 }; // one more
    for ( int snrdb=-30; snrdb<31; ++snrdb) {
	wns::Ratio snr = wns::Ratio::from_dB(snrdb);
	double mi[] = {
		0.0,
		testSNR2MItable->convertSNR2MIB(snr,BPSK()),
		testSNR2MItable->convertSNR2MIB(snr,QPSK()),
		testSNR2MItable->convertSNR2MIB(snr,QAM8()),
		testSNR2MItable->convertSNR2MIB(snr,QAM16()),
		testSNR2MItable->convertSNR2MIB(snr,QAM32()),
		testSNR2MItable->convertSNR2MIB(snr,QAM64()),
		0.0 // for comparison of PHY modes
		 };
	if (writeOutputFile)
		outputStream << snr.get_dB() << "\t";
	for (Modulation modulation=1; modulation<7; ++modulation) {
		if (writeOutputFile)
			outputStream << mi[modulation] << "\t";
		// monotonous:
		CPPUNIT_ASSERT(mi[modulation] >= lastmi[modulation]);
		// lower PHY mode always more robust:
		CPPUNIT_ASSERT(mi[modulation] >= mi[modulation+1]);
		lastmi[modulation] = mi[modulation];
	}
	if (writeOutputFile)
		outputStream << std::endl;
    }
    if (writeOutputFile)
	    outputStream.close();
}

/* this test checks for monotonous values when using interpolation */
void SNR2MITest::testInterpolation()
{
    //std::ofstream outputStream;
    //outputStream.open("/tmp/snr2mi_interpolated.tab", std::ios::out);
    //CPPUNIT_ASSERT(outputStream);
    //outputStream << "dB\t";
    //for (Modulation modulation=1; modulation<7; ++modulation) {
    //	outputStream << Modulations::tostd::string(modulation) << "\t";
    //}
    //outputStream << std::endl;
    double lastmi[8] = { 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0 }; // one more
    for ( double snrdb=-20.0; snrdb<=30.0; snrdb+=0.20) {
	//wns::Ratio::from_factor(1000)
	wns::Ratio snr = wns::Ratio::from_dB(snrdb);
	double mi[] = {
		0.0,
		testSNR2MItable->convertSNR2MIB(snr,BPSK()),
		testSNR2MItable->convertSNR2MIB(snr,QPSK()),
		testSNR2MItable->convertSNR2MIB(snr,QAM8()),
		testSNR2MItable->convertSNR2MIB(snr,QAM16()),
		testSNR2MItable->convertSNR2MIB(snr,QAM32()),
		testSNR2MItable->convertSNR2MIB(snr,QAM64()),
		0.0 // for comparison of PHY modes
		 };
	//outputStream << snr.get_dB() << "\t";
	for (Modulation modulation=1; modulation<7; ++modulation) {
		//outputStream << mi[modulation] << "\t";
		// monotonous:
		CPPUNIT_ASSERT(mi[modulation] >= lastmi[modulation]);
		// lower PHY mode always more robust:
		CPPUNIT_ASSERT(mi[modulation] >= mi[modulation+1]);
		lastmi[modulation] = mi[modulation];
	}
	//outputStream << std::endl;
    }
    //outputStream.close();
}

/* this test checks for values monotonous in SINR and lower PHY-mode */
void SNR2MITest::testFormulaSNR2MI()
{
    //std::ofstream outputStream;
    //outputStream.open("/tmp/snr2mi.tab", std::ios::out);
    //CPPUNIT_ASSERT(outputStream);
    //outputStream << "dB\t";
    //for (Modulation modulation=1; modulation<9; ++modulation) {
    //	outputStream << Modulations::tostd::string(modulation) << "\t";
    //}
    //outputStream << std::endl;
    double lastmi[10] = { 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0 }; // one more
    const double errorTol = 0.005; // 0.5%
    for ( double snrdb=-40.0; snrdb<41.0; snrdb+=0.5) {
    //for ( wns::Ratio snr=-30.0; snr<31.0; snr+=0.5) {
	//wns::Ratio::from_factor(1000)
	wns::Ratio snr = wns::Ratio::from_dB(snrdb);
	double mi[] = {
		0.0,
		testSNR2MIformula->convertSNR2MIB(snr,BPSK()),
		testSNR2MIformula->convertSNR2MIB(snr,QPSK()),
		testSNR2MIformula->convertSNR2MIB(snr,QAM8()),
		testSNR2MIformula->convertSNR2MIB(snr,QAM16()),
		testSNR2MIformula->convertSNR2MIB(snr,QAM32()),
		testSNR2MIformula->convertSNR2MIB(snr,QAM64()),
		testSNR2MIformula->convertSNR2MIB(snr,QAM128()),
		testSNR2MIformula->convertSNR2MIB(snr,QAM256()),
		0.0 // for comparison of PHY modes
		 };
	//outputStream << snr.get_dB() << "\t";
	for (Modulation modulation=1; modulation<9; ++modulation) {
		//outputStream << mi[modulation] << "\t";
		// monotonous:
		CPPUNIT_ASSERT(mi[modulation] >= lastmi[modulation]);
		// lower PHY mode always more robust:
		CPPUNIT_ASSERT(mi[modulation] >= mi[modulation+1] - errorTol);
		//CPPUNIT_ASSERT_DOUBLES_EQUAL(soll,ist,errorTol)
		lastmi[modulation] = mi[modulation];
	}
	//outputStream << std::endl;
    }
    //outputStream.close();
}

void SNR2MITest::testMIB2SNR()
{
    // results using formula
    for (Modulation modulation=1; modulation<=MAX_MODULATIONS; ++modulation) {
	    for ( double snrdb=-40.0; snrdb<=40.0; snrdb+=0.5) {
		    wns::Ratio snr = wns::Ratio::from_dB(snrdb);
		    double mib = testSNR2MIformula->convertSNR2MIB(snr,modulation);
		    // inverse function:
		    double snr2 = testSNR2MIformula->MIB2SNR(mib,modulation); // inversion
		    //if (useCout) std::cout << "testMIB2SNR(mod="<<modulation<<"): SNR="<<snr<<" => MIB="<<mib<<" => SNR="<<snr2<<std::endl;
		    CPPUNIT_ASSERT_DOUBLES_EQUAL(snrdb,snr2,1e-3); // tight
	    } // forall sinrs
    } // forall modulations
    // results using table are much less precise; especially inversion
    for (Modulation modulation=1; modulation<=MAX_MODULATIONS_FOR_TABLE; ++modulation) {
	    for ( double snrdb=-20.0; snrdb<=30.0; snrdb+=0.5) {
		    wns::Ratio snr = wns::Ratio::from_dB(snrdb);
		    double mib = testSNR2MItable->convertSNR2MIB(snr,modulation);
		    // inverse function:
		    double snr2 = testSNR2MItable->MIB2SNR(mib,modulation); // inversion
		    //if (useCout) std::cout << "testMIB2SNR(mod="<<modulation<<"): SNR="<<snr<<" => MIB="<<mib<<" => SNR="<<snr2<<std::endl;
		    if ((mib>0.0) && (mib<1.0)) { // useful region
			    CPPUNIT_ASSERT_DOUBLES_EQUAL(snrdb,snr2,1.0); // loose
		    }
	    } // forall sinrs
    } // forall modulations
}

void SNR2MITest::testBERfunctions()
{
	for ( double ber=0.0; ber<=0.5; ber+=0.01) {
		double mib = SNR2MI::BER2MIB(ber);
		CPPUNIT_ASSERT(mib >= 0);
		CPPUNIT_ASSERT(mib <= 1.0);
		//if (useCout) std::cout << "BER2MIB("<<ber<<")="<<mib<< std::endl;
		double ber2 = SNR2MI::MIB2BER(mib); // inversion
		//if (useCout) std::cout << "       MIB2BER("<<mib<<")="<<ber2<< std::endl;
		WNS_ASSERT_MAX_REL_ERROR( ber, ber2, 0.01 ); // allow 1%
	}
}


