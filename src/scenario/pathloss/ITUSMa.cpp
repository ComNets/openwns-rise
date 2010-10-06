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
#include <RISE/stations/station.hpp>

#include <WNS/distribution/Uniform.hpp>

#include <boost/random.hpp>
#include <boost/functional/hash.hpp>

#include <WNS/StaticFactoryBroker.hpp>

STATIC_FACTORY_BROKER_REGISTER(rise::scenario::pathloss::ITUSMa, rise::scenario::pathloss::Pathloss, "ITUSMa");

using namespace rise::scenario::pathloss;

ITUSMa::ITUSMa(const wns::pyconfig::View& pyco):
    rise::scenario::pathloss::DistanceDependent(pyco),
    losProbabilityCC_("rise.scenario.pathloss.ITUPathloss.losProbability"),
    shadowingCC_("rise.scenario.pathloss.ITUPathloss.shadowing"),
    useShadowing_(pyco.get<bool>("useShadowing")),
    useCarPenetration_(pyco.get<bool>("useCarPenetration"))
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
    static unsigned int initialSeed = dis() * pow(2, sizeof(unsigned int)*8);

    detail::HashRNG hrng(initialSeed,
                         source.getPosition(),
                         target.getPosition(),
                         true, true);

    detail::HashRNG hrngOnlyUTPos(initialSeed + 2773,
                                  source.getPosition(),
                                  target.getPosition(),
                                  false, true);

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

    bool isIndoor = hrngOnlyUTPos() < 0.5;
    wns::Ratio sh = wns::Ratio::from_dB(0.0);

    if (hrng() < getLOSProbability(distance))
    {
        losProbabilityCC_.put(distance);

        pl = getLOSPathloss(source, target, frequency, distance);

        double shadowingStd = 0.0;
        double shadowingMean = 0.0;
        if (isIndoor)
        {
            if (useShadowing_)
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
                boost::normal_distribution<double> shadow(shadowingMean, shadowingStd);
                sh = wns::Ratio::from_dB(shadow(hrng));
            }
        }else
        {
            if (useShadowing_)
            {
                shadowingMean = 0.0;

                if (distance < dBP)
                {
                    shadowingStd = 4.0;
                }
                else
                {
                    shadowingStd = 6.0;
                }
            
                boost::normal_distribution<double> shadow(shadowingMean, shadowingStd);
                sh = wns::Ratio::from_dB(shadow(hrng));
            }

            if (useCarPenetration_)
            {
                boost::normal_distribution<double> carPenetration(9.0, 5.0);
                sh += wns::Ratio::from_dB(carPenetration(hrngOnlyUTPos));
            }
        }

        shadowingCC_.put(sh.get_dB());
        pl += sh;
    }
    else
    {
        pl = getNLOSPathloss(source, target, frequency, distance);

        double shadowingStd = 0.0;
        double shadowingMean = 0.0;
        wns::Ratio sh = wns::Ratio::from_dB(0.0);

        if (isIndoor)
        {
            if (useShadowing_)
            {
                shadowingMean = 20.0;
                shadowingStd = 8.0;
                boost::normal_distribution<double> shadow(shadowingMean, shadowingStd);
                sh = wns::Ratio::from_dB(shadow(hrng));
            }
        }else
        {
            if (useShadowing_)
            {
                shadowingMean = 0.0;
                shadowingStd = 8.0;
                boost::normal_distribution<double> shadow(shadowingMean, shadowingStd);
                sh = wns::Ratio::from_dB(shadow(hrng));
            }

            if (useCarPenetration_)
            {
                boost::normal_distribution<double> carPenetration(9.0, 5.0);
                sh += wns::Ratio::from_dB(carPenetration(hrngOnlyUTPos));
            }
        }
        shadowingCC_.put(sh.get_dB());
        pl += sh;
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

double
ITUSMa::getCarPenetrationStd() const
{
    // Handled above, because of mix of indoor and outdoor users
    return 0.0;
}

double
ITUSMa::getCarPenetrationMean() const
{
    // Handled above, because of mix of indoor and outdoor users
    return 0.0;
}