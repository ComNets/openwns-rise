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

#ifndef _TRANSMISSIONOBJECT_HPP
#define _TRANSMISSIONOBJECT_HPP

#include <WNS/service/phy/phymode/PhyModeInterface.hpp>

#include <WNS/PowerRatio.hpp>
#include <WNS/container/FastListEnabler.hpp>
#include <WNS/SmartPtr.hpp>
#include <WNS/Assure.hpp>
#include <WNS/Position.hpp>
#include <WNS/osi/PDU.hpp>
#include <WNS/Birthmark.hpp>


namespace rise
{
    namespace receiver {
        class ReceiverInterface;
    }

    class Transmitter;

    class TransmitterAspect
    {
    public:
        TransmitterAspect() :
            txPower(),
            numberOfSpatialStreams(1)
            {}

        virtual ~TransmitterAspect()
            {}


        /**
		 * @brief Calculates the Gain of the antenna of the transmitter of this
		 * transmission
		 */
        virtual wns::Ratio
        getTransmittersAntennaGain(const wns::Position& receiverPosition) const = 0;

        virtual Transmitter*
        getTransmitter() const = 0;

        const wns::Power&
        getTxPower() const
            {
                return this->txPower;
            }

        void
        setTxPower(wns::Power power)
            {
                this->txPower=power;
            }

        /**
         * @brief give PhyMode used on this object (reference)
         */
        const wns::service::phy::phymode::PhyModeInterface&
        getPhyMode() const
            {
                return *(this->phyModePtr); // returns reference only
            }

        /**
         * @brief give PhyMode used on this object (Pointer)
         */
        const wns::service::phy::phymode::PhyModeInterfacePtr
        getPhyModePtr() const
            {
                return this->phyModePtr;
            }

        void
        setPhyModePtr(const wns::service::phy::phymode::PhyModeInterfacePtr _phyModePtr)
            {
                this->phyModePtr=_phyModePtr; // SmartPtr
            }

        const int
        getNumberOfSpatialStreams() const
            {
                return this->numberOfSpatialStreams;
            }

        void
        setNumberOfSpatialStreams(int _numSS)
            {
                assure(_numSS > 0,
                       "Cannot have negative number of spatial streams");
                this->numberOfSpatialStreams = _numSS;
            }

    private:
        /**
         * @brief The transmission power which was used to send this transmission
         */
        wns::Power txPower;
        /**
         * @brief The PhyMode (Modulation&Coding) used for this transmission
         */
        wns::service::phy::phymode::PhyModeInterfacePtr phyModePtr;

        /**
         * @brief The number of spatial streams for MIMO transmissions
         */
        int numberOfSpatialStreams;
    };

    class CastingAspect
    {
    public:
        /**
         * @brief The transmission object returns, whether it is for the
         * receiver, or not.
         */
        virtual bool isForMe(const receiver::ReceiverInterface* aReceiver) const = 0;
        virtual ~CastingAspect() {}
    };


    namespace medium {
        class PhysicalResource;
    }

    class TransmissionInterface :
        virtual public wns::RefCountable,
        virtual public wns::HasBirthmark,
        virtual public TransmitterAspect,
        virtual public CastingAspect
    {
        /**
		 * Physical Resource is friend, because it needs to set the
		 * PhysicalResource
		 */
        friend class medium::PhysicalResource;
    public:
        TransmissionInterface();

        virtual ~TransmissionInterface();

        bool getIsStart() const
            {
                return this->onAir;
            }

        void setIsStart(bool iS)
            {
                this->onAir = iS;
            }

        medium::PhysicalResource* getPhysicalResource() const
            {
                assure(this->pr != NULL, "Hasn't been set (set by PhysicalResource::startTransmission())");
                return this->pr;
            }

    private:
        /** @brief True until the transmission didn't stop */
        bool onAir;

        /** @brief The PhysicalResource where the TransmissionObject is transmitted on */
        medium::PhysicalResource* pr;

        void setPhysicalResource(medium::PhysicalResource* _pr)
            {
                assure(pr == NULL, "The TransmissionObject may only be transmitted on ONE physical resource");
                this->pr = _pr;
            }
    };

    /**
	 * @brief Gives the implicit information needed for cir calculation.
	 *
	 * This class gives the information needed in cir calculation i.e. who is
	 * the transceiver sending/I am listening to and the user data needed to give
	 * back to the protocol. This class is pure virtual; you have to use one of
	 * the following: UnicastTransmissionObject, MulticastTransmissionObject,
	 * BroadcastTransmissionObject
	 *
	 * @ingroup TRANSMISSIONOBJECT
	 */
    class TransmissionObject :
        virtual public TransmissionInterface,
        virtual public wns::container::SingleFastListEnabler<wns::SmartPtr<TransmissionObject> >

    {
    public:
        //! Constructor without PDU
        TransmissionObject(Transmitter* _transmitter,
                           const wns::Power& _txPower,
                           unsigned long int _linkMode = 0,
                           int numberOfSpatialStreams = 1);

        //! Default constructor, used to create a TransmissionObject
        TransmissionObject(Transmitter* _transmitter,
                           wns::osi::PDUPtr _payload,
                           const wns::Power& _txPower,
                           unsigned long int _linkMode = 0,
                           int numberOfSpatialStreams = 1);

        //! Constructor with spectial case numberOfSpatialStreams = 1 and phyMode
        //! given
        TransmissionObject(Transmitter* _transmitter,
                           const wns::Power& _txPower,
                           const wns::service::phy::phymode::PhyModeInterfacePtr _phyModePtr,
                           unsigned long int _linkMode = 0);

        //! Default constructor, used to create a TransmissionObject
        TransmissionObject(Transmitter* _transmitter,
                           wns::osi::PDUPtr _payload,
                           const wns::Power& _txPower,
                           const wns::service::phy::phymode::PhyModeInterfacePtr _phyModePtr,
                           unsigned long int _linkMode = 0);

        //! Default destructor
        virtual ~TransmissionObject();

        //! Returns the link mode. Can be downlink, uplink or direct link
        unsigned long int getLinkMode() const
            {
                return this->linkMode;
            }

        //! return payload
        wns::osi::PDUPtr getPayload() const
            {
                return this->payload;
            }

        virtual Transmitter* getTransmitter() const
            {
                return this->transmitter;
            }

        std::string toString() const;

        enum linkmode
        {
            downlink   = 0,
            uplink,
            directlink
        };
        virtual wns::Ratio getTransmittersAntennaGain(const wns::Position& receiverPosition) const;

    private:
        Transmitter* transmitter;

        wns::osi::PDUPtr payload;

        /**
         * Variable describing in which linkmode this TransmissionObject is
         * sent.
         */
        unsigned long int linkMode;
    };
}

#endif // _TRANSMISSIONOBJECT_HPP


