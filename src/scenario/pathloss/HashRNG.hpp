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

#ifndef RISE_SCENARIO_PATHLOSS_DETAIL_HASHRNG_HPP
#define RISE_SCENARIO_PATHLOSS_DETAIL_HASHRNG_HPP

#include <RISE/scenario/pathloss/DistanceDependent.hpp>

namespace rise { namespace scenario { namespace pathloss {

namespace detail {

/**
 * @brief Helper that can be used as pseudo-random number generator for
 * boost distributions. This may only be used to draw one random number
 * from the distribution. DO NOT USE as a random number generator!!!
 *
 * @author Daniel Bueltmann <openwns@doender.de>
 */
class HashRNG
{
public:
    HashRNG(size_t initialSeed, wns::Position p1, wns::Position p2, int32_t id1, int32_t id2, double distance);

    static const bool has_fixed_range = true;

    static const double min_value = 0.0;

    static const double max_value = 1.0;

    double
    operator()();

    double
    min()
    {
        return 0.0;
    }

    double
    max()
    {
        return 1.0;
    }

    bool giveA;
    double a;
    double b;
    double c;
    double d;
    double e;
};

} // detail
} // pathloss
} // scenario
} // rise

#endif // RISE_SCENARIO_PATHLOSS_DETAIL_HASHRNG_HPP
