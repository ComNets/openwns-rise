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

#ifndef RISE_PLMAPPING_PHYMODEMAPPER_HPP
#define RISE_PLMAPPING_PHYMODEMAPPER_HPP

#include <string>
#include <vector>
#include <WNS/PowerRatio.hpp>
#include <WNS/Interval.hpp>
#include <WNS/container/Registry.hpp>
#include <WNS/container/RangeMap.hpp>
#include <WNS/StaticFactory.hpp>
#include <WNS/pyconfig/View.hpp>
#include <WNS/service/phy/phymode/PhyModeInterface.hpp>
#include <WNS/service/phy/phymode/PhyModeMapperInterface.hpp>

#include <RISE/plmapping/PhyMode.hpp>
#include <RISE/plmapping/CodeRates.hpp>
#include <RISE/plmapping/MI2PER.hpp>
#include <RISE/plmapping/SNR2MI.hpp>

//namespace wns { namespace service { namespace phy { namespace phymode {
namespace rise { namespace plmapping {

        /** @brief Helper class to find the best PHYmode depending on SINR */
        /** @see used in RegistryProxyWINPROST for ResourceScheduler */
        /** rise::plmapping::PhyModeMapper */
        /** @author Rainer Schoenen (rs@comnets.rwth-aachen.de) */
        class PhyModeMapper :
            public virtual wns::service::phy::phymode::PhyModeMapperInterface // in libwns
        {
        public:
            /** @brief indexed list of all usable PhyModes, in order of quality */
            typedef wns::SmartPtr<PhyMode> PhyModePtr;
            typedef std::vector< PhyModePtr > PhyModeVector;

        private:
            unsigned int subCarriersPerSubChannel;
            simTimeType symbolDuration;
            unsigned int phyModeCount;
            double minimumSINR;

            /** @brief indexed list of all usable PhyModes, in order of quality */
            PhyModeVector phyModeVector;

            /** @brief map SINR values (double dB) to phymodeIndex */
            typedef wns::container::RangeMap<double, unsigned int> PhyModeRangeMap;

            /** @brief map SINR range/interval (double dB) to PHYMode */
            PhyModeRangeMap phyModeRangeMap;

            /** @brief maps from the PhyMode to the suitable SINRRange */
            /** (opposite of PhyModeRangeMap) */
            typedef wns::container::Registry< PhyMode, wns::service::phy::phymode::SINRRange > PhyModeSINRRangeRegistry;

            /** @brief maps from the PhyMode to the suitable SINRRange */
            PhyModeSINRRangeRegistry phyModeSINRRangeRegistry;

            /** @brief required global mappers to support calculateSINRRanges() */
            wns::service::phy::phymode::MI2PERMappingInterface* mi2perMapper;
            wns::service::phy::phymode::SNR2MIInterface* SNR2MImapper;
            /** @brief the Logger */
            wns::logger::Logger logger;

        public:
            // Constructor
            PhyModeMapper(const wns::pyconfig::View& config);

            virtual ~PhyModeMapper() {}

            /** @brief find best PhyMode for a given SINR */
            virtual wns::service::phy::phymode::PhyModeInterfacePtr
            getBestPhyMode(const wns::Ratio& sinr) const;

            /** @brief find best PhyMode for a given SINR */
            //virtual wns::SmartPtr<const wns::service::phy::phymode::PhyModeInterface>
            virtual wns::service::phy::phymode::PhyModeInterfacePtr
            getBestPhyMode(double sinr /* dB */) const;

            /** @brief find best PhyModeIndex for a given SINR (faster than above) */
            int
            getBestPhyModeIndex(double sinr /* dB */) const;

            /** @brief find required (minimum) SINR for a certain PhyMode */
            virtual wns::Ratio
            getMinSINRRatio(const wns::service::phy::phymode::PhyModeInterfacePtr phyMode) const;

            /** @brief find required (minimum) SINR for a certain PhyMode */
            virtual double
            getMinSINR(const wns::service::phy::phymode::PhyModeInterfacePtr phyMode) const;

            /** @brief find possible SINR range for a certain PhyMode */
            virtual wns::service::phy::phymode::SINRRange
            getSINRRange(const wns::service::phy::phymode::PhyModeInterfacePtr phyMode) const;

            /** @brief best PhyMode of all available (most bits/symbol) */
            //virtual wns::SmartPtr<const wns::service::phy::phymode::PhyModeInterface>
            virtual const wns::service::phy::phymode::PhyModeInterfacePtr
            getHighestPhyMode() const;

            /** @brief worst PhyMode of all available (least bits/symbol) */
            //virtual wns::SmartPtr<const wns::service::phy::phymode::PhyModeInterface>
            virtual const wns::service::phy::phymode::PhyModeInterfacePtr
            getLowestPhyMode() const;

            /** @brief get number of available PhyModes */
            virtual int
            getPhyModeCount() const;

            /** @brief list (reference) of all available PhyModes */
            virtual PhyModeVector
            getListOfPhyModes() const;

            /** @brief list (copy) of all available PhyModes */
            virtual const std::vector< wns::service::phy::phymode::PhyModeInterfacePtr >
            getListOfPhyModePtr() const;

            /** @brief get PhyMode at index position. O(1) */
            virtual wns::service::phy::phymode::PhyModeInterfacePtr
            getPhyModeForIndex(int index) const;

            /** @brief get index for PhyMode in the mapper list. O(n) */
            virtual int
            getIndexForPhyMode(const wns::service::phy::phymode::PhyModeInterface& phyMode) const;

            /** @brief below this minimumSINR [dB] there is no useful transmission (PER too high) */
            virtual double getMinimumSINR() const { return minimumSINR; };

            /** @brief tells if a useful transmission is possible */
            virtual bool sinrIsAboveLimit(double sinr /* dB */) const { return (sinr>=minimumSINR); };
            virtual bool sinrIsAboveLimit(const wns::Ratio& sinr) const { return (sinr.get_dB()>=minimumSINR); };

            /**
             * @brief Determine suitable SINR ranges.
             *
             * Depends on maximum tolerated PER and assumed
             * blockLength a small blockLength results in
             * conservative, more reliable ranges.
             */
            virtual void
            calculateSINRRanges(double targetPER, unsigned int bl);

            /** @brief return a line containing all switching points in Matlab-suitable format
             * [  0.9    2.1    3.8    7.7    9.8   12.6   15.0   18.2  23.0 ] */
            std::string
            printSwitchingPoints() const;

            /** @brief return a line containing all PhyMode names in Matlab-suitable format
             */
            std::string
            printPhyModeNames() const;

        protected:
            /** @brief set minimum SINR required for lowest PhyMode */
            virtual void setMinimumSINR(double sinr) { minimumSINR=sinr; }
            /** @brief get PhyModeSINRRangeRegistry (only for jxu) */
            //virtual const rise::plmapping::PhyModeSINRRangeRegistry&
            //getPhyModeSINRRangeRegistry() const;
        };

}}
//}}}}

#endif // RISE_PLMAPPING_PHYMODEMAPPER_HPP
