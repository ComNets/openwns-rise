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

#include <RISE/antenna/tests/BeamformingTest.hpp>
#include <RISE/antenna/Antenna.hpp>
#include <RISE/antenna/Static.hpp>
#include <RISE/stations/tests/StationDropIn.hpp>
#include <iostream>


using namespace rise;
using namespace rise::tests;
using namespace rise::antenna;

CPPUNIT_TEST_SUITE_REGISTRATION( BeamformingTest );

BeamformingTest::BeamformingTest()
{
}

BeamformingTest::~BeamformingTest()
{
}

void BeamformingTest::prepare()
{
	manager = new SystemManagerDropIn();
}

void BeamformingTest::cleanup()
{
	delete manager;
}

void BeamformingTest::simple()
{
	Station* station1 = new StationDropIn(manager);

	// Test if the pyconfig::Parser stuff is working
	std::stringstream ss;
	ss << "from rise.Antenna import * \n"
	   << "beamforming = BFAntenna(4, [0,0,0]) \n"
	   << "beamforming.positionErrorVariance = 0.0\n";
	wns::pyconfig::Parser pyConfig;
	pyConfig.loadString(ss.str());
	Beamforming antenna1(pyConfig.getView("beamforming"), station1);
	delete station1;
}

void BeamformingTest::testAlgorithm()
{
	Station* station1 = new StationDropIn(manager);	wns::Position p(0, 0, 0); station1->moveTo(p);
	Station* station2 = new StationDropIn(manager); p.set( 30,  20, 0); station2->moveTo(p);
	Station* station3 = new StationDropIn(manager); p.set(100,  10, 0); station3->moveTo(p);
	Station* station4 = new StationDropIn(manager); p.set( 40, 100, 0); station4->moveTo(p);

	wns::pyconfig::Parser pyConfig;
	std::stringstream ss;
	ss << "from rise.Antenna import * \n"
	   << "beamforming = BFAntenna(4, [0,0,0]) \n"
	   << "beamforming.patternOutputFile = \"BFAntenna.dat\" \n";
	pyConfig.loadString(ss.str());
	wns::Power noise = wns::Power::from_dBm(-93);
	wns::Power x_friendlyness = noise;
	wns::Power zero = wns::Power::from_mW(0.0);
	Beamforming antenna1(pyConfig.getView("beamforming"), station1);
	Beamforming antenna2(pyConfig.getView("beamforming"), station2);
	Beamforming antenna3(pyConfig.getView("beamforming"), station3);
	Beamforming antenna4(pyConfig.getView("beamforming"), station4);


	// Stations must be registered at the beamforming antenna in advance:
	// The power that is received at the BF antenna has to be specified
	antenna1.setPowerReceivedForStation(station2, wns::Power::from_dBm(-70)); //pathloss of 30dB
	antenna1.setPowerReceivedForStation(station3, wns::Power::from_dBm(-50)); //pathloss of 10dB
	antenna1.setPowerReceivedForStation(station4, wns::Power::from_dBm(-60)); //pathloss of 20dB

	// as well as the power that the station uses to send 
	// (only necessary for calculateSINRsTx)
	antenna1.setTxPowerForStation(station2, wns::Power::from_dBm(-40));
	antenna1.setTxPowerForStation(station3, wns::Power::from_dBm(-40));
	antenna1.setTxPowerForStation(station4, wns::Power::from_dBm(-40));

	std::vector<Station*> combination;
 	combination.push_back(station2);
 	combination.push_back(station3);
	combination.push_back(station4);
	wns::Power txPower = wns::Power::from_dBm(-40);
	std::map<Station*, wns::Power> Station2NoisePlusIintercell;
	Station2NoisePlusIintercell[station2] = zero;
	Station2NoisePlusIintercell[station3] = zero;
	Station2NoisePlusIintercell[station4] = zero;

	// test default mode: txPower limited by antennna txPower:
	std::map<rise::Station*, wns::CandI> candis = 
		antenna1.calculateCandIsTx(Station2NoisePlusIintercell, x_friendlyness, txPower, false);
	std::map<rise::Station*, wns::CandI>::iterator itr, itrEnd;
	CPPUNIT_ASSERT(!candis.empty());
	itrEnd = candis.end();
	itr = candis.find(station2);
	CPPUNIT_ASSERT( itr != itrEnd);
	CPPUNIT_ASSERT_DOUBLES_EQUAL(38.29, ((*itr).second.C/(*itr).second.I).get_dB(), 1E-2);
	//ideally this should be:
        // (txPower - pathloss + antenna gain (see below) - ( Iintra (unknown) + Iinter(=0) + noise(=0))
        // (-40dBm -30dB + 6.154dBi) - (-102.136dBm) = 38.29 dBm
	itr = candis.find(station3);
	CPPUNIT_ASSERT( itr != itrEnd);
	CPPUNIT_ASSERT_DOUBLES_EQUAL(35.87, ((*itr).second.C/(*itr).second.I).get_dB(), 1E-2);
	itr = candis.find(station4);
	CPPUNIT_ASSERT( itr != itrEnd);
	CPPUNIT_ASSERT_DOUBLES_EQUAL(42.75, ((*itr).second.C/(*itr).second.I).get_dB(), 1E-2);

	// test optional eirplimited mode: txPower limited by EIRP:
	// without noise or inter-cell interference both modes perform equal
	candis = antenna1.calculateCandIsTx(Station2NoisePlusIintercell, x_friendlyness, txPower, true);
	itr = candis.find(station2);
	CPPUNIT_ASSERT( itr != candis.end());
	CPPUNIT_ASSERT_DOUBLES_EQUAL(38.29, ((*itr).second.C/(*itr).second.I).get_dB(), 1E-2);

        // now with omni noise at the receiver (default mode)
	Station2NoisePlusIintercell[station2] = noise;
	Station2NoisePlusIintercell[station3] = noise;
	Station2NoisePlusIintercell[station4] = noise;
	candis = antenna1.calculateCandIsTx(Station2NoisePlusIintercell, x_friendlyness, txPower, false);
	CPPUNIT_ASSERT_DOUBLES_EQUAL(24.216, (candis.find(station2)->second.C / candis.find(station2)->second.I).get_dB(), 1E-2);
        // SINR decreases by approx. 14dB because of omni noise and intercell interference

        // EIRP limited mode with omni noise at the receiver
	candis = antenna1.calculateCandIsTx(Station2NoisePlusIintercell, x_friendlyness, txPower, true);
	CPPUNIT_ASSERT_DOUBLES_EQUAL(17.98249, (candis.find(station2)->second.C / candis.find(station2)->second.I).get_dB(), 1E-4);
        // now SINR decreases by approx. 20dB

	std::vector<Station*> combination1;
 	combination1.push_back(station3);
	combination1.push_back(station4);

	PatternPtr pattern = antenna1.calculateAndSetBeam(station2, combination1, noise);
	std::string outputDir = wns::simulator::getConfiguration().get<std::string>("outputDir");
 	antenna1.drawRadiationPattern(outputDir + "/" + pyConfig.get<std::string>("beamforming.patternOutputFile"), pattern);

        double sumPower = 0.0;
 	for(int i = 0; i < 360; i++)
	{
		wns::PositionOffset pOffset; wns::Position pos;
		pOffset.setPolar(1, double(i)*M_PI/180, M_PI/2);
		pos = antenna1.getPosition() + pOffset;
		sumPower += antenna1.getGain(pos, pattern).get_factor();
	}
	CPPUNIT_ASSERT_DOUBLES_EQUAL( 1, sumPower/360, 1E-2);

	CPPUNIT_ASSERT_DOUBLES_EQUAL(   6.154, antenna1.getGain(station2->getAntenna()->getPosition(), pattern).get_dB(), 1E-3);
	CPPUNIT_ASSERT_DOUBLES_EQUAL( -29.498, antenna1.getGain(station3->getAntenna()->getPosition(), pattern).get_dB(), 1E-3);
	CPPUNIT_ASSERT_DOUBLES_EQUAL( -40.904, antenna1.getGain(station4->getAntenna()->getPosition(), pattern).get_dB(), 1E-3);


	std::vector<Station*> combination2;
  	combination2.push_back(station2);
  	combination2.push_back(station4);
	PatternPtr pattern1 = antenna1.calculateAndSetBeam(station3, combination2, noise);
        sumPower = 0.0;
 	for(int i = 0; i < 360; i++)
	{
		wns::PositionOffset pOffset; wns::Position pos;
		pOffset.setPolar(1, double(i)*M_PI/180, M_PI/2);
		pos = antenna1.getPosition() + pOffset;
		sumPower += antenna1.getGain(pos, pattern1).get_factor();
	}
	CPPUNIT_ASSERT_DOUBLES_EQUAL( 1, sumPower/360, 1E-2);


	CPPUNIT_ASSERT_DOUBLES_EQUAL( -34.789, antenna1.getGain(station2->getAntenna()->getPosition(), pattern1).get_dB(), 1E-3);
	CPPUNIT_ASSERT_DOUBLES_EQUAL(   7.152, antenna1.getGain(station3->getAntenna()->getPosition(), pattern1).get_dB(), 1E-3);
	CPPUNIT_ASSERT_DOUBLES_EQUAL( -46.560, antenna1.getGain(station4->getAntenna()->getPosition(), pattern1).get_dB(), 1E-3);

}


void BeamformingTest::testPositionError()
{
	Station* station1 = new StationDropIn(manager);	wns::Position p(0, 0, 0); station1->moveTo(p);
	Station* station2 = new StationDropIn(manager); p.set( 30,  20, 0); station2->moveTo(p);
	Station* station3 = new StationDropIn(manager); p.set(100,  10, 0); station3->moveTo(p);
	Station* station4 = new StationDropIn(manager); p.set( 40, 100, 0); station4->moveTo(p);

	wns::pyconfig::Parser pyConfig;
	std::stringstream ss;
	ss << "from rise.Antenna import * \n"
	   << "beamforming = BFAntenna(4, [0,0,0]) \n"
	   << "beamforming.positionErrorVariance = 6.0 \n"
	   << "beamforming.patternOutputFile = \"BFAntennaError.dat\" \n";
	pyConfig.loadString(ss.str());
	wns::Power noise = wns::Power::from_dBm(-93);
	wns::Power x_friendlyness = noise;
	wns::Power zero = wns::Power::from_mW(0.0);
	Beamforming antenna1(pyConfig.getView("beamforming"), station1);
	Beamforming antenna2(pyConfig.getView("beamforming"), station2);
	Beamforming antenna3(pyConfig.getView("beamforming"), station3);
	Beamforming antenna4(pyConfig.getView("beamforming"), station4);


	// Stations must be registered at the beamforming antenna in advance:
	// The power that is received at the BF antenna has to be specified
	antenna1.setPowerReceivedForStation(station2, wns::Power::from_dBm(-70)); //pathloss of 30dB
	antenna1.setPowerReceivedForStation(station3, wns::Power::from_dBm(-50)); //pathloss of 10dB
	antenna1.setPowerReceivedForStation(station4, wns::Power::from_dBm(-60)); //pathloss of 20dB

	// as well as the power that the station uses to send 
	// (only necessary for calculateSINRsTx)
	antenna1.setTxPowerForStation(station2, wns::Power::from_dBm(-40));
	antenna1.setTxPowerForStation(station3, wns::Power::from_dBm(-40));
	antenna1.setTxPowerForStation(station4, wns::Power::from_dBm(-40));

	std::vector<Station*> combination1;
 	combination1.push_back(station3);
	combination1.push_back(station4);
	PatternPtr pattern = antenna1.calculateAndSetBeam(station2, combination1, noise);
	std::string outputDir = wns::simulator::getConfiguration().get<std::string>("outputDir");
 	antenna1.drawRadiationPattern(outputDir + "/" + pyConfig.get<std::string>("beamforming.patternOutputFile"), pattern);

        double sumPower = 0.0;
 	for(int i = 0; i < 360; i++)
	{
		wns::PositionOffset pOffset; wns::Position pos;
		pOffset.setPolar(1, double(i)*M_PI/180, M_PI/2);
		pos = antenna1.getPosition() + pOffset;
		sumPower += antenna1.getGain(pos, pattern).get_factor();
	}
	CPPUNIT_ASSERT_DOUBLES_EQUAL( 1, sumPower/360, 1E-2);

	// SINR calculation regarding the wrong positions
        // the gain into the SSs' directions are no longer perfect
	// NOTE THAT these values depend on the random number generator
	// NOTE: This has been disabled, since the test is useless this way
	// CPPUNIT_ASSERT_DOUBLES_EQUAL(  6.535, antenna1.getGain(station2->getAntenna()->getPosition(), pattern).get_dB(), 1E-3);
        // was 6.154
	// CPPUNIT_ASSERT_DOUBLES_EQUAL( -6.368, antenna1.getGain(station3->getAntenna()->getPosition(), pattern).get_dB(), 1E-3);
	// was -29.498
	// CPPUNIT_ASSERT_DOUBLES_EQUAL(-23.6838, antenna1.getGain(station4->getAntenna()->getPosition(), pattern).get_dB(), 1E-3);
	// was -40.904
}


void BeamformingTest::testCircularLayout()
{
	Station* station1 = new StationDropIn(manager);	wns::Position p(0, 0, 0); 
	station1->moveTo(p);
	Station* station2 = new StationDropIn(manager); p.set( 30,  20, 0); station2->moveTo(p);
	Station* station3 = new StationDropIn(manager); p.set(100,  10, 0); station3->moveTo(p);
	Station* station4 = new StationDropIn(manager); p.set( 40, 100, 0); station4->moveTo(p);

	wns::pyconfig::Parser pyConfig;
	std::stringstream ss;
	ss << "from rise.Antenna import * \n"
	   << "beamforming = BFAntenna(7, [0,0,0], \"circular\") \n"
	   << "beamforming.patternOutputFile = \"BFAntennaCircular.dat\" \n";
	pyConfig.loadString(ss.str());
	wns::Power noise = wns::Power::from_dBm(-93);
	wns::Power x_friendlyness = noise;
	wns::Power zero = wns::Power::from_mW(0.0);
	Beamforming antenna1(pyConfig.getView("beamforming"), station1);
	Beamforming antenna2(pyConfig.getView("beamforming"), station2);
	Beamforming antenna3(pyConfig.getView("beamforming"), station3);
	Beamforming antenna4(pyConfig.getView("beamforming"), station4);


	// Stations must be registered at the beamforming antenna in advance:
	// The power that is received at the BF antenna has to be specified
	antenna1.setPowerReceivedForStation(station2, wns::Power::from_dBm(-70)); //pathloss of 30dB
	antenna1.setPowerReceivedForStation(station3, wns::Power::from_dBm(-50)); //pathloss of 10dB
	antenna1.setPowerReceivedForStation(station4, wns::Power::from_dBm(-60)); //pathloss of 20dB

	// as well as the power that the station uses to send
	// (only necessary for calculateSINRsTx)
	antenna1.setTxPowerForStation(station2, wns::Power::from_dBm(-40));
	antenna1.setTxPowerForStation(station3, wns::Power::from_dBm(-40));
	antenna1.setTxPowerForStation(station4, wns::Power::from_dBm(-40));

	std::vector<Station*> combination1;
 	combination1.push_back(station3);
	combination1.push_back(station4);

	PatternPtr pattern = antenna1.calculateAndSetBeam(station2, combination1, noise);
	std::string outputDir = wns::simulator::getConfiguration().get<std::string>("outputDir");
 	antenna1.drawRadiationPattern(outputDir + "/" + pyConfig.get<std::string>("beamforming.patternOutputFile"), pattern);

	CPPUNIT_ASSERT_DOUBLES_EQUAL(   5.709, antenna1.getGain(station2->getAntenna()->getPosition(), pattern).get_dB(), 1E-3);
	CPPUNIT_ASSERT_DOUBLES_EQUAL( -31.856, antenna1.getGain(station3->getAntenna()->getPosition(), pattern).get_dB(), 1E-3);
	CPPUNIT_ASSERT_DOUBLES_EQUAL( -35.575, antenna1.getGain(station4->getAntenna()->getPosition(), pattern).get_dB(), 1E-3);
	double sumPower = 0.0;
 	for(int i = 0; i < 360; i++)
	{
		wns::PositionOffset pOffset; wns::Position pos;
		pOffset.setPolar(1, double(i)*M_PI/180, M_PI/2);
		pos = antenna1.getPosition() + pOffset;
		sumPower += antenna1.getGain(pos, pattern).get_factor();
	}
	CPPUNIT_ASSERT_DOUBLES_EQUAL( 1, sumPower/360, 1E-2);
}




