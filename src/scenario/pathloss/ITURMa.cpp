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

#include <RISE/scenario/pathloss/ITURMa.hpp>

#include <WNS/StaticFactoryBroker.hpp>

STATIC_FACTORY_BROKER_REGISTER(rise::scenario::pathloss::ITURMa, rise::scenario::pathloss::Pathloss, "ITURMa");

using namespace rise::scenario::pathloss;

ITURMa::ITURMa(const wns::pyconfig::View& pyco):
ITUPathloss(pyco)
{
    streetWidth_ = pyco.get<double>("streetWidth");
    buildingHeight_ = pyco.get<double>("buildingHeight");
}

double
ITURMa::getLOSProbability(double distance) const
{
    if (distance <= 10.0)
    {
        return 1.0;
    }

    double pLOS = exp(- (distance-10.0)/1000.0);
    return pLOS;
}

wns::Ratio
ITURMa::getLOSPathloss(const rise::antenna::Antenna& source,
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
        double pl = 9.0 + 20.0 * log10(distance * 40 * 3.14 * frequency/3000.0);
        pl += std::min(0.03 * pow(buildingHeight_, 1.72), 10.0) * log10(distance);
        pl -= std::min(0.044 * pow(buildingHeight_, 1.72), 14.77);
        pl += 0.002*log10(buildingHeight_)*distance;

        return wns::Ratio::from_dB(pl);
    }
    else
    {
        double pl = 9.0 + 20.0 * log10(dBP * 40 * 3.14 * frequency/3000.0);
        pl += std::min(0.03 * pow(buildingHeight_, 1.72), 10.0) * log10(dBP);
        pl -= std::min(0.044 * pow(buildingHeight_, 1.72), 14.77);
        pl += 0.002*log10(buildingHeight_)*dBP;

        pl += 40 * log10(distance / dBP);
        return wns::Ratio::from_dB(pl);
    }
}

wns::Ratio
ITURMa::getNLOSPathloss(const rise::antenna::Antenna& source,
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

    // Also all users are outdoors and in cars, 9 dB loss for that cmp. Table 8.2
    pl += 9.0;

    return wns::Ratio::from_dB(pl);
}

double
ITURMa::getLOSShadowingStd(const rise::antenna::Antenna& source,
                           const rise::antenna::Antenna& target,
                           const wns::Frequency& frequency,
                           const wns::Distance& distance) const
{
    assure(distance > 10.0, "This model is only valid for a minimum distance of 10m");
    assure(distance < 5000.0, "This model is only valid for a maximum distance of 5000m");

    double bsHeight = source.getPosition().getZ();
    double utHeight = target.getPosition().getZ();

    // We must assume here that the higher one is the BS
    if (bsHeight < utHeight) std::swap(bsHeight, utHeight);

    assure(bsHeight > 10.0, "BS Height must be at least 10 m");
    assure(bsHeight < 150.0, "BS Height cannot be larger than 150 m");
    assure(utHeight > 1.0, "BS Height must be at least 1 m");
    assure(utHeight < 10.0, "BS Height cannot be larger than 10 m");

    double dBP = 2 * 3.14 * bsHeight * utHeight * frequency / 3.0e02;

    // Superposition of Car penentration loss variance and shadowing
    // Sum of two normal distributions X1 = N(mu1, var1) and  X 2= N(mu2, var2)
    // results in Xsum = N(mu1 + mu2, var1 + var2)
    if (distance < dBP)
    {
        return 6.40312;
    }
    else
    {
        return 7.810249676;
    }
}

double
ITURMa::getNLOSShadowingStd(const rise::antenna::Antenna& source,
                            const rise::antenna::Antenna& target,
                            const wns::Frequency& frequency,
                            const wns::Distance& distance) const
{
    assure(distance > 10.0, "This model is only valid for a minimum distance of 10m");
    assure(distance < 5000.0, "This model is only valid for a maximum distance of 5000m");

    // Superposition of Car penentration loss variance and shadowing
    // Sum of two normal distributions X1 = N(mu1, var1) and  X 2= N(mu2, var2)
    // results in Xsum = N(mu1 + mu2, var1 + var2)
    return 9.433981132;
}
