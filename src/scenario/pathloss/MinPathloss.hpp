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

#ifndef _RISE_SCENARIO_PATHLOSS_MINPATHLOSS_HPP
#define _RISE_SCENARIO_PATHLOSS_MINPATHLOSS_HPP

#include <RISE/scenario/pathloss/Pathloss.hpp>

#include <WNS/pyconfig/View.hpp>
#include <WNS/PowerRatio.hpp>

namespace rise { namespace scenario { namespace pathloss {

    /**
     * @brief Simple return value transformation garantueeing a minimal pathloss.
     */
    class MinPathloss : public Pathloss::ReturnValueTransformation
    {
    public:
	MinPathloss(const wns::pyconfig::View& config);

	/**
	 * Returns max(pathloss, minPathloss), where minPathloss
	 * is set by pyconfig.
	 */
	virtual wns::Ratio operator()(const wns::Ratio& pathloss) const;
    private:
	const wns::Ratio minPathloss;
    };

}}}

#endif // NOT defined _RISE_SCENARIO_PATHLOSS_MINPATHLOSS_HPP
