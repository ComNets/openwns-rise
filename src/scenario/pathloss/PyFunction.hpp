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

#ifndef _RISE_SCENARIO_PATHLOSS_PYFUNCTION_HPP
#define _RISE_SCENARIO_PATHLOSS_PYFUNCTION_HPP

#include <RISE/scenario/pathloss/DistanceDependent.hpp>
#include <RISE/stations/base/base.hpp>

#include <WNS/pyconfig/View.hpp>
#include <WNS/PowerRatio.hpp>
#include <WNS/isClass.hpp>

namespace rise { namespace scenario { namespace pathloss {

    /** @brief PyConfig defined pathloss
     */
    class PyFunction:
        public DistanceDependent
    {
    public:
        PyFunction(const wns::pyconfig::View& config_);

        virtual ~PyFunction()
            { }
    protected:
        wns::Ratio calculatePathloss(const antenna::Antenna& source,
                                     const antenna::Antenna& target,
                                     const wns::Frequency& frequency,
                                     const wns::Distance& distance) const;
    private:
        std::string
        preciseDtos(double value) const;

        const wns::pyconfig::View config;
    };
}}}

#endif // NOT defined _RISE_SCENARIO_PATHLOSS_SINGLESLOPE_HPP
