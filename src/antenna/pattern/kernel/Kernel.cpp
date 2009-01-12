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

#include <RISE/antenna/pattern/kernel/Kernel.hpp>
#include <RISE/antenna/pattern/kernel/TwoDimensional.hpp>
#include <RISE/antenna/pattern/kernel/ThreeDimensional.hpp>
#include <RISE/antenna/pattern/kernel/Planet.hpp>
#include <RISE/RISE.hpp>

using namespace rise::antenna::pattern::kernel;

Kernel::Kernel()
	: type(""),
	  pattern(""),
	  log("PatternKernel"),
	  pd_gain()

{}
Kernel::Kernel(const std::string t, const std::string p)
	: type(t),
	  pattern(p),
	  log("PatternKernel"),
	  pd_gain()
{
	if(RISE::getPyConfigView().get<bool>("debug.antennaPatterns")) {
		log.switchOn();
	} else {
		log.switchOff();
	}
}
Kernel::~Kernel()
{}

Kernel* Kernel::getPatternKernel(const wns::pyconfig::View& antennaView)
{
	std::string type = antennaView.get<std::string>("__plugin__");
	std::string pattern = antennaView.get<std::string>("pattern");
	PKContainer::iterator patternItr = p_availablePatterns.find(pattern);
	if(patternItr == p_availablePatterns.end()) {
		Kernel* pk =
			Kernel::createPatternKernel(std::string(type.c_str()),
													 std::string(pattern.c_str()));
		p_availablePatterns[pattern] = pk;
		return pk;
	} else {
		return patternItr->second;
	}

}

Kernel* Kernel::createPatternKernel(const std::string type,
												  const std::string pattern)
{
	Kernel* pk = 0;
	if (type == std::string("Internal"))
		pk = TwoDimensional::createPatternKernel(type, pattern);
	else if (type == std::string("Antenna3D"))
		pk = ThreeDimensional::createPatternKernel(type, pattern);
	else if (type == std::string("Planet"))
		pk = Planet::createPatternKernel(type, pattern);
	else
		throw wns::Exception(std::string("PatternKernel: No such static antenna pattern type: ") + type);
	return pk;
}

Kernel::PKContainer Kernel::p_availablePatterns = Kernel::PKContainer();


