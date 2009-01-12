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

#include <RISE/antenna/tests/AntennaTest.hpp>
#include <RISE/antenna/Static.hpp>
#include <RISE/stations/tests/StationDropIn.hpp>
#include <RISE/antenna/Beamforming.hpp>
#include <WNS/pyconfig/Parser.hpp>
#include <WNS/CppUnit.hpp>

#include <iostream>

using namespace rise;
using namespace rise::antenna;

CPPUNIT_TEST_SUITE_REGISTRATION( AntennaTest );

void AntennaTest::prepare()
{
	systemManager = new tests::SystemManagerDropIn();
}

void AntennaTest::cleanup()
{
	delete systemManager;
}

void AntennaTest::testConfig()
{
	tests::StationDropIn station(systemManager);
	// Test if the pyconfig::Parser stuff is working
	std::stringstream ss;
	ss << "from rise.Antenna import * \n"
	   << "isotropic = Isotropic([0,0,0]) \n"
	   << "lambdahalf = LambdaHalf([0,0,0]) \n"
	   << "omnidirectional = OmniDirectional([0,0,0]) \n"
	   << "directed13 = Directed(13, [0,0,0], 0, 0) \n"
	   << "directed17 = Directed(17, [0,0,0], 0, 0) \n"
	   << "directed30 = Directed(30, [0,0,0], 0, 0) \n"
	   << "directed66 = Directed(66, [0,0,0], 0, 0) \n"
	   << "directed90 = Directed(90, [0,0,0], 0, 0) \n"
	   << "directed120 = Directed(120, [0,0,0], 0, 0) \n"
	   << "bfantenna = BFAntenna(4, [0,0,0])";

	wns::pyconfig::Parser p;
	p.loadString(ss.str());
	Static(p.getView("isotropic"), &station);
	Static(p.getView("lambdahalf"), &station);
	Static(p.getView("omnidirectional"), &station);
	Static(p.getView("directed13"), &station);
	Static(p.getView("directed17"), &station);
	Static(p.getView("directed30"), &station);
	Static(p.getView("directed66"), &station);
	Static(p.getView("directed90"), &station);
	Static(p.getView("directed120"), &station);
	Beamforming(p.getView("bfantenna"), &station);
}

void AntennaTest::testGetPosition()
{
	std::stringstream ss;
	ss << "from rise.Antenna import * \n"
	   << "isotropic = Isotropic([0,0,3]) \n";
	wns::pyconfig::Parser p;
	p.loadString(ss.str());
	tests::StationDropIn station(systemManager);
	Static a(p.getView("isotropic"), &station);
	// inform atenna about its position
	station.positionChanged();

	CPPUNIT_ASSERT( a.getPosition() == wns::Position(0,0,3) );
	station.moveTo(wns::Position(1,2,3));
	CPPUNIT_ASSERT( a.getPosition() == wns::Position(1,2,6) );
}

void AntennaTest::testIsotropic()
{
	wns::pyconfig::Parser p;
	p.loadString("from rise.Antenna import * \nisotropic = Isotropic([0,0,0]) \n");
	tests::StationDropIn station(systemManager);
	Static isotropic(p.getView("isotropic"), &station);
	for(int elevation = 0; elevation < 180; elevation += 6) {
		for(int azimuth = 0; azimuth < 360; azimuth += 6) {
			// The directivity interface of the antenna needs the
			// position of the calling object, as well as a pointer
			// to a station and a flag "sending"
			// The latter two are only used for BeamForming
			// antennas. May be this can be solved in a different
			// way, since it is very confusing if you don't want to
			// use beamforming antennas.

			// One idea is to put the antenna pattern into the
			// transmission object and use the TO for gain
			// calculation
			wns::Position p = calcPosition(isotropic, azimuth, elevation);
 			PatternPtr pattern(new Pattern);
			WNS_ASSERT_MAX_REL_ERROR(
				wns::Ratio(),
				isotropic.getGain(p, pattern),
				0.00001);
		}
	}
}

void AntennaTest::testOmniDirectional()
{
	wns::pyconfig::Parser p;
	p.loadString("from rise.Antenna import * \nomni = OmniDirectional([0,0,0]) \n");
	tests::StationDropIn station(systemManager);
	Static omni(p.getView("omni"), &station);
 	PatternPtr pattern(new Pattern);
	for(int elevation = 0; elevation < 180; elevation += 6) {
		for(int azimuth = 0; azimuth < 360; azimuth += 6) {
			wns::Position p = calcPosition(omni, azimuth, elevation);
			WNS_ASSERT_MAX_REL_ERROR(
				wns::Ratio::from_dB(5),
				omni.getGain(p, pattern),
				0.00001);
		}
	}
}

void AntennaTest::testLambdaHalf()
{
	wns::pyconfig::Parser p;
	p.loadString("from rise.Antenna import * \nlambdahalf = LambdaHalf([0,0,0]) \n");
	tests::StationDropIn station(systemManager);
	Static a(p.getView("lambdahalf"), &station);
 	PatternPtr pattern(new Pattern);
	for(int elevationIndex = 0; elevationIndex < 360; elevationIndex += 6) {
		for(int azimuth = 0; azimuth < 360; azimuth += 6) {
			wns::Position p = calcPosition(a, azimuth, elevationIndex/2.0);
			WNS_ASSERT_MAX_REL_ERROR(
				wns::Ratio::from_dB(lambdaHalf[elevationIndex]+2.15),
				a.getGain(p, pattern),
				0.00001);
		}
	}
}

void AntennaTest::testAzimuth()
{
	tests::StationDropIn station1(systemManager);
	tests::StationDropIn station2(systemManager);
	tests::StationDropIn station3(systemManager);
 	PatternPtr pattern(new Pattern);

	wns::pyconfig::Parser p;
	std::stringstream ss;
	ss << "from rise.Antenna import * \n"
	   << "import math\n"
	   << "directed1 = Directed(66, [0,0,0],  60, 0)\n"
	   << "directed2 = Directed(66, [0,0,0], -60, 0)\n"
	   << "directed3 = Directed(66, [0,0,0], 300, 0)\n";

	p.loadString(ss.str());
	Static ant1 = Static(p.getView("directed1"), &station1);
	Static ant2 = Static(p.getView("directed2"), &station2);
	Static ant3 = Static(p.getView("directed3"), &station3);

	CPPUNIT_ASSERT(ant1.getPosition() == ant2.getPosition());
	CPPUNIT_ASSERT(ant1.getPosition() == ant3.getPosition());

	int nSteps = 360;
	for (double angle=0; angle<2*M_PI; angle=angle+(2*M_PI/nSteps))
	{
		wns::PositionOffset direction;
		direction.setPolar(100,angle,M_PI/2);
		direction.setDeltaZ(0.0);

		wns::Position pos = ant3.getPosition() + direction;

		WNS_ASSERT_MAX_REL_ERROR(ant2.getGain(pos, pattern).get_dB(), 
					 ant3.getGain(pos, pattern).get_dB(), 
					 1e-8);
	}
}

wns::Position AntennaTest::calcPosition(const Antenna& a, double azimuth, double elevation)
{
	wns::PositionOffset p;
	p.setPolar(1, (azimuth/180.0)*M_PI, (elevation/180.0)*M_PI);
	return a.getPosition() + p;
}

AntennaTest::AntennaTest() :
	lambdaHalf(361)
{
	lambdaHalf[0] =   -255;
	lambdaHalf[1] =   -43.3071;
	lambdaHalf[2] =   -37.2861;
	lambdaHalf[3] =   -33.7634;
	lambdaHalf[4] =   -31.2632;
	lambdaHalf[5] =   -29.3039;
	lambdaHalf[6] =   -27.722;
	lambdaHalf[7] =   -26.3832;
	lambdaHalf[8] =   -25.2224;
	lambdaHalf[9] =   -24.1983;
	lambdaHalf[10] =  -23.2805;
	lambdaHalf[11] =  -22.4501;
	lambdaHalf[12] =  -21.692;
	lambdaHalf[13] =  -20.9928;
	lambdaHalf[14] =  -20.3526;
	lambdaHalf[15] =  -19.7486;
	lambdaHalf[16] =  -19.189;
	lambdaHalf[17] =  -18.6572;
	lambdaHalf[18] =  -18.1596;
	lambdaHalf[19] =  -17.6887;
	lambdaHalf[20] =  -17.2417;
	lambdaHalf[21] =  -16.8147;
	lambdaHalf[22] =  -16.4074;
	lambdaHalf[23] =  -16.0216;
	lambdaHalf[24] =  -15.647;
	lambdaHalf[25] =  -15.2906;
	lambdaHalf[26] =  -14.9481;
	lambdaHalf[27] =  -14.6174;
	lambdaHalf[28] =  -14.2978;
	lambdaHalf[29] =  -13.9927;
	lambdaHalf[30] =  -13.6936;
	lambdaHalf[31] =  -13.4065;
	lambdaHalf[32] =  -13.1275;
	lambdaHalf[33] =  -12.857;
	lambdaHalf[34] =  -12.5964;
	lambdaHalf[35] =  -12.3397;
	lambdaHalf[36] =  -12.0919;
	lambdaHalf[37] =  -11.8508;
	lambdaHalf[38] =  -11.6176;
	lambdaHalf[39] =  -11.3871;
	lambdaHalf[40] =  -11.1639;
	lambdaHalf[41] =  -10.9461;
	lambdaHalf[42] =  -10.7349;
	lambdaHalf[43] =  -10.5285;
	lambdaHalf[44] =  -10.3229;
	lambdaHalf[45] =  -10.1261;
	lambdaHalf[46] =  -9.93152;
	lambdaHalf[47] =  -9.74136;
	lambdaHalf[48] =  -9.55514;
	lambdaHalf[49] =  -9.37376;
	lambdaHalf[50] =  -9.19414;
	lambdaHalf[51] =  -9.01819;
	lambdaHalf[52] =  -8.84738;
	lambdaHalf[53] =  -8.67978;
	lambdaHalf[54] =  -8.51446;
	lambdaHalf[55] =  -8.35133;
	lambdaHalf[56] =  -8.19192;
	lambdaHalf[57] =  -8.03533;
	lambdaHalf[58] =  -7.88065;
	lambdaHalf[59] =  -7.72931;
	lambdaHalf[60] =  -7.58201;
	lambdaHalf[61] =  -7.43489;
	lambdaHalf[62] =  -7.2908;
	lambdaHalf[63] =  -7.15044;
	lambdaHalf[64] =  -7.01016;
	lambdaHalf[65] =  -6.87395;
	lambdaHalf[66] =  -6.73859;
	lambdaHalf[67] =  -6.6058;
	lambdaHalf[68] =  -6.47631;
	lambdaHalf[69] =  -6.34677;
	lambdaHalf[70] =  -6.22077;
	lambdaHalf[71] =  -6.0955;
	lambdaHalf[72] =  -5.97358;
	lambdaHalf[73] =  -5.85154;
	lambdaHalf[74] =  -5.7324;
	lambdaHalf[75] =  -5.61527;
	lambdaHalf[76] =  -5.49983;
	lambdaHalf[77] =  -5.38528;
	lambdaHalf[78] =  -5.27259;
	lambdaHalf[79] =  -5.16249;
	lambdaHalf[80] =  -5.05316;
	lambdaHalf[81] =  -4.9454;
	lambdaHalf[82] =  -4.8393;
	lambdaHalf[83] =  -4.73392;
	lambdaHalf[84] =  -4.63092;
	lambdaHalf[85] =  -4.52944;
	lambdaHalf[86] =  -4.42939;
	lambdaHalf[87] =  -4.32998;
	lambdaHalf[88] =  -4.23278;
	lambdaHalf[89] =  -4.13618;
	lambdaHalf[90] =  -4.04173;
	lambdaHalf[91] =  -3.9486;
	lambdaHalf[92] =  -3.85603;
	lambdaHalf[93] =  -3.76548;
	lambdaHalf[94] =  -3.67621;
	lambdaHalf[95] =  -3.58742;
	lambdaHalf[96] =  -3.50063;
	lambdaHalf[97] =  -3.415;
	lambdaHalf[98] =  -3.32989;
	lambdaHalf[99] =  -3.24662;
	lambdaHalf[100] = -3.16445;
	lambdaHalf[101] = -3.08348;
	lambdaHalf[102] = -3.00426;
	lambdaHalf[103] = -2.92552;
	lambdaHalf[104] = -2.84779;
	lambdaHalf[105] = -2.77176;
	lambdaHalf[106] = -2.69687;
	lambdaHalf[107] = -2.62293;
	lambdaHalf[108] = -2.54946;
	lambdaHalf[109] = -2.47826;
	lambdaHalf[110] = -2.40731;
	lambdaHalf[111] = -2.33813;
	lambdaHalf[112] = -2.26919;
	lambdaHalf[113] = -2.20241;
	lambdaHalf[114] = -2.13607;
	lambdaHalf[115] = -2.07121;
	lambdaHalf[116] = -2.0068;
	lambdaHalf[117] = -1.94385;
	lambdaHalf[118] = -1.88173;
	lambdaHalf[119] = -1.82066;
	lambdaHalf[120] = -1.761;
	lambdaHalf[121] = -1.70213;
	lambdaHalf[122] = -1.64431;
	lambdaHalf[123] = -1.58727;
	lambdaHalf[124] = -1.53158;
	lambdaHalf[125] = -1.47692;
	lambdaHalf[126] = -1.42303;
	lambdaHalf[127] = -1.37098;
	lambdaHalf[128] = -1.31913;
	lambdaHalf[129] = -1.26883;
	lambdaHalf[130] = -1.21902;
	lambdaHalf[131] = -1.1702;
	lambdaHalf[132] = -1.1229;
	lambdaHalf[133] = -1.07632;
	lambdaHalf[134] = -1.03075;
	lambdaHalf[135] = -0.98611;
	lambdaHalf[136] = -0.94293;
	lambdaHalf[137] = -0.90029;
	lambdaHalf[138] = -0.858558;
	lambdaHalf[139] = -0.818256;
	lambdaHalf[140] = -0.778673;
	lambdaHalf[141] = -0.740084;
	lambdaHalf[142] = -0.70223;
	lambdaHalf[143] = -0.665906;
	lambdaHalf[144] = -0.630302;
	lambdaHalf[145] = -0.595684;
	lambdaHalf[146] = -0.561805;
	lambdaHalf[147] = -0.529385;
	lambdaHalf[148] = -0.497693;
	lambdaHalf[149] = -0.466984;
	lambdaHalf[150] = -0.43735;
	lambdaHalf[151] = -0.408462;
	lambdaHalf[152] = -0.380945;
	lambdaHalf[153] = -0.354099;
	lambdaHalf[154] = -0.328305;
	lambdaHalf[155] = -0.303553;
	lambdaHalf[156] = -0.279774;
	lambdaHalf[157] = -0.256772;
	lambdaHalf[158] = -0.235049;
	lambdaHalf[159] = -0.214102;
	lambdaHalf[160] = -0.194135;
	lambdaHalf[161] = -0.17522;
	lambdaHalf[162] = -0.157274;
	lambdaHalf[163] = -0.140301;
	lambdaHalf[164] = -0.124174;
	lambdaHalf[165] = -0.109197;
	lambdaHalf[166] = -0.0950497;
	lambdaHalf[167] = -0.0820451;
	lambdaHalf[168] = -0.069865;
	lambdaHalf[169] = -0.0586888;
	lambdaHalf[170] = -0.0484859;
	lambdaHalf[171] = -0.0392576;
	lambdaHalf[172] = -0.031017;
	lambdaHalf[173] = -0.0237462;
	lambdaHalf[174] = -0.0174494;
	lambdaHalf[175] = -0.0121238;
	lambdaHalf[176] = -0.00776691;
	lambdaHalf[177] = -0.0043428;
	lambdaHalf[178] = -0.00192983;
	lambdaHalf[179] = -0.000482433;
	lambdaHalf[180] = -0.0000;
	lambdaHalf[181] = -0.000482433;
	lambdaHalf[182] = -0.00192983;
	lambdaHalf[183] = -0.0043428;
	lambdaHalf[184] = -0.0077225;
	lambdaHalf[185] = -0.0121238;
	lambdaHalf[186] = -0.0174494;
	lambdaHalf[187] = -0.0237462;
	lambdaHalf[188] = -0.031017;
	lambdaHalf[189] = -0.0392576;
	lambdaHalf[190] = -0.0484859;
	lambdaHalf[191] = -0.0586888;
	lambdaHalf[192] = -0.069865;
	lambdaHalf[193] = -0.0819014;
	lambdaHalf[194] = -0.0950497;
	lambdaHalf[195] = -0.109197;
	lambdaHalf[196] = -0.124174;
	lambdaHalf[197] = -0.140301;
	lambdaHalf[198] = -0.157274;
	lambdaHalf[199] = -0.17522;
	lambdaHalf[200] = -0.194135;
	lambdaHalf[201] = -0.214102;
	lambdaHalf[202] = -0.234808;
	lambdaHalf[203] = -0.256772;
	lambdaHalf[204] = -0.279774;
	lambdaHalf[205] = -0.303553;
	lambdaHalf[206] = -0.328305;
	lambdaHalf[207] = -0.354099;
	lambdaHalf[208] = -0.380945;
	lambdaHalf[209] = -0.408462;
	lambdaHalf[210] = -0.43735;
	lambdaHalf[211] = -0.466984;
	lambdaHalf[212] = -0.497693;
	lambdaHalf[213] = -0.529385;
	lambdaHalf[214] = -0.561805;
	lambdaHalf[215] = -0.595311;
	lambdaHalf[216] = -0.630302;
	lambdaHalf[217] = -0.665514;
	lambdaHalf[218] = -0.70223;
	lambdaHalf[219] = -0.739673;
	lambdaHalf[220] = -0.778673;
	lambdaHalf[221] = -0.817826;
	lambdaHalf[222] = -0.858558;
	lambdaHalf[223] = -0.899842;
	lambdaHalf[224] = -0.942473;
	lambdaHalf[225] = -0.98611;
	lambdaHalf[226] = -1.03075;
	lambdaHalf[227] = -1.07632;
	lambdaHalf[228] = -1.1229;
	lambdaHalf[229] = -1.1702;
	lambdaHalf[230] = -1.21902;
	lambdaHalf[231] = -1.26883;
	lambdaHalf[232] = -1.31913;
	lambdaHalf[233] = -1.37044;
	lambdaHalf[234] = -1.42303;
	lambdaHalf[235] = -1.47692;
	lambdaHalf[236] = -1.53158;
	lambdaHalf[237] = -1.58727;
	lambdaHalf[238] = -1.64373;
	lambdaHalf[239] = -1.70213;
	lambdaHalf[240] = -1.76041;
	lambdaHalf[241] = -1.82066;
	lambdaHalf[242] = -1.88173;
	lambdaHalf[243] = -1.94324;
	lambdaHalf[244] = -2.0068;
	lambdaHalf[245] = -2.07059;
	lambdaHalf[246] = -2.13607;
	lambdaHalf[247] = -2.20241;
	lambdaHalf[248] = -2.26919;
	lambdaHalf[249] = -2.33748;
	lambdaHalf[250] = -2.40731;
	lambdaHalf[251] = -2.4776;
	lambdaHalf[252] = -2.54946;
	lambdaHalf[253] = -2.62293;
	lambdaHalf[254] = -2.69687;
	lambdaHalf[255] = -2.77176;
	lambdaHalf[256] = -2.84779;
	lambdaHalf[257] = -2.92552;
	lambdaHalf[258] = -3.00426;
	lambdaHalf[259] = -3.08348;
	lambdaHalf[260] = -3.16445;
	lambdaHalf[261] = -3.24662;
	lambdaHalf[262] = -3.32989;
	lambdaHalf[263] = -3.415;
	lambdaHalf[264] = -3.50063;
	lambdaHalf[265] = -3.58742;
	lambdaHalf[266] = -3.67547;
	lambdaHalf[267] = -3.76548;
	lambdaHalf[268] = -3.85603;
	lambdaHalf[269] = -3.9486;
	lambdaHalf[270] = -4.04097;
	lambdaHalf[271] = -4.13618;
	lambdaHalf[272] = -4.23278;
	lambdaHalf[273] = -4.32998;
	lambdaHalf[274] = -4.42856;
	lambdaHalf[275] = -4.52944;
	lambdaHalf[276] = -4.63092;
	lambdaHalf[277] = -4.73392;
	lambdaHalf[278] = -4.8393;
	lambdaHalf[279] = -4.9446;
	lambdaHalf[280] = -5.05316;
	lambdaHalf[281] = -5.16249;
	lambdaHalf[282] = -5.27259;
	lambdaHalf[283] = -5.38528;
	lambdaHalf[284] = -5.49983;
	lambdaHalf[285] = -5.61527;
	lambdaHalf[286] = -5.7324;
	lambdaHalf[287] = -5.85154;
	lambdaHalf[288] = -5.97358;
	lambdaHalf[289] = -6.0955;
	lambdaHalf[290] = -6.22077;
	lambdaHalf[291] = -6.34677;
	lambdaHalf[292] = -6.47508;
	lambdaHalf[293] = -6.6058;
	lambdaHalf[294] = -6.73859;
	lambdaHalf[295] = -6.87264;
	lambdaHalf[296] = -7.01016;
	lambdaHalf[297] = -7.14907;
	lambdaHalf[298] = -7.2908;
	lambdaHalf[299] = -7.43489;
	lambdaHalf[300] = -7.58055;
	lambdaHalf[301] = -7.72931;
	lambdaHalf[302] = -7.88065;
	lambdaHalf[303] = -8.03533;
	lambdaHalf[304] = -8.19192;
	lambdaHalf[305] = -8.35133;
	lambdaHalf[306] = -8.51279;
	lambdaHalf[307] = -8.67807;
	lambdaHalf[308] = -8.84738;
	lambdaHalf[309] = -9.01819;
	lambdaHalf[310] = -9.19414;
	lambdaHalf[311] = -9.37188;
	lambdaHalf[312] = -9.55514;
	lambdaHalf[313] = -9.74136;
	lambdaHalf[314] = -9.93066;
	lambdaHalf[315] = -10.1261;
	lambdaHalf[316] = -10.3229;
	lambdaHalf[317] = -10.5263;
	lambdaHalf[318] = -10.7349;
	lambdaHalf[319] = -10.9461;
	lambdaHalf[320] = -11.1639;
	lambdaHalf[321] = -11.3871;
	lambdaHalf[322] = -11.6176;
	lambdaHalf[323] = -11.8508;
	lambdaHalf[324] = -12.0919;
	lambdaHalf[325] = -12.3397;
	lambdaHalf[326] = -12.5964;
	lambdaHalf[327] = -12.857;
	lambdaHalf[328] = -13.1275;
	lambdaHalf[329] = -13.4065;
	lambdaHalf[330] = -13.6936;
	lambdaHalf[331] = -13.9927;
	lambdaHalf[332] = -14.2978;
	lambdaHalf[333] = -14.6174;
	lambdaHalf[334] = -14.9481;
	lambdaHalf[335] = -15.2906;
	lambdaHalf[336] = -15.647;
	lambdaHalf[337] = -16.0216;
	lambdaHalf[338] = -16.4074;
	lambdaHalf[339] = -16.8147;
	lambdaHalf[340] = -17.2417;
	lambdaHalf[341] = -17.6887;
	lambdaHalf[342] = -18.1596;
	lambdaHalf[343] = -18.6572;
	lambdaHalf[344] = -19.189;
	lambdaHalf[345] = -19.7486;
	lambdaHalf[346] = -20.3526;
	lambdaHalf[347] = -20.9928;
	lambdaHalf[348] = -21.692;
	lambdaHalf[349] = -22.4501;
	lambdaHalf[350] = -23.2805;
	lambdaHalf[351] = -24.1983;
	lambdaHalf[352] = -25.2224;
	lambdaHalf[353] = -26.3832;
	lambdaHalf[354] = -27.722;
	lambdaHalf[355] = -29.3039;
	lambdaHalf[356] = -31.2383;
	lambdaHalf[357] = -33.7634;
	lambdaHalf[358] = -37.2861;
	lambdaHalf[359] = -43.3071;
	lambdaHalf[360] = -255;
}

AntennaTest::~AntennaTest()
{
}


