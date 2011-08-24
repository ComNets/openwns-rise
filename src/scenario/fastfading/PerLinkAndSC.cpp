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

#include <RISE/scenario/fastfading/PerLinkAndSC.hpp>
#include <RISE/stations/station.hpp>

#include <RISE/scenario/pathloss/Pathloss.hpp>
#include <RISE/scenario/pathloss/ILoSDependent.hpp>
#include <RISE/manager/metasystemmanager.hpp>
#include <RISE/manager/systemmanager.hpp>

#include <WNS/pyconfig/View.hpp>
#include <WNS/PowerRatio.hpp>

using namespace rise::scenario::fastfading;


PerLinkAndSC::PerLinkAndSC(const wns::pyconfig::View& config) : 
    FastFading(),
    nextLinkId_(0),
    logger_(wns::logger::Logger("RISE", std::string("FastFading.PerLinkAndSC"))),
    f2ScCache_(config.get<unsigned int>("numberOfSubchannels")),
    systemName_(config.get<std::string>("systemName")),
    usedBy_(0)
{
}

void
PerLinkAndSC::onWorldCreated()
{
    usedBy_++;
    assure(usedBy_ <= 1, "This FastFading model can only be used for one link type pair." 
        << " Please create seperate object instances in Python to assure that, instead of "
        << " assigning a reference to the same object."); 

    std::string lName = logger_.getLoggerName();
    lName+= std::string("_");
    lName+= getLinkTypeString();
    logger_.setLoggerName(lName);

    rise::MetaSystemManager* mSysMan;
    mSysMan = rise::MetaSystemManager::getInstance();

    assure(mSysMan != NULL, "Failed to get MetaSystemmanager");

    rise::SystemManager* sMan;
    sMan = mSysMan->getSystemManagerBySystemName(systemName_);

    rise::SystemManager::StationContainer sc;
    sc = sMan->getAllStations();

    rise::SystemManager::StationIterator it;

    rise::SystemManager::StationContainer senders;
    rise::SystemManager::StationContainer receivers;

    for(it = sc.begin(); it != sc.end(); it++)
    {
        if((*it)->getTransmitterType() == getTransmitterType())
            senders.push_back(*it);
        if((*it)->getReceiverType() == getReceiverType())
            receivers.push_back(*it);
    }

    rise::SystemManager::StationIterator itS;
    rise::SystemManager::StationIterator itR;

    for(itS = senders.begin(); itS != senders.end(); itS++)
    {
        for(itR = receivers.begin(); itR != receivers.end(); itR++)
        {
            createNewLink(*(*itS)->getAntenna(), *(*itR)->getAntenna());
        }
    }

    MESSAGE_SINGLE(NORMAL, logger_, "Managing " << senders.size()
        << " senders and " << receivers.size() << " receivers; " 
        << senders.size() * receivers.size()
        << " links in total .");

    setLinks(links_, losLinks_);
}

wns::Ratio 
PerLinkAndSC::getFastFading(const antenna::Antenna& source,
				                 const antenna::Antenna& target,
				                 const wns::Frequency& frequency) const
{
    unsigned int sourceId = source.getStation()->getStationId();
    unsigned int targetId = target.getStation()->getStationId();            

    StationPair sp(sourceId, targetId);

    assure(links_.find(sp) != links_.end(), "No link " << sourceId << "->" << targetId);

    unsigned int sc = f2ScCache_.getSubchannelIndex(frequency);

    return doGetFastFading(links_.find(sp)->second, sc);
}

void
PerLinkAndSC::createNewLink(const antenna::Antenna& source,
				                const antenna::Antenna& target)
{
    unsigned int sourceId = source.getStation()->getStationId();
    unsigned int targetId = target.getStation()->getStationId(); 

    StationPair sp(sourceId, targetId);
    links_[sp] = nextLinkId_;

    nextLinkId_++;

    /* We use pointers here to assure for not NULL rather than to */
    /* catch type cast exceptions */
    static const pathloss::Pathloss* pl = &getParent()->getPathlossModel(
        getTransmitterType(), getReceiverType());    

    static const rise::scenario::pathloss::ILoSDependent* losDecider = 
        dynamic_cast<const rise::scenario::pathloss::ILoSDependent*>(pl);

    assure(losDecider != NULL, "Need pathloss model of base class ILoSDependent");

    double distance = (source.getPosition() - target.getPosition()).abs();
    bool los = losDecider->isLoS(source, target, distance);
    if(los)
        losLinks_.insert(links_[sp]);

    MESSAGE_SINGLE(NORMAL, logger_, "Added link from " << sourceId
        << " to " << targetId << " with LinkId " << links_[sp]
        << " and " << (los?"LoS":"NLoS") << " channel conditions."); 
}

