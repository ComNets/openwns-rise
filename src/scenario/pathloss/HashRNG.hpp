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
#include <boost/random.hpp>

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

/*    static const double min_value = boost::mt19937::min_value;

    static const double max_value = boost::mt19937::max_value;*/

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

private:
    template<typename T>
    void combine( unsigned int& hash, T t)
    {
        unsigned int* it= (unsigned int*)(&t);

        assure(sizeof(T) % sizeof(unsigned int) == 0, "Incompatible hash types in HashRNG::combineDouble");

        int count = sizeof(T) / sizeof(unsigned int);

        for (int ii=0; ii < count; ++ii)
        {
            hash = ((hash << 5) + hash) + *it;
            it++;
        }
    }

    /*boost::mt19937 rng;
    boost::uniform_real<> uni;
    boost::variate_generator<boost::mt19937&, boost::uniform_real<> > dis;*/

    unsigned int myHash;
    double normalize;
    
    /*bool giveA;
    double a;
    double b;
    double c;
    double d;
    double e;*/
};

} // detail
} // pathloss
} // scenario
} // rise

#endif // RISE_SCENARIO_PATHLOSS_DETAIL_HASHRNG_HPP
