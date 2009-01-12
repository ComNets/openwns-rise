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

#ifndef _RISE_SCENARIO_PATHLOSS_MULTISLOPE_HPP
#define _RISE_SCENARIO_PATHLOSS_MULTISLOPE_HPP

#include <RISE/scenario/pathloss/RangeChecked.hpp>
#include <RISE/antenna/Antenna.hpp>
#include <WNS/pyconfig/View.hpp>

namespace rise { namespace scenario { namespace pathloss {

    /** @brief User defined one slope pathloss.
     *
     *  This class implements following formula:
     *
     *  pl = offset + distFactor * log10(distance * distNormFactor)
     *              + freqFactor * log10(frequency)
     *
     *  As in all Pathloss-models, minPathloss is guaranteed, so
     *  the returned value is max(minPathloss, pl).
     *
     *  offset, distFactor and freqFactor are configurable via pyconfig::Parser,
     *  distNormFactor is 1, if distanceUnit was set to "m", 1/1000,
     *  if it was "km".
     *
     *  You have to provide sensible values for validDistances and
     *  validFrequencies in your configuration. You can use
     *  whichever Pathloss model you like as out of range strategy
     *  (e.g. Constant, Deny, FreeSpace, another MultiSlope), but keep in mind
     *  that the pathloss will be discontinuous at the validDistances borders,
     *  if set up improperly.
     *
     *  Pathloss.py contains two example configurations, one sets
     *  up the Umts3003Outdoor model that was used in SGOOSE,
     *  the other sets up a free space model.
     */
    class MultiSlope :
		public RangeChecked
    {
    public:
		MultiSlope(const wns::pyconfig::View& config);

		virtual ~MultiSlope();

		virtual wns::Ratio calculatePathloss(const antenna::Antenna& source,
											 const antenna::Antenna& target,
											 const wns::Frequency& frequency,
											 const wns::Distance& distance) const;

    private:
		Pathloss* outOfMinRange;
		Pathloss* outOfMaxRange;

		std::list<Pathloss*> slopes;
    };

}}}

#endif // NOT defined _RISE_SCENARIO_PATHLOSS_MULTISLOPE_HPP
