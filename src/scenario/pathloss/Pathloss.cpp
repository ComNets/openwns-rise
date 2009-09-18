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

#include <RISE/scenario/pathloss/Pathloss.hpp>
#include <WNS/Assure.hpp>
#include <WNS/Ttos.hpp>

#include <string>

using namespace rise::scenario::pathloss;

Pathloss::Pathloss(const ReturnValueTransformation* rvt)
    : transform(rvt)
{
}

Pathloss::~Pathloss()
{
    delete transform;
}

wns::Ratio Pathloss::getPathloss(const antenna::Antenna& source,
				 const antenna::Antenna& target,
				 const wns::Frequency& frequency) const
{
    assure(frequency > 0, "Illegal frequency (" + wns::Ttos(frequency) + " MHz) passed to Pathloss::getPathloss()");
    assure(frequency < std::numeric_limits<wns::Frequency>::infinity(),
	   "Infinite frequency passed to Pathloss::getPathloss()");
    wns::Ratio result;
    try
    { // out-of-range must be catched so that distant cells can be simulated
      result = (*transform)(calculatePathloss(source, target, frequency));
    }
    catch(...)
    { // probably out of range
        /**
         * @todo dbn: Remove OutOfRangePathloss completely
         */
        std::cout << "WARNING : Setting pathloss to out of range value! Probably this is a misconfiguration!" << std::endl;
      result = OutOfRangePathloss;
    }
    return result;
}
