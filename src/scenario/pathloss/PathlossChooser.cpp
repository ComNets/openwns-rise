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

#include <RISE/scenario/pathloss/PathlossChooser.hpp>
#include <RISE/scenario/pathloss/defs.hpp>
#include <RISE/antenna/Antenna.hpp>
#include <RISE/scenario/Propagation.hpp>
#include <WNS/distribution/Uniform.hpp>
#include <WNS/StaticFactoryBroker.hpp>
#include <WNS/Types.hpp>
#include <WNS/Assure.hpp>

#include <WNS/probe/bus/ProbeBus.hpp>
#include <WNS/probe/bus/ProbeBusRegistry.hpp>
#include <WNS/probe/bus/ContextCollector.hpp>


using namespace rise::scenario::pathloss;

STATIC_FACTORY_BROKER_REGISTER(PathlossChooser, Pathloss, "PathlossChooser");

PathlossChooser::PathlossChooser(const wns::pyconfig::View& config)
    : DistanceDependent(config),
	plLOS(Propagation::create<Pathloss>(config.getView("plLOS"))),
	plNLOS(Propagation::create<Pathloss>(config.getView("plNLOS"))),
	notYetDecided(true),
	decisionDistribution(NULL),
	plModelName(config.get<std::string>("plModelName")),
	logger(config.get("logger")),
    losProbability("PathlossChooser.losProbability"),
    losDecision("PathlossChooser.losDecision"),
    usedDistance(-1)
	{
        MESSAGE_SINGLE(NORMAL, logger, "new PathlossChooser model created ");
    }

wns::Ratio PathlossChooser::calculatePathloss(const antenna::Antenna& source,
					     const antenna::Antenna& target,
					     const wns::Frequency& frequency,
					     const wns::Distance& distance) const
{
	// choosing pathloss only once at the beginning when distance between stations defined
	if (notYetDecided)
	{
		pathLossModelLos = Distribution(distance);
		notYetDecided = false;
	}
    
//     if(usedDistance!=distance)
//         {
//         usedDistance = distance;
// 		pathLossModelLos = Distribution(distance);
//         }
    
	if (pathLossModelLos)
		{
            losDecision.put(1);
			MESSAGE_SINGLE(NORMAL, logger, "Using LOS pathloss model for scenario: "<<plModelName<<",source:"<<source.getPosition()<<",target:"<<target.getPosition()<<",frequency:"<<frequency);
			return plLOS->getPathloss(source, target, frequency);
		}
    else
		{
            losDecision.put(0);
			MESSAGE_SINGLE(NORMAL, logger, "Using NLOS pathloss model for scenario: "<<plModelName<<",source:"<<source.getPosition()<<",target:"<<target.getPosition()<<",frequency:"<<frequency);
			return plNLOS->getPathloss(source, target, frequency);
		}
   // }
	
}


bool PathlossChooser::Distribution(const wns::Distance& distance) const
{
	double p;
	
	decisionDistribution = new wns::distribution::Uniform(0.0, 1.0);
    //decisionDistribution = new wns::distribution::Uniform(0.4, 0.7);
	double random = (*decisionDistribution)();
	
    if(plModelName == "UrbanMicro")
	{
		p=std::min(18 / distance,1.0) * (1 - exp(-distance / 36))+exp(-distance / 36);
	}
	else if(plModelName == "UrbanMacro")
	{
		p=std::min(18 / distance,1.0) * (1 - exp(-distance / 63))+exp(-distance / 63);
	}
	else if(plModelName == "SuburbanMacro")
	{
		if(distance <= 10) p=1;
		else  p=exp(-(distance - 10) / 200);
	}
	else if(plModelName == "RuralMacro")
	{
		if(distance <= 10) p=1;
		else p=exp(-(distance - 10) / 1000);
	}
	MESSAGE_SINGLE(NORMAL, logger, "distance:"<<distance<<",random number:"<<random<<",probability:"<<p);
    losProbability.put(p);
	if(random > p)
			return false;
		else 
			return true;
	
};
