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

#include <RISE/scenario/pathloss/MultiModel.hpp>
#include <RISE/scenario/Propagation.hpp>

#include <WNS/pyconfig/View.hpp>
#include <WNS/StaticFactoryBroker.hpp>
#include <WNS/PowerRatio.hpp>

#include <limits>

using namespace rise::scenario::pathloss;

STATIC_FACTORY_BROKER_REGISTER(MultiModel, Pathloss, "MultiModel");

MultiModel::MultiModel(const wns::pyconfig::View& config)
    : RangeChecked(config)
{
	for (int ii=0; ii<config.len("ranges"); ++ii)
	{
		wns::pyconfig::View rangeConfig = config.get("ranges",ii);

		const DistanceRange thisRangesDistances = DistanceRange::CreateFrom(rangeConfig.getView("distRange"));
		wns::pyconfig::View modelConfig = rangeConfig.get("modelConfig");
		Pathloss* thisSlope = Propagation::create<Pathloss>(modelConfig);
		addRange(thisRangesDistances, thisSlope);
	}
}

wns::Ratio
MultiModel::calculatePathloss(const rise::antenna::Antenna& source,
				  const rise::antenna::Antenna& target,
				  const wns::Frequency& frequency) const
{
    const wns::Distance distance = calculateDistance(source, target);
    return this->getModel(distance)->getPathloss(source, target, frequency);
}

wns::Ratio
MultiModel::calculatePathloss(const rise::antenna::Antenna& source,
				  const rise::antenna::Antenna& target,
				  const wns::Frequency& frequency,
				  const wns::Distance& /*distance*/) const
{
    const wns::Distance distance = calculateDistance(source, target);
    return this->getModel(distance)->getPathloss(source, target, frequency);
}
