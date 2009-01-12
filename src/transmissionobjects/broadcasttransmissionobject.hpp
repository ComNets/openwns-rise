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

#ifndef _BROADCASTTRANSMISSIONOBJECT_HPP
#define _BROADCASTTRANSMISSIONOBJECT_HPP

#include <RISE/transmissionobjects/transmissionobject.hpp>

namespace rise
{
	//! Gives the implicit information needed for cir calculation.
	/** This class gives the information needed in cir calculation,
	    when broadcasting. */
	/** @ingroup TRANSMISSIONOBJECT */
	class BroadcastTransmissionObject : public TransmissionObject {
	public:
		//! Default constructor, used to create a TransmissionPbject
		// obsolete soon, if we always have a PhyMode:
		/** @todo obsolete interface if PhyMode is always specified */
		BroadcastTransmissionObject(Transmitter* aTransmitter,
									wns::osi::PDUPtr aPayload,
									wns::Power power,
									uint32_t aLinkMode = 0);
		//! Default constructor, used to create a TransmissionPbject
		BroadcastTransmissionObject(Transmitter* aTransmitter,
									wns::osi::PDUPtr aPayload,
									wns::Power power,
									//const wns::service::phy::phymode::PhyModeInterface& _phyMode,
									const wns::service::phy::phymode::PhyModeInterfacePtr _phyModePtr,
									uint32_t aLinkMode = 0);
		/** isForMe of BroadcastTransmissionObject always returns true. You have to
		    check for yourself, if you are associated to the sender. */
		virtual bool isForMe(const receiver::ReceiverInterface*) const {
			return true;
		};

		//! Default destructor
		virtual ~BroadcastTransmissionObject();

	protected:
	};
}

#endif // _BROADCASTTRANSMISSIONOBJECT_HPP


