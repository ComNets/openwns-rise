/*******************************************************************************
 * This file is part of openWNS (open Wireless Network Simulator)
 * _____________________________________________________________________________
 *
 * Copyright (C) 2004-2007
 * Chair of Communication Networks (ComNets)
 * Kopernikusstr. 5, D-52074 Aachen, GeSMany
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

#include <RISE/scenario/pathloss/ITUSMa.hpp>
#include <RISE/scenario/pathloss/HashRNG.hpp>

#include <WNS/distribution/Uniform.hpp>

#include <boost/random.hpp>
#include <boost/functional/hash.hpp>

#include <WNS/StaticFactoryBroker.hpp>

STATIC_FACTORY_BROKER_REGISTER(rise::scenario::pathloss::ITUSMa, rise::scenario::pathloss::Pathloss, "ITUSMa");

using namespace rise::scenario::pathloss;

ITUSMa::ITUSMa(const wns::pyconfig::View& pyco):
    rise::scenario::pathloss::DistanceDependent(pyco),
    losProbabilityCC_("rise.scenario.pathloss.ITUPathloss.losProbability"),
    shadowingCC_("rise.scenario.pathloss.ITUPathloss.shadowing")
{
    streetWidth_ = pyco.get<double>("streetWidth");
    buildingHeight_ = pyco.get<double>("buildingHeight");
}

wns::Ratio
ITUSMa::calculatePathloss(const rise::antenna::Antenna& source,
                               const rise::antenna::Antenna& target,
                               const wns::Frequency& frequency,
                               const wns::Distance& distance) const
{
    static wns::distribution::Uniform dis(0.0, 1.0, wns::simulator::getRNG());
    static size_t initialSeed = dis() * pow(2, sizeof(size_t)*8);
    static double normalize = pow(2, sizeof(std::size_t) * 8);

    detail::HashRNG hrng(initialSeed, source.getPosition(), target.getPosition(), distance);

    wns::Ratio pl;

    double bsHeight = source.getPosition().getZ();
    double utHeight = target.getPosition().getZ();

    // We must assume here that the higher one is the BS
    if (bsHeight < utHeight) std::swap(bsHeight, utHeight);

    assure(bsHeight > 10.0, "BS Height must be at least 10 m");
    assure(bsHeight < 150.0, "BS Height cannot be larger than 150 m");
    assure(utHeight > 1.0, "BS Height must be at least 1 m");
    assure(utHeight < 10.0, "BS Height cannot be larger than 10 m");

    double dBP = 2 * 3.14 * bsHeight * utHeight * frequency / 3.0e02;

    size_t seed = initialSeed;
    if (source.getPosition().getZ() < target.getPosition().getZ())
    {
        boost::hash_combine(seed, source.getPosition().getX());
        boost::hash_combine(seed, source.getPosition().getY());
        boost::hash_combine(seed, source.getPosition().getZ());
    }
    else
    {
        boost::hash_combine(seed, target.getPosition().getX());
        boost::hash_combine(seed, target.getPosition().getY());
        boost::hash_combine(seed, target.getPosition().getZ());
    }

    bool isIndoor = (seed/normalize) < 0.5;

    if (hrng.c < getLOSProbability(distance))
    {
        losProbabilityCC_.put(distance);

        pl = getLOSPathloss(source, target, frequency, distance);

        double shadowingStd = 0.0;
        double shadowingMean = 0.0;
        if (isIndoor)
        {
            shadowingMean = 20.0;
            if (distance < dBP)
            {
                shadowingStd = 4.0;
            }
            else
            {
                shadowingStd = 6.0;
            }
        }else
        {
            shadowingMean = 9.0;

            if (distance < dBP)
            {
                shadowingStd = 6.403124237;
            }
            else
            {
                shadowingStd = 7.810249676;
            }
        }
        boost::normal_distribution<double> shadow(shadowingMean, shadowingStd);
        double sh = shadow(hrng);
        shadowingCC_.put(sh);
        pl += wns::Ratio::from_dB(sh);
        pl.los = true;
    }
    else
    {
        pl = getNLOSPathloss(source, target, frequency, distance);

        double shadowingStd = 0.0;
        double shadowingMean = 0.0;
        if (isIndoor)
        {
            shadowingMean = 20.0;
            shadowingStd = 8.0;
        }else
        {
            shadowingMean = 9.0;
            shadowingStd = 9.433981132;
        }
        boost::normal_distribution<double> shadow(shadowingMean, shadowingStd);
        double sh = shadow(hrng);
        shadowingCC_.put(sh);
        pl += wns::Ratio::from_dB(sh);
        pl.los = false;
    }
    return pl;
}

double
ITUSMa::getLOSProbability(double distance) const
{
    if (distance <= 10.0)
    {
        return 1.0;
    }

    double pLOS = exp(- (distance-10.0)/200.0);
    return pLOS;
}

wns::Ratio
ITUSMa::getLOSPathloss(const rise::antenna::Antenna& source,
                       const rise::antenna::Antenna& target,
                       const wns::Frequency& frequency,
                       const wns::Distance& distance) const
{
    double bsHeight = source.getPosition().getZ();
    double utHeight = target.getPosition().getZ();

    // We must assume here that the higher one is the BS
    if (bsHeight < utHeight) std::swap(bsHeight, utHeight);

    assure(bsHeight > 10.0, "BS Height must be at least 10 m");
    assure(bsHeight < 150.0, "BS Height cannot be larger than 150 m");
    assure(utHeight > 1.0, "BS Height must be at least 1 m");
    assure(utHeight < 10.0, "BS Height cannot be larger than 10 m");

    double dBP = 2 * 3.14 * bsHeight * utHeight * frequency / 3.0e02;

    if (distance < dBP)
    {
        double pl = 20.0 * log10(distance * 40 * 3.14 * frequency/3000.0);
        pl += std::min(0.03 * pow(buildingHeight_, 1.72), 10.0) * log10(distance);
        pl -= std::min(0.044 * pow(buildingHeight_, 1.72), 14.77);
        pl += 0.002*log10(buildingHeight_)*distance;

        return wns::Ratio::from_dB(pl);
    }
    else
    {
        double pl = 20.0 * log10(dBP * 40 * 3.14 * frequency/3000.0);
        pl += std::min(0.03 * pow(buildingHeight_, 1.72), 10.0) * log10(dBP);
        pl -= std::min(0.044 * pow(buildingHeight_, 1.72), 14.77);
        pl += 0.002*log10(buildingHeight_)*dBP;

        pl += 40 * log10(distance / dBP);
        return wns::Ratio::from_dB(pl);
    }
}

wns::Ratio
ITUSMa::getNLOSPathloss(const rise::antenna::Antenna& source,
                        const rise::antenna::Antenna& target,
                        const wns::Frequency& frequency,
                        const wns::Distance& distance) const
{
    double bsHeight = source.getPosition().getZ();
    double utHeight = target.getPosition().getZ();

    // We must assume here that the higher one is the BS
    if (bsHeight < utHeight) std::swap(bsHeight, utHeight);

    assure(bsHeight > 10.0, "BS Height must be at least 10 m");
    assure(bsHeight < 150.0, "BS Height cannot be larger than 150 m");
    assure(utHeight > 1.0, "BS Height must be at least 1 m");
    assure(utHeight < 10.0, "BS Height cannot be larger than 10 m");

    double pl = 161.04 - 7.1 * log10(streetWidth_) + 7.5 * log10(buildingHeight_);
    pl -= (24.37 - 3.7 * pow(buildingHeight_ / bsHeight, 2)) * log10(bsHeight);
    pl += (43.42 - 3.1 * log10(bsHeight) ) * (log10(distance) - 3.0);
    pl += 20.0 * log10(frequency/1000.0);
    pl -= 3.2 * pow(log10(11.75 * utHeight), 2) - 4.97;

    return wns::Ratio::from_dB(pl);
}
