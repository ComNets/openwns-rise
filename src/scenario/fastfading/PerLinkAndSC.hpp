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

#ifndef _RISE_SCENARIO_IMTADVANCEDTRACE_HPP
#define _RISE_SCENARIO_IMTADVANCEDTRACE_HPP

#include <RISE/scenario/fastfading/FastFading.hpp>
#include <RISE/scenario/fastfading/Freq2SubchannelCache.hpp>
#include <RISE/scenario/pathloss/ILoSDependent.hpp>       

#include <WNS/PowerRatio.hpp>
#include <WNS/pyconfig/View.hpp>
#include <WNS/StaticFactory.hpp>
#include <WNS/logger/Logger.hpp>
#include <WNS/container/Registry.hpp>

namespace rise { namespace scenario { namespace fastfading {

    class PerLinkAndSC : 
        public FastFading
    {
        public:
            typedef std::pair<unsigned int, unsigned int> StationPair;
            typedef std::map<StationPair, unsigned int> LinkMap;

    	    PerLinkAndSC(const wns::pyconfig::View&);

    	    virtual 
            wns::Ratio 
            getFastFading(const antenna::Antenna& source,
		                 const antenna::Antenna& target,
		                 const wns::Frequency& frequency) const;

            virtual void
            onWorldCreated();

        protected:
            unsigned int numberOfSubchannels_;

            std::string systemName_;

        private:
            virtual wns::Ratio
            doGetFastFading(const unsigned int link, const unsigned int sc) const = 0;

            virtual void
            setLinks(const LinkMap& links, const std::set<unsigned int>& losSet) = 0;

            void
            createNewLink(const antenna::Antenna& source,
		                  const antenna::Antenna& target);

            unsigned int nextLinkId_;
            LinkMap links_;
            std::set<unsigned int> losLinks_;

            Freq2SubchannelCache f2ScCache_;

            /* Using double as key is dangerous, but should work */
            mutable wns::container::Registry<double, unsigned int> frequencyToSubchannel_;

            unsigned int usedBy_;

            wns::logger::Logger logger_;
    };

} // fastfading
} // scenario
} // rise

#endif // NOT defined _RISE_SCENARIO_PERLINKANDSC_HPP
