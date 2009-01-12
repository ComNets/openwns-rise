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

#include <WNS/osi/PDU.hpp>

#include <RISE/transmissionobjects/multicasttransmissionobject.hpp>
#include <RISE/transceiver/transmitter.hpp>

using namespace std;
using namespace rise;

MulticastTransmissionObject::MulticastTransmissionObject(Transmitter* aTransmitter,
														 ReceiverContainer aMultiGroup,
														 uint32_t,
														 wns::osi::PDUPtr aPayload,
														 wns::Power power)
	: TransmissionObject(aTransmitter, aPayload, power),
	  multiGroup(aMultiGroup)
{}

MulticastTransmissionObject::MulticastTransmissionObject(Transmitter* aTransmitter,
														 ReceiverContainer aMultiGroup,
														 uint32_t aLinkMode,
														 wns::osi::PDUPtr aPayload,
														 wns::Power power,
														 const wns::service::phy::phymode::PhyModeInterfacePtr _phyModePtr)
	: TransmissionObject(aTransmitter, aPayload, power, _phyModePtr, aLinkMode),
	  multiGroup(aMultiGroup)
{}

MulticastTransmissionObject::~MulticastTransmissionObject() {}

bool MulticastTransmissionObject::isForMe(const receiver::ReceiverInterface* r) const
{
	for (uint32_t i=0; i <= multiGroup.size(); ++i)
	{
		if (multiGroup.at(i) == r) return true;
	}
	return false;
}

void MulticastTransmissionObject::addReceiver(receiver::ReceiverInterface* r)
{
	multiGroup.push_back(r);
}


