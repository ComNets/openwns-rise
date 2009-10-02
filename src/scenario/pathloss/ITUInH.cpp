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

#include <RISE/scenario/pathloss/ITUInH.hpp>

using namespace rise::scenario::pathloss;

ITUInH::ITUInH(const wns::pyconfig::View& pyco):
    ITUPathloss(pyco)
{
}

double
ITUInH::getLOSProbability(double distance) const
{
    if (distance <= 18.0) { return 1; }

    if (distance >= 37.0) { return 0.5; }

    double pLOS = exp(-(distance - 18.0)/27.0);

    return pLOS;
}

wns::Ratio
ITUInH::getLOSPathloss(const rise::antenna::Antenna& source,
                       const rise::antenna::Antenna& target,
                       const wns::Frequency& frequency,
                       const wns::Distance& distance) const
{
    assure(distance > 3.0, "This model is only valid for a minimum distance of 3m");
    assure(distance < 100.0, "This model is only valid for a maximum distance of 100m");

    double pl = 32.8 + 16.9 * log10(distance);
    // Frequency is given in MHz (model uses GHz)
    pl = 20 * log10(frequency/1000.0);

    return wns::Ratio::from_dB(pl);
}

wns::Ratio
ITUInH::getNLOSPathloss(const rise::antenna::Antenna& source,
                        const rise::antenna::Antenna& target,
                        const wns::Frequency& frequency,
                        const wns::Distance& distance) const
{
    assure(distance > 10.0, "This model is only valid for a minimum distance of 10m");
    assure(distance < 150.0, "This model is only valid for a maximum distance of 150m");

    double pl = 11.5 + 43.3 * log10(distance);
    // Frequency is given in MHz (model uses GHz)
    pl = 20 * log10(frequency/1000.0);

    return wns::Ratio::from_dB(pl);
}

double
ITUInH::getLOSShadowingStd(double distance) const
{
    assure(distance > 3.0, "This model is only valid for a minimum distance of 3m");
    assure(distance < 100.0, "This model is only valid for a maximum distance of 100m");
    return 3.0;
}

double
ITUInH::getNLOSShadowingStd(double distance) const
{
    assure(distance > 10.0, "This model is only valid for a minimum distance of 10m");
    assure(distance < 150.0, "This model is only valid for a maximum distance of 150m");

    return 4.0;
}
