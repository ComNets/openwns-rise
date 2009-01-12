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

#ifndef __RISE_POWERMEASUREMENT_HPP
#define __RISE_POWERMEASUREMENT_HPP

#include <RISE/misc/pointer.hpp>
#include <RISE/stations/station.hpp>
#include <RISE/transceiver/transmitter.hpp>
#include <WNS/service/phy/power/PowerMeasurement.hpp>
#include <WNS/service/phy/phymode/PhyModeInterface.hpp>
#include <WNS/PowerRatio.hpp>
#include <WNS/Types.hpp>
#include <WNS/pyconfig/View.hpp>

namespace rise { namespace receiver {

	/** @brief Class encapsulating all measured values for one transmission */
    class PowerMeasurement :
		public wns::service::phy::power::PowerMeasurementInterface,
		public wns::Cloneable<PowerMeasurement> // allow SmartPtr
    {
    public:
		PowerMeasurement(); // default constructor
		PowerMeasurement(wns::Power rxPower, wns::Power interference, wns::Ratio omniAttenuation);
		PowerMeasurement(rise::TransmissionObjectPtr t, wns::node::Interface* _sourceNode, wns::Power _rxPower, wns::Power _interference, wns::Ratio _omniAttenuation, int _subChannel=0);
		virtual ~PowerMeasurement();
		virtual const wns::Power getRxPower() const;
		virtual const wns::Power getInterferencePower() const; // interference contains noise here
		virtual const wns::Power getOmniInterferencePower() const;
		virtual const wns::Power getNoisePower() const;
		virtual const wns::Ratio getSINR() const;
		virtual const double     getMI()  const; // mutual information
		virtual const double     getMIB() const; // mutual information per bit [0..1]
		virtual const wns::Power getTxPower() const;
		virtual const wns::Ratio getPathLoss() const; // total including dynamic fading
		virtual const wns::Power getRSS() const; // received signal strength (S+I)
		virtual const wns::service::phy::phymode::PhyModeInterfacePtr getPhyMode() const;

		virtual const int getSubChannel() const; // OFDMA subchannel used
		virtual const rise::Station* getSourceStation() const; // transmitter station

		virtual wns::node::Interface* getSourceNode() const; // transmitter node (only available for OFDMAPhy)

		virtual std::string getString() const;
    private:
		wns::Power rxPower; // got from receiveData()
		wns::Power interference; // got from receiveData() // interference contains noise
		wns::Ratio omniAttenuation; // got from receiveData()
		wns::Power noise; // TODO
		wns::Power txPower; // from TransmissionObjectPtr
		wns::service::phy::phymode::PhyModeInterfacePtr phyModePtr; // got from TransmissionObjectPtr; is a SmartPtr

		int subChannel; // OFDMA subchannel used
		rise::TransmissionObjectPtr transmissionObjectPtr;
		rise::Station* transmitterStation; // transmitter station

		wns::node::Interface* sourceNode; // transmitter node (id)
   };
	/** @brief define stream operator for class PowerMeasurement */
	inline std::ostream&
	operator<< (std::ostream& s, const rise::receiver::PowerMeasurement& p) {
		return s << p.getString();
	}
}}

#endif // NOT defined __RISE_POWERMEASUREMENT_HPP
