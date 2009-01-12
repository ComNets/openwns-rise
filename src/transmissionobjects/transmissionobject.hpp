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


namespace rise
{
	namespace receiver {
		class ReceiverInterface;
	}

	class Transmitter;

	class TransmitterAspect
	{
	public:
		virtual ~TransmitterAspect()
		{}

		/**
		 * @brief Calculates the Gain of the antenna of the transmitter of this
		 * transmission
		 */
		virtual wns::Ratio getTransmittersAntennaGain(const wns::Position& receiverPosition) const = 0;

		virtual Transmitter* getTransmitter() const = 0;

		const wns::Power& getTxPower() const
		{
			return this->txPower;
		}

		void setTxPower(wns::Power power)
		{
			this->txPower=power;
		}

        /**
		 * @brief give PhyMode used on this object (reference)
		 */
		const wns::service::phy::phymode::PhyModeInterface& getPhyMode() const
		{
			//return this->phyMode;
			return *(this->phyModePtr); // returns reference only
		}

        /**
		 * @brief give PhyMode used on this object (Pointer)
		 */
		//const wns::service::phy::phymode::PhyModeInterface* getPhyModePtr() const
		const wns::service::phy::phymode::PhyModeInterfacePtr getPhyModePtr() const
		{
			//return &(this->phyMode);
			return this->phyModePtr;
		}
/*
		void setPhyMode(const wns::service::phy::phymode::PhyModeInterface& _phyMode)
		{
			assureNotNull(&_phyMode);
			//this->phyMode=_phyMode; // copy
			this->phyModePtr=&_phyMode; // ref
		}
*/
		//void setPhyModePtr(const wns::service::phy::phymode::PhyModeInterface* _phyModePtr)
		void setPhyModePtr(const wns::service::phy::phymode::PhyModeInterfacePtr _phyModePtr)
		{
			// using the old constructor, a NULL value is possible
			//assureNotNull(_phyModePtr);
			//this->phyMode=*_phyModePtr; // copy
			this->phyModePtr=_phyModePtr; // SmartPtr
		}

	private:
		/**
		 * @brief The transmission power which was used to send this transmission
		 */
		wns::Power txPower;
		/**
		 * @brief The PhyMode (Modulation&Coding) used for this transmission
		 */
		wns::service::phy::phymode::PhyModeInterfacePtr phyModePtr; // SmartPtr. better?
		//const wns::service::phy::phymode::PhyModeInterface* phyModePtr;
	};


// 	class NonBeamforming :
// 		virtual public TransmitterAspect
// 	{
// 	public:
// 		virtual wns::Ratio getTransmittersAntennaGain(const wns::Position& receiverPosition) const;
// 	};

// 	class Beamforming :
// 		virtual public TransmitterAspect
// 	{
// 		friend class AntennaBF;
// 	public:
// 		virtual wns::Ratio getTransmittersAntennaGain(const wns::Position& receiverPosition) const;
// 		AntennaBF* getAntenna() const;
// 	protected:
// 		AntennaBF* antennaBF;
// 		std::vector<double>* pattern;
// 	};

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
		/**
		 * @brief True until the transmission didn't stop
		 */
		bool onAir;

		/**
		 * @brief The PhysicalResource where the TransmissionObject is transmitted on
		 */
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
// 		virtual public NonBeamforming,
		virtual public wns::container::SingleFastListEnabler<wns::SmartPtr<TransmissionObject> >

	{
	public:
		// obsolete soon, if we always have a PhyMode:
		/** @todo obsolete interface if PhyMode is always specified */
		//! Constructor without PDU
		TransmissionObject(Transmitter* _transmitter,
						   const wns::Power& _txPower,
						   uint32_t _linkMode = 0);

		// obsolete soon, if we always have a PhyMode:
		/** @todo obsolete interface if PhyMode is always specified */
		//! Default constructor, used to create a TransmissionObject
		TransmissionObject(Transmitter* _transmitter,
						   wns::osi::PDUPtr _payload,
						   const wns::Power& _txPower,
						   uint32_t _linkMode = 0);

		//! Constructor without PDU
		TransmissionObject(Transmitter* _transmitter,
						   const wns::Power& _txPower,
						   //const wns::service::phy::phymode::PhyModeInterface& _phyMode,
						   const wns::service::phy::phymode::PhyModeInterfacePtr _phyModePtr,
						   uint32_t _linkMode = 0);

		//! Default constructor, used to create a TransmissionObject
		TransmissionObject(Transmitter* _transmitter,
						   wns::osi::PDUPtr _payload,
						   const wns::Power& _txPower,
						   //const wns::service::phy::phymode::PhyModeInterface& _phyMode,
						   const wns::service::phy::phymode::PhyModeInterfacePtr _phyModePtr,
						   uint32_t _linkMode = 0);

		//! Default destructor
		virtual ~TransmissionObject();

		//! Returns the link mode. Can be downlink, uplink or direct link
		uint32_t getLinkMode() const
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

		enum linkmode
		{
			downlink   = 0,
			uplink,
			directlink
		};
 		virtual wns::Ratio getTransmittersAntennaGain(const wns::Position& receiverPosition) const;
/*
		virtual std::ostream& print(std::ostream& s) const
		{
			return s << "TransmissionObject=(transmitter="<<this->transmitter<<", TxPower="<<getTxPower()<<", linkMode="<<linkMode<<", payload="<<payload<<")";
		}

		 @brief define stream operator for class PhyMode
		friend std::ostream&
		operator<< (std::ostream& s, const TransmissionObject& t) {
			//return s << "TransmissionObject=(transmitter="<<t.transmitter<<",TxPower="<<t.getTxPower()<<", linkMode="<<t.linkMode<<",payload="<<t.payload<<")";
			return s << t.print(s);
		}
*/

	private:
		Transmitter* transmitter;

		wns::osi::PDUPtr payload;

		/**
		 * Variable describing in which linkmode this TransmissionObject is
		 * sent.
		 */
		uint32_t linkMode;

	};



}

#endif // _TRANSMISSIONOBJECT_HPP


