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

#include <RISE/scenario/Propagation.hpp>
#include <RISE/antenna/tests/AntennaDropIn.hpp>
#include <RISE/stations/tests/StationDropIn.hpp>
#include <RISE/transceiver/tests/ReceiverDropIn.hpp>
#include <RISE/transceiver/tests/TransmitterDropIn.hpp>

#include <RISE/scenario/pathloss/Pathloss.hpp>
#include <RISE/scenario/shadowing/Shadowing.hpp>
#include <RISE/scenario/fastfading/FastFading.hpp>

#include <WNS/pyconfig/helper/Functions.hpp>
#include <WNS/pyconfig/View.hpp>
#include <WNS/CppUnit.hpp>

#include <cppunit/extensions/HelperMacros.h>

#include <string>

namespace rise { namespace scenario { namespace tests {

    class PropagationTest :
	public wns::TestFixture
    {
	CPPUNIT_TEST_SUITE( PropagationTest );
	CPPUNIT_TEST( checkConfiguration );
	CPPUNIT_TEST( getNames );
	CPPUNIT_TEST( runSimpleModels );
	CPPUNIT_TEST_SUITE_END();
    public:
	virtual void
	prepare();
	
	virtual void
	cleanup();
	
	void
	checkConfiguration();
	
	void
	getNames();
	
	void
	runSimpleModels();
    private:
	wns::pyconfig::View* config;
	scenario::Propagation* propagation;
    };

    CPPUNIT_TEST_SUITE_REGISTRATION( PropagationTest );

    void
    PropagationTest::prepare()
    {
	config = new wns::pyconfig::View(wns::pyconfig::helper::createViewFromString("from rise.scenario.PropagationCppUnitTests import PropagationForUnitTest\n").get<wns::pyconfig::View>("PropagationForUnitTest.getInstance()"));
	propagation = GlobalPropagationBroker::getInstance()->procure(*config);
    }

    void
    PropagationTest::cleanup()
    {
	delete config;
    }

    void
    PropagationTest::checkConfiguration()
    {
	CPPUNIT_ASSERT( config->get<bool>("knowsName(\"Test1\")") );
	CPPUNIT_ASSERT( config->get<bool>("knowsName(\"Test2\")") );

	std::string idTest1 = wns::Ttos(config->get<int>("findId(\"Test1\")"));
	std::string idTest2 = wns::Ttos(config->get<int>("findId(\"Test2\")"));
	CPPUNIT_ASSERT( config->get<bool>("knowsPairById(" + idTest1 + ", " + idTest1 + ")") );
	CPPUNIT_ASSERT( config->get<bool>("knowsPairById(" + idTest2 + ", " + idTest2 + ")") );
	CPPUNIT_ASSERT( config->get<bool>("knowsPairById(" + idTest1 + ", " + idTest2 + ")") );
	CPPUNIT_ASSERT( !config->get<bool>("knowsPairById(" + idTest2 + ", " + idTest1 + ")") );
	CPPUNIT_ASSERT_EQUAL( std::string("Constant"), config->get<std::string>("getPair(" + idTest1 + ", " + idTest1 + ").pathloss.__plugin__") );
	CPPUNIT_ASSERT_EQUAL( std::string("Constant"), config->get<std::string>("getPair(" + idTest2 + ", " + idTest2 + ").pathloss.__plugin__") );
	CPPUNIT_ASSERT_EQUAL( std::string("Constant"), config->get<std::string>("getPair(" + idTest1 + ", " + idTest2 + ").pathloss.__plugin__") );
	CPPUNIT_ASSERT_EQUAL( std::string("3 dB"), config->get<std::string>("getPair(" + idTest1 + ", " + idTest1 + ").pathloss.value") );
	CPPUNIT_ASSERT_EQUAL( std::string("4 dB"), config->get<std::string>("getPair(" + idTest2 + ", " + idTest2 + ").pathloss.value") );
	CPPUNIT_ASSERT_EQUAL( std::string("5 dB"), config->get<std::string>("getPair(" + idTest1 + ", " + idTest2 + ").pathloss.value") );
	CPPUNIT_ASSERT_EQUAL( std::string("None"), config->get<std::string>("getPair(" + idTest1 + ", " + idTest1 + ").shadowing.__plugin__") );
	CPPUNIT_ASSERT_EQUAL( std::string("None"), config->get<std::string>("getPair(" + idTest1 + ", " + idTest1 + ").fastFading.__plugin__") );
    }

    void
    PropagationTest::getNames()
    {
	int idTest1 = config->get<int>("findId(\"Test1\")");
	int idTest2 = config->get<int>("findId(\"Test2\")");
	CPPUNIT_ASSERT_EQUAL( std::string("Test1"), propagation->getName(idTest1) );
	CPPUNIT_ASSERT_EQUAL( std::string("Test2"), propagation->getName(idTest2) );
    }

    void
    PropagationTest::runSimpleModels()
    {
	wns::pyconfig::View config11 = wns::pyconfig::helper::createViewFromDropInConfig("rise.scenario.PropagationCppUnitTests",
											 "TestStation1");
	wns::pyconfig::View config12 = wns::pyconfig::helper::createViewFromDropInConfig("rise.scenario.PropagationCppUnitTests",
											 "TestStation1");
	wns::pyconfig::View config21 = wns::pyconfig::helper::createViewFromDropInConfig("rise.scenario.PropagationCppUnitTests",
											 "TestStation2");
	wns::pyconfig::View config22 = wns::pyconfig::helper::createViewFromDropInConfig("rise.scenario.PropagationCppUnitTests",
											 "TestStation2");
    
	rise::tests::SystemManagerDropIn* systemManager = new rise::tests::SystemManagerDropIn();
	rise::tests::StationDropIn* station11 = new rise::tests::StationDropIn(systemManager, config11);
	rise::tests::StationDropIn* station12 = new rise::tests::StationDropIn(systemManager, config12);
	rise::tests::StationDropIn* station21 = new rise::tests::StationDropIn(systemManager, config21);
	rise::tests::StationDropIn* station22 = new rise::tests::StationDropIn(systemManager, config22);
	rise::tests::AntennaDropIn* antenna11 = new rise::tests::AntennaDropIn(station11);
	rise::tests::AntennaDropIn* antenna12 = new rise::tests::AntennaDropIn(station12);
	rise::tests::AntennaDropIn* antenna21 = new rise::tests::AntennaDropIn(station21);
	rise::tests::AntennaDropIn* antenna22 = new rise::tests::AntennaDropIn(station22);

	rise::tests::ReceiverDropIn* receiver11 = new rise::tests::ReceiverDropIn(station11, antenna11, config11.get<wns::pyconfig::View>("receiver[0]"));
	rise::tests::ReceiverDropIn* receiver12 = new rise::tests::ReceiverDropIn(station12, antenna12, config12.get<wns::pyconfig::View>("receiver[0]"));
	rise::tests::ReceiverDropIn* receiver21 = new rise::tests::ReceiverDropIn(station21, antenna21, config21.get<wns::pyconfig::View>("receiver[0]"));
	rise::tests::ReceiverDropIn* receiver22 = new rise::tests::ReceiverDropIn(station22, antenna22, config22.get<wns::pyconfig::View>("receiver[0]"));

	rise::tests::TransmitterDropIn* transmitter11 = new rise::tests::TransmitterDropIn(station11, antenna11, config11.get<wns::pyconfig::View>("transmitter[0]"));
	rise::tests::TransmitterDropIn* transmitter12 = new rise::tests::TransmitterDropIn(station12, antenna12, config12.get<wns::pyconfig::View>("transmitter[0]"));
	rise::tests::TransmitterDropIn* transmitter21 = new rise::tests::TransmitterDropIn(station21, antenna21, config21.get<wns::pyconfig::View>("transmitter[0]"));
	rise::tests::TransmitterDropIn* transmitter22 = new rise::tests::TransmitterDropIn(station22, antenna22, config22.get<wns::pyconfig::View>("transmitter[0]"));

	CPPUNIT_ASSERT_EQUAL( wns::Ratio::from_dB(3.0), propagation->getPathlossModel(transmitter11->getPropagationCharacteristicId(),
										      receiver12->getPropagationCharacteristicId()).getPathloss(*antenna11, *antenna12, 450));
	CPPUNIT_ASSERT_EQUAL( wns::Ratio::from_dB(4.0), propagation->getPathlossModel(transmitter21->getPropagationCharacteristicId(),
										      receiver22->getPropagationCharacteristicId()).getPathloss(*antenna21, *antenna22, 450));
	CPPUNIT_ASSERT_EQUAL( wns::Ratio::from_dB(5.0), propagation->getPathlossModel(transmitter11->getPropagationCharacteristicId(),
										      receiver21->getPropagationCharacteristicId()).getPathloss(*antenna11, *antenna21, 450));
	CPPUNIT_ASSERT_THROW( propagation->getPathlossModel(transmitter21->getPropagationCharacteristicId(),
							    receiver11->getPropagationCharacteristicId()).getPathloss(*antenna21, *antenna11, 450), rise::scenario::Propagation::NoModelError);

	delete transmitter22;
	delete transmitter21;
	delete transmitter12;
	delete transmitter11;

	delete receiver22;
	delete receiver21;
	delete receiver12;
	delete receiver11;

	delete antenna22;
	delete antenna21;
	delete antenna12;
	delete antenna11;

	delete station22;
	delete station21;
	delete station12;
	delete station11;

	delete systemManager;
    }

} // tests
} // scenario
} // rise
