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

#include <RISE/antenna/pattern/kernel/TwoDimensional.hpp>

#include <RISE/antenna/pattern/twodimensional/Isotropic.hpp>
#include <RISE/antenna/pattern/twodimensional/OmniDirectional.hpp>
#include <RISE/antenna/pattern/twodimensional/OmniDirectional7.hpp>
#include <RISE/antenna/pattern/twodimensional/OmniDirectional9.hpp>
#include <RISE/antenna/pattern/twodimensional/LambdaHalf.hpp>
#include <RISE/antenna/pattern/twodimensional/Directed13.hpp>
#include <RISE/antenna/pattern/twodimensional/Directed17.hpp>
#include <RISE/antenna/pattern/twodimensional/Directed30.hpp>
#include <RISE/antenna/pattern/twodimensional/Directed66.hpp>
#include <RISE/antenna/pattern/twodimensional/Directed90.hpp>
#include <RISE/antenna/pattern/twodimensional/Directed120.hpp>
#include <RISE/antenna/pattern/twodimensional/WINNER70.hpp>

using namespace rise::antenna::pattern::kernel;

TwoDimensional::TwoDimensional()
	: Kernel(),
	  pd_elevationAngleStep(0),
	  pd_azimuthAngleStep(0),
	  pd_elevationVector(Double64Vector()),
	  pd_azimuthVector(Double64Vector())
{}


TwoDimensional::TwoDimensional(const std::string type,
								 const std::string pattern)
	: Kernel(type, pattern),
	  pd_elevationAngleStep(0),
	  pd_azimuthAngleStep(0),
	  pd_elevationVector(Double64Vector()),
	  pd_azimuthVector(Double64Vector())
{}

Kernel* TwoDimensional::createPatternKernel(const std::string,
											const std::string pattern)
{
	Kernel* pk = 0;
	if (pattern == (std::string)"Isotropic")
		pk = twodimensional::Isotropic::getInstance();
	else if (pattern == (std::string)"Omnidirectional")
		pk = twodimensional::OmniDirectional::getInstance();
	else if (pattern == (std::string)"Omnidirectional7")
		pk = twodimensional::OmniDirectional7::getInstance();
	else if (pattern == (std::string)"Omnidirectional9")
		pk = twodimensional::OmniDirectional9::getInstance();
	else if (pattern == (std::string)"Lambda_Half")
		pk = twodimensional::LambdaHalf::getInstance();
	else if (pattern == (std::string)"Directed_13")
		pk = twodimensional::Directed13::getInstance();
	else if (pattern == (std::string)"Directed_17")
		pk = twodimensional::Directed17::getInstance();
 	else if (pattern == (std::string)"Directed_30")
 		pk = twodimensional::Directed30::getInstance();
	else if (pattern == (std::string)"Directed_66")
		pk = twodimensional::Directed66::getInstance();
	else if (pattern == (std::string)"Directed_90")
		pk = twodimensional::Directed90::getInstance();
	else if (pattern == (std::string)"Directed_120")
		pk = twodimensional::Directed120::getInstance();
	else if (pattern == (std::string)"WINNER_70")
		pk = twodimensional::WINNER70::getInstance();
	else throw wns::Exception("TwoDimensional: The following antenna pattern is not supported Internal pattern: " + pattern);

	return pk;
}

void TwoDimensional::pd_startUp()
{
	pd_elevationAngleStep = M_PI / pd_elevationVector.size();
	pd_azimuthAngleStep = 2 * M_PI / pd_azimuthVector.size();
}

wns::Ratio TwoDimensional::getGain(const wns::Direction& direction)
{
 	uint32_t elevation_index = ((uint32_t)(direction.getElevation()/pd_elevationAngleStep));
	elevation_index %= pd_elevationVector.size();

 	uint32_t azimuth_index = ((uint32_t)(direction.getAzimuth()/pd_azimuthAngleStep));
	azimuth_index %= pd_azimuthVector.size();

	assure(pd_elevationVector.size() > elevation_index, "Elevation Range exception");
	assure(pd_azimuthVector.size() > azimuth_index, "Azimuth Range exception");

	return wns::Ratio::from_dB(pd_elevationVector[elevation_index] +
							   pd_azimuthVector[azimuth_index] +
							   pd_gain);
}

