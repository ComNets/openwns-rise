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

#include <RISE/transmissionobjects/unicasttransmissionobject.hpp>

using namespace std;
using namespace rise;

UnicastTransmissionObject::UnicastTransmissionObject(Transmitter* aTransmitter,
                                                     receiver::ReceiverInterface* aReceiver,
                                                     unsigned long int aLinkMode,
                                                     wns::osi::PDUPtr aPayload,
                                                     wns::Power power,
                                                     int numberOfSpatialStreams)
    : TransmissionObject(aTransmitter, aPayload, power, aLinkMode, numberOfSpatialStreams),
      pd_receiver(aReceiver)
{}

UnicastTransmissionObject::UnicastTransmissionObject(Transmitter* aTransmitter,
                                                     receiver::ReceiverInterface* aReceiver,
                                                     unsigned long int aLinkMode,
                                                     wns::osi::PDUPtr aPayload,
                                                     wns::Power power,
                                                     const wns::service::phy::phymode::PhyModeInterfacePtr _phyModePtr)
    : TransmissionObject(aTransmitter, aPayload, power, _phyModePtr, aLinkMode),
      pd_receiver(aReceiver)
{}

UnicastTransmissionObject::~UnicastTransmissionObject()
{}


