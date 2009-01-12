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

#include <RISE/antenna/pattern/twodimensional/Directed30.hpp>

using namespace rise::antenna::pattern::twodimensional;

//! Default constructor
Directed30::Directed30()
{
	pd_startUp();
}

//! Destructor
Directed30::~Directed30()
{}

Directed30* Directed30::myself = 0;

void Directed30::pd_startUp()
{
	type = "Internal";
	pattern = "Directed_30";
	uint32_t horSize = 360;
	uint32_t vertSize = 180;
	pd_elevationVector = Double64Vector(vertSize + 1);
	pd_azimuthVector = Double64Vector(horSize + 1);
	pd_gain = 13;
	pd_azimuthVector[0] = 0;
	pd_azimuthVector[1] = -0.0122937;
	pd_azimuthVector[2] = -0.04920067;
	pd_azimuthVector[3] = -0.1107844;
	pd_azimuthVector[4] = -0.1971586;
	pd_azimuthVector[5] = -0.3084809;
	pd_azimuthVector[6] = -0.4449612;
	pd_azimuthVector[7] = -0.6068594;
	pd_azimuthVector[8] = -0.7944959;
	pd_azimuthVector[9] = -1.00825;
	pd_azimuthVector[10] = -1.248572;
	pd_azimuthVector[11] = -1.515989;
	pd_azimuthVector[12] = -1.811112;
	pd_azimuthVector[13] = -2.134656;
	pd_azimuthVector[14] = -2.487442;
	pd_azimuthVector[15] = -2.870427;
	pd_azimuthVector[16] = -3.284719;
	pd_azimuthVector[17] = -3.731603;
	pd_azimuthVector[18] = -4.212579;
	pd_azimuthVector[19] = -4.729405;
	pd_azimuthVector[20] = -5.284142;
	pd_azimuthVector[21] = -5.879233;
	pd_azimuthVector[22] = -6.517591;
	pd_azimuthVector[23] = -7.202707;
	pd_azimuthVector[24] = -7.938824;
	pd_azimuthVector[25] = -8.731137;
	pd_azimuthVector[26] = -9.586109;
	pd_azimuthVector[27] = -10.51188;
	pd_azimuthVector[28] = -11.51892;
	pd_azimuthVector[29] = -12.62094;
	pd_azimuthVector[30] = -13.8364;
	pd_azimuthVector[31] = -15.19097;
	pd_azimuthVector[32] = -16.72175;
	pd_azimuthVector[33] = -18.4852;
	pd_azimuthVector[34] = -20.57352;
	pd_azimuthVector[35] = -23.15276;
	pd_azimuthVector[36] = -26.57141;
	pd_azimuthVector[37] = -31.79229;
	pd_azimuthVector[38] = -44.49251;
	pd_azimuthVector[39] = -38.15111;
	pd_azimuthVector[40] = -30.66576;
	pd_azimuthVector[41] = -26.97215;
	pd_azimuthVector[42] = -24.58771;
	pd_azimuthVector[43] = -22.87838;
	pd_azimuthVector[44] = -21.58416;
	pd_azimuthVector[45] = -20.57263;
	pd_azimuthVector[46] = -19.767;
	pd_azimuthVector[47] = -19.11874;
	pd_azimuthVector[48] = -18.59512;
	pd_azimuthVector[49] = -18.17301;
	pd_azimuthVector[50] = -17.83539;
	pd_azimuthVector[51] = -17.56938;
	pd_azimuthVector[52] = -17.36492;
	pd_azimuthVector[53] = -17.21402;
	pd_azimuthVector[54] = -17.11016;
	pd_azimuthVector[55] = -17.04796;
	pd_azimuthVector[56] = -17.02288;
	pd_azimuthVector[57] = -17.03104;
	pd_azimuthVector[58] = -17.06907;
	pd_azimuthVector[59] = -17.13403;
	pd_azimuthVector[60] = -17.22326;
	pd_azimuthVector[61] = -17.33438;
	pd_azimuthVector[62] = -17.4652;
	pd_azimuthVector[63] = -17.61371;
	pd_azimuthVector[64] = -17.77801;
	pd_azimuthVector[65] = -17.95628;
	pd_azimuthVector[66] = -18.14682;
	pd_azimuthVector[67] = -18.34793;
	pd_azimuthVector[68] = -18.55798;
	pd_azimuthVector[69] = -18.77535;
	pd_azimuthVector[70] = -18.99844;
	pd_azimuthVector[71] = -19.22564;
	pd_azimuthVector[72] = -19.45534;
	pd_azimuthVector[73] = -19.68589;
	pd_azimuthVector[74] = -19.91569;
	pd_azimuthVector[75] = -20.14307;
	pd_azimuthVector[76] = -20.36635;
	pd_azimuthVector[77] = -20.58388;
	pd_azimuthVector[78] = -20.79396;
	pd_azimuthVector[79] = -20.99494;
	pd_azimuthVector[80] = -21.18517;
	pd_azimuthVector[81] = -21.36303;
	pd_azimuthVector[82] = -21.52699;
	pd_azimuthVector[83] = -21.67557;
	pd_azimuthVector[84] = -21.80741;
	pd_azimuthVector[85] = -21.92125;
	pd_azimuthVector[86] = -22.01598;
	pd_azimuthVector[87] = -22.09068;
	pd_azimuthVector[88] = -22.14458;
	pd_azimuthVector[89] = -22.17715;
	pd_azimuthVector[90] = -22.18804;
	pd_azimuthVector[91] = -60;
	pd_azimuthVector[92] = -60;
	pd_azimuthVector[93] = -60;
	pd_azimuthVector[94] = -60;
	pd_azimuthVector[95] = -60;
	pd_azimuthVector[96] = -60;
	pd_azimuthVector[97] = -60;
	pd_azimuthVector[98] = -60;
	pd_azimuthVector[99] = -60;
	pd_azimuthVector[100] = -60;
	pd_azimuthVector[101] = -60;
	pd_azimuthVector[102] = -60;
	pd_azimuthVector[103] = -60;
	pd_azimuthVector[104] = -60;
	pd_azimuthVector[105] = -60;
	pd_azimuthVector[106] = -60;
	pd_azimuthVector[107] = -60;
	pd_azimuthVector[108] = -60;
	pd_azimuthVector[109] = -60;
	pd_azimuthVector[110] = -60;
	pd_azimuthVector[111] = -60;
	pd_azimuthVector[112] = -60;
	pd_azimuthVector[113] = -60;
	pd_azimuthVector[114] = -60;
	pd_azimuthVector[115] = -60;
	pd_azimuthVector[116] = -60;
	pd_azimuthVector[117] = -60;
	pd_azimuthVector[118] = -60;
	pd_azimuthVector[119] = -60;
	pd_azimuthVector[120] = -60;
	pd_azimuthVector[121] = -60;
	pd_azimuthVector[122] = -60;
	pd_azimuthVector[123] = -60;
	pd_azimuthVector[124] = -60;
	pd_azimuthVector[125] = -60;
	pd_azimuthVector[126] = -60;
	pd_azimuthVector[127] = -60;
	pd_azimuthVector[128] = -60;
	pd_azimuthVector[129] = -60;
	pd_azimuthVector[130] = -60;
	pd_azimuthVector[131] = -60;
	pd_azimuthVector[132] = -60;
	pd_azimuthVector[133] = -60;
	pd_azimuthVector[134] = -60;
	pd_azimuthVector[135] = -60;
	pd_azimuthVector[136] = -60;
	pd_azimuthVector[137] = -60;
	pd_azimuthVector[138] = -60;
	pd_azimuthVector[139] = -60;
	pd_azimuthVector[140] = -60;
	pd_azimuthVector[141] = -60;
	pd_azimuthVector[142] = -60;
	pd_azimuthVector[143] = -60;
	pd_azimuthVector[144] = -60;
	pd_azimuthVector[145] = -60;
	pd_azimuthVector[146] = -60;
	pd_azimuthVector[147] = -60;
	pd_azimuthVector[148] = -60;
	pd_azimuthVector[149] = -60;
	pd_azimuthVector[150] = -60;
	pd_azimuthVector[151] = -60;
	pd_azimuthVector[152] = -60;
	pd_azimuthVector[153] = -60;
	pd_azimuthVector[154] = -60;
	pd_azimuthVector[155] = -60;
	pd_azimuthVector[156] = -60;
	pd_azimuthVector[157] = -60;
	pd_azimuthVector[158] = -60;
	pd_azimuthVector[159] = -60;
	pd_azimuthVector[160] = -60;
	pd_azimuthVector[161] = -60;
	pd_azimuthVector[162] = -60;
	pd_azimuthVector[163] = -60;
	pd_azimuthVector[164] = -60;
	pd_azimuthVector[165] = -60;
	pd_azimuthVector[166] = -60;
	pd_azimuthVector[167] = -60;
	pd_azimuthVector[168] = -60;
	pd_azimuthVector[169] = -60;
	pd_azimuthVector[170] = -60;
	pd_azimuthVector[171] = -60;
	pd_azimuthVector[172] = -60;
	pd_azimuthVector[173] = -60;
	pd_azimuthVector[174] = -60;
	pd_azimuthVector[175] = -60;
	pd_azimuthVector[176] = -60;
	pd_azimuthVector[177] = -60;
	pd_azimuthVector[178] = -60;
	pd_azimuthVector[179] = -60;
	pd_azimuthVector[180] = -60;
	pd_azimuthVector[181] = -60;
	pd_azimuthVector[182] = -60;
	pd_azimuthVector[183] = -60;
	pd_azimuthVector[184] = -60;
	pd_azimuthVector[185] = -60;
	pd_azimuthVector[186] = -60;
	pd_azimuthVector[187] = -60;
	pd_azimuthVector[188] = -60;
	pd_azimuthVector[189] = -60;
	pd_azimuthVector[190] = -60;
	pd_azimuthVector[191] = -60;
	pd_azimuthVector[192] = -60;
	pd_azimuthVector[193] = -60;
	pd_azimuthVector[194] = -60;
	pd_azimuthVector[195] = -60;
	pd_azimuthVector[196] = -60;
	pd_azimuthVector[197] = -60;
	pd_azimuthVector[198] = -60;
	pd_azimuthVector[199] = -60;
	pd_azimuthVector[200] = -60;
	pd_azimuthVector[201] = -60;
	pd_azimuthVector[202] = -60;
	pd_azimuthVector[203] = -60;
	pd_azimuthVector[204] = -60;
	pd_azimuthVector[205] = -60;
	pd_azimuthVector[206] = -60;
	pd_azimuthVector[207] = -60;
	pd_azimuthVector[208] = -60;
	pd_azimuthVector[209] = -60;
	pd_azimuthVector[210] = -60;
	pd_azimuthVector[211] = -60;
	pd_azimuthVector[212] = -60;
	pd_azimuthVector[213] = -60;
	pd_azimuthVector[214] = -60;
	pd_azimuthVector[215] = -60;
	pd_azimuthVector[216] = -60;
	pd_azimuthVector[217] = -60;
	pd_azimuthVector[218] = -60;
	pd_azimuthVector[219] = -60;
	pd_azimuthVector[220] = -60;
	pd_azimuthVector[221] = -60;
	pd_azimuthVector[222] = -60;
	pd_azimuthVector[223] = -60;
	pd_azimuthVector[224] = -60;
	pd_azimuthVector[225] = -60;
	pd_azimuthVector[226] = -60;
	pd_azimuthVector[227] = -60;
	pd_azimuthVector[228] = -60;
	pd_azimuthVector[229] = -60;
	pd_azimuthVector[230] = -60;
	pd_azimuthVector[231] = -60;
	pd_azimuthVector[232] = -60;
	pd_azimuthVector[233] = -60;
	pd_azimuthVector[234] = -60;
	pd_azimuthVector[235] = -60;
	pd_azimuthVector[236] = -60;
	pd_azimuthVector[237] = -60;
	pd_azimuthVector[238] = -60;
	pd_azimuthVector[239] = -60;
	pd_azimuthVector[240] = -60;
	pd_azimuthVector[241] = -60;
	pd_azimuthVector[242] = -60;
	pd_azimuthVector[243] = -60;
	pd_azimuthVector[244] = -60;
	pd_azimuthVector[245] = -60;
	pd_azimuthVector[246] = -60;
	pd_azimuthVector[247] = -60;
	pd_azimuthVector[248] = -60;
	pd_azimuthVector[249] = -60;
	pd_azimuthVector[250] = -60;
	pd_azimuthVector[251] = -60;
	pd_azimuthVector[252] = -60;
	pd_azimuthVector[253] = -60;
	pd_azimuthVector[254] = -60;
	pd_azimuthVector[255] = -60;
	pd_azimuthVector[256] = -60;
	pd_azimuthVector[257] = -60;
	pd_azimuthVector[258] = -60;
	pd_azimuthVector[259] = -60;
	pd_azimuthVector[260] = -60;
	pd_azimuthVector[261] = -60;
	pd_azimuthVector[262] = -60;
	pd_azimuthVector[263] = -60;
	pd_azimuthVector[264] = -60;
	pd_azimuthVector[265] = -60;
	pd_azimuthVector[266] = -60;
	pd_azimuthVector[267] = -60;
	pd_azimuthVector[268] = -60;
	pd_azimuthVector[269] = -60;
	pd_azimuthVector[270] = -22.18804;
	pd_azimuthVector[271] = -22.17715;
	pd_azimuthVector[272] = -22.14458;
	pd_azimuthVector[273] = -22.09068;
	pd_azimuthVector[274] = -22.01598;
	pd_azimuthVector[275] = -21.92125;
	pd_azimuthVector[276] = -21.80741;
	pd_azimuthVector[277] = -21.67557;
	pd_azimuthVector[278] = -21.52699;
	pd_azimuthVector[279] = -21.36303;
	pd_azimuthVector[280] = -21.18517;
	pd_azimuthVector[281] = -20.99494;
	pd_azimuthVector[282] = -20.79396;
	pd_azimuthVector[283] = -20.58388;
	pd_azimuthVector[284] = -20.36635;
	pd_azimuthVector[285] = -20.14307;
	pd_azimuthVector[286] = -19.91569;
	pd_azimuthVector[287] = -19.68589;
	pd_azimuthVector[288] = -19.45534;
	pd_azimuthVector[289] = -19.22564;
	pd_azimuthVector[290] = -18.99844;
	pd_azimuthVector[291] = -18.77535;
	pd_azimuthVector[292] = -18.55798;
	pd_azimuthVector[293] = -18.34793;
	pd_azimuthVector[294] = -18.14682;
	pd_azimuthVector[295] = -17.95628;
	pd_azimuthVector[296] = -17.77801;
	pd_azimuthVector[297] = -17.61371;
	pd_azimuthVector[298] = -17.4652;
	pd_azimuthVector[299] = -17.33438;
	pd_azimuthVector[300] = -17.22326;
	pd_azimuthVector[301] = -17.13403;
	pd_azimuthVector[302] = -17.06907;
	pd_azimuthVector[303] = -17.03104;
	pd_azimuthVector[304] = -17.02288;
	pd_azimuthVector[305] = -17.04796;
	pd_azimuthVector[306] = -17.11016;
	pd_azimuthVector[307] = -17.21402;
	pd_azimuthVector[308] = -17.36492;
	pd_azimuthVector[309] = -17.56938;
	pd_azimuthVector[310] = -17.83539;
	pd_azimuthVector[311] = -18.17301;
	pd_azimuthVector[312] = -18.59512;
	pd_azimuthVector[313] = -19.11874;
	pd_azimuthVector[314] = -19.767;
	pd_azimuthVector[315] = -20.57263;
	pd_azimuthVector[316] = -21.58416;
	pd_azimuthVector[317] = -22.87838;
	pd_azimuthVector[318] = -24.58771;
	pd_azimuthVector[319] = -26.97215;
	pd_azimuthVector[320] = -30.66576;
	pd_azimuthVector[321] = -38.15111;
	pd_azimuthVector[322] = -44.49251;
	pd_azimuthVector[323] = -31.79229;
	pd_azimuthVector[324] = -26.57141;
	pd_azimuthVector[325] = -23.15276;
	pd_azimuthVector[326] = -20.57352;
	pd_azimuthVector[327] = -18.4852;
	pd_azimuthVector[328] = -16.72175;
	pd_azimuthVector[329] = -15.19097;
	pd_azimuthVector[330] = -13.8364;
	pd_azimuthVector[331] = -12.62094;
	pd_azimuthVector[332] = -11.51892;
	pd_azimuthVector[333] = -10.51188;
	pd_azimuthVector[334] = -9.586109;
	pd_azimuthVector[335] = -8.731137;
	pd_azimuthVector[336] = -7.938824;
	pd_azimuthVector[337] = -7.202707;
	pd_azimuthVector[338] = -6.517591;
	pd_azimuthVector[339] = -5.879233;
	pd_azimuthVector[340] = -5.284142;
	pd_azimuthVector[341] = -4.729405;
	pd_azimuthVector[342] = -4.212579;
	pd_azimuthVector[343] = -3.731603;
	pd_azimuthVector[344] = -3.284719;
	pd_azimuthVector[345] = -2.870427;
	pd_azimuthVector[346] = -2.487442;
	pd_azimuthVector[347] = -2.134656;
	pd_azimuthVector[348] = -1.811112;
	pd_azimuthVector[349] = -1.515989;
	pd_azimuthVector[350] = -1.248572;
	pd_azimuthVector[351] = -1.00825;
	pd_azimuthVector[352] = -0.7944959;
	pd_azimuthVector[353] = -0.6068594;
	pd_azimuthVector[354] = -0.4449612;
	pd_azimuthVector[355] = -0.3084809;
	pd_azimuthVector[356] = -0.1971586;
	pd_azimuthVector[357] = -0.1107844;
	pd_azimuthVector[358] = -0.04920067;
	pd_azimuthVector[359] = -0.0122937;
	pd_azimuthVector[360] = 0;
	pd_elevationVector[180] =  -27.86;
	pd_elevationVector[179] =  -27.35;
	pd_elevationVector[178] =  -27.11;
	pd_elevationVector[177] =  -26.87;
	pd_elevationVector[176] =  -26.64;
	pd_elevationVector[175] =  -26.41;
	pd_elevationVector[174] =  -25.98;
	pd_elevationVector[173] =  -25.56;
	pd_elevationVector[172] =  -25.17;
	pd_elevationVector[171] =  -24.79;
	pd_elevationVector[170] =  -24.43;
	pd_elevationVector[169] =  -24.08;
	pd_elevationVector[168] =  -23.75;
	pd_elevationVector[167] =  -23.43;
	pd_elevationVector[166] =  -23.12;
	pd_elevationVector[165] =  -22.82;
	pd_elevationVector[164] =  -22.39;
	pd_elevationVector[163] =  -22.25;
	pd_elevationVector[162] =  -21.84;
	pd_elevationVector[161] =  -21.46;
	pd_elevationVector[160] =  -21.09;
	pd_elevationVector[159] =  -20.62;
	pd_elevationVector[158] =  -20.28;
	pd_elevationVector[157] =  -19.96;
	pd_elevationVector[156] =  -19.75;
	pd_elevationVector[155] =  -19.34;
	pd_elevationVector[154] =  -19.15;
	pd_elevationVector[153] =  -18.77;
	pd_elevationVector[152] =  -18.41;
	pd_elevationVector[151] =  -18.06;
	pd_elevationVector[150] =  -17.73;
	pd_elevationVector[149] =  -17.41;
	pd_elevationVector[148] =  -17.10;
	pd_elevationVector[147] =  -16.94;
	pd_elevationVector[146] =  -16.58;
	pd_elevationVector[145] =  -16.44;
	pd_elevationVector[144] =  -16.37;
	pd_elevationVector[143] =  -16.23;
	pd_elevationVector[142] =  -16.16;
	pd_elevationVector[141] =  -16.09;
	pd_elevationVector[140] =  -16.02;
	pd_elevationVector[139] =  -16.02;
	pd_elevationVector[138] =  -16.09;   
	pd_elevationVector[137] =  -16.23;
	pd_elevationVector[136] =  -16.30;
	pd_elevationVector[135] =  -16.44;
	pd_elevationVector[134] =  -16.65;
	pd_elevationVector[133] =  -17.02;
	pd_elevationVector[132] =  -17.65;
	pd_elevationVector[131] =  -21.21;
	pd_elevationVector[130] =  -22.25;
	pd_elevationVector[129] =  -23.75;
	pd_elevationVector[128] =  -25.56;
	pd_elevationVector[127] =  -28.41;
	pd_elevationVector[126] =  -29.61;
	pd_elevationVector[125] =  -31.00;
	pd_elevationVector[124] =  -31.00;
	pd_elevationVector[123] =  -29.61;
	pd_elevationVector[122] =  -28.41;
	pd_elevationVector[121] =  -27.35;
	pd_elevationVector[120] =  -24.43;
	pd_elevationVector[119] =  -21.98;
	pd_elevationVector[118] =  -20.06;
	pd_elevationVector[117] =  -17.41;
	pd_elevationVector[116] =  -15.44;
	pd_elevationVector[115] =  -13.83;
	pd_elevationVector[114] =  -12.39;
	pd_elevationVector[113] =  -10.78;
	pd_elevationVector[112] =  -9.77;
	pd_elevationVector[111] =  -8.69;
	pd_elevationVector[110] =  -7.63;
	pd_elevationVector[109] =  -6.73;
	pd_elevationVector[108] =  -5.94;
	pd_elevationVector[107] =  -5.29;
	pd_elevationVector[106] =  -4.65;
	pd_elevationVector[105] =  -4.01;
	pd_elevationVector[104] =  -3.44;
	pd_elevationVector[103] =  -2.98;
	pd_elevationVector[102] =  -2.54;
	pd_elevationVector[101] =  -2.11;
	pd_elevationVector[100] =  -1.75;
	pd_elevationVector[99] =   -1.42;
	pd_elevationVector[98] =   -1.14;
	pd_elevationVector[97] =   -0.87;
	pd_elevationVector[96] =   -0.63;
	pd_elevationVector[95] =   -0.44;
	pd_elevationVector[94] =   -0.29;
	pd_elevationVector[93] =   -0.18;
	pd_elevationVector[92] =   -0.09;
	pd_elevationVector[91] =   -0.03;
	pd_elevationVector[90] =   -0.00;
	pd_elevationVector[0] =  -27.86;
	pd_elevationVector[1] =  -27.35;
	pd_elevationVector[2] =  -27.11;
	pd_elevationVector[3] =  -26.87;
	pd_elevationVector[4] =  -26.64;
	pd_elevationVector[5] =  -26.41;
	pd_elevationVector[6] =  -25.98;
	pd_elevationVector[7] =  -25.56;
	pd_elevationVector[8] =  -25.17;
	pd_elevationVector[9] =  -24.79;
	pd_elevationVector[10] =  -24.43;
	pd_elevationVector[11] =  -24.08;
	pd_elevationVector[12] =  -23.75;
	pd_elevationVector[13] =  -23.43;
	pd_elevationVector[14] =  -23.12;
	pd_elevationVector[15] =  -22.82;
	pd_elevationVector[16] =  -22.39;
	pd_elevationVector[17] =  -22.25;
	pd_elevationVector[18] =  -21.84;
	pd_elevationVector[19] =  -21.46;
	pd_elevationVector[20] =  -21.09;
	pd_elevationVector[21] =  -20.62;
	pd_elevationVector[22] =  -20.28;
	pd_elevationVector[23] =  -19.96;
	pd_elevationVector[24] =  -19.75;
	pd_elevationVector[25] =  -19.34;
	pd_elevationVector[26] =  -19.15;
	pd_elevationVector[27] =  -18.77;
	pd_elevationVector[28] =  -18.41;
	pd_elevationVector[29] =  -18.06;
	pd_elevationVector[30] =  -17.73;
	pd_elevationVector[31] =  -17.41;
	pd_elevationVector[32] =  -17.10;
	pd_elevationVector[33] =  -16.94;
	pd_elevationVector[34] =  -16.58;
	pd_elevationVector[35] =  -16.44;
	pd_elevationVector[36] =  -16.37;
	pd_elevationVector[37] =  -16.23;
	pd_elevationVector[38] =  -16.16;
	pd_elevationVector[39] =  -16.09;
	pd_elevationVector[40] =  -16.02;
	pd_elevationVector[41] =  -16.02;
	pd_elevationVector[42] =  -16.09;   
	pd_elevationVector[43] =  -16.23;
	pd_elevationVector[44] =  -16.30;
	pd_elevationVector[45] =  -16.44;
	pd_elevationVector[46] =  -16.65;
	pd_elevationVector[47] =  -17.02;
	pd_elevationVector[48] =  -17.65;
	pd_elevationVector[49] =  -21.21;
	pd_elevationVector[50] =  -22.25;
	pd_elevationVector[51] =  -23.75;
	pd_elevationVector[52] =  -25.56;
	pd_elevationVector[53] =  -28.41;
	pd_elevationVector[54] =  -29.61;
	pd_elevationVector[55] =  -31.00;
	pd_elevationVector[56] =  -31.00;
	pd_elevationVector[57] =  -29.61;
	pd_elevationVector[58] =  -28.41;
	pd_elevationVector[59] =  -27.35;
	pd_elevationVector[60] =  -24.43;
	pd_elevationVector[61] =  -21.98;
	pd_elevationVector[62] =  -20.06;
	pd_elevationVector[63] =  -17.41;
	pd_elevationVector[64] =  -15.44;
	pd_elevationVector[65] =  -13.83;
	pd_elevationVector[66] =  -12.39;
	pd_elevationVector[67] =  -10.78;
	pd_elevationVector[68] =  -9.77;
	pd_elevationVector[69] =  -8.69;
	pd_elevationVector[70] =  -7.63;
	pd_elevationVector[71] =  -6.73;
	pd_elevationVector[72] =  -5.94;
	pd_elevationVector[73] =  -5.29;
	pd_elevationVector[74] =  -4.65;
	pd_elevationVector[75] =  -4.01;
	pd_elevationVector[76] =  -3.44;
	pd_elevationVector[77] =  -2.98;
	pd_elevationVector[78] =  -2.54;
	pd_elevationVector[79] =  -2.11;
	pd_elevationVector[80] =  -1.75;
	pd_elevationVector[81] =   -1.42;
	pd_elevationVector[82] =   -1.14;
	pd_elevationVector[83] =   -0.87;
	pd_elevationVector[84] =   -0.63;
	pd_elevationVector[85] =   -0.44;
	pd_elevationVector[86] =   -0.29;
	pd_elevationVector[87] =   -0.18;
	pd_elevationVector[88] =   -0.09;
	pd_elevationVector[89] =   -0.03;
	TwoDimensional::pd_startUp();
}
