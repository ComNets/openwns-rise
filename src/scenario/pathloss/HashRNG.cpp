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

using namespace rise::scenario::pathloss::detail;

HashRNG::HashRNG(size_t initialSeed, wns::Position p1, wns::Position p2, double distance)
{
    static double normalize = pow(2, sizeof(std::size_t) * 8);
    size_t seed = initialSeed;
    boost::hash_combine(seed, std::max(p1.getX(),p2.getX()));
    boost::hash_combine(seed, std::max(p1.getY(),p2.getY()));
    boost::hash_combine(seed, std::max(p1.getZ(),p2.getZ()));
    boost::hash_combine(seed, std::min(p1.getX(),p2.getX()));
    boost::hash_combine(seed, std::min(p1.getY(),p2.getY()));
    boost::hash_combine(seed, std::min(p1.getZ(),p2.getZ()));
    size_t seed2 = seed;
    boost::hash_combine(seed2, distance);
    size_t seed3 = seed2;
    boost::hash_combine(seed3, distance);
    size_t seed4 = seed2;
    boost::hash_combine(seed4, seed);
    size_t seed5 = seed2;
    boost::hash_combine(seed5, seed);

    a = ( (double) seed / normalize);
    b = ( (double) seed2 / normalize);
    c = ( (double) seed3 / normalize);
    d = ( (double) seed4 / normalize);
    e = ( (double) seed5 / normalize);
    giveA = true;
}

double
HashRNG::operator()()
{
    if (giveA)
    {
        giveA = false;
        return a;
    }
    else
    {
        giveA = true;
        return b;
    }
}
