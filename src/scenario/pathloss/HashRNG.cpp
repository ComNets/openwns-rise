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
#include <RISE/scenario/pathloss/HashRNG.hpp>
#include <WNS/distribution/Uniform.hpp>

#include <boost/random.hpp>
#include <boost/functional/hash.hpp>
#include <sstream>

using namespace rise::scenario::pathloss::detail;

HashRNG::HashRNG(unsigned int initialSeed,
                 wns::Position p1,
                 wns::Position p2,
                 bool correlateBS, bool correlateUT):
    myHash(5381),
    uni(0.0, 1.0),
    dis(rng, uni)
{
    // First take care of the initial seed
    combine(myHash, initialSeed);

    // BS and UT are defined by the z coordinate
    // the higher one is the BS
    wns::Position bs;
    wns::Position ut;

    if (p1.getZ() > p2.getZ())
    {
        bs = p1;
        ut = p2;
    }
    else
    {
        bs = p2;
        ut = p1;
    }

    if (correlateBS && correlateUT)
    {
        // Feed to seed, but such that swapping p1 and p2 yields the same results
        // channel is reciprocal
        double xMax = std::max(bs.getX(), ut.getX());
        double xMin = std::min(bs.getX(), ut.getX());
        double yMax = std::max(bs.getY(), ut.getY());
        double yMin = std::min(bs.getY(), ut.getY());
        double zMax = std::max(bs.getZ(), ut.getZ());
        double zMin = std::min(bs.getZ(), ut.getZ());

        combine(myHash, xMax);
        combine(myHash, xMin);
        combine(myHash, yMax);
        combine(myHash, yMin);
        combine(myHash, zMax);
        combine(myHash, zMin);
    }
    else if (correlateBS)
    {
        combine(myHash, bs.getX());
        combine(myHash, bs.getY());
        combine(myHash, bs.getZ());
    }
    else if (correlateUT)
    {
        combine(myHash, ut.getX());
        combine(myHash, ut.getY());
        combine(myHash, ut.getZ());
    }
    else
    {
        assure(false, "Unsupported case in HashRNG. Correlate the RNG either to BS, UT or to both!");
    }

    rng.seed(myHash);
}

double
HashRNG::operator()()
{
    return dis();
}
