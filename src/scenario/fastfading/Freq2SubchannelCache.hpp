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

#ifndef _RISE_SCENARIO_FREQ2SUBCHANNELCACHE_HPP
#define _RISE_SCENARIO_FREQ2SUBCHANNELCACHE_HPP

#include <WNS/container/Registry.hpp>
#include <WNS/Types.hpp>
#include <WNS/logger/Logger.hpp>

namespace rise { namespace scenario { namespace fastfading {

    class Freq2SubchannelCache
    {
        public:
            Freq2SubchannelCache(unsigned int numberOfSubchannels);
            ~Freq2SubchannelCache();

            unsigned int
            getSubchannelIndex(const wns::Frequency& frequency) const;

        private:
            /* Using double as key is dangerous, but should work */
            mutable wns::container::Registry<double, unsigned int> frequencyToSubchannel_;

            unsigned int numberOfSubchannels_;

            wns::logger::Logger logger_;
    };

} // fastfading
} // scenario
} // rise

#endif // NOT defined _RISE_SCENARIO_FREQ2SUBCHANNELCACHE_HPP
