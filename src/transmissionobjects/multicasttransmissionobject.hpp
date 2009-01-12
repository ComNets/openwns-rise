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

#ifndef _MULTICASTTRANSMISSIONOBJECT_HPP
#define _MULTICASTTRANSMISSIONOBJECT_HPP

#include <RISE/transmissionobjects/transmissionobject.hpp>
#include <vector>

namespace rise {
	//! Gives the implicit information needed for cir calculation.
	/** This class gives the information needed in cir calculation, when
	    multicasting. */
	/** @ingroup TRANSMISSIONOBJECT */
	class MulticastTransmissionObject :
		public TransmissionObject
	{
		typedef std::vector<receiver::ReceiverInterface*> ReceiverContainer;
	public:
		//! Default constructor, used to create a TransmissionObject
		MulticastTransmissionObject(Transmitter* aTransmitter,
									ReceiverContainer multiGroup,
									uint32_t aLinkMode,
									wns::osi::PDUPtr aPayload,
									wns::Power power);

		MulticastTransmissionObject(Transmitter* aTransmitter,
									ReceiverContainer multiGroup,
									uint32_t aLinkMode,
									wns::osi::PDUPtr aPayload,
									wns::Power power,
									const wns::service::phy::phymode::PhyModeInterfacePtr _phyModePtr);

		//! Default destructor
		virtual ~MulticastTransmissionObject();

		/** If the receiver is in the multicast group, isForMe should
		    return true, else false.*/
		virtual bool isForMe(const receiver::ReceiverInterface* r) const;

		//! Add a receiver to the multicast group
		void addReceiver(receiver::ReceiverInterface* r);

	private:
		//! The group of receivers intended to receive this transmission object
		ReceiverContainer multiGroup;
	};
}

#endif // _MULTICASTTRANSMISSIONOBJECT_HPP



