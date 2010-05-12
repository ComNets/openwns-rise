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

#include <RISE/antenna/pattern/kernel/Planet.hpp>

#include <fstream>

using namespace rise::antenna::pattern::kernel;

Planet::Planet()
	: TwoDimensional()
{
	pd_startUp();
}

Planet::Planet(const std::string type,
										 const std::string pattern)
	: TwoDimensional(type, pattern)
{}

Planet::~Planet()
{}

Kernel* Planet::createPatternKernel(const std::string type,
														const std::string pattern)
{
	Planet* pk = new Planet(type, pattern);
	pk -> readPattern();
	return pk;
}

void Planet::readPattern()
{
	MESSAGE_BEGIN(NORMAL, log, m,"Reading new pattern from file: ");
	m << pattern << "... ";
	MESSAGE_END();
	std::ifstream patternfile;
	patternfile.open(pattern.c_str());
	if(patternfile.good() == false)
	{
		MESSAGE_BEGIN(NORMAL, log, m,"The following file could not be open: ");
		m << pattern;
		m << ". Check if the file name is correct. Exiting...";
		MESSAGE_END();
		exit(0);
	}
	std::string tmp_string = "";
	while (tmp_string != "GAIN")
		patternfile >> tmp_string;
	wns::Ratio r;
	patternfile >> r;
	pd_gain = r.get_dB();

	unsigned long int i = 0, horSize = 0, vertSize = 0;
	double tmp_double = 0;

	while (tmp_string != "HORIZONTAL")
		patternfile >> tmp_string;
	patternfile >> horSize;
    pd_azimuthVector = Double64Vector (horSize + 1);
	for (i = 0; i < horSize; i++)
    {
		patternfile >> tmp_double;
		patternfile >> tmp_double;
		pd_azimuthVector[i] = -tmp_double;
	}
	pd_azimuthVector[i] = pd_azimuthVector[0];

	while (tmp_string != "VERTICAL")
		patternfile >> tmp_string;
	patternfile >> vertSize;
    pd_elevationVector = Double64Vector (vertSize + 1);
	for (i = 0; i < vertSize; i++)
    {
		patternfile >> tmp_double;
		patternfile >> tmp_double;
		pd_elevationVector[i] = -tmp_double;
	}
	pd_elevationVector[i] = pd_elevationVector[0];

    patternfile.close();
	pd_startUp();
	MESSAGE_BEGIN(NORMAL, log, m,"Pattern read.");
	MESSAGE_END();
}

void Planet::pd_startUp()
{
	pd_elevationAngleStep = 2 * M_PI / pd_elevationVector.size();
	pd_azimuthAngleStep = 2 * M_PI / pd_azimuthVector.size();
}

wns::Ratio Planet::getGain(const wns::Direction& direction)
{
 	unsigned long int azimuth_index      = (unsigned long int)(direction.getAzimuth()/pd_azimuthAngleStep);
 	unsigned long int elevation_index    = 0;
 	double vert_plane_angle = 0;
 	if (((direction.getAzimuth() >= 0)        && (direction.getAzimuth() <= M_PI/2)) ||
 		((direction.getAzimuth() >= 3*M_PI/2) && (direction.getAzimuth() <= 2*M_PI)))
 	{
 		vert_plane_angle = direction.getElevation() + 3*M_PI/2;
 		if (vert_plane_angle > M_PI + M_PI)
 			vert_plane_angle -= (M_PI + M_PI);
 		elevation_index =(unsigned long int)(vert_plane_angle/pd_elevationAngleStep);
 	} else {
 		vert_plane_angle = 3*M_PI/2 - direction.getElevation();
 		elevation_index =(unsigned long int)(vert_plane_angle/pd_elevationAngleStep);
 	}
 	wns::Ratio directivity;
  	directivity.set_dB(pd_elevationVector.at(elevation_index) +
  					   pd_azimuthVector.at(azimuth_index) + pd_gain);
 	return directivity;
}


