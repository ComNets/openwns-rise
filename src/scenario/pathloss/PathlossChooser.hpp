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
#ifndef _RISE_SCENARIO_PATHLOSS_PATHLOSSCHOOSER_HPP
#define _RISE_SCENARIO_PATHLOSS_PATHLOSSCHOOSER_HPP


#include <RISE/scenario/pathloss/DistanceDependent.hpp>
#include <RISE/scenario/pathloss/Pathloss.hpp>
#include <RISE/scenario/pathloss/defs.hpp>
#include <RISE/antenna/Antenna.hpp>

#include <WNS/distribution/Uniform.hpp>
#include <WNS/pyconfig/View.hpp>
#include <WNS/PowerRatio.hpp>
#include <WNS/container/RangeMap.hpp>
#include <WNS/Types.hpp>
#include <string>

#include <WNS/probe/bus/ProbeBus.hpp>
#include <WNS/probe/bus/ProbeBusRegistry.hpp>
#include <WNS/probe/bus/ContextCollector.hpp>

namespace rise { namespace scenario { namespace pathloss {
     /**
     * @brief This function chooses between NLoS or LoS scenario based on the LoS probability 
     *
     */
    class PathlossChooser :
	public DistanceDependent
    {
	public:
		std::string plModelName;
		PathlossChooser(const wns::pyconfig::View& config);
		bool Distribution(const wns::Distance& distance) const;

    protected:

		virtual wns::Ratio calculatePathloss(const antenna::Antenna& source,
					     const antenna::Antenna& target,
					     const wns::Frequency& frequency,
					     const wns::Distance& distance) const;

	private:
		wns::logger::Logger logger;
		mutable wns::distribution::Uniform* decisionDistribution;
		Pathloss* plLOS;
		Pathloss* plNLOS;
		mutable bool pathLossModelLos;
		mutable bool notYetDecided;
        mutable double usedDistance;
        wns::probe::bus::ContextCollector losProbability;
        wns::probe::bus::ContextCollector losDecision;
		
    };
}}} 

#endif // NOT defined _RISE_SCENARIO_PATHLOSS_PATHLOSSCHOOSER_HPP