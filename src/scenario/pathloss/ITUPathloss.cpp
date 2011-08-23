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
#include <RISE/stations/station.hpp>

#include <boost/random.hpp>
#include <boost/functional/hash.hpp>

using namespace rise::scenario::pathloss;

ITUPathloss::ITUPathloss(const wns::pyconfig::View& pyco):
    rise::scenario::pathloss::DistanceDependent(pyco),
    losProbabilityCC_("rise.scenario.pathloss.ITUPathloss.losProbability"),
    shadowingCC_("rise.scenario.pathloss.ITUPathloss.shadowing"),
    useShadowing_(pyco.get<bool>("useShadowing")),
    useCarPenetration_(pyco.get<bool>("useCarPenetration"))
{}

wns::Ratio
ITUPathloss::calculatePathloss(const rise::antenna::Antenna& source,
                               const rise::antenna::Antenna& target,
                               const wns::Frequency& frequency,
                               const wns::Distance& distance) const
{   
    unsigned int initialSeed = getInitialSeed();

    detail::HashRNG hrng(initialSeed,
                         source.getPosition(),
                         target.getPosition(),
                         true, true);

    wns::Ratio pl;

    if (hrng() < getLOSProbability(distance))
    {
        assure(isLoS(source, target, frequency, distance), 
            "calculatePathloss() and is isLoS() disagree");

        losProbabilityCC_.put(distance);
        pl = getLOSPathloss(source, target, frequency, distance);

	double sh = 0.0;
        if (useShadowing_)
        {
            boost::normal_distribution<double> shadow(0.0, getLOSShadowingStd(source, target, frequency, distance));
            sh = shadow(hrng);
            pl += wns::Ratio::from_dB(sh);
        }
        shadowingCC_.put(sh);
   }
    else
    {
        pl = getNLOSPathloss(source, target, frequency, distance);
	double sh = 0.0;
        if (useShadowing_)
        {
            boost::normal_distribution<double> shadow(0.0, getNLOSShadowingStd(source, target, frequency, distance));
            sh = shadow(hrng);
            pl += wns::Ratio::from_dB(sh);
        }
        shadowingCC_.put(sh);
    }

    double penetrationLoss = 0.0;
    if (useCarPenetration_)
    {
        // Take car of car penetration loss
        // Pathloss models that do not exhibit car penetration
        // should return zero mean and zero std as distribution parameters

        // Only take into account the UT for in-car penetration loss
        detail::HashRNG onlyUT(initialSeed + 2637,
                                source.getPosition(),
                                target.getPosition(),
                                false, true);

        boost::normal_distribution<double> shadow(getCarPenetrationMean(), getCarPenetrationStd());
        penetrationLoss = shadow(onlyUT);
    }

    return pl + wns::Ratio::from_dB(penetrationLoss);;
}

/* This is not a nice way to do it since it is the same code as above */
/* Still we do not want to loose performance by implementing a "getHashRNG()" */
/* method. */
bool
ITUPathloss::isLoS(const rise::antenna::Antenna& source,
                               const rise::antenna::Antenna& target,
                               const wns::Frequency& frequency,
                               const wns::Distance& distance) const
{   
    detail::HashRNG hrng(getInitialSeed(),
                         source.getPosition(),
                         target.getPosition(),
                         true, true);

    return(hrng() < getLOSProbability(distance));
}

unsigned int 
ITUPathloss::getInitialSeed() const
{
    static wns::distribution::Uniform dis(0.0, 1.0, wns::simulator::getRNG());
    static unsigned int initialSeed = dis() * pow(2, sizeof(unsigned int)*8);
    return initialSeed;
}


