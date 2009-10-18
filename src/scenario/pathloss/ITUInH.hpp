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

#ifndef RISE_SCENARIO_PATHLOSS_ITUInH_HPP
#define RISE_SCENARIO_PATHLOSS_ITUInH_HPP

#include <RISE/scenario/pathloss/ITUPathloss.hpp>

namespace rise { namespace scenario { namespace pathloss {

/**
 * @brief Realization of the pathloss + shadowing model for the Indoor Hotspot scenario
 *
 * @author Daniel Bueltmann <openwns@doender.de>
 */
class ITUInH:
    public ITUPathloss
{
public:
    ITUInH(const wns::pyconfig::View&);

    virtual ~ITUInH() {}

    virtual double
    getLOSProbability(double distance) const;

    virtual wns::Ratio
    getLOSPathloss(const rise::antenna::Antenna& source,
                    const rise::antenna::Antenna& target,
                    const wns::Frequency& frequency,
                    const wns::Distance& distance) const;

    virtual wns::Ratio
    getNLOSPathloss(const rise::antenna::Antenna& source,
                    const rise::antenna::Antenna& target,
                    const wns::Frequency& frequency,
                    const wns::Distance& distance) const;

    virtual double
    getLOSShadowingStd(const rise::antenna::Antenna& source,
                       const rise::antenna::Antenna& target,
                       const wns::Frequency& frequency,
                       const wns::Distance& distance) const;

    virtual double
    getNLOSShadowingStd(const rise::antenna::Antenna& source,
                        const rise::antenna::Antenna& target,
                        const wns::Frequency& frequency,
                        const wns::Distance& distance) const;

};

} // pathloss
} // scenario
} // rise

#endif // RISE_SCENARIO_PATHLOSS_ITUInH_HPP
