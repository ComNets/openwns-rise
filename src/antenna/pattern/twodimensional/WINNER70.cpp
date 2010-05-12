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

#include <RISE/antenna/pattern/twodimensional/WINNER70.hpp>

using namespace rise::antenna::pattern::twodimensional;

WINNER70::WINNER70()
{
	pd_startUp();
}

WINNER70::~WINNER70()
{}

WINNER70* WINNER70::myself = 0;

void WINNER70::pd_startUp ()
{
	type = "Internal";
	pattern = "WINNER_70";
	unsigned long int horSize = 360;
	unsigned long int vertSize = 180;
	pd_elevationVector = Double64Vector(vertSize + 1);
	pd_azimuthVector = Double64Vector(horSize + 1);
	pd_gain = 14.00;
	// No elevation angle influence on gain
	for (int ii=0; ii<=180; ++ii)
		pd_elevationVector[ii] = 0.0 ;

	// Preinitialize Pattern with -20dB
	for (unsigned long int i = 0; i <= horSize; i++)
	{
		pd_azimuthVector[i] = -2.0000000000000000e+01;
	}

	// Set those angles where the pattern attenuation is less than -20dB
    pd_azimuthVector[91]  =  -2.0000000000000000e+01;
    pd_azimuthVector[90]  =  -1.9836734693877556e+01;
    pd_azimuthVector[89]  =  -1.9398367346938773e+01;
    pd_azimuthVector[88]  =  -1.8964897959183673e+01;
    pd_azimuthVector[87]  =  -1.8536326530612246e+01;
    pd_azimuthVector[86]  =  -1.8112653061224492e+01;
    pd_azimuthVector[85]  =  -1.7693877551020403e+01;
    pd_azimuthVector[84]  =  -1.7279999999999998e+01;
    pd_azimuthVector[83]  =  -1.6871020408163265e+01;
    pd_azimuthVector[82]  =  -1.6466938775510204e+01;
    pd_azimuthVector[81]  =  -1.6067755102040817e+01;
    pd_azimuthVector[80]  =  -1.5673469387755100e+01;
    pd_azimuthVector[79]  =  -1.5284081632653061e+01;
    pd_azimuthVector[78]  =  -1.4899591836734695e+01;
    pd_azimuthVector[77]  =  -1.4520000000000003e+01;
    pd_azimuthVector[76]  =  -1.4145306122448979e+01;
    pd_azimuthVector[75]  =  -1.3775510204081632e+01;
    pd_azimuthVector[74]  =  -1.3410612244897960e+01;
    pd_azimuthVector[73]  =  -1.3050612244897962e+01;
    pd_azimuthVector[72]  =  -1.2695510204081630e+01;
    pd_azimuthVector[71]  =  -1.2345306122448978e+01;
    pd_azimuthVector[70]  =  -1.2000000000000000e+01;
    pd_azimuthVector[69]  =  -1.1659591836734695e+01;
    pd_azimuthVector[68]  =  -1.1324081632653060e+01;
    pd_azimuthVector[67]  =  -1.0993469387755102e+01;
    pd_azimuthVector[66]  =  -1.0667755102040815e+01;
    pd_azimuthVector[65]  =  -1.0346938775510203e+01;
    pd_azimuthVector[64]  =  -1.0031020408163265e+01;
    pd_azimuthVector[63]  =  -9.7200000000000006e+00;
    pd_azimuthVector[62]  =  -9.4138775510204074e+00;
    pd_azimuthVector[61]  =  -9.1126530612244903e+00;
    pd_azimuthVector[60]  =  -8.8163265306122440e+00;
    pd_azimuthVector[59]  =  -8.5248979591836740e+00;
    pd_azimuthVector[58]  =  -8.2383673469387766e+00;
    pd_azimuthVector[57]  =  -7.9567346938775518e+00;
    pd_azimuthVector[56]  =  -7.6800000000000015e+00;
    pd_azimuthVector[55]  =  -7.4081632653061229e+00;
    pd_azimuthVector[54]  =  -7.1412244897959196e+00;
    pd_azimuthVector[53]  =  -6.8791836734693872e+00;
    pd_azimuthVector[52]  =  -6.6220408163265310e+00;
    pd_azimuthVector[51]  =  -6.3697959183673456e+00;
    pd_azimuthVector[50]  =  -6.1224489795918364e+00;
    pd_azimuthVector[49]  =  -5.8799999999999990e+00;
    pd_azimuthVector[48]  =  -5.6424489795918360e+00;
    pd_azimuthVector[47]  =  -5.4097959183673465e+00;
    pd_azimuthVector[46]  =  -5.1820408163265306e+00;
    pd_azimuthVector[45]  =  -4.9591836734693890e+00;
    pd_azimuthVector[44]  =  -4.7412244897959184e+00;
    pd_azimuthVector[43]  =  -4.5281632653061230e+00;
    pd_azimuthVector[42]  =  -4.3199999999999994e+00;
    pd_azimuthVector[41]  =  -4.1167346938775511e+00;
    pd_azimuthVector[40]  =  -3.9183673469387750e+00;
    pd_azimuthVector[39]  =  -3.7248979591836737e+00;
    pd_azimuthVector[38]  =  -3.5363265306122447e+00;
    pd_azimuthVector[37]  =  -3.3526530612244900e+00;
    pd_azimuthVector[36]  =  -3.1738775510204076e+00;
    pd_azimuthVector[35]  =  -3.0000000000000000e+00;
    pd_azimuthVector[34]  =  -2.8310204081632651e+00;
    pd_azimuthVector[33]  =  -2.6669387755102036e+00;
    pd_azimuthVector[32]  =  -2.5077551020408162e+00;
    pd_azimuthVector[31]  =  -2.3534693877551018e+00;
    pd_azimuthVector[30]  =  -2.2040816326530610e+00;
    pd_azimuthVector[29]  =  -2.0595918367346941e+00;
    pd_azimuthVector[28]  =  -1.9200000000000004e+00;
    pd_azimuthVector[27]  =  -1.7853061224489799e+00;
    pd_azimuthVector[26]  =  -1.6555102040816327e+00;
    pd_azimuthVector[25]  =  -1.5306122448979591e+00;
    pd_azimuthVector[24]  =  -1.4106122448979590e+00;
    pd_azimuthVector[23]  =  -1.2955102040816326e+00;
    pd_azimuthVector[22]  =  -1.1853061224489796e+00;
    pd_azimuthVector[21]  =  -1.0799999999999998e+00;
    pd_azimuthVector[20]  =  -9.7959183673469374e-01;
    pd_azimuthVector[19]  =  -8.8408163265306117e-01;
    pd_azimuthVector[18]  =  -7.9346938775510190e-01;
    pd_azimuthVector[17]  =  -7.0775510204081626e-01;
    pd_azimuthVector[16]  =  -6.2693877551020405e-01;
    pd_azimuthVector[15]  =  -5.5102040816326525e-01;
    pd_azimuthVector[14]  =  -4.8000000000000009e-01;
    pd_azimuthVector[13]  =  -4.1387755102040819e-01;
    pd_azimuthVector[12]  =  -3.5265306122448975e-01;
    pd_azimuthVector[11]  =  -2.9632653061224490e-01;
    pd_azimuthVector[10]  =  -2.4489795918367344e-01;
    pd_azimuthVector[9]   =  -1.9836734693877547e-01;
    pd_azimuthVector[8]   =  -1.5673469387755101e-01;
    pd_azimuthVector[7]   =  -1.2000000000000002e-01;
    pd_azimuthVector[6]   =  -8.8163265306122438e-02;
    pd_azimuthVector[5]   =  -6.1224489795918359e-02;
    pd_azimuthVector[4]   =  -3.9183673469387753e-02;
    pd_azimuthVector[3]   =  -2.2040816326530609e-02;
    pd_azimuthVector[2]   =  -9.7959183673469383e-03;
    pd_azimuthVector[1]   =  -2.4489795918367346e-03;
    pd_azimuthVector[0]   =   0.0000000000000000e-00;

    pd_azimuthVector[269] =  -2.0000000000000000e+01;
    pd_azimuthVector[270] =  -1.9836734693877556e+01;
    pd_azimuthVector[271] =  -1.9398367346938773e+01;
    pd_azimuthVector[272] =  -1.8964897959183673e+01;
    pd_azimuthVector[273] =  -1.8536326530612246e+01;
    pd_azimuthVector[274] =  -1.8112653061224492e+01;
    pd_azimuthVector[275] =  -1.7693877551020403e+01;
    pd_azimuthVector[276] =  -1.7279999999999998e+01;
    pd_azimuthVector[277] =  -1.6871020408163265e+01;
    pd_azimuthVector[278] =  -1.6466938775510204e+01;
    pd_azimuthVector[279] =  -1.6067755102040817e+01;
    pd_azimuthVector[280] =  -1.5673469387755100e+01;
    pd_azimuthVector[281] =  -1.5284081632653061e+01;
    pd_azimuthVector[282] =  -1.4899591836734695e+01;
    pd_azimuthVector[283] =  -1.4520000000000003e+01;
    pd_azimuthVector[284] =  -1.4145306122448979e+01;
    pd_azimuthVector[285] =  -1.3775510204081632e+01;
    pd_azimuthVector[286] =  -1.3410612244897960e+01;
    pd_azimuthVector[287] =  -1.3050612244897962e+01;
    pd_azimuthVector[288] =  -1.2695510204081630e+01;
    pd_azimuthVector[289] =  -1.2345306122448978e+01;
    pd_azimuthVector[290] =  -1.2000000000000000e+01;
    pd_azimuthVector[291] =  -1.1659591836734695e+01;
    pd_azimuthVector[292] =  -1.1324081632653060e+01;
    pd_azimuthVector[293] =  -1.0993469387755102e+01;
    pd_azimuthVector[294] =  -1.0667755102040815e+01;
    pd_azimuthVector[295] =  -1.0346938775510203e+01;
    pd_azimuthVector[296] =  -1.0031020408163265e+01;
    pd_azimuthVector[297] =  -9.7200000000000006e+00;
    pd_azimuthVector[298] =  -9.4138775510204074e+00;
    pd_azimuthVector[299] =  -9.1126530612244903e+00;
    pd_azimuthVector[300] =  -8.8163265306122440e+00;
    pd_azimuthVector[301] =  -8.5248979591836740e+00;
    pd_azimuthVector[302] =  -8.2383673469387766e+00;
    pd_azimuthVector[303] =  -7.9567346938775518e+00;
    pd_azimuthVector[304] =  -7.6800000000000015e+00;
    pd_azimuthVector[305] =  -7.4081632653061229e+00;
    pd_azimuthVector[306] =  -7.1412244897959196e+00;
    pd_azimuthVector[307] =  -6.8791836734693872e+00;
    pd_azimuthVector[308] =  -6.6220408163265310e+00;
    pd_azimuthVector[309] =  -6.3697959183673456e+00;
    pd_azimuthVector[310] =  -6.1224489795918364e+00;
    pd_azimuthVector[311] =  -5.8799999999999990e+00;
    pd_azimuthVector[312] =  -5.6424489795918360e+00;
    pd_azimuthVector[313] =  -5.4097959183673465e+00;
    pd_azimuthVector[314] =  -5.1820408163265306e+00;
    pd_azimuthVector[315] =  -4.9591836734693890e+00;
    pd_azimuthVector[316] =  -4.7412244897959184e+00;
    pd_azimuthVector[317] =  -4.5281632653061230e+00;
    pd_azimuthVector[318] =  -4.3199999999999994e+00;
    pd_azimuthVector[319] =  -4.1167346938775511e+00;
    pd_azimuthVector[320] =  -3.9183673469387750e+00;
    pd_azimuthVector[321] =  -3.7248979591836737e+00;
    pd_azimuthVector[322] =  -3.5363265306122447e+00;
    pd_azimuthVector[323] =  -3.3526530612244900e+00;
    pd_azimuthVector[324] =  -3.1738775510204076e+00;
    pd_azimuthVector[325] =  -3.0000000000000000e+00;
    pd_azimuthVector[326] =  -2.8310204081632651e+00;
    pd_azimuthVector[327] =  -2.6669387755102036e+00;
    pd_azimuthVector[328] =  -2.5077551020408162e+00;
    pd_azimuthVector[329] =  -2.3534693877551018e+00;
    pd_azimuthVector[330] =  -2.2040816326530610e+00;
    pd_azimuthVector[331] =  -2.0595918367346941e+00;
    pd_azimuthVector[332] =  -1.9200000000000004e+00;
    pd_azimuthVector[333] =  -1.7853061224489799e+00;
    pd_azimuthVector[334] =  -1.6555102040816327e+00;
    pd_azimuthVector[335] =  -1.5306122448979591e+00;
    pd_azimuthVector[336] =  -1.4106122448979590e+00;
    pd_azimuthVector[337] =  -1.2955102040816326e+00;
    pd_azimuthVector[338] =  -1.1853061224489796e+00;
    pd_azimuthVector[339] =  -1.0799999999999998e+00;
    pd_azimuthVector[340] =  -9.7959183673469374e-01;
    pd_azimuthVector[341] =  -8.8408163265306117e-01;
    pd_azimuthVector[342] =  -7.9346938775510190e-01;
    pd_azimuthVector[343] =  -7.0775510204081626e-01;
    pd_azimuthVector[344] =  -6.2693877551020405e-01;
    pd_azimuthVector[345] =  -5.5102040816326525e-01;
    pd_azimuthVector[346] =  -4.8000000000000009e-01;
    pd_azimuthVector[347] =  -4.1387755102040819e-01;
    pd_azimuthVector[348] =  -3.5265306122448975e-01;
    pd_azimuthVector[349] =  -2.9632653061224490e-01;
    pd_azimuthVector[350] =  -2.4489795918367344e-01;
    pd_azimuthVector[351] =  -1.9836734693877547e-01;
    pd_azimuthVector[352] =  -1.5673469387755101e-01;
    pd_azimuthVector[353] =  -1.2000000000000002e-01;
    pd_azimuthVector[354] =  -8.8163265306122438e-02;
    pd_azimuthVector[355] =  -6.1224489795918359e-02;
    pd_azimuthVector[356] =  -3.9183673469387753e-02;
    pd_azimuthVector[357] =  -2.2040816326530609e-02;
    pd_azimuthVector[358] =  -9.7959183673469383e-03;
    pd_azimuthVector[359] =  -2.4489795918367346e-03;
    pd_azimuthVector[360] =   0.0000000000000000e-00;
	TwoDimensional::pd_startUp();
}


