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

#include <RISE/antenna/pattern/twodimensional/Directed13.hpp>

using namespace rise::antenna::pattern::twodimensional;

Directed13::Directed13()
{
	pd_startUp();
}

Directed13::~Directed13()
{}

Directed13* Directed13::myself = 0;

void Directed13::pd_startUp ()
{
	type = "Internal";
	pattern = "Directed_13";
	uint32_t horSize = 720;
	uint32_t vertSize = 1;
	pd_elevationVector = Double64Vector(vertSize + 1);
	pd_azimuthVector = Double64Vector(horSize + 1);
	pd_gain = 18;
	for (uint32_t i = 0; i <= vertSize; i++)
	{
		pd_elevationVector[i] = 0;
	}
	for (uint32_t i = 0; i <= horSize; i++)
	{
		pd_azimuthVector[i] = -35;
	}
	pd_azimuthVector[50] = -33.20;
	pd_azimuthVector[49] = -32.60;
	pd_azimuthVector[48] = -32.20;
	pd_azimuthVector[47] = -30.60;
	pd_azimuthVector[46] = -31.00;
	pd_azimuthVector[45] = -33.00;
	pd_azimuthVector[44] = -34.00;
	pd_azimuthVector[43] = -32.40;
	pd_azimuthVector[42] = -33.00;
	pd_azimuthVector[41] = -33.00;
	pd_azimuthVector[40] = -31.20;
	pd_azimuthVector[39] = -31.40;
	pd_azimuthVector[38] = -32.00;
	pd_azimuthVector[37] = -32.40;
	pd_azimuthVector[36] = -31.00;
	pd_azimuthVector[35] = -32.60;
	pd_azimuthVector[34] = -31.40;
	pd_azimuthVector[33] = -31.20;
	pd_azimuthVector[32] = -28.00;
	pd_azimuthVector[31] = -26.00;
	pd_azimuthVector[30] = -23.00;
	pd_azimuthVector[29] = -21.80;
	pd_azimuthVector[28] = -20.00;
	pd_azimuthVector[27] = -17.60;
	pd_azimuthVector[26] = -16.00;
	pd_azimuthVector[25] = -14.10;
	pd_azimuthVector[24] = -12.80;
	pd_azimuthVector[23] = -11.30;
	pd_azimuthVector[22] = -10.20;
	pd_azimuthVector[21] = -9.20;
	pd_azimuthVector[20] = -7.80;
	pd_azimuthVector[19] = -6.80;
	pd_azimuthVector[18] = -5.90;
	pd_azimuthVector[17] = -5.30;
	pd_azimuthVector[16] = -4.60;
	pd_azimuthVector[15] = -4.20;
	pd_azimuthVector[14] = -3.60;
	pd_azimuthVector[13] = -3.20;
	pd_azimuthVector[12] = -2.70;
	pd_azimuthVector[11] = -2.30;
	pd_azimuthVector[10] = -1.90;
	pd_azimuthVector[9] =  -1.70;
	pd_azimuthVector[8] =  -1.50;
	pd_azimuthVector[7] =  -1.30;
	pd_azimuthVector[6] =  -1.10;
	pd_azimuthVector[5] =  -0.90;
	pd_azimuthVector[4] =  -0.70;
	pd_azimuthVector[3] =  -0.50;
	pd_azimuthVector[2] =  -0.30;
	pd_azimuthVector[1] =  -0.20;
	pd_azimuthVector[0] =  0.00;
	pd_azimuthVector[670] = -33.20;
	pd_azimuthVector[671] = -32.60;
	pd_azimuthVector[672] = -32.20;
	pd_azimuthVector[673] = -30.60;
	pd_azimuthVector[674] = -31.00;
	pd_azimuthVector[675] = -33.00;
	pd_azimuthVector[676] = -34.00;
	pd_azimuthVector[677] = -32.40;
	pd_azimuthVector[678] = -33.00;
	pd_azimuthVector[679] = -33.00;
	pd_azimuthVector[680] = -31.20;
	pd_azimuthVector[681] = -31.40;
	pd_azimuthVector[682] = -32.00;
	pd_azimuthVector[683] = -32.40;
	pd_azimuthVector[684] = -31.00;
	pd_azimuthVector[685] = -32.60;
	pd_azimuthVector[686] = -31.40;
	pd_azimuthVector[687] = -31.20;
	pd_azimuthVector[688] = -28.00;
	pd_azimuthVector[689] = -26.00;
	pd_azimuthVector[690] = -23.00;
	pd_azimuthVector[691] = -21.80;
	pd_azimuthVector[692] = -20.00;
	pd_azimuthVector[693] = -17.60;
	pd_azimuthVector[694] = -16.00;
	pd_azimuthVector[695] = -14.10;
	pd_azimuthVector[696] = -12.80;
	pd_azimuthVector[697] = -11.30;
	pd_azimuthVector[698] = -10.20;
	pd_azimuthVector[699] = -9.20;
	pd_azimuthVector[700] = -7.80;
	pd_azimuthVector[701] = -6.80;
	pd_azimuthVector[702] = -5.90;
	pd_azimuthVector[703] = -5.30;
	pd_azimuthVector[704] = -4.60;
	pd_azimuthVector[705] = -4.20;
	pd_azimuthVector[706] = -3.60;
	pd_azimuthVector[707] = -3.20;
	pd_azimuthVector[708] = -2.70;
	pd_azimuthVector[709] = -2.30;
	pd_azimuthVector[710] = -1.90;
	pd_azimuthVector[711] =  -1.70;
	pd_azimuthVector[712] =  -1.50;
	pd_azimuthVector[713] =  -1.30;
	pd_azimuthVector[714] =  -1.10;
	pd_azimuthVector[715] =  -0.90;
	pd_azimuthVector[716] =  -0.70;
	pd_azimuthVector[717] =  -0.50;
	pd_azimuthVector[718] =  -0.30;
	pd_azimuthVector[719] =  -0.20;
	pd_azimuthVector[720] =  0.0;
	TwoDimensional::pd_startUp();
}


