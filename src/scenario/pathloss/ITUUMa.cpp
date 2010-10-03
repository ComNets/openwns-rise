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

#include <RISE/scenario/pathloss/ITUUMa.hpp>

#include <WNS/StaticFactoryBroker.hpp>

STATIC_FACTORY_BROKER_REGISTER(rise::scenario::pathloss::ITUUMa, rise::scenario::pathloss::Pathloss, "ITUUMa");

using namespace rise::scenario::pathloss;

ITUUMa::ITUUMa(const wns::pyconfig::View& pyco):
ITUPathloss(pyco)
{
    streetWidth_ = pyco.get<double>("streetWidth");
    buildingHeight_ = pyco.get<double>("buildingHeight");
}

double
ITUUMa::getLOSProbability(double distance) const
{
    double pLOS = std::min(1.0, 18.0/distance) * (1 - exp(-distance/63.0)) + exp(-distance/63.0);
    return pLOS;
}

wns::Ratio
ITUUMa::getLOSPathloss(const rise::antenna::Antenna& source,
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

    // For UMaLOS the effictive heights are used heff = h - 1.0
    bsHeight -= 1.0;
    utHeight -= 1.0;

    double dBP = 4 * bsHeight * utHeight * frequency / 3.0e02;

    if (distance < dBP)
    {
        return wns::Ratio::from_dB(22.0 * log10(distance) + 28.0 + 20 * log10(frequency/1000.0));
    }
    else
    {
        double pl = 40 * log10(distance) + 7.8;
        pl -= 18.0 * log10(bsHeight) + 18.0 * log10(utHeight);
        pl += 2 * log10(frequency / 1000.0);

        return wns::Ratio::from_dB(pl);
    }
}

wns::Ratio
ITUUMa::getNLOSPathloss(const rise::antenna::Antenna& source,
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
ITUUMa::getLOSShadowingStd(const rise::antenna::Antenna& source,
                           const rise::antenna::Antenna& target,
                           const wns::Frequency& frequency,
                           const wns::Distance& distance) const
{
    assure(distance > 10.0, "This model is only valid for a minimum distance of 10m");
    assure(distance < 5000.0, "This model is only valid for a maximum distance of 5000m");

    return 4.0;
}

double
ITUUMa::getNLOSShadowingStd(const rise::antenna::Antenna& source,
                            const rise::antenna::Antenna& target,
                            const wns::Frequency& frequency,
                            const wns::Distance& distance) const
{
    assure(distance > 10.0, "This model is only valid for a minimum distance of 10m");
    assure(distance < 5000.0, "This model is only valid for a maximum distance of 5000m");

    return 6.0;
}

double
ITUUMa::getCarPenetrationStd() const
{
    return 5.0;
}

double
ITUUMa::getCarPenetrationMean() const
{
    return 9.0;
}