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

#ifndef _RISE_SCENARIO_PATHLOSS_UMTS3003VEHICULAR_HPP
#define _RISE_SCENARIO_PATHLOSS_UMTS3003VEHICULAR_HPP

#include <RISE/scenario/pathloss/OneSlope.hpp>
#include <RISE/scenario/pathloss/Deny.hpp>

#include <WNS/pyconfig/View.hpp>

namespace rise { namespace scenario { namespace pathloss {

    /**
     * @brief UMTS 30.03 vehicular model.
     *
     * Pathloss is 80 + 21 * log10(freq) - 18 * log10(baseheight) + (distance - 0.16 * base_height) * log10(distance).
     */
    class Umts3003Vehicular : public OneSlope<DefaultFreqAttFormula, Umts3003DistAttFormula>
    {
	typedef OneSlope<DefaultFreqAttFormula, Umts3003DistAttFormula> Super;
    public:
	Umts3003Vehicular(const wns::pyconfig::View& config);
    private:
	Deny deny;
    };
	    
}}}

#endif // NOT defined _RISE_SCENARIO_PATHLOSS_UMTS3003VEHICULAR_HPP
