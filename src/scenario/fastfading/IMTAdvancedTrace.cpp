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

#include <RISE/scenario/fastfading/IMTAdvancedTrace.hpp>
#include <RISE/stations/station.hpp>
#include <RISE/medium/Medium.hpp>

#include <RISE/scenario/pathloss/Pathloss.hpp>
#include <RISE/scenario/pathloss/ILoSDependent.hpp>

#include <WNS/StaticFactoryBroker.hpp>
#include <WNS/pyconfig/View.hpp>
#include <WNS/PowerRatio.hpp>
using namespace rise::scenario::fastfading;

STATIC_FACTORY_BROKER_REGISTER(IMTAdvancedTrace, FastFading, "IMTAdvancedTrace");

IMTAdvancedTrace::IMTAdvancedTrace(const wns::pyconfig::View&) : 
    FastFading(),
    nextLinkId_(0),
    logger_(wns::logger::Logger("RISE", std::string("FastFading.IMTAdvancedTrace")))
{
}

void
IMTAdvancedTrace::onWorldCreated()
{
    std::string lName = logger_.getLoggerName();
    lName+= std::string("_");
    lName+= getLinkTypeString();
    logger_.setLoggerName(lName);
}

wns::Ratio 
IMTAdvancedTrace::getFastFading(const antenna::Antenna& source,
				                 const antenna::Antenna& target,
				                 const wns::Frequency& frequency) const
{
    unsigned int sourceId = source.getStation()->getStationId();
    unsigned int targetId = target.getStation()->getStationId();

    assure(sourceId != targetId, "Cannot transmit to myself");

    StationPair np(sourceId, targetId);

    if(links_.find(np) == links_.end())
    {
        createNewLink(np, source, target, frequency);
    }

    unsigned int sc = getSubchannelIndex(frequency);

    return wns::Ratio::from_dB(0.0);
}

unsigned int
IMTAdvancedTrace::getSubchannelIndex(const wns::Frequency& frequency) const
{
    unsigned int sc;
    try
    {
        sc = frequencyToSubchannel_.find(frequency);
    }
    catch(wns::container::Registry<double, unsigned int>::UnknownKeyValue&)
    {
        sc = rise::medium::Medium::getInstance()->getPhysicalResourceIndex(frequency);
        frequencyToSubchannel_.insert(frequency, sc);
        MESSAGE_SINGLE(NORMAL, logger_, "Frequency " << frequency
            << " mapped to subchannel index " << sc);
    }
    return sc;
}

void
IMTAdvancedTrace::createNewLink(const StationPair& np,
                                 const antenna::Antenna& source,
				                 const antenna::Antenna& target,
				                 const wns::Frequency& frequency) const
{
    links_[np] = nextLinkId_;
    nextLinkId_++;

    /* We use pointers here to assure for not NULL rather than to */
    /* catch type cast exceptions */
    const pathloss::Pathloss* pl = &getParent()->getPathlossModel(
        getTransmitterType(), getReceiverType());    

    const rise::scenario::pathloss::ILoSDependent* losDecider = 
        dynamic_cast<const rise::scenario::pathloss::ILoSDependent*>(pl);

    assure(losDecider != NULL, "Need pathloss model of base class ILoSDependent");

    double distance = (source.getPosition() - target.getPosition()).abs();
    bool los = losDecider->isLoS(source, target, frequency, distance);
    if(los)
        losLinks_.insert(links_[np]);

    MESSAGE_SINGLE(NORMAL, logger_, "Created link from " << np.first
        << " to " << np.second << " with LinkId " << links_[np] 
        << " and " << (los?"LoS":"NLoS") << " channel conditions."); 
}

