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

#include <RISE/transmissionobjects/transmissionobjectbf.hpp>
#include <RISE/transceiver/transmitter.hpp>

using namespace rise;


//! Default constructor, used to create a TransmissionObject
TransmissionObjectBF::TransmissionObjectBF(Transmitter* _transmitter,
										   receiver::ReceiverInterface* _aReceiver,
										   antenna::Beamforming* _antennaBF,
										   wns::osi::PDUPtr _payload,
										   wns::Power _txPower,
										   rise::antenna::PatternPtr _pattern,
										   uint32_t _linkMode)
	: UnicastTransmissionObject(_transmitter, _aReceiver, _linkMode, _payload, _txPower),
	  antennaBF(_antennaBF),
	  pattern(_pattern)
{}

//! Default constructor, used to create a TransmissionObject
TransmissionObjectBF::TransmissionObjectBF(Transmitter* _transmitter,
										   receiver::ReceiverInterface* _aReceiver,
										   antenna::Beamforming* _antennaBF,
										   wns::osi::PDUPtr _payload,
										   wns::Power _txPower,
										   //const wns::service::phy::phymode::PhyModeInterface& _phyMode,
										   const wns::service::phy::phymode::PhyModeInterfacePtr _phyModePtr,
										   rise::antenna::PatternPtr _pattern,
										   uint32_t _linkMode)
	: UnicastTransmissionObject(_transmitter, _aReceiver, _linkMode, _payload, _txPower, _phyModePtr),
	  antennaBF(_antennaBF),
	  pattern(_pattern)
{}

// //! Constructor without PDU
// BeamformingTransmissionObject::BeamformingTransmissionObject(Transmitter* _transmitter,
// 										   receiver::ReceiverInterface* _aReceiver,
// 										   wns::Power _txPower,
// 										   rise::antenna::PatternId _patternId,
// 										   uint32_t _linkMode)
// 	: UnicastTransmissionObject(_transmitter, _aReceiver, _linkMode, _txPower),
// 	  pattern(_patternId)
// {
// 	//antenna must be a beamforming antenna !!!
// 	assure(dynamic_cast<antenna::Beamforming*>(_transmitter->getAntenna()), "transmitter is not beamforming capable");
// 	antennaBF = dynamic_cast<antenna::Beamforming*>(_transmitter->getAntenna());
// }

//! Default destructor
TransmissionObjectBF::~TransmissionObjectBF()
{}

wns::Ratio TransmissionObjectBF::getTransmittersAntennaGain(const wns::Position& receiverPosition) const
{
	return antennaBF->getGain(receiverPosition, pattern);
}
