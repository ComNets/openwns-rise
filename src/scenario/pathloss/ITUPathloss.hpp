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

#ifndef RISE_SCENARIO_PATHLOSS_ITUPATHLOSS_HPP
#define RISE_SCENARIO_PATHLOSS_ITUPATHLOSS_HPP

#include <RISE/scenario/pathloss/DistanceDependent.hpp>
#include <WNS/probe/bus/ContextCollector.hpp>

namespace rise { namespace scenario { namespace pathloss {

namespace detail {

/**
 * @brief Helper that can be used as pseudo-random number generator for
 * boost distributions. This may only be used to draw one random number
 * from the distribution. DO NOT USE as a random number generator!!!
 *
 * @author Daniel Bueltmann <openwns@doender.de>
 */
class HashRNG
{
public:
    HashRNG(size_t initialSeed, wns::Position p1, wns::Position p2, double distance);

    static const bool has_fixed_range = true;

    static const double min_value = 0.0;

    static const double max_value = 1.0;

    double
    operator()();

    double
    min()
    {
        return 0.0;
    }

    double
    max()
    {
        return 1.0;
    }

    bool giveA;
    double a;
    double b;
    double c;
};

}

/**
 * @ ITUPathloss chooses the LOS/NLOS pathloss and shadowing models
 * according to a distance-dependent probability.
 *
 * In ITU-R M.2135 (IMT.EVAL) pp. 31 the probability distributions
 * for each scenario are given.
 *
 * This class implements a semi-random probabilistic pathloss distribution
 * by hashing the position of the two antennas and the distance between them.
 * These hashes have random number properties and are then used to determine
 * the LOS/NLOS model and the log-normal shadowing component.
 *
 * Using hashes makes it possible to imlement the ITU-R models without
 * any memory overhead. At the time of writing this model is able to calculate
 * ~ 16250 pathloss values per second.
 *
 * Usage:
 *
 * This model is a base for all ITU-R specific pathloss for the ITU-R scenarios.
 * The resulting models however, may all be used as any other RISE pathloss model.
 *
 * @author Daniel Bueltmann <openwns@doender.de>
 */
class ITUPathloss:
    public rise::scenario::pathloss::DistanceDependent
{
public:
    ITUPathloss(const wns::pyconfig::View&);

    virtual ~ITUPathloss() {}

    virtual wns::Ratio
    calculatePathloss(const rise::antenna::Antenna& source,
                        const rise::antenna::Antenna& target,
                        const wns::Frequency& frequency,
                        const wns::Distance& distance) const;

    /**
     * @brief Return the probability of LOS pathlos at distance distance
     */
    virtual double
    getLOSProbability(double distance) const = 0;

    /**
     * @brief Return the LOS pathloss according to your specific model
     */
    virtual wns::Ratio
    getLOSPathloss(const rise::antenna::Antenna& source,
                    const rise::antenna::Antenna& target,
                    const wns::Frequency& frequency,
                    const wns::Distance& distance) const = 0;

    /**
     * @brief Return the NLOS pathloss according to your specific model
     */
    virtual wns::Ratio
    getNLOSPathloss(const rise::antenna::Antenna& source,
                    const rise::antenna::Antenna& target,
                    const wns::Frequency& frequency,
                    const wns::Distance& distance) const = 0;

    /**
     * @brief Return a realization of the shadowing in LOS case. This may be distance dependent
     */
    virtual double
    getLOSShadowingStd(double distance) const = 0;

    /**
     * @brief Return a realization of the shadowing in NLOS case. This may be distance dependent
     */
    virtual double
    getNLOSShadowingStd(double distance) const = 0;

private:
    wns::probe::bus::ContextCollector losProbabilityCC_;
    wns::probe::bus::ContextCollector shadowingCC_;
};

} // pathloss
} // scenario
} // rise

#endif // RISE_SCENARIO_PATHLOSS_ITUPathloss_HPP
