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

#include <RISE/transceiver/SCTransmitter.hpp>
#include <RISE/medium/PhysicalResource.hpp>

#include <WNS/pyconfig/View.hpp>

#include <sstream>

using namespace rise;

SCTransmitter::SCTransmitter(const wns::pyconfig::View& config, Station* s, antenna::Antenna* a) :
	Transmitter(config, s, a),
	active(false)
{
}

SCTransmitter::~SCTransmitter()
{
}

void SCTransmitter::startTransmitting(TransmissionObjectPtr transmissionObject)
{
	assure(!prc.empty(), "PhysicalResource is missing");
	assure(!active, "This Transmitter allows only one Transmission at a time");
	active = true;
	Transmitter::startTransmitting(transmissionObject, 0);
}

void SCTransmitter::stopTransmitting(TransmissionObjectPtr transmissionObject)
{
	assure(active, "Transmitter not active, although trying to stop a transmission");
	active = false;
	Transmitter::stopTransmitting(transmissionObject);
}

void SCTransmitter::tune(double f, double b)
{
	Transmitter::tune(f, b, 1);
}

void SCTransmitter::mobilityUpdate()
{
  	assure(!prc.empty(), "SCTransmitter must be tuned to PhysicalResource");
 	prc.at(0)->mobilityUpdate(this);
}

double SCTransmitter::getFrequency()
{
	assure(!prc.empty(), "SCTransmitter not tuned.");
	return prc.at(0)->getFrequency();
}

bool SCTransmitter::isActive() const
{
	return active;
}


