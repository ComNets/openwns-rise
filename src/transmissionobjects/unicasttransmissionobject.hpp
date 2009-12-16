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

#ifndef _UNICASTTRANSMISSIONOBJECT_HPP
#define _UNICASTTRANSMISSIONOBJECT_HPP

#include <RISE/transmissionobjects/transmissionobject.hpp>

namespace rise {
    //! Gives the implicit information needed for cir calculation.
    /** This class gives the information needed in cir calculation, when
     *  unicasting. */
    /** @ingroup TRANSMISSIONOBJECT */
    class UnicastTransmissionObject
        : public TransmissionObject
    {
    public:
        //! Default constructor, used to create a TransmissionObject
        UnicastTransmissionObject(Transmitter* aTransmitter,
                                  receiver::ReceiverInterface* aReceiver,
                                  uint32_t aLinkMode,
                                  wns::osi::PDUPtr aPayload,
                                  wns::Power power,
                                  int numberOfSpatialStreams);

        //! Default constructor, used to create a TransmissionObject
        UnicastTransmissionObject(Transmitter* aTransmitter,
                                  receiver::ReceiverInterface* aReceiver,
                                  uint32_t aLinkMode,
                                  wns::osi::PDUPtr aPayload,
                                  wns::Power power,
                                  const wns::service::phy::phymode::PhyModeInterfacePtr _phyModePtr);

        /** If the receiver is the intended receiver of the unicast, it
         * should return true, else false. */
        virtual bool
        isForMe(const receiver::ReceiverInterface* aReceiver) const
            {
                return (pd_receiver == aReceiver);
            };

        virtual receiver::ReceiverInterface*
        getReceiver()
            {
                return pd_receiver;
            }

        //! Default destructor
        virtual ~UnicastTransmissionObject();

    protected:
        //! The receiver intended to receive this transmission object.
        receiver::ReceiverInterface* pd_receiver;
    };
}

#endif // _UNICASTTRANSMISSIONOBJECT_HPP


