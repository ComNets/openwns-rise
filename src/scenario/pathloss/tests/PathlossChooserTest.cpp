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

#include <RISE/scenario/pathloss/tests/PathlossTestsBase.hpp>
#include <RISE/scenario/pathloss/DistanceDependent.hpp>
#include <RISE/scenario/pathloss/Pathloss.hpp>
//#include <RISE/scenario/pathloss/PathlossChooser.cpp>
#include <RISE/scenario/pathloss/PathlossChooser.hpp>
#include <RISE/antenna/Antenna.hpp>

#include <WNS/PowerRatio.hpp>
#include <WNS/Position.hpp>
#include <WNS/CppUnit.hpp>
#include <WNS/pyconfig/helper/Functions.hpp>
#include <WNS/Types.hpp>

#include <cppunit/extensions/HelperMacros.h>

//#######################
#include <iostream>
using namespace std;

namespace rise { namespace scenario { namespace pathloss { namespace tests {

    class PathlossChooserTest :
	public PathlossTestsBase,
	public wns::TestFixture
    {
	CPPUNIT_TEST_SUITE( PathlossChooserTest );
	CPPUNIT_TEST( testInput );
	CPPUNIT_TEST( testOutput );
	CPPUNIT_TEST_SUITE_END();
	using PathlossTestsBase::testInput;
    public:
	virtual void prepare();
	virtual void cleanup();
	void testInput();
	void testOutput();
    private:
	Pathloss* pathlossLOSUrbanMicro;
	Pathloss* pathlossNLOSUrbanMicro;
	Pathloss* pathlossLOSUrbanMacro;
	Pathloss* pathlossNLOSUrbanMacro;
	Pathloss* pathlossLOSSubUrbanMacro;
	Pathloss* pathlossNLOSSubUrbanMacro;
	Pathloss* pathlossLOSRuralMacro;
	Pathloss* pathlossNLOSRuralMacro;
	bool deletePathloss;
    };

    CPPUNIT_TEST_SUITE_REGISTRATION( PathlossChooserTest);

    void PathlossChooserTest::prepare()
    {
	deletePathloss = false;
	PathlossTestsBase::prepare();
	
	pathlossLOSUrbanMicro = new PathlossChooser(wns::pyconfig::helper::createViewFromDropInConfig("rise.scenario.PathlossChooser",   "PathlossChooserTestUrbanMicro"));
	pathlossNLOSUrbanMicro = new PathlossChooser(wns::pyconfig::helper::createViewFromDropInConfig("rise.scenario.PathlossChooser",   "PathlossChooserTestUrbanMicro"));
	
	pathlossLOSUrbanMacro = new PathlossChooser(wns::pyconfig::helper::createViewFromDropInConfig("rise.scenario.PathlossChooser",   "PathlossChooserTestUrbanMacro"));
	pathlossNLOSUrbanMacro = new PathlossChooser(wns::pyconfig::helper::createViewFromDropInConfig("rise.scenario.PathlossChooser",   "PathlossChooserTestUrbanMacro"));
	
	pathlossLOSSubUrbanMacro = new PathlossChooser(wns::pyconfig::helper::createViewFromDropInConfig("rise.scenario.PathlossChooser",   "PathlossChooserTestSubUrbanMacro"));
	pathlossNLOSSubUrbanMacro = new PathlossChooser(wns::pyconfig::helper::createViewFromDropInConfig("rise.scenario.PathlossChooser",   "PathlossChooserTestSubUrbanMacro"));
	
	pathlossLOSRuralMacro = new PathlossChooser(wns::pyconfig::helper::createViewFromDropInConfig("rise.scenario.PathlossChooser",   "PathlossChooserTestRuralMacro"));
	pathlossNLOSRuralMacro = new PathlossChooser(wns::pyconfig::helper::createViewFromDropInConfig("rise.scenario.PathlossChooser",   "PathlossChooserTestRuralMacro"));

		
	deletePathloss = true;
    }

    void PathlossChooserTest::cleanup()
    {
	if (deletePathloss) 
	{
		delete pathlossLOSUrbanMicro;
		delete pathlossNLOSUrbanMicro;
		
		delete pathlossLOSUrbanMacro;
		delete pathlossNLOSUrbanMacro;
		
		delete pathlossLOSSubUrbanMacro;
		delete pathlossNLOSSubUrbanMacro;
		
		delete pathlossLOSRuralMacro;
		delete pathlossNLOSRuralMacro;
		
	}
	PathlossTestsBase::cleanup();
    }

    void PathlossChooserTest::testInput()
    {
	testInput(pathlossLOSUrbanMicro);
	testInput(pathlossNLOSUrbanMicro);
	
	testInput(pathlossLOSUrbanMacro);
	testInput(pathlossNLOSUrbanMacro);
	
	testInput(pathlossLOSSubUrbanMacro);
	testInput(pathlossNLOSSubUrbanMacro);
		
	testInput(pathlossLOSRuralMacro);
	testInput(pathlossLOSRuralMacro);
    }
	
	// Testing pathloss models with parameters taken from the tests defined in PathlossChooser.py
	
	void PathlossChooserTest::testOutput()
    {
	
	wns::Frequency frequency1 = 2500;
	wns::Frequency frequency2 = 2000;
	wns::Frequency frequency3 = 800;
		
	// Testing LOS models at 11m distance
	station1->moveTo(wns::Position(0, 0, 10));
	station2->moveTo(wns::Position(11, 0, 10));
 	WNS_ASSERT_MAX_REL_ERROR( 58.8694,
 				  pathlossLOSUrbanMicro->getPathloss(*antenna1, *antenna2, frequency1).get_dB(),
 				  1E-5 );
	
	WNS_ASSERT_MAX_REL_ERROR( 56.9312,
 				  pathlossLOSUrbanMacro->getPathloss(*antenna1, *antenna2, frequency2).get_dB(),
 				  1E-5 );
	
	WNS_ASSERT_MAX_REL_ERROR( 59.6247,
 				  pathlossLOSSubUrbanMacro->getPathloss(*antenna1, *antenna2, frequency2).get_dB(),
 				  1E-5 );
	
	WNS_ASSERT_MAX_REL_ERROR( 52.1317,
 				  pathlossLOSRuralMacro->getPathloss(*antenna1, *antenna2, frequency3).get_dB(),
 				  1E-5 );
	
	// Testing LOS models at 4999m distance
 	station2->moveTo(wns::Position(4999, 0, 10));
		
 	WNS_ASSERT_MAX_REL_ERROR( 168.7955,
 				  pathlossNLOSUrbanMicro->getPathloss(*antenna1, *antenna2, frequency1).get_dB(),
 				  1E-5 );

	WNS_ASSERT_MAX_REL_ERROR( 177.4478,
 				  pathlossNLOSUrbanMacro->getPathloss(*antenna1, *antenna2, frequency2).get_dB(),
 				  1E-5 );
				  
	WNS_ASSERT_MAX_REL_ERROR( 164.0901,
 				  pathlossNLOSSubUrbanMacro->getPathloss(*antenna1, *antenna2, frequency2).get_dB(),
 				  1E-5 );
	
	WNS_ASSERT_MAX_REL_ERROR( 151.0985,
 				  pathlossNLOSRuralMacro->getPathloss(*antenna1, *antenna2, frequency3).get_dB(),
 				  1E-5 );
    }


}}}}
