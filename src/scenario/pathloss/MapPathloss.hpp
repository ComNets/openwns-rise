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

#ifndef _RISE_SCENARIO_PATHLOSS_MAPPATHLOSS_HPP
#define _RISE_SCENARIO_PATHLOSS_MAPPATHLOSS_HPP

#include <RISE/scenario/pathloss/DistanceIndependent.hpp>
#include <RISE/scenario/scenerymap/SceneryMap.hpp>
#include <RISE/scenario/sceneryfile/SC.hpp>

#include <WNS/SmartPtr.hpp>
#include <WNS/pyconfig/View.hpp>
#include <WNS/Interpolation.hpp>
#include <WNS/PowerRatio.hpp>

#include <vector>

namespace rise { namespace scenario { namespace pathloss {

    //! This class provides pathloss values out of pathloss maps read from scenery files.
    class Map : public DistanceIndependent
    {
    public:
	typedef scenerymap::PathlossMap2D PathlossMap2D;
	typedef scenerymap::Interpolation2D Interpolation2D;

	Map(const wns::pyconfig::View& config);

	virtual ~Map();

    protected:
	virtual wns::Ratio calculatePathloss(const antenna::Antenna& source,
					     const antenna::Antenna& target,
					     const wns::Frequency& frequency) const;

    };

}}}

#endif // NOT defined _RISE_SCENARIO_PATHLOSS_MAPPATHLOSS_HPP
