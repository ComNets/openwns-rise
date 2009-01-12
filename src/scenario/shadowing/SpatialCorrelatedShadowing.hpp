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

#ifndef _RISE_SCENARIO_SPATIALCORRELATEDSHADOWING_HPP
#define _RISE_SCENARIO_SPATIALCORRELATEDSHADOWING_HPP

#include <RISE/scenario/shadowing/Shadowing.hpp>
#include <RISE/scenario/scenerymap/SceneryMap.hpp>

#include <WNS/pyconfig/View.hpp>
#include <WNS/Position.hpp>
#include <WNS/Interpolation.hpp>
#include <WNS/distribution/Uniform.hpp>

#include <vector>
#include <memory>

namespace rise { namespace scenario { namespace shadowing {

    /**
     * @brief Provides shadowing values out of a spatial corelated random variabled.
     */
    class SpatialCorrelated : public Shadowing
    {
    public:

	SpatialCorrelated(const wns::pyconfig::View& config);
	virtual ~SpatialCorrelated() {}

	virtual wns::Ratio getShadowing(const antenna::Antenna& source,
									const antenna::Antenna& target) const;
	private:
		const wns::pyconfig::View config;
    };

} // shadowing
} // scenario
} // rise

#endif // NOT defined _RISE_SCENARIO_SPATIALCORRELATEDSHADOWING_HPP
