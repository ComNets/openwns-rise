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

#include <RISE/antenna/pattern/twodimensional/Directed90.hpp>

using namespace rise::antenna::pattern::twodimensional;

Directed90::Directed90()
{
	pd_startUp();
}

Directed90::~Directed90()
{}

Directed90* Directed90::myself = 0;

void Directed90::pd_startUp ()
{
	type = "Internal";
	pattern = "Directed_90";
	uint32_t horSize = 720;
	uint32_t vertSize = 1;
	pd_elevationVector = Double64Vector(vertSize + 1);
	pd_azimuthVector = Double64Vector(horSize + 1);
	pd_gain = 8.01;
	pd_azimuthVector[0] = 0;
	pd_azimuthVector[1] = -0.000368;
	pd_azimuthVector[2] = -0.00147;
	pd_azimuthVector[3] = -0.00332;
	pd_azimuthVector[4] = -0.0059;
	pd_azimuthVector[5] = -0.00922;
	pd_azimuthVector[6] = -0.0133;
	pd_azimuthVector[7] = -0.018065;
	pd_azimuthVector[8] = -0.0236;
	pd_azimuthVector[9] = -0.0299;
	pd_azimuthVector[10] = -0.0369;
	pd_azimuthVector[11] = -0.0446;
	pd_azimuthVector[12] = -0.0530822;
	pd_azimuthVector[13] = -0.0622953;
	pd_azimuthVector[14] = -0.0722;
	pd_azimuthVector[15] = -0.0829;
	pd_azimuthVector[16] = -0.0944;
	pd_azimuthVector[17] = -0.1065069;
	pd_azimuthVector[18] = -0.1193989;
	pd_azimuthVector[19] = -0.1330259;
	pd_azimuthVector[20] = -0.1473877;
	pd_azimuthVector[21] = -0.162484;
	pd_azimuthVector[22] = -0.1783135;
	pd_azimuthVector[23] = -0.1948778;
	pd_azimuthVector[24] = -0.2121746;
	pd_azimuthVector[25] = -0.2302061;
	pd_azimuthVector[26] = -0.2489691;
	pd_azimuthVector[27] = -0.2684641;
	pd_azimuthVector[28] = -0.2886924;
	pd_azimuthVector[29] = -0.309652;
	pd_azimuthVector[30] = -0.331342;
	pd_azimuthVector[31] = -0.3537635;
	pd_azimuthVector[32] = -0.3769142;
	pd_azimuthVector[33] = -0.4007962;
	pd_azimuthVector[34] = -0.4254063;
	pd_azimuthVector[35] = -0.4507453;
	pd_azimuthVector[36] = -0.4768129;
	pd_azimuthVector[37] = -0.5036084;
	pd_azimuthVector[38] = -0.5311305;
	pd_azimuthVector[39] = -0.5593795;
	pd_azimuthVector[40] = -0.5883544;
	pd_azimuthVector[41] = -0.6180558;
	pd_azimuthVector[42] = -0.6484804;
	pd_azimuthVector[43] = -0.6796299;
	pd_azimuthVector[44] = -0.7115031;
	pd_azimuthVector[45] = -0.7440991;
	pd_azimuthVector[46] = -0.7774172;
	pd_azimuthVector[47] = -0.8114575;
	pd_azimuthVector[48] = -0.8462184;
	pd_azimuthVector[49] = -0.8816986;
	pd_azimuthVector[50] = -0.9178993;
	pd_azimuthVector[51] = -0.9548182;
	pd_azimuthVector[52] = -0.9924549;
	pd_azimuthVector[53] = -1.030808;
	pd_azimuthVector[54] = -1.069879;
	pd_azimuthVector[55] = -1.109665;
	pd_azimuthVector[56] = -1.150165;
	pd_azimuthVector[57] = -1.191379;
	pd_azimuthVector[58] = -1.233307;
	pd_azimuthVector[59] = -1.275946;
	pd_azimuthVector[60] = -1.319296;
	pd_azimuthVector[61] = -1.363357;
	pd_azimuthVector[62] = -1.408128;
	pd_azimuthVector[63] = -1.453607;
	pd_azimuthVector[64] = -1.499793;
	pd_azimuthVector[65] = -1.546686;
	pd_azimuthVector[66] = -1.594285;
	pd_azimuthVector[67] = -1.642587;
	pd_azimuthVector[68] = -1.691594;
	pd_azimuthVector[69] = -1.741303;
	pd_azimuthVector[70] = -1.791713;
	pd_azimuthVector[71] = -1.842823;
	pd_azimuthVector[72] = -1.894634;
	pd_azimuthVector[73] = -1.947142;
	pd_azimuthVector[74] = -2.000348;
	pd_azimuthVector[75] = -2.054248;
	pd_azimuthVector[76] = -2.108845;
	pd_azimuthVector[77] = -2.164135;
	pd_azimuthVector[78] = -2.220118;
	pd_azimuthVector[79] = -2.276792;
	pd_azimuthVector[80] = -2.334156;
	pd_azimuthVector[81] = -2.392209;
	pd_azimuthVector[82] = -2.45095;
	pd_azimuthVector[83] = -2.510376;
	pd_azimuthVector[84] = -2.570488;
	pd_azimuthVector[85] = -2.631285;
	pd_azimuthVector[86] = -2.692763;
	pd_azimuthVector[87] = -2.754922;
	pd_azimuthVector[88] = -2.817762;
	pd_azimuthVector[89] = -2.88128;
	pd_azimuthVector[90] = -2.945475;
	pd_azimuthVector[91] = -3.010346;
	pd_azimuthVector[92] = -3.075891;
	pd_azimuthVector[93] = -3.14211;
	pd_azimuthVector[94] = -3.209;
	pd_azimuthVector[95] = -3.27656;
	pd_azimuthVector[96] = -3.344789;
	pd_azimuthVector[97] = -3.413683;
	pd_azimuthVector[98] = -3.483245;
	pd_azimuthVector[99] = -3.553471;
	pd_azimuthVector[100] = -3.624358;
	pd_azimuthVector[101] = -3.695907;
	pd_azimuthVector[102] = -3.768115;
	pd_azimuthVector[103] = -3.840981;
	pd_azimuthVector[104] = -3.914504;
	pd_azimuthVector[105] = -3.98868;
	pd_azimuthVector[106] = -4.06351;
	pd_azimuthVector[107] = -4.138991;
	pd_azimuthVector[108] = -4.215122;
	pd_azimuthVector[109] = -4.291901;
	pd_azimuthVector[110] = -4.369325;
	pd_azimuthVector[111] = -4.447395;
	pd_azimuthVector[112] = -4.526109;
	pd_azimuthVector[113] = -4.605462;
	pd_azimuthVector[114] = -4.685455;
	pd_azimuthVector[115] = -4.766084;
	pd_azimuthVector[116] = -4.847351;
	pd_azimuthVector[117] = -4.92925;
	pd_azimuthVector[118] = -5.011782;
	pd_azimuthVector[119] = -5.094944;
	pd_azimuthVector[120] = -5.178733;
	pd_azimuthVector[121] = -5.263149;
	pd_azimuthVector[122] = -5.34819;
	pd_azimuthVector[123] = -5.433855;
	pd_azimuthVector[124] = -5.520137;
	pd_azimuthVector[125] = -5.60704;
	pd_azimuthVector[126] = -5.694557;
	pd_azimuthVector[127] = -5.782692;
	pd_azimuthVector[128] = -5.871437;
	pd_azimuthVector[129] = -5.960793;
	pd_azimuthVector[130] = -6.050758;
	pd_azimuthVector[131] = -6.14133;
	pd_azimuthVector[132] = -6.232504;
	pd_azimuthVector[133] = -6.324282;
	pd_azimuthVector[134] = -6.416661;
	pd_azimuthVector[135] = -6.509634;
	pd_azimuthVector[136] = -6.603205;
	pd_azimuthVector[137] = -6.697368;
	pd_azimuthVector[138] = -6.792123;
	pd_azimuthVector[139] = -6.887466;
	pd_azimuthVector[140] = -6.983397;
	pd_azimuthVector[141] = -7.079912;
	pd_azimuthVector[142] = -7.177009;
	pd_azimuthVector[143] = -7.274685;
	pd_azimuthVector[144] = -7.372939;
	pd_azimuthVector[145] = -7.471767;
	pd_azimuthVector[146] = -7.571167;
	pd_azimuthVector[147] = -7.671135;
	pd_azimuthVector[148] = -7.771674;
	pd_azimuthVector[149] = -7.872777;
	pd_azimuthVector[150] = -7.974444;
	pd_azimuthVector[151] = -8.07667;
	pd_azimuthVector[152] = -8.179451;
	pd_azimuthVector[153] = -8.282788;
	pd_azimuthVector[154] = -8.386681;
	pd_azimuthVector[155] = -8.491117;
	pd_azimuthVector[156] = -8.596103;
	pd_azimuthVector[157] = -8.701633;
	pd_azimuthVector[158] = -8.807705;
	pd_azimuthVector[159] = -8.914314;
	pd_azimuthVector[160] = -9.021459;
	pd_azimuthVector[161] = -9.129137;
	pd_azimuthVector[162] = -9.237345;
	pd_azimuthVector[163] = -9.346079;
	pd_azimuthVector[164] = -9.455337;
	pd_azimuthVector[165] = -9.565116;
	pd_azimuthVector[166] = -9.675415;
	pd_azimuthVector[167] = -9.786225;
	pd_azimuthVector[168] = -9.897549;
	pd_azimuthVector[169] = -10.00938;
	pd_azimuthVector[170] = -10.12172;
	pd_azimuthVector[171] = -10.23456;
	pd_azimuthVector[172] = -10.34789;
	pd_azimuthVector[173] = -10.46173;
	pd_azimuthVector[174] = -10.57605;
	pd_azimuthVector[175] = -10.69086;
	pd_azimuthVector[176] = -10.80616;
	pd_azimuthVector[177] = -10.92194;
	pd_azimuthVector[178] = -11.03819;
	pd_azimuthVector[179] = -11.15492;
	pd_azimuthVector[180] = -11.27212;
	pd_azimuthVector[181] = -11.38978;
	pd_azimuthVector[182] = -11.50791;
	pd_azimuthVector[183] = -11.62649;
	pd_azimuthVector[184] = -11.74553;
	pd_azimuthVector[185] = -11.86501;
	pd_azimuthVector[186] = -11.98495;
	pd_azimuthVector[187] = -12.10532;
	pd_azimuthVector[188] = -12.22613;
	pd_azimuthVector[189] = -12.34737;
	pd_azimuthVector[190] = -12.46903;
	pd_azimuthVector[191] = -12.59112;
	pd_azimuthVector[192] = -12.71363;
	pd_azimuthVector[193] = -12.83655;
	pd_azimuthVector[194] = -12.95987;
	pd_azimuthVector[195] = -13.0836;
	pd_azimuthVector[196] = -13.20772;
	pd_azimuthVector[197] = -13.33224;
	pd_azimuthVector[198] = -13.45714;
	pd_azimuthVector[199] = -13.58242;
	pd_azimuthVector[200] = -13.70807;
	pd_azimuthVector[201] = -13.8341;
	pd_azimuthVector[202] = -13.96048;
	pd_azimuthVector[203] = -14.08721;
	pd_azimuthVector[204] = -14.2143;
	pd_azimuthVector[205] = -14.34173;
	pd_azimuthVector[206] = -14.46949;
	pd_azimuthVector[207] = -14.59758;
	pd_azimuthVector[208] = -14.72599;
	pd_azimuthVector[209] = -14.85471;
	pd_azimuthVector[210] = -14.98373;
	pd_azimuthVector[211] = -15.11305;
	pd_azimuthVector[212] = -15.24266;
	pd_azimuthVector[213] = -15.37255;
	pd_azimuthVector[214] = -15.50271;
	pd_azimuthVector[215] = -15.63313;
	pd_azimuthVector[216] = -15.76381;
	pd_azimuthVector[217] = -15.89472;
	pd_azimuthVector[218] = -16.02587;
	pd_azimuthVector[219] = -16.15724;
	pd_azimuthVector[220] = -16.28883;
	pd_azimuthVector[221] = -16.42061;
	pd_azimuthVector[222] = -16.55258;
	pd_azimuthVector[223] = -16.68473;
	pd_azimuthVector[224] = -16.81706;
	pd_azimuthVector[225] = -16.94953;
	pd_azimuthVector[226] = -17.08215;
	pd_azimuthVector[227] = -17.21489;
	pd_azimuthVector[228] = -17.34776;
	pd_azimuthVector[229] = -17.48071;
	pd_azimuthVector[230] = -17.61376;
	pd_azimuthVector[231] = -17.74688;
	pd_azimuthVector[232] = -17.88006;
	pd_azimuthVector[233] = -18.01328;
	pd_azimuthVector[234] = -18.14652;
	pd_azimuthVector[235] = -18.27977;
	pd_azimuthVector[236] = -18.41302;
	pd_azimuthVector[237] = -18.54623;
	pd_azimuthVector[238] = -18.67941;
	pd_azimuthVector[239] = -18.81251;
	pd_azimuthVector[240] = -18.94554;
	pd_azimuthVector[241] = -19.07846;
	pd_azimuthVector[242] = -19.21126;
	pd_azimuthVector[243] = -19.34392;
	pd_azimuthVector[244] = -19.47641;
	pd_azimuthVector[245] = -19.60872;
	pd_azimuthVector[246] = -19.74081;
	pd_azimuthVector[247] = -19.87266;
	pd_azimuthVector[248] = -20.00426;
	pd_azimuthVector[249] = -20.13558;
	pd_azimuthVector[250] = -20.26659;
	pd_azimuthVector[251] = -20.39727;
	pd_azimuthVector[252] = -20.52758;
	pd_azimuthVector[253] = -20.65752;
	pd_azimuthVector[254] = -20.78703;
	pd_azimuthVector[255] = -20.9161;
	pd_azimuthVector[256] = -21.0447;
	pd_azimuthVector[257] = -21.17281;
	pd_azimuthVector[258] = -21.30037;
	pd_azimuthVector[259] = -21.42738;
	pd_azimuthVector[260] = -21.5538;
	pd_azimuthVector[261] = -21.67959;
	pd_azimuthVector[262] = -21.80473;
	pd_azimuthVector[263] = -21.92917;
	pd_azimuthVector[264] = -22.0529;
	pd_azimuthVector[265] = -22.17588;
	pd_azimuthVector[266] = -22.29806;
	pd_azimuthVector[267] = -22.41942;
	pd_azimuthVector[268] = -22.53993;
	pd_azimuthVector[269] = -22.65955;
	pd_azimuthVector[270] = -22.77825;
	pd_azimuthVector[271] = -22.89598;
	pd_azimuthVector[272] = -23.01272;
	pd_azimuthVector[273] = -23.12844;
	pd_azimuthVector[274] = -23.24309;
	pd_azimuthVector[275] = -23.35663;
	pd_azimuthVector[276] = -23.46906;
	pd_azimuthVector[277] = -23.58031;
	pd_azimuthVector[278] = -23.69037;
	pd_azimuthVector[279] = -23.79919;
	pd_azimuthVector[280] = -23.90675;
	pd_azimuthVector[281] = -24.013;
	pd_azimuthVector[282] = -24.11793;
	pd_azimuthVector[283] = -24.2215;
	pd_azimuthVector[284] = -24.32367;
	pd_azimuthVector[285] = -24.42443;
	pd_azimuthVector[286] = -24.52374;
	pd_azimuthVector[287] = -24.62157;
	pd_azimuthVector[288] = -24.7179;
	pd_azimuthVector[289] = -24.81271;
	pd_azimuthVector[290] = -24.90597;
	pd_azimuthVector[291] = -24.99766;
	pd_azimuthVector[292] = -25.08774;
	pd_azimuthVector[293] = -25.17621;
	pd_azimuthVector[294] = -25.26305;
	pd_azimuthVector[295] = -25.34826;
	pd_azimuthVector[296] = -25.43179;
	pd_azimuthVector[297] = -25.51365;
	pd_azimuthVector[298] = -25.59382;
	pd_azimuthVector[299] = -25.67229;
	pd_azimuthVector[300] = -25.74905;
	pd_azimuthVector[301] = -25.8241;
	pd_azimuthVector[302] = -25.89744;
	pd_azimuthVector[303] = -25.96906;
	pd_azimuthVector[304] = -26.03896;
	pd_azimuthVector[305] = -26.10713;
	pd_azimuthVector[306] = -26.17358;
	pd_azimuthVector[307] = -26.23833;
	pd_azimuthVector[308] = -26.30136;
	pd_azimuthVector[309] = -26.3627;
	pd_azimuthVector[310] = -26.42235;
	pd_azimuthVector[311] = -26.4803;
	pd_azimuthVector[312] = -26.53659;
	pd_azimuthVector[313] = -26.59123;
	pd_azimuthVector[314] = -26.64423;
	pd_azimuthVector[315] = -26.69559;
	pd_azimuthVector[316] = -26.74535;
	pd_azimuthVector[317] = -26.79353;
	pd_azimuthVector[318] = -26.84013;
	pd_azimuthVector[319] = -26.88518;
	pd_azimuthVector[320] = -26.9287;
	pd_azimuthVector[321] = -26.97071;
	pd_azimuthVector[322] = -27.01124;
	pd_azimuthVector[323] = -27.05032;
	pd_azimuthVector[324] = -27.08795;
	pd_azimuthVector[325] = -27.12418;
	pd_azimuthVector[326] = -27.15901;
	pd_azimuthVector[327] = -27.1925;
	pd_azimuthVector[328] = -27.22463;
	pd_azimuthVector[329] = -27.25546;
	pd_azimuthVector[330] = -27.285;
	pd_azimuthVector[331] = -27.31329;
	pd_azimuthVector[332] = -27.34034;
	pd_azimuthVector[333] = -27.36618;
	pd_azimuthVector[334] = -27.39084;
	pd_azimuthVector[335] = -27.41433;
	pd_azimuthVector[336] = -27.4367;
	pd_azimuthVector[337] = -27.45795;
	pd_azimuthVector[338] = -27.47811;
	pd_azimuthVector[339] = -27.4972;
	pd_azimuthVector[340] = -27.51526;
	pd_azimuthVector[341] = -27.53228;
	pd_azimuthVector[342] = -27.54831;
	pd_azimuthVector[343] = -27.56334;
	pd_azimuthVector[344] = -27.57742;
	pd_azimuthVector[345] = -27.59055;
	pd_azimuthVector[346] = -27.60276;
	pd_azimuthVector[347] = -27.61404;
	pd_azimuthVector[348] = -27.62443;
	pd_azimuthVector[349] = -27.63394;
	pd_azimuthVector[350] = -27.64257;
	pd_azimuthVector[351] = -27.65035;
	pd_azimuthVector[352] = -27.65728;
	pd_azimuthVector[353] = -27.66336;
	pd_azimuthVector[354] = -27.66863;
	pd_azimuthVector[355] = -27.67307;
	pd_azimuthVector[356] = -27.67669;
	pd_azimuthVector[357] = -27.6795;
	pd_azimuthVector[358] = -27.6815;
	pd_azimuthVector[359] = -27.6827;
	pd_azimuthVector[360] = -27.6831;
	pd_azimuthVector[361] = -27.6827;
	pd_azimuthVector[362] = -27.6815;
	pd_azimuthVector[363] = -27.6795;
	pd_azimuthVector[364] = -27.67669;
	pd_azimuthVector[365] = -27.67307;
	pd_azimuthVector[366] = -27.66863;
	pd_azimuthVector[367] = -27.66336;
	pd_azimuthVector[368] = -27.65728;
	pd_azimuthVector[369] = -27.65035;
	pd_azimuthVector[370] = -27.64257;
	pd_azimuthVector[371] = -27.63394;
	pd_azimuthVector[372] = -27.62443;
	pd_azimuthVector[373] = -27.61404;
	pd_azimuthVector[374] = -27.60276;
	pd_azimuthVector[375] = -27.59055;
	pd_azimuthVector[376] = -27.57742;
	pd_azimuthVector[377] = -27.56334;
	pd_azimuthVector[378] = -27.54831;
	pd_azimuthVector[379] = -27.53228;
	pd_azimuthVector[380] = -27.51526;
	pd_azimuthVector[381] = -27.4972;
	pd_azimuthVector[382] = -27.47811;
	pd_azimuthVector[383] = -27.45795;
	pd_azimuthVector[384] = -27.4367;
	pd_azimuthVector[385] = -27.41433;
	pd_azimuthVector[386] = -27.39084;
	pd_azimuthVector[387] = -27.36618;
	pd_azimuthVector[388] = -27.34034;
	pd_azimuthVector[389] = -27.31329;
	pd_azimuthVector[390] = -27.285;
	pd_azimuthVector[391] = -27.25546;
	pd_azimuthVector[392] = -27.22463;
	pd_azimuthVector[393] = -27.1925;
	pd_azimuthVector[394] = -27.15901;
	pd_azimuthVector[395] = -27.12418;
	pd_azimuthVector[396] = -27.08795;
	pd_azimuthVector[397] = -27.05032;
	pd_azimuthVector[398] = -27.01124;
	pd_azimuthVector[399] = -26.97071;
	pd_azimuthVector[400] = -26.9287;
	pd_azimuthVector[401] = -26.88518;
	pd_azimuthVector[402] = -26.84013;
	pd_azimuthVector[403] = -26.79353;
	pd_azimuthVector[404] = -26.74535;
	pd_azimuthVector[405] = -26.69559;
	pd_azimuthVector[406] = -26.64423;
	pd_azimuthVector[407] = -26.59123;
	pd_azimuthVector[408] = -26.53659;
	pd_azimuthVector[409] = -26.4803;
	pd_azimuthVector[410] = -26.42235;
	pd_azimuthVector[411] = -26.3627;
	pd_azimuthVector[412] = -26.30136;
	pd_azimuthVector[413] = -26.23833;
	pd_azimuthVector[414] = -26.17358;
	pd_azimuthVector[415] = -26.10713;
	pd_azimuthVector[416] = -26.03896;
	pd_azimuthVector[417] = -25.96906;
	pd_azimuthVector[418] = -25.89744;
	pd_azimuthVector[419] = -25.8241;
	pd_azimuthVector[420] = -25.74905;
	pd_azimuthVector[421] = -25.67229;
	pd_azimuthVector[422] = -25.59382;
	pd_azimuthVector[423] = -25.51365;
	pd_azimuthVector[424] = -25.43179;
	pd_azimuthVector[425] = -25.34826;
	pd_azimuthVector[426] = -25.26305;
	pd_azimuthVector[427] = -25.17621;
	pd_azimuthVector[428] = -25.08774;
	pd_azimuthVector[429] = -24.99766;
	pd_azimuthVector[430] = -24.90597;
	pd_azimuthVector[431] = -24.81271;
	pd_azimuthVector[432] = -24.7179;
	pd_azimuthVector[433] = -24.62157;
	pd_azimuthVector[434] = -24.52374;
	pd_azimuthVector[435] = -24.42443;
	pd_azimuthVector[436] = -24.32367;
	pd_azimuthVector[437] = -24.2215;
	pd_azimuthVector[438] = -24.11793;
	pd_azimuthVector[439] = -24.013;
	pd_azimuthVector[440] = -23.90675;
	pd_azimuthVector[441] = -23.79919;
	pd_azimuthVector[442] = -23.69037;
	pd_azimuthVector[443] = -23.58031;
	pd_azimuthVector[444] = -23.46906;
	pd_azimuthVector[445] = -23.35663;
	pd_azimuthVector[446] = -23.24309;
	pd_azimuthVector[447] = -23.12844;
	pd_azimuthVector[448] = -23.01272;
	pd_azimuthVector[449] = -22.89598;
	pd_azimuthVector[450] = -22.77825;
	pd_azimuthVector[451] = -22.65955;
	pd_azimuthVector[452] = -22.53993;
	pd_azimuthVector[453] = -22.41942;
	pd_azimuthVector[454] = -22.29806;
	pd_azimuthVector[455] = -22.17588;
	pd_azimuthVector[456] = -22.0529;
	pd_azimuthVector[457] = -21.92917;
	pd_azimuthVector[458] = -21.80473;
	pd_azimuthVector[459] = -21.67959;
	pd_azimuthVector[460] = -21.5538;
	pd_azimuthVector[461] = -21.42738;
	pd_azimuthVector[462] = -21.30037;
	pd_azimuthVector[463] = -21.17281;
	pd_azimuthVector[464] = -21.0447;
	pd_azimuthVector[465] = -20.9161;
	pd_azimuthVector[466] = -20.78703;
	pd_azimuthVector[467] = -20.65752;
	pd_azimuthVector[468] = -20.52758;
	pd_azimuthVector[469] = -20.39727;
	pd_azimuthVector[470] = -20.26659;
	pd_azimuthVector[471] = -20.13558;
	pd_azimuthVector[472] = -20.00426;
	pd_azimuthVector[473] = -19.87266;
	pd_azimuthVector[474] = -19.74081;
	pd_azimuthVector[475] = -19.60872;
	pd_azimuthVector[476] = -19.47641;
	pd_azimuthVector[477] = -19.34392;
	pd_azimuthVector[478] = -19.21126;
	pd_azimuthVector[479] = -19.07846;
	pd_azimuthVector[480] = -18.94554;
	pd_azimuthVector[481] = -18.81251;
	pd_azimuthVector[482] = -18.67941;
	pd_azimuthVector[483] = -18.54623;
	pd_azimuthVector[484] = -18.41302;
	pd_azimuthVector[485] = -18.27977;
	pd_azimuthVector[486] = -18.14652;
	pd_azimuthVector[487] = -18.01328;
	pd_azimuthVector[488] = -17.88006;
	pd_azimuthVector[489] = -17.74688;
	pd_azimuthVector[490] = -17.61376;
	pd_azimuthVector[491] = -17.48071;
	pd_azimuthVector[492] = -17.34776;
	pd_azimuthVector[493] = -17.21489;
	pd_azimuthVector[494] = -17.08215;
	pd_azimuthVector[495] = -16.94953;
	pd_azimuthVector[496] = -16.81706;
	pd_azimuthVector[497] = -16.68473;
	pd_azimuthVector[498] = -16.55258;
	pd_azimuthVector[499] = -16.42061;
	pd_azimuthVector[500] = -16.28883;
	pd_azimuthVector[501] = -16.15724;
	pd_azimuthVector[502] = -16.02587;
	pd_azimuthVector[503] = -15.89472;
	pd_azimuthVector[504] = -15.76381;
	pd_azimuthVector[505] = -15.63313;
	pd_azimuthVector[506] = -15.50271;
	pd_azimuthVector[507] = -15.37255;
	pd_azimuthVector[508] = -15.24266;
	pd_azimuthVector[509] = -15.11305;
	pd_azimuthVector[510] = -14.98373;
	pd_azimuthVector[511] = -14.85471;
	pd_azimuthVector[512] = -14.72599;
	pd_azimuthVector[513] = -14.59758;
	pd_azimuthVector[514] = -14.46949;
	pd_azimuthVector[515] = -14.34173;
	pd_azimuthVector[516] = -14.2143;
	pd_azimuthVector[517] = -14.08721;
	pd_azimuthVector[518] = -13.96048;
	pd_azimuthVector[519] = -13.8341;
	pd_azimuthVector[520] = -13.70807;
	pd_azimuthVector[521] = -13.58242;
	pd_azimuthVector[522] = -13.45714;
	pd_azimuthVector[523] = -13.33224;
	pd_azimuthVector[524] = -13.20772;
	pd_azimuthVector[525] = -13.0836;
	pd_azimuthVector[526] = -12.95987;
	pd_azimuthVector[527] = -12.83655;
	pd_azimuthVector[528] = -12.71363;
	pd_azimuthVector[529] = -12.59112;
	pd_azimuthVector[530] = -12.46903;
	pd_azimuthVector[531] = -12.34737;
	pd_azimuthVector[532] = -12.22613;
	pd_azimuthVector[533] = -12.10532;
	pd_azimuthVector[534] = -11.98495;
	pd_azimuthVector[535] = -11.86501;
	pd_azimuthVector[536] = -11.74553;
	pd_azimuthVector[537] = -11.62649;
	pd_azimuthVector[538] = -11.50791;
	pd_azimuthVector[539] = -11.38978;
	pd_azimuthVector[540] = -11.27212;
	pd_azimuthVector[541] = -11.15492;
	pd_azimuthVector[542] = -11.03819;
	pd_azimuthVector[543] = -10.92194;
	pd_azimuthVector[544] = -10.80616;
	pd_azimuthVector[545] = -10.69086;
	pd_azimuthVector[546] = -10.57605;
	pd_azimuthVector[547] = -10.46173;
	pd_azimuthVector[548] = -10.34789;
	pd_azimuthVector[549] = -10.23456;
	pd_azimuthVector[550] = -10.12172;
	pd_azimuthVector[551] = -10.00938;
	pd_azimuthVector[552] = -9.897549;
	pd_azimuthVector[553] = -9.786225;
	pd_azimuthVector[554] = -9.675415;
	pd_azimuthVector[555] = -9.565116;
	pd_azimuthVector[556] = -9.455337;
	pd_azimuthVector[557] = -9.346079;
	pd_azimuthVector[558] = -9.237345;
	pd_azimuthVector[559] = -9.129137;
	pd_azimuthVector[560] = -9.021459;
	pd_azimuthVector[561] = -8.914314;
	pd_azimuthVector[562] = -8.807705;
	pd_azimuthVector[563] = -8.701633;
	pd_azimuthVector[564] = -8.596103;
	pd_azimuthVector[565] = -8.491117;
	pd_azimuthVector[566] = -8.386681;
	pd_azimuthVector[567] = -8.282788;
	pd_azimuthVector[568] = -8.179451;
	pd_azimuthVector[569] = -8.07667;
	pd_azimuthVector[570] = -7.974444;
	pd_azimuthVector[571] = -7.872777;
	pd_azimuthVector[572] = -7.771674;
	pd_azimuthVector[573] = -7.671135;
	pd_azimuthVector[574] = -7.571167;
	pd_azimuthVector[575] = -7.471767;
	pd_azimuthVector[576] = -7.372939;
	pd_azimuthVector[577] = -7.274685;
	pd_azimuthVector[578] = -7.177009;
	pd_azimuthVector[579] = -7.079912;
	pd_azimuthVector[580] = -6.983397;
	pd_azimuthVector[581] = -6.887466;
	pd_azimuthVector[582] = -6.792123;
	pd_azimuthVector[583] = -6.697368;
	pd_azimuthVector[584] = -6.603205;
	pd_azimuthVector[585] = -6.509634;
	pd_azimuthVector[586] = -6.416661;
	pd_azimuthVector[587] = -6.324282;
	pd_azimuthVector[588] = -6.232504;
	pd_azimuthVector[589] = -6.14133;
	pd_azimuthVector[590] = -6.050758;
	pd_azimuthVector[591] = -5.960793;
	pd_azimuthVector[592] = -5.871437;
	pd_azimuthVector[593] = -5.782692;
	pd_azimuthVector[594] = -5.694557;
	pd_azimuthVector[595] = -5.60704;
	pd_azimuthVector[596] = -5.520137;
	pd_azimuthVector[597] = -5.433855;
	pd_azimuthVector[598] = -5.34819;
	pd_azimuthVector[599] = -5.263149;
	pd_azimuthVector[600] = -5.178733;
	pd_azimuthVector[601] = -5.094944;
	pd_azimuthVector[602] = -5.011782;
	pd_azimuthVector[603] = -4.92925;
	pd_azimuthVector[604] = -4.847351;
	pd_azimuthVector[605] = -4.766084;
	pd_azimuthVector[606] = -4.685455;
	pd_azimuthVector[607] = -4.605462;
	pd_azimuthVector[608] = -4.526109;
	pd_azimuthVector[609] = -4.447395;
	pd_azimuthVector[610] = -4.369325;
	pd_azimuthVector[611] = -4.291901;
	pd_azimuthVector[612] = -4.215122;
	pd_azimuthVector[613] = -4.138991;
	pd_azimuthVector[614] = -4.06351;
	pd_azimuthVector[615] = -3.98868;
	pd_azimuthVector[616] = -3.914504;
	pd_azimuthVector[617] = -3.840981;
	pd_azimuthVector[618] = -3.768115;
	pd_azimuthVector[619] = -3.695907;
	pd_azimuthVector[620] = -3.624358;
	pd_azimuthVector[621] = -3.553471;
	pd_azimuthVector[622] = -3.483245;
	pd_azimuthVector[623] = -3.413683;
	pd_azimuthVector[624] = -3.344789;
	pd_azimuthVector[625] = -3.27656;
	pd_azimuthVector[626] = -3.209;
	pd_azimuthVector[627] = -3.14211;
	pd_azimuthVector[628] = -3.075891;
	pd_azimuthVector[629] = -3.010346;
	pd_azimuthVector[630] = -2.945475;
	pd_azimuthVector[631] = -2.88128;
	pd_azimuthVector[632] = -2.817762;
	pd_azimuthVector[633] = -2.754922;
	pd_azimuthVector[634] = -2.692763;
	pd_azimuthVector[635] = -2.631285;
	pd_azimuthVector[636] = -2.570488;
	pd_azimuthVector[637] = -2.510376;
	pd_azimuthVector[638] = -2.45095;
	pd_azimuthVector[639] = -2.392209;
	pd_azimuthVector[640] = -2.334156;
	pd_azimuthVector[641] = -2.276792;
	pd_azimuthVector[642] = -2.220118;
	pd_azimuthVector[643] = -2.164135;
	pd_azimuthVector[644] = -2.108845;
	pd_azimuthVector[645] = -2.054248;
	pd_azimuthVector[646] = -2.000348;
	pd_azimuthVector[647] = -1.947142;
	pd_azimuthVector[648] = -1.894634;
	pd_azimuthVector[649] = -1.842823;
	pd_azimuthVector[650] = -1.791713;
	pd_azimuthVector[651] = -1.741303;
	pd_azimuthVector[652] = -1.691594;
	pd_azimuthVector[653] = -1.642587;
	pd_azimuthVector[654] = -1.594285;
	pd_azimuthVector[655] = -1.546686;
	pd_azimuthVector[656] = -1.499793;
	pd_azimuthVector[657] = -1.453607;
	pd_azimuthVector[658] = -1.408128;
	pd_azimuthVector[659] = -1.363357;
	pd_azimuthVector[660] = -1.319296;
	pd_azimuthVector[661] = -1.275946;
	pd_azimuthVector[662] = -1.233307;
	pd_azimuthVector[663] = -1.191379;
	pd_azimuthVector[664] = -1.150165;
	pd_azimuthVector[665] = -1.109665;
	pd_azimuthVector[666] = -1.069879;
	pd_azimuthVector[667] = -1.030808;
	pd_azimuthVector[668] = -0.9924549;
	pd_azimuthVector[669] = -0.9548182;
	pd_azimuthVector[670] = -0.9178993;
	pd_azimuthVector[671] = -0.8816986;
	pd_azimuthVector[672] = -0.8462184;
	pd_azimuthVector[673] = -0.8114575;
	pd_azimuthVector[674] = -0.7774172;
	pd_azimuthVector[675] = -0.7440991;
	pd_azimuthVector[676] = -0.7115031;
	pd_azimuthVector[677] = -0.6796299;
	pd_azimuthVector[678] = -0.6484804;
	pd_azimuthVector[679] = -0.6180558;
	pd_azimuthVector[680] = -0.5883544;
	pd_azimuthVector[681] = -0.5593795;
	pd_azimuthVector[682] = -0.5311305;
	pd_azimuthVector[683] = -0.5036084;
	pd_azimuthVector[684] = -0.4768129;
	pd_azimuthVector[685] = -0.4507453;
	pd_azimuthVector[686] = -0.4254063;
	pd_azimuthVector[687] = -0.4007962;
	pd_azimuthVector[688] = -0.3769142;
	pd_azimuthVector[689] = -0.3537635;
	pd_azimuthVector[690] = -0.331342;
	pd_azimuthVector[691] = -0.309652;
	pd_azimuthVector[692] = -0.2886924;
	pd_azimuthVector[693] = -0.2684641;
	pd_azimuthVector[694] = -0.2489691;
	pd_azimuthVector[695] = -0.2302061;
	pd_azimuthVector[696] = -0.2121746;
	pd_azimuthVector[697] = -0.1948778;
	pd_azimuthVector[698] = -0.1783135;
	pd_azimuthVector[699] = -0.162484;
	pd_azimuthVector[700] = -0.1473877;
	pd_azimuthVector[701] = -0.1330259;
	pd_azimuthVector[702] = -0.1193989;
	pd_azimuthVector[703] = -0.1065069;
	pd_azimuthVector[704] = -0.0944;
	pd_azimuthVector[705] = -0.0829;
	pd_azimuthVector[706] = -0.0722;
	pd_azimuthVector[707] = -0.0622953;
	pd_azimuthVector[708] = -0.0530822;
	pd_azimuthVector[709] = -0.0446;
	pd_azimuthVector[710] = -0.0369;
	pd_azimuthVector[711] = -0.0299;
	pd_azimuthVector[712] = -0.0236;
	pd_azimuthVector[713] = -0.018065;
	pd_azimuthVector[714] = -0.0133;
	pd_azimuthVector[715] = -0.00922;
	pd_azimuthVector[716] = -0.0059;
	pd_azimuthVector[717] = -0.00332;
	pd_azimuthVector[718] = -0.00147;
	pd_azimuthVector[719] = -0.000368;
	pd_azimuthVector[720] = 0;
	TwoDimensional::pd_startUp();
}



