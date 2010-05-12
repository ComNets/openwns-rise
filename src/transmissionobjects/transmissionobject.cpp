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
#include <WNS/service/phy/phymode/PhyModeInterface.hpp>

#include <RISE/transmissionobjects/transmissionobject.hpp>
#include <RISE/transceiver/transmitter.hpp>
#include <RISE/antenna/Beamforming.hpp>
#include <RISE/misc/pointer.hpp>

using namespace std;
using namespace rise;

TransmissionInterface::TransmissionInterface() :
    onAir(false),
    pr(NULL)
{
}

TransmissionInterface::~TransmissionInterface()
{
}

TransmissionObject::TransmissionObject(Transmitter* _transmitter,
                                       const wns::Power& _txPower,
                                       unsigned long int _linkMode,
                                       int _numberOfSpatialStreams) :
    transmitter(_transmitter),
    payload(NULL),
    linkMode(_linkMode)
{
    this->setTxPower(_txPower);
    this->setNumberOfSpatialStreams(_numberOfSpatialStreams);
}


TransmissionObject::TransmissionObject(Transmitter* _transmitter,
                                       wns::osi::PDUPtr _payload,
                                       const wns::Power& _txPower,
                                       unsigned long int _linkMode,
                                       int _numberOfSpatialStreams) :
    transmitter(_transmitter),
    payload(_payload),
    linkMode(_linkMode)
{
    this->setTxPower(_txPower);
    this->setNumberOfSpatialStreams(_numberOfSpatialStreams);
}

TransmissionObject::TransmissionObject(Transmitter* _transmitter,
                                       const wns::Power& _txPower,
                                       const wns::service::phy::phymode::PhyModeInterfacePtr _phyModePtr,
                                       unsigned long int _linkMode) :
    transmitter(_transmitter),
    payload(NULL),
    linkMode(_linkMode)
{
    this->setTxPower(_txPower);
    assure(_phyModePtr,"phyModePtr==NULL");
    this->setPhyModePtr(_phyModePtr);
}


TransmissionObject::TransmissionObject(Transmitter* _transmitter,
                                       wns::osi::PDUPtr _payload,
                                       const wns::Power& _txPower,
                                       const wns::service::phy::phymode::PhyModeInterfacePtr _phyModePtr,
                                       unsigned long int _linkMode) :
    transmitter(_transmitter),
    payload(_payload),
    linkMode(_linkMode)
{
    this->setTxPower(_txPower);
    assure(_phyModePtr,"phyModePtr==NULL");
    this->setPhyModePtr(_phyModePtr);
}

TransmissionObject::~TransmissionObject()
{
}

wns::Ratio TransmissionObject::getTransmittersAntennaGain(const wns::Position& receiverPosition) const
{
    return getTransmitter()->getAntenna()->getGain(receiverPosition, rise::antenna::PatternPtr());
}

std::string
TransmissionObject::toString() const
{
    std::stringstream s;
    s << "TO(";
    s << transmitter->getDebugInfo()<<","; // rise::Transmitter
    s << "phyMode="<<getPhyModePtr()->getString();
    s <<",linkMode="<<getLinkMode();
    s << ",payload="<<*payload; // wns::osi::PDUPtr
    s << ")";
    return s.str();
}
