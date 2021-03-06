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

#include <RISE/scenario/pathloss/ITUUMi.hpp>
#include <RISE/stations/station.hpp>

#include <WNS/StaticFactoryBroker.hpp>
#include <RISE/scenario/pathloss/HashRNG.hpp>

#include <boost/functional/hash.hpp>

STATIC_FACTORY_BROKER_REGISTER(rise::scenario::pathloss::ITUUMi, rise::scenario::pathloss::Pathloss, "ITUUMi");

using namespace rise::scenario::pathloss;

ITUUMi::ITUUMi(const wns::pyconfig::View& pyco):
    DistanceDependent(pyco),
    losProbabilityCC_("rise.scenario.pathloss.ITUPathloss.losProbability"),
    shadowingCC_("rise.scenario.pathloss.ITUPathloss.shadowing"),
    useShadowing_(pyco.get<bool>("useShadowing"))
{
    outdoorProbability = pyco.get<double>("outdoorProbability");
}

wns::Ratio
ITUUMi::calculatePathloss(const antenna::Antenna& source,
			  const antenna::Antenna& target,
			  const wns::Frequency& frequency,
			  const wns::Distance& distance) const
{
    unsigned int initialSeed = getInitialSeed();

    detail::HashRNG hrng(initialSeed,
                         source.getPosition(),
                         target.getPosition(),
                         true, true);

    detail::HashRNG hrngOnlyUTPos(initialSeed + 2637,
                                source.getPosition(),
                                target.getPosition(),
                                false, true);

    wns::Ratio pl;
    double sh = 0.0;
    if (hrng() < getLOSProbability(distance))
    {
        assure(isLoS(source, target, distance), 
            "calculatePathloss() and is isLoS() disagree");

        losProbabilityCC_.put(distance);
        pl = getLOSPathloss(source, target, frequency, distance);
        if (useShadowing_)
        {
            boost::normal_distribution<double> shadow(0.0, getLOSShadowingStd(source, target, frequency, distance));
            sh = shadow(hrng);
        }
    }
    else
    {
        pl = getNLOSPathloss(source, target, frequency, distance);
        if (useShadowing_)
        {
            boost::normal_distribution<double> shadow(0.0, getNLOSShadowingStd(source, target, frequency, distance));
            sh = shadow(hrng);
        }
    }

    bool isIndoor = hrngOnlyUTPos() > outdoorProbability;

    if (isIndoor)
    {
        double d_in = 0.0;
        if (distance > 25.0)
        {
            //d_in is uniformly distributed between 0 and 25
            d_in = hrng() * 25.0;
        }
        else
        {
            //d_in is uniformly distributed between 0 and distance
            d_in = hrng() * distance;
        }
        pl += wns::Ratio::from_dB(20.0 + 0.5 * d_in);

        if (useShadowing_)
        {
            boost::normal_distribution<double> shadow(0.0, getIndoorShadowingStd(source, target, frequency, distance));
            sh = shadow(hrng);
        }
      }
    shadowingCC_.put(sh);
    pl += wns::Ratio::from_dB(sh);
    return pl;
}

double
ITUUMi::getLOSProbability(double distance) const
{
    double pLOS = std::min(1.0, 18.0/distance) * (1 - exp(-distance/36.0)) + exp(-distance/36.0);
    return pLOS;

}

wns::Ratio
ITUUMi::getLOSPathloss(const rise::antenna::Antenna& source,
                       const rise::antenna::Antenna& target,
                       const wns::Frequency& frequency,
                       const wns::Distance& distance) const
{
    double bsHeight = source.getPosition().getZ();
    double utHeight = target.getPosition().getZ();

    // We must assume here that the higher one is the BS
    if (bsHeight < utHeight) std::swap(bsHeight, utHeight);

    assure(bsHeight == 10.0, "BS Height must be 10 m");
    assure(utHeight >= 1.5, "BS Height must be at least 1 m");
    assure(utHeight < 10.0, "BS Height cannot be larger than 10 m");

    // For UMiLOS the effictive heights are used heff = h - 1.0
    bsHeight -= 1.0;
    utHeight -= 1.0;

    double dBP = 4 * bsHeight * utHeight * frequency / 3.0e02;
    double pl;
    if (distance < dBP)
    {
      pl = 22.0 * log10(distance) + 28.0 + 20 * log10(frequency/1000.0);
      return wns::Ratio::from_dB(pl);
    }
    else
    {
        pl = 40 * log10(distance) + 7.8;
        pl -= 18.0 * log10(bsHeight) + 18.0 * log10(utHeight);
        pl += 2.0 * log10(frequency/1000.0);
	return wns::Ratio::from_dB(pl);
    }
}

wns::Ratio
ITUUMi::getNLOSPathloss(const rise::antenna::Antenna& source,
                        const rise::antenna::Antenna& target,
                        const wns::Frequency& frequency,
                        const wns::Distance& distance) const
{
    double bsHeight = source.getPosition().getZ();
    double utHeight = target.getPosition().getZ();

    // We must assume here that the higher one is the BS
    if (bsHeight < utHeight) std::swap(bsHeight, utHeight);

    assure(bsHeight == 10.0, "BS Height must be 10 m");
    assure(utHeight >= 1.0, "BS Height must be at least 1 m");
    assure(utHeight <= 2.5, "BS Height cannot be larger than 2.5 m");

    double pl =  36.7 * log10(distance) + 22.7 + 26.0 * log10(frequency/1000.0);
    return wns::Ratio::from_dB(pl);
}

double
ITUUMi::getLOSShadowingStd(const rise::antenna::Antenna& source,
                           const rise::antenna::Antenna& target,
                           const wns::Frequency& frequency,
                           const wns::Distance& distance) const
{
      assure(distance > 10.0, "This model is only valid for a minimum distance of 10m");
      assure(distance < 5000.0, "This model is only valid for a maximum distance of 5000m");
      //standard deviation of 3dB
      return 3;
}

double
ITUUMi::getNLOSShadowingStd(const rise::antenna::Antenna& source,
                            const rise::antenna::Antenna& target,
                            const wns::Frequency& frequency,
                            const wns::Distance& distance) const
{
       assure(distance > 10.0, "This model is only valid for a minimum distance of 10m");
      assure(distance < 2000.0, "This model is only valid for a maximum distance of 2000m");
      //standard deviation of 4dB
      return 4;
}

double
ITUUMi::getIndoorShadowingStd(const rise::antenna::Antenna& source,
                            const rise::antenna::Antenna& target,
                            const wns::Frequency& frequency,
                            const wns::Distance& distance) const
{
    assure(distance > 10.0, "This model is only valid for a minimum distance of 10m");
    assure(distance < 1000.0, "This model is only valid for a maximum distance of 1000m");

    //standard deviation of 7dB
    return 7;
}

double
ITUUMi::getCarPenetrationStd() const
{
    return 0.0;
}

double
ITUUMi::getCarPenetrationMean() const
{
    return 0.0;
}

/* This is not a nice way to do it since it is the same code as above */
/* Still we do not want to loose performance by implementing a "getHashRNG()" */
/* method. */
bool
ITUUMi::isLoS(const rise::antenna::Antenna& source,
                               const rise::antenna::Antenna& target,
                               const wns::Distance& distance) const
{   
    detail::HashRNG hrng(getInitialSeed(),
                         source.getPosition(),
                         target.getPosition(),
                         true, true);

    return(hrng() < getLOSProbability(distance));
}

unsigned int 
ITUUMi::getInitialSeed() const
{
    static wns::distribution::Uniform dis(0.0, 1.0, wns::simulator::getRNG());
    static unsigned int initialSeed = dis() * pow(2, sizeof(unsigned int)*8);
    return initialSeed;
}



