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

#include <RISE/antenna/pattern/twodimensional/LambdaHalf.hpp>

using namespace rise::antenna::pattern::twodimensional;

LambdaHalf::LambdaHalf()
{
	pd_startUp();
}

LambdaHalf::~LambdaHalf()
{}

LambdaHalf* LambdaHalf::myself = NULL;

void LambdaHalf::pd_startUp ()
{
	type = "Internal";
	pattern = "Lambda_Half";
 	uint32_t horSize = 1;
 	uint32_t vertSize = 360;
	pd_elevationVector = Double64Vector(vertSize + 1);
	pd_azimuthVector = Double64Vector(horSize + 1);
 	pd_gain = 2.15;
	for (uint32_t i = 0; i <= horSize; i++)
	{
		pd_azimuthVector[i] = 0;
	}
	pd_elevationVector[0] =   -255;
	pd_elevationVector[1] =   -43.3071;
	pd_elevationVector[2] =   -37.2861;
	pd_elevationVector[3] =   -33.7634;
	pd_elevationVector[4] =   -31.2632;
	pd_elevationVector[5] =   -29.3039;
	pd_elevationVector[6] =   -27.722;
	pd_elevationVector[7] =   -26.3832;
	pd_elevationVector[8] =   -25.2224;
	pd_elevationVector[9] =   -24.1983;
	pd_elevationVector[10] =  -23.2805;
	pd_elevationVector[11] =  -22.4501;
	pd_elevationVector[12] =  -21.692;
	pd_elevationVector[13] =  -20.9928;
	pd_elevationVector[14] =  -20.3526;
	pd_elevationVector[15] =  -19.7486;
	pd_elevationVector[16] =  -19.189;
	pd_elevationVector[17] =  -18.6572;
	pd_elevationVector[18] =  -18.1596;
	pd_elevationVector[19] =  -17.6887;
	pd_elevationVector[20] =  -17.2417;
	pd_elevationVector[21] =  -16.8147;
	pd_elevationVector[22] =  -16.4074;
	pd_elevationVector[23] =  -16.0216;
	pd_elevationVector[24] =  -15.647;
	pd_elevationVector[25] =  -15.2906;
	pd_elevationVector[26] =  -14.9481;
	pd_elevationVector[27] =  -14.6174;
	pd_elevationVector[28] =  -14.2978;
	pd_elevationVector[29] =  -13.9927;
	pd_elevationVector[30] =  -13.6936;
	pd_elevationVector[31] =  -13.4065;
	pd_elevationVector[32] =  -13.1275;
	pd_elevationVector[33] =  -12.857;
	pd_elevationVector[34] =  -12.5964;
	pd_elevationVector[35] =  -12.3397;
	pd_elevationVector[36] =  -12.0919;
	pd_elevationVector[37] =  -11.8508;
	pd_elevationVector[38] =  -11.6176;
	pd_elevationVector[39] =  -11.3871;
	pd_elevationVector[40] =  -11.1639;
	pd_elevationVector[41] =  -10.9461;
	pd_elevationVector[42] =  -10.7349;
	pd_elevationVector[43] =  -10.5285;
	pd_elevationVector[44] =  -10.3229;
	pd_elevationVector[45] =  -10.1261;
	pd_elevationVector[46] =  -9.93152;
	pd_elevationVector[47] =  -9.74136;
	pd_elevationVector[48] =  -9.55514;
	pd_elevationVector[49] =  -9.37376;
	pd_elevationVector[50] =  -9.19414;
	pd_elevationVector[51] =  -9.01819;
	pd_elevationVector[52] =  -8.84738;
	pd_elevationVector[53] =  -8.67978;
	pd_elevationVector[54] =  -8.51446;
	pd_elevationVector[55] =  -8.35133;
	pd_elevationVector[56] =  -8.19192;
	pd_elevationVector[57] =  -8.03533;
	pd_elevationVector[58] =  -7.88065;
	pd_elevationVector[59] =  -7.72931;
	pd_elevationVector[60] =  -7.58201;
	pd_elevationVector[61] =  -7.43489;
	pd_elevationVector[62] =  -7.2908;
	pd_elevationVector[63] =  -7.15044;
	pd_elevationVector[64] =  -7.01016;
	pd_elevationVector[65] =  -6.87395;
	pd_elevationVector[66] =  -6.73859;
	pd_elevationVector[67] =  -6.6058;
	pd_elevationVector[68] =  -6.47631;
	pd_elevationVector[69] =  -6.34677;
	pd_elevationVector[70] =  -6.22077;
	pd_elevationVector[71] =  -6.0955;
	pd_elevationVector[72] =  -5.97358;
	pd_elevationVector[73] =  -5.85154;
	pd_elevationVector[74] =  -5.7324;
	pd_elevationVector[75] =  -5.61527;
	pd_elevationVector[76] =  -5.49983;
	pd_elevationVector[77] =  -5.38528;
	pd_elevationVector[78] =  -5.27259;
	pd_elevationVector[79] =  -5.16249;
	pd_elevationVector[80] =  -5.05316;
	pd_elevationVector[81] =  -4.9454;
	pd_elevationVector[82] =  -4.8393;
	pd_elevationVector[83] =  -4.73392;
	pd_elevationVector[84] =  -4.63092;
	pd_elevationVector[85] =  -4.52944;
	pd_elevationVector[86] =  -4.42939;
	pd_elevationVector[87] =  -4.32998;
	pd_elevationVector[88] =  -4.23278;
	pd_elevationVector[89] =  -4.13618;
	pd_elevationVector[90] =  -4.04173;
	pd_elevationVector[91] =  -3.9486;
	pd_elevationVector[92] =  -3.85603;
	pd_elevationVector[93] =  -3.76548;
	pd_elevationVector[94] =  -3.67621;
	pd_elevationVector[95] =  -3.58742;
	pd_elevationVector[96] =  -3.50063;
	pd_elevationVector[97] =  -3.415;
	pd_elevationVector[98] =  -3.32989;
	pd_elevationVector[99] =  -3.24662;
	pd_elevationVector[100] = -3.16445;
	pd_elevationVector[101] = -3.08348;
	pd_elevationVector[102] = -3.00426;
	pd_elevationVector[103] = -2.92552;
	pd_elevationVector[104] = -2.84779;
	pd_elevationVector[105] = -2.77176;
	pd_elevationVector[106] = -2.69687;
	pd_elevationVector[107] = -2.62293;
	pd_elevationVector[108] = -2.54946;
	pd_elevationVector[109] = -2.47826;
	pd_elevationVector[110] = -2.40731;
	pd_elevationVector[111] = -2.33813;
	pd_elevationVector[112] = -2.26919;
	pd_elevationVector[113] = -2.20241;
	pd_elevationVector[114] = -2.13607;
	pd_elevationVector[115] = -2.07121;
	pd_elevationVector[116] = -2.0068;
	pd_elevationVector[117] = -1.94385;
	pd_elevationVector[118] = -1.88173;
	pd_elevationVector[119] = -1.82066;
	pd_elevationVector[120] = -1.761;
	pd_elevationVector[121] = -1.70213;
	pd_elevationVector[122] = -1.64431;
	pd_elevationVector[123] = -1.58727;
	pd_elevationVector[124] = -1.53158;
	pd_elevationVector[125] = -1.47692;
	pd_elevationVector[126] = -1.42303;
	pd_elevationVector[127] = -1.37098;
	pd_elevationVector[128] = -1.31913;
	pd_elevationVector[129] = -1.26883;
	pd_elevationVector[130] = -1.21902;
	pd_elevationVector[131] = -1.1702;
	pd_elevationVector[132] = -1.1229;
	pd_elevationVector[133] = -1.07632;
	pd_elevationVector[134] = -1.03075;
	pd_elevationVector[135] = -0.98611;
	pd_elevationVector[136] = -0.94293;
	pd_elevationVector[137] = -0.90029;
	pd_elevationVector[138] = -0.858558;
	pd_elevationVector[139] = -0.818256;
	pd_elevationVector[140] = -0.778673;
	pd_elevationVector[141] = -0.740084;
	pd_elevationVector[142] = -0.70223;
	pd_elevationVector[143] = -0.665906;
	pd_elevationVector[144] = -0.630302;
	pd_elevationVector[145] = -0.595684;
	pd_elevationVector[146] = -0.561805;
	pd_elevationVector[147] = -0.529385;
	pd_elevationVector[148] = -0.497693;
	pd_elevationVector[149] = -0.466984;
	pd_elevationVector[150] = -0.43735;
	pd_elevationVector[151] = -0.408462;
	pd_elevationVector[152] = -0.380945;
	pd_elevationVector[153] = -0.354099;
	pd_elevationVector[154] = -0.328305;
	pd_elevationVector[155] = -0.303553;
	pd_elevationVector[156] = -0.279774;
	pd_elevationVector[157] = -0.256772;
	pd_elevationVector[158] = -0.235049;
	pd_elevationVector[159] = -0.214102;
	pd_elevationVector[160] = -0.194135;
	pd_elevationVector[161] = -0.17522;
	pd_elevationVector[162] = -0.157274;
	pd_elevationVector[163] = -0.140301;
	pd_elevationVector[164] = -0.124174;
	pd_elevationVector[165] = -0.109197;
	pd_elevationVector[166] = -0.0950497;
	pd_elevationVector[167] = -0.0820451;
	pd_elevationVector[168] = -0.069865;
	pd_elevationVector[169] = -0.0586888;
	pd_elevationVector[170] = -0.0484859;
	pd_elevationVector[171] = -0.0392576;
	pd_elevationVector[172] = -0.031017;
	pd_elevationVector[173] = -0.0237462;
	pd_elevationVector[174] = -0.0174494;
	pd_elevationVector[175] = -0.0121238;
	pd_elevationVector[176] = -0.00776691;
	pd_elevationVector[177] = -0.0043428;
	pd_elevationVector[178] = -0.00192983;
	pd_elevationVector[179] = -0.000482433;
	pd_elevationVector[180] = -0.0000;
	pd_elevationVector[181] = -0.000482433;
	pd_elevationVector[182] = -0.00192983;
	pd_elevationVector[183] = -0.0043428;
	pd_elevationVector[184] = -0.0077225;
	pd_elevationVector[185] = -0.0121238;
	pd_elevationVector[186] = -0.0174494;
	pd_elevationVector[187] = -0.0237462;
	pd_elevationVector[188] = -0.031017;
	pd_elevationVector[189] = -0.0392576;
	pd_elevationVector[190] = -0.0484859;
	pd_elevationVector[191] = -0.0586888;
	pd_elevationVector[192] = -0.069865;
	pd_elevationVector[193] = -0.0819014;
	pd_elevationVector[194] = -0.0950497;
	pd_elevationVector[195] = -0.109197;
	pd_elevationVector[196] = -0.124174;
	pd_elevationVector[197] = -0.140301;
	pd_elevationVector[198] = -0.157274;
	pd_elevationVector[199] = -0.17522;
	pd_elevationVector[200] = -0.194135;
	pd_elevationVector[201] = -0.214102;
	pd_elevationVector[202] = -0.234808;
	pd_elevationVector[203] = -0.256772;
	pd_elevationVector[204] = -0.279774;
	pd_elevationVector[205] = -0.303553;
	pd_elevationVector[206] = -0.328305;
	pd_elevationVector[207] = -0.354099;
	pd_elevationVector[208] = -0.380945;
	pd_elevationVector[209] = -0.408462;
	pd_elevationVector[210] = -0.43735;
	pd_elevationVector[211] = -0.466984;
	pd_elevationVector[212] = -0.497693;
	pd_elevationVector[213] = -0.529385;
	pd_elevationVector[214] = -0.561805;
	pd_elevationVector[215] = -0.595311;
	pd_elevationVector[216] = -0.630302;
	pd_elevationVector[217] = -0.665514;
	pd_elevationVector[218] = -0.70223;
	pd_elevationVector[219] = -0.739673;
	pd_elevationVector[220] = -0.778673;
	pd_elevationVector[221] = -0.817826;
	pd_elevationVector[222] = -0.858558;
	pd_elevationVector[223] = -0.899842;
	pd_elevationVector[224] = -0.942473;
	pd_elevationVector[225] = -0.98611;
	pd_elevationVector[226] = -1.03075;
	pd_elevationVector[227] = -1.07632;
	pd_elevationVector[228] = -1.1229;
	pd_elevationVector[229] = -1.1702;
	pd_elevationVector[230] = -1.21902;
	pd_elevationVector[231] = -1.26883;
	pd_elevationVector[232] = -1.31913;
	pd_elevationVector[233] = -1.37044;
	pd_elevationVector[234] = -1.42303;
	pd_elevationVector[235] = -1.47692;
	pd_elevationVector[236] = -1.53158;
	pd_elevationVector[237] = -1.58727;
	pd_elevationVector[238] = -1.64373;
	pd_elevationVector[239] = -1.70213;
	pd_elevationVector[240] = -1.76041;
	pd_elevationVector[241] = -1.82066;
	pd_elevationVector[242] = -1.88173;
	pd_elevationVector[243] = -1.94324;
	pd_elevationVector[244] = -2.0068;
	pd_elevationVector[245] = -2.07059;
	pd_elevationVector[246] = -2.13607;
	pd_elevationVector[247] = -2.20241;
	pd_elevationVector[248] = -2.26919;
	pd_elevationVector[249] = -2.33748;
	pd_elevationVector[250] = -2.40731;
	pd_elevationVector[251] = -2.4776;
	pd_elevationVector[252] = -2.54946;
	pd_elevationVector[253] = -2.62293;
	pd_elevationVector[254] = -2.69687;
	pd_elevationVector[255] = -2.77176;
	pd_elevationVector[256] = -2.84779;
	pd_elevationVector[257] = -2.92552;
	pd_elevationVector[258] = -3.00426;
	pd_elevationVector[259] = -3.08348;
	pd_elevationVector[260] = -3.16445;
	pd_elevationVector[261] = -3.24662;
	pd_elevationVector[262] = -3.32989;
	pd_elevationVector[263] = -3.415;
	pd_elevationVector[264] = -3.50063;
	pd_elevationVector[265] = -3.58742;
	pd_elevationVector[266] = -3.67547;
	pd_elevationVector[267] = -3.76548;
	pd_elevationVector[268] = -3.85603;
	pd_elevationVector[269] = -3.9486;
	pd_elevationVector[270] = -4.04097;
	pd_elevationVector[271] = -4.13618;
	pd_elevationVector[272] = -4.23278;
	pd_elevationVector[273] = -4.32998;
	pd_elevationVector[274] = -4.42856;
	pd_elevationVector[275] = -4.52944;
	pd_elevationVector[276] = -4.63092;
	pd_elevationVector[277] = -4.73392;
	pd_elevationVector[278] = -4.8393;
	pd_elevationVector[279] = -4.9446;
	pd_elevationVector[280] = -5.05316;
	pd_elevationVector[281] = -5.16249;
	pd_elevationVector[282] = -5.27259;
	pd_elevationVector[283] = -5.38528;
	pd_elevationVector[284] = -5.49983;
	pd_elevationVector[285] = -5.61527;
	pd_elevationVector[286] = -5.7324;
	pd_elevationVector[287] = -5.85154;
	pd_elevationVector[288] = -5.97358;
	pd_elevationVector[289] = -6.0955;
	pd_elevationVector[290] = -6.22077;
	pd_elevationVector[291] = -6.34677;
	pd_elevationVector[292] = -6.47508;
	pd_elevationVector[293] = -6.6058;
	pd_elevationVector[294] = -6.73859;
	pd_elevationVector[295] = -6.87264;
	pd_elevationVector[296] = -7.01016;
	pd_elevationVector[297] = -7.14907;
	pd_elevationVector[298] = -7.2908;
	pd_elevationVector[299] = -7.43489;
	pd_elevationVector[300] = -7.58055;
	pd_elevationVector[301] = -7.72931;
	pd_elevationVector[302] = -7.88065;
	pd_elevationVector[303] = -8.03533;
	pd_elevationVector[304] = -8.19192;
	pd_elevationVector[305] = -8.35133;
	pd_elevationVector[306] = -8.51279;
	pd_elevationVector[307] = -8.67807;
	pd_elevationVector[308] = -8.84738;
	pd_elevationVector[309] = -9.01819;
	pd_elevationVector[310] = -9.19414;
	pd_elevationVector[311] = -9.37188;
	pd_elevationVector[312] = -9.55514;
	pd_elevationVector[313] = -9.74136;
	pd_elevationVector[314] = -9.93066;
	pd_elevationVector[315] = -10.1261;
	pd_elevationVector[316] = -10.3229;
	pd_elevationVector[317] = -10.5263;
	pd_elevationVector[318] = -10.7349;
	pd_elevationVector[319] = -10.9461;
	pd_elevationVector[320] = -11.1639;
	pd_elevationVector[321] = -11.3871;
	pd_elevationVector[322] = -11.6176;
	pd_elevationVector[323] = -11.8508;
	pd_elevationVector[324] = -12.0919;
	pd_elevationVector[325] = -12.3397;
	pd_elevationVector[326] = -12.5964;
	pd_elevationVector[327] = -12.857;
	pd_elevationVector[328] = -13.1275;
	pd_elevationVector[329] = -13.4065;
	pd_elevationVector[330] = -13.6936;
	pd_elevationVector[331] = -13.9927;
	pd_elevationVector[332] = -14.2978;
	pd_elevationVector[333] = -14.6174;
	pd_elevationVector[334] = -14.9481;
	pd_elevationVector[335] = -15.2906;
	pd_elevationVector[336] = -15.647;
	pd_elevationVector[337] = -16.0216;
	pd_elevationVector[338] = -16.4074;
	pd_elevationVector[339] = -16.8147;
	pd_elevationVector[340] = -17.2417;
	pd_elevationVector[341] = -17.6887;
	pd_elevationVector[342] = -18.1596;
	pd_elevationVector[343] = -18.6572;
	pd_elevationVector[344] = -19.189;
	pd_elevationVector[345] = -19.7486;
	pd_elevationVector[346] = -20.3526;
	pd_elevationVector[347] = -20.9928;
	pd_elevationVector[348] = -21.692;
	pd_elevationVector[349] = -22.4501;
	pd_elevationVector[350] = -23.2805;
	pd_elevationVector[351] = -24.1983;
	pd_elevationVector[352] = -25.2224;
	pd_elevationVector[353] = -26.3832;
	pd_elevationVector[354] = -27.722;
	pd_elevationVector[355] = -29.3039;
	pd_elevationVector[356] = -31.2383;
	pd_elevationVector[357] = -33.7634;
	pd_elevationVector[358] = -37.2861;
	pd_elevationVector[359] = -43.3071;
	pd_elevationVector[360] = -255;
	TwoDimensional::pd_startUp();
}


