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

#include <RISE/antenna/pattern/kernel/ThreeDimensional.hpp>

#include <fstream>

using namespace rise::antenna::pattern::kernel;

ThreeDimensional::ThreeDimensional()
	: Kernel(),
	  pd_pattern3DVector(181, Double64Vector(361))
{}

ThreeDimensional::ThreeDimensional(const std::string type,
								 const std::string pattern)
	: Kernel(type, pattern),
	  pd_pattern3DVector(181, Double64Vector(361))
{}

wns::Ratio ThreeDimensional::getGain(const wns::Direction& direction)
{
	uint32_t elevation_index = (uint32_t)(direction.getElevation() * 180 / M_PI);
	uint32_t azimuth_index = (uint32_t)(direction.getAzimuth() * 180 / M_PI);
	wns::Ratio directivity;
	directivity.set_dB(pd_pattern3DVector[elevation_index][azimuth_index]
					   + pd_gain);
	return directivity;
}

Kernel* ThreeDimensional::createPatternKernel(const std::string type,
											  const std::string pattern)
{
	ThreeDimensional* pk = new ThreeDimensional (type, pattern);
	pk -> readPattern();
	return pk;
}
void ThreeDimensional::readPattern()
{
	MESSAGE_BEGIN(NORMAL, log, m,"Reading new pattern from file: ");
	m << pattern << "... ";
	MESSAGE_END();
	std::ifstream patternfile;
	patternfile.open(pattern.c_str());
	if(patternfile.good() == false)
	{
		MESSAGE_BEGIN(NORMAL, log, m,"The following file could not be open: ");
		m << ". Check if the file name is correct. Exiting...";
		MESSAGE_END();
		exit(0);
	}
	std::string tmp_string = "";
	double elevation_index = 0, azimuth_index = 0, directivity = 0;

	while (tmp_string != "GAIN")
		patternfile >> tmp_string;
 	wns::Ratio r;
 	patternfile >> r;
 	pd_gain = r.get_dB();

	for (int j = -180; j<= 180; j++)
		for (int i = 0; i<= 180; i++)
		{
 			patternfile >> elevation_index;
			patternfile >> azimuth_index;
 			patternfile >> directivity;
			patternfile >> tmp_string;
			if (j < 0)
 				pd_pattern3DVector[i][j+360] = directivity;
 			else
 				pd_pattern3DVector[i][j] = directivity;
		}
	patternfile.close();
	MESSAGE_BEGIN(NORMAL, log, m,"Pattern read.");
	MESSAGE_END();
}


