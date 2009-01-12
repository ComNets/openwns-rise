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

#include <RISE/antenna/Static.hpp>
#include <RISE/antenna/pattern/kernel/Kernel.hpp>

using namespace rise::antenna;

Static::Static(const wns::pyconfig::View& pyConfigView,
			   Station* const station)
	: Antenna(pyConfigView, station),
	  direction(wns::Direction()),
	  kernel(0)
{
 	kernel = pattern::kernel::Kernel::getPatternKernel(pyConfigView);
	double elevation = pyConfigView.get<double>("elevation");
	elevation *= M_PI/180;
	double azimuth = pyConfigView.get<double>("azimuth");
	azimuth *= M_PI/180;
 	direction = wns::Direction(elevation, azimuth);
	if (drawPattern == true)
		drawRadiationPattern();
}


Static::~Static()
{
	kernel = 0;
}

wns::Ratio Static::getGain(const wns::Position& pos,
						   const PatternPtr) const
{
	return getGain(pos);
}

wns::Ratio Static::getGain(const wns::Position& pos) const
{
	wns::PositionOffset p = pos - getPosition();
	wns::Direction d = direction.calcAngles(p);
 	wns::Ratio directivity = kernel->getGain(d);
	MESSAGE_BEGIN(NORMAL, log, m,"Directivity in (");
	m << pos.getX() << "," << pos.getY() <<  "," << pos.getZ() << "): "
 	  << directivity.get_dB();
	MESSAGE_END();
    return directivity;
}


