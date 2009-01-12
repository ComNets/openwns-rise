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

#include <RISE/scenario/pathloss/OneSlope.hpp>
#include <RISE/scenario/pathloss/MultiSlope.hpp>
#include <RISE/scenario/Propagation.hpp>

#include <WNS/pyconfig/View.hpp>
#include <WNS/StaticFactoryBroker.hpp>
#include <WNS/PowerRatio.hpp>

#include <limits>
#include <sstream>

using namespace rise::scenario::pathloss;

STATIC_FACTORY_BROKER_REGISTER(MultiSlope, Pathloss, "MultiSlope");

MultiSlope::MultiSlope(const wns::pyconfig::View& config)
    : RangeChecked(config),
      outOfMinRange(Propagation::create<Pathloss>(config.getView("outOfMinRange"))),
      outOfMaxRange(Propagation::create<Pathloss>(config.getView("outOfMaxRange")))
{
    const wns::Frequency infFreq = std::numeric_limits<wns::Frequency>::infinity();
    const FrequencyRange validFrequencies  = FrequencyRange::CreateFrom(config.getView("validFrequencies"));
    const FrequencyRange toLowFrequencies  = FrequencyRange::FromIncluding(0).ToExcluding(validFrequencies.min());
    const FrequencyRange toHighFrequencies = FrequencyRange::Between(validFrequencies.max()).And(infFreq);
    addRange(validFrequencies, true);
    if (!toLowFrequencies.isEmpty()) addRange(toLowFrequencies, false);
    if (!toHighFrequencies.isEmpty()) addRange(toHighFrequencies, false);

    const DistanceRange validDistances  = DistanceRange::CreateFrom(config.getView("validDistances"));

    const wns::Distance infDist = std::numeric_limits<wns::Frequency>::infinity();
    const DistanceRange toLowDistances  = DistanceRange::FromIncluding(0).ToExcluding(validDistances.min());
    const DistanceRange toHighDistances = DistanceRange::Between(validDistances.max()).And(infDist);
    if (!toLowDistances.isEmpty()) addRange(toLowDistances, outOfMinRange);
    if (!toHighDistances.isEmpty()) addRange(toHighDistances, outOfMaxRange);


	const double distNormFactor = config.get<double>("distNormFactor");

	for (int ii=0; ii<config.len("ranges"); ++ii)
	{
		wns::pyconfig::View rangeConfig = config.get("ranges",ii);

		const DistanceRange thisRangesDistances = DistanceRange::CreateFrom(rangeConfig.getView("distRange"));

		std::stringstream complaint;
		complaint << "The distance range for one of the slopes exceeds the validDistances Range:\n"
			      << "Valid: " << validDistances.min() << " - " << validDistances.max() << "\n"
			      << "This:  " << thisRangesDistances.min() << " - " << thisRangesDistances.max() << "\n";
		assure(validDistances.contains(thisRangesDistances), complaint.str());

		const wns::Ratio offset = rangeConfig.get<wns::Ratio>("offset");
		const double freqFactor = rangeConfig.get<double>("freqFactor");
		const double distFactor = rangeConfig.get<double>("distFactor");

		Pathloss* thisSlope = new OneSlope<NoHeightFreqAttFormula, NoHeightDistAttFormula>(
			config, offset, freqFactor, distFactor, distNormFactor);

		addRange(thisRangesDistances, thisSlope);

		// The RangeChecked can not delete the slopes when it is destroyed. So
		// we have to remember them for later destruction
		slopes.push_back(thisSlope);
	}
}

MultiSlope::~MultiSlope()
{
	while (!slopes.empty())
	{
		delete *slopes.begin();
		slopes.pop_front();
	}
}

wns::Ratio
MultiSlope::calculatePathloss(const rise::antenna::Antenna& source,
				  const rise::antenna::Antenna& target,
				  const wns::Frequency& frequency,
				  const wns::Distance& /*distance*/) const
{
	return RangeChecked::calculatePathloss(source, target, frequency);
}
