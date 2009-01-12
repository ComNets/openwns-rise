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

#include <RISE/decoder/DecoderConvolutional.hpp>

#include <WNS/Assure.hpp>

#include <cmath>
#include <cassert>
#include <stdexcept>

using namespace std;
using namespace rise;

DecoderConvolutional::~DecoderConvolutional()
{}

DecoderConvolutional::DecoderConvolutional(int k,
										   int bdfree,
										   int dfree,
										   double punctuation) :
	K(k),
	B_dfree(bdfree),
	D_free(dfree),
	Punctuation(punctuation)
{
	assure(B_dfree > 0, "B_dfree must be > 0");
	assure(D_free > 0, "D_free must be > 0");
	assure(K > 0, "K must be > 0");
	assure(Punctuation >= 1.0, "Punctuation must be >= 1");
}

double DecoderConvolutional::getBER(double p_raw)
{
	return getBER(p_raw, Punctuation);
}

double DecoderConvolutional::getBER(double p_raw, double aPuncturingFactor)
{
	assure(p_raw>=0 && p_raw<=0.5, "Raw BER must be in [0, 0.5]");
	double ber = double(B_dfree)/K*pow(2*sqrt(p_raw*(1-p_raw)),
									   (D_free/aPuncturingFactor));
	if (ber > 0.5)
		ber = 0.5;
	return ber;
}


