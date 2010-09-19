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

#include <vector>

namespace rise { namespace receiver {

    /** @brief Class encapsulating all measured values for one transmission */
    class PowerMeasurement :
        public wns::service::phy::power::PowerMeasurementInterface,
        public wns::Cloneable<PowerMeasurement> // allow SmartPtr
    {
    public:

        PowerMeasurement(rise::TransmissionObjectPtr t,
                         wns::node::Interface* _sourceNode,
                         wns::Power _rxPower,
                         wns::Power _interference,
                         wns::Ratio _iot,
                         wns::Ratio _fading,
                         wns::Ratio _omniAttenuation,
                         std::vector<wns::Ratio> _postProcessingSINRFactor);

        virtual ~PowerMeasurement();

        virtual const wns::Power
        getRxPower() const;

        // interference contains noise here
        virtual const wns::Power
        getInterferencePower() const;

        virtual const wns::Power
        getOmniInterferencePower() const;

        /**
         * @brief Returns the Interference over Thermal Noise Ration
         */
        virtual const wns::Ratio
        getIoT() const;

        virtual const wns::Ratio
        getSINR() const;

        virtual const std::vector<wns::Ratio>
        getPostProcessingSINRFactor() const;

        // mutual information
        virtual const double
        getMI()  const;

        // mutual information per bit [0..1]
        virtual const double
        getMIB() const;

        virtual const wns::Power
        getTxPower() const;

        // not including dynamic fading
        virtual const wns::Ratio
        getPathLoss() const;

        // total including dynamic fading
        virtual const wns::Ratio
        getLoss() const;

        // dynamic fading
        virtual const wns::Ratio
        getFading() const;

         // received signal strength (S+I)
        virtual const wns::Power
        getRSS() const;

        virtual const wns::service::phy::phymode::PhyModeInterfacePtr
        getPhyMode() const;

         // transmitter station
        virtual const rise::Station*
        getSourceStation() const;

         // transmitter node (only available for OFDMAPhy)
        virtual wns::node::Interface*
        getSourceNode() const;

        virtual std::string
        getString() const;

    private:

        wns::Power rxPower;
        wns::Power interference;
        wns::Ratio iot;
        wns::Ratio omniAttenuation;
        wns::Ratio fading;
        std::vector<wns::Ratio> postProcessingSINRFactor;

        wns::service::phy::phymode::PhyModeInterfacePtr phyModePtr;

        rise::TransmissionObjectPtr transmissionObjectPtr;
        rise::Station* transmitterStation;

        wns::node::Interface* sourceNode; // transmitter node (id)
    };
    /** @brief define stream operator for class PowerMeasurement */
    inline std::ostream&
    operator<< (std::ostream& s, const rise::receiver::PowerMeasurement& p) {
        return s << p.getString();
    }
}}

#endif // NOT defined __RISE_POWERMEASUREMENT_HPP
