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
#include <RISE/receiver/PowerMeasurement.hpp>

using namespace rise::receiver;

PowerMeasurement::PowerMeasurement(rise::TransmissionObjectPtr t,
                                   wns::node::Interface* _sourceNode,
                                   wns::Power _rxPower,
                                   wns::Power _interference,
                                   wns::Ratio _omniAttenuation,
                                   std::vector<wns::Ratio> _postProcessingSINRFactor
                                   )
    : wns::service::phy::power::PowerMeasurementInterface(), // inherited
      rxPower(_rxPower),
      interference(_interference),
      omniAttenuation(_omniAttenuation),
      transmissionObjectPtr(t),
      sourceNode(_sourceNode),
      postProcessingSINRFactor(_postProcessingSINRFactor)
{
    assure(transmissionObjectPtr,"transmissionObjectPtr==NULL");

    wns::service::phy::phymode::PhyModeInterfacePtr t_phyModePtr = transmissionObjectPtr->getPhyModePtr();
    if (t_phyModePtr)
    {
        this->phyModePtr = t_phyModePtr;
    }
    else
    {
        this->phyModePtr = wns::service::phy::phymode::PhyModeInterfacePtr();
    }

    rise::Transmitter* transmitter = transmissionObjectPtr->getTransmitter();
    assure(transmitter != NULL,"transmitter==NULL");

    this->transmitterStation = transmitter->getStation();
    assure(transmitterStation != NULL,"transmitterStation==NULL");
}

PowerMeasurement::~PowerMeasurement()
{
}

const wns::Power
PowerMeasurement::getRxPower() const
{
    return rxPower;
}

const wns::Power
PowerMeasurement::getInterferencePower() const
{
    return interference;
}

const wns::Power
PowerMeasurement::getOmniInterferencePower() const
{
    return wns::Power::from_mW(interference.get_mW() / omniAttenuation.get_factor());
}

const wns::Ratio
PowerMeasurement::getSINR() const
{
    // interference contains noise
    return rxPower / interference;
}

const std::vector<wns::Ratio>
PowerMeasurement::getPostProcessingSINRFactor() const
{
    return postProcessingSINRFactor;
}

const double
PowerMeasurement::getMI() const
{
    assure(phyModePtr, "invalid phyModePtr");
    return phyModePtr->getSINR2MI(getSINR());
}

const double
PowerMeasurement::getMIB() const
{
    assure(phyModePtr, "invalid phyModePtr");
    return phyModePtr->getSINR2MIB(getSINR());
}

const wns::Power
PowerMeasurement::getTxPower() const
{
    return transmissionObjectPtr->getTxPower();
}

const wns::Ratio
PowerMeasurement::getPathLoss() const
{
    return transmissionObjectPtr->getTxPower()/rxPower;
}

const wns::Power
PowerMeasurement::getRSS() const
{
    return rxPower+interference;
}

const wns::service::phy::phymode::PhyModeInterfacePtr
PowerMeasurement::getPhyMode() const
{
    assure(phyModePtr,"phyModePtr==NULL");
    return phyModePtr;
}

const
rise::Station* PowerMeasurement::getSourceStation() const
{
    assure(transmitterStation != NULL,"transmitterStation==NULL");
    return transmitterStation;
}

//const
wns::node::Interface* PowerMeasurement::getSourceNode() const
{
    assure(sourceNode != NULL,"sourceNode==NULL");
    return sourceNode;
}

std::string PowerMeasurement::getString() const
{
    std::stringstream s;
    s << "[RxP="<<getRxPower()<<", I="<<getInterferencePower()<<", SINR="<<getSINR();
    if(postProcessingSINRFactor.size() > 1 or (not postProcessingSINRFactor.empty() and postProcessingSINRFactor[0].get_factor() != 1.0))
    {
        s << " +(";
        for(std::vector<wns::Ratio>::const_iterator it = postProcessingSINRFactor.begin();
            it != postProcessingSINRFactor.end();
            ++it)
        {
            s << *it << " ";
        }
        s << ")]";
    }
    else
    {
        s << "]";
    }
    return s.str();
}
