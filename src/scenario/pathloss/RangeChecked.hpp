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

#ifndef _RISE_SCENARIO_PATHLOSS_RANGECHECKED_HPP
#define _RISE_SCENARIO_PATHLOSS_RANGECHECKED_HPP

#include <RISE/scenario/pathloss/DistanceDependent.hpp>
#include <RISE/scenario/pathloss/Pathloss.hpp>
#include <RISE/scenario/pathloss/defs.hpp>
#include <RISE/antenna/Antenna.hpp>

#include <WNS/pyconfig/View.hpp>
#include <WNS/PowerRatio.hpp>
#include <WNS/container/RangeMap.hpp>
#include <WNS/Types.hpp>

namespace rise { namespace scenario { namespace pathloss {

    /**
     * @brief Base class for models, that are composite of simple models at different distances.
     */
    class RangeChecked : public DistanceDependent
    {
    protected:
	RangeChecked(const wns::pyconfig::View& config);

	virtual wns::Ratio calculatePathloss(const antenna::Antenna& source,
					     const antenna::Antenna& target,
					     const wns::Frequency& frequency) const;

	/**
	 * @brief Add a pathloss model for a distance range.
	 */
	void addRange(const DistanceRange& distances, const Pathloss* model);

	/**
	 * @brief Add a frequency range to allow or disallow.
	 */
	void addRange(const FrequencyRange& frequencies, const bool& allowed = true);

	/**
	 * @brief Check, if a model for distance was set up.
	 */
	bool knowsDistance(const wns::Distance& distance) const;
	/**
	 * @brief Check, if a frequency was allowed or disallowed.
	 */
	bool knowsFrequency(const wns::Frequency& frequency) const;

	/**
	 * @brief return model that's valid at a certain distance
	 */
	const Pathloss* getModel(const wns::Distance& distance) const;

    private:
	typedef wns::container::RangeMap<wns::Frequency, bool> FrequencyMap;
	typedef wns::container::RangeMap<wns::Distance, const Pathloss*> DistanceMap;
	FrequencyMap frequencyMap;
	DistanceMap distanceMap;
    };

}}}

#endif // NOT defined _RISE_SCENARIO_PATHLOSS_RANGECHECKED_HPP
