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

#include <RISE/scenario/pathloss/ITUPathloss.hpp>
#include <WNS/distribution/Uniform.hpp>

#include <boost/random.hpp>
#include <boost/functional/hash.hpp>

using namespace rise::scenario::pathloss;

ITUPathloss::ITUPathloss(const wns::pyconfig::View& pyco):
    rise::scenario::pathloss::DistanceDependent(pyco),
    losProbabilityCC_("rise.scenario.pathloss.ITUPathloss.losProbability"),
    shadowingCC_("rise.scenario.pathloss.ITUPathloss.shadowing")
{}

wns::Ratio
ITUPathloss::calculatePathloss(const rise::antenna::Antenna& source,
                               const rise::antenna::Antenna& target,
                               const wns::Frequency& frequency,
                               const wns::Distance& distance) const
{
    static wns::distribution::Uniform dis(0.0, 1.0, wns::simulator::getRNG());
    static size_t initialSeed = dis() * pow(2, sizeof(size_t)*8);

    detail::HashRNG hrng(initialSeed, source.getPosition(), target.getPosition(), distance);

    wns::Ratio pl;

    if (hrng.c < getLOSProbability(distance))
    {
        losProbabilityCC_.put(distance);
        pl = getLOSPathloss(source, target, frequency, distance);

        boost::normal_distribution<double> shadow(0.0, getLOSShadowingStd(source, target, frequency, distance));
        double sh = shadow(hrng);
        shadowingCC_.put(sh);
        pl += wns::Ratio::from_dB(sh);
        pl.los = true;
    }
    else
    {
        pl = getNLOSPathloss(source, target, frequency, distance);
        boost::normal_distribution<double> shadow(0.0, getNLOSShadowingStd(source, target, frequency, distance));
        double sh = shadow(hrng);
        shadowingCC_.put(sh);
        pl += wns::Ratio::from_dB(sh);
        pl.los = false;
    }

    return pl;
}
