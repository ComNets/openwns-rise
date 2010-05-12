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

#include <RISE/antenna/pattern/twodimensional/Directed17.hpp>

using namespace rise::antenna::pattern::twodimensional;

Directed17::Directed17()
{
	pd_startUp();
}

Directed17::~Directed17()
{}

Directed17* Directed17::myself = 0;

void Directed17::pd_startUp ()
{
	type = "Internal";
	pattern = "Directed_17";
	unsigned long int horSize = 360;
	unsigned long int vertSize = 1;
	pd_elevationVector = Double64Vector(vertSize + 1);
	pd_azimuthVector = Double64Vector(horSize + 1);
	pd_gain = 15.65;
	for (unsigned long int i = 0; i <= vertSize; i++)
	{
		pd_elevationVector[i] = 0;
	}
	for (unsigned long int i = 0; i <= horSize; i++)
	{
		pd_azimuthVector[i] = -40;
	}
	pd_azimuthVector[53] = -30.00;
	pd_azimuthVector[52] = -28.00;
	pd_azimuthVector[51] = -27.00;
	pd_azimuthVector[50] = -26.00;
	pd_azimuthVector[49] = -25.70;
	pd_azimuthVector[48] = -25.30;
	pd_azimuthVector[47] = -25.20;
	pd_azimuthVector[46] = -25.40;
	pd_azimuthVector[45] = -26.00;
	pd_azimuthVector[44] = -27.00;
	pd_azimuthVector[43] = -28.60;
	pd_azimuthVector[42] = -31.00;
	pd_azimuthVector[41] = -34.00;
	pd_azimuthVector[40] = -39.00;
	pd_azimuthVector[39] = -35.00;
	pd_azimuthVector[38] = -30.50;
	pd_azimuthVector[37] = -27.00;
	pd_azimuthVector[36] = -24.20;
	pd_azimuthVector[35] = -22.30;
	pd_azimuthVector[34] = -20.60;
	pd_azimuthVector[33] = -19.30;
	pd_azimuthVector[32] = -18.40;
	pd_azimuthVector[31] = -17.60;
	pd_azimuthVector[30] = -17.20;
	pd_azimuthVector[29] = -17.00;
	pd_azimuthVector[28] = -17.00;
	pd_azimuthVector[27] = -17.20;
	pd_azimuthVector[26] = -17.80;
	pd_azimuthVector[25] = -18.80;
	pd_azimuthVector[24] = -20.40;
	pd_azimuthVector[23] = -22.50;
	pd_azimuthVector[22] = -26.00;
	pd_azimuthVector[21] = -30.50;
	pd_azimuthVector[20] = -25.00;
	pd_azimuthVector[19] = -21.00;
	pd_azimuthVector[18] = -17.30;
	pd_azimuthVector[17] = -14.20;
	pd_azimuthVector[16] = -12.30;
	pd_azimuthVector[15] = -10.10;
	pd_azimuthVector[14] = -8.70;
	pd_azimuthVector[13] = -7.20;
	pd_azimuthVector[12] = -6.10;
	pd_azimuthVector[11] = -5.00;
	pd_azimuthVector[10] = -4.00;
	pd_azimuthVector[9] =  -3.30;
	pd_azimuthVector[8] =  -2.50;
	pd_azimuthVector[7] =  -2.00;
	pd_azimuthVector[6] =  -1.40;
	pd_azimuthVector[5] =  -1.00;
	pd_azimuthVector[4] =  -0.70;
	pd_azimuthVector[3] =  -0.40;
	pd_azimuthVector[2] =  -0.06;
	pd_azimuthVector[1] =  -0.02;
	pd_azimuthVector[0] =  0.00;
	pd_azimuthVector[307] = -30.00;
	pd_azimuthVector[308] = -28.00;
	pd_azimuthVector[309] = -27.00;
	pd_azimuthVector[310] = -26.00;
	pd_azimuthVector[311] = -25.70;
	pd_azimuthVector[312] = -25.30;
	pd_azimuthVector[313] = -25.20;
	pd_azimuthVector[314] = -25.40;
	pd_azimuthVector[315] = -26.00;
	pd_azimuthVector[316] = -27.00;
	pd_azimuthVector[317] = -28.60;
	pd_azimuthVector[318] = -31.00;
	pd_azimuthVector[319] = -34.00;
	pd_azimuthVector[320] = -39.00;
	pd_azimuthVector[321] = -35.00;
	pd_azimuthVector[322] = -30.50;
	pd_azimuthVector[323] = -27.00;
	pd_azimuthVector[324] = -24.20;
	pd_azimuthVector[325] = -22.30;
	pd_azimuthVector[326] = -20.60;
	pd_azimuthVector[327] = -19.30;
	pd_azimuthVector[328] = -18.40;
	pd_azimuthVector[329] = -17.60;
	pd_azimuthVector[330] = -17.20;
	pd_azimuthVector[331] = -17.00;
	pd_azimuthVector[332] = -17.00;
	pd_azimuthVector[333] = -17.20;
	pd_azimuthVector[334] = -17.80;
	pd_azimuthVector[335] = -18.80;
	pd_azimuthVector[336] = -20.40;
	pd_azimuthVector[337] = -22.50;
	pd_azimuthVector[338] = -26.00;
	pd_azimuthVector[339] = -30.50;
	pd_azimuthVector[340] = -25.00;
	pd_azimuthVector[341] = -21.00;
	pd_azimuthVector[342] = -17.30;
	pd_azimuthVector[343] = -14.20;
	pd_azimuthVector[344] = -12.30;
	pd_azimuthVector[345] = -10.10;
	pd_azimuthVector[346] = -8.70;
	pd_azimuthVector[347] = -7.20;
	pd_azimuthVector[348] = -6.10;
	pd_azimuthVector[349] = -5.00;
	pd_azimuthVector[350] = -4.00;
	pd_azimuthVector[351] =  -3.30;
	pd_azimuthVector[352] =  -2.50;
	pd_azimuthVector[353] =  -2.00;
	pd_azimuthVector[354] =  -1.40;
	pd_azimuthVector[355] =  -1.00;
	pd_azimuthVector[356] =  -0.70;
	pd_azimuthVector[357] =  -0.40;
	pd_azimuthVector[358] =  -0.06;
	pd_azimuthVector[359] =  -0.02;
	pd_azimuthVector[360] =  0.0;
	TwoDimensional::pd_startUp();
}


