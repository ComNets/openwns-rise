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

PowerMeasurement::PowerMeasurement()
    : wns::service::phy::power::PowerMeasurementInterface(),
      rxPower(),
      interference(),
      omniAttenuation(),
      noise(),
      txPower(),
      phyModePtr(NULL),
      //distance(0),
      subChannel(0),
      beam(0),
      transmitterStation(NULL)
      //sourceNode(NULL),
      //systemManager(NULL)
{
}

// same interface values as OFDMAPhy::Station::receiveData()
PowerMeasurement::PowerMeasurement(wns::Power rxPower, wns::Power interference, wns::Ratio omniAttenuation)
    : wns::service::phy::power::PowerMeasurementInterface(),
      rxPower(rxPower),
      interference(interference),
      omniAttenuation(omniAttenuation),
      noise(), // todo
      txPower(), // todo
      phyModePtr(), // todo
      //distance(0), // todo
      subChannel(0), // todo
      beam(0),
      transmissionObjectPtr(NULL),
      transmitterStation(NULL)
      //sourceNode(NULL), // todo
      //systemManager(NULL)
{
}

// same interface values as OFDMAPhy::Station::receiveData()
PowerMeasurement::PowerMeasurement(rise::TransmissionObjectPtr t, wns::node::Interface* _sourceNode, wns::Power _rxPower, wns::Power _interference, wns::Ratio _omniAttenuation, int _subChannel, int _beam)
    : wns::service::phy::power::PowerMeasurementInterface(), // inherited
      rxPower(_rxPower),
      interference(_interference),
      omniAttenuation(_omniAttenuation),
      noise(), // todo
      //txPower(t->getTxPower()),
      //phyModePtr(),
      //distance(0), // todo
      subChannel(_subChannel),
      beam(_beam),
      transmissionObjectPtr(t),
      transmitterStation(NULL),
      sourceNode(_sourceNode) // we need a node provider to get that ourself (rise:no, ofdmaphy:yes)
      //systemManager(NULL)
{
	// these are the useful things we can get from the TransmissionObjectPtr t:
	assure(t,"transmissionObjectPtr==NULL");
	assure(transmissionObjectPtr,"transmissionObjectPtr==NULL");
	// TODO: replace t by member transmissionObjectPtr
	txPower = t->getTxPower();
	wns::service::phy::phymode::PhyModeInterfacePtr t_phyModePtr = t->getPhyModePtr(); // SmartPtr
	if (t_phyModePtr) { // !=NULL
		assure(t_phyModePtr,"phyModePtr==NULL");
		//phyModePtr = wns::service::phy::phymode::PhyModeInterfacePtr(t->getPhyModePtr());
		phyModePtr = t_phyModePtr;
	} else { // phyModePtr == NULL (happens in ofdmaphy::tests::OFDMATest::testGetRxPower)
		//phyModePtr = wns::service::phy::phymode::PhyModeInterfacePtr(&wns::service::phy::phymode::emptyPhyMode());
		phyModePtr = wns::service::phy::phymode::PhyModeInterfacePtr(); // empty
	}
	rise::Transmitter* transmitter = t->getTransmitter();
	assure(transmitter != NULL,"transmitter==NULL");
	// int32_t transmitterId = transmitter->getTransmitterId();
	// int32_t stationId = transmitter->getStation()->getStationId();
	// antenna::Antenna* antennaTx = transmitter->getAntenna();
	// antenna::Antenna* antennaRx = rise::receiver->getAntenna();
	//medium::PhysicalResource* physicalResource = t->getPhysicalResource();
	//assure(physicalResource != NULL,"physicalResource==NULL");
	// double f=physicalResource->getFrequency(); // Returns the mid-frequency
	// double b=physicalResource->getBandwidth();
	//rise::Station*
	transmitterStation = transmitter->getStation(); // static_cast<rise::Station*>(transmitter->getStation())
	assure(transmitterStation != NULL,"transmitterStation==NULL");
	//ofdmaphy::Station* transmitterStation = transmitter->getStation(); // static_cast<rise::Station*>(transmitter->getStation())
	//ofdmaphy::Station* OFDMAStation = dynamic_cast<ofdmaphy::Station*>(t->getTransmitter()->getStation());
	//std::string userName = OFDMAStation->getNode()->getName();
	//int32_t transmitterStationId = transmitterStation->getStationId();
	//wns::node::Interface* peerNode = transmitterStation->getNode(); // returns component->getNode()
	//wns::node::Interface* peerNode = ofdmaStation->getNode(); // returns component->getNode()
	// rise::SystemManager* systemManager = Station::getSystemManager()
	//ofdmaStation->systemManager->getStation(peerNode); // opposite way
	//Station* recipient = ofdmaStation->systemManager->getStation(wns::node::Interface* _recipient)
	//wns::osi::PDUPtr payload = t->getPayload();
	// /*rise::TransmissionObject::linkmode*/ uint32_t linkmode = t->getLinkMode(); // DL=0, UL=1, direct=2
	//txPower = t->getTxPower();
	//phyModePtr = t->getPhyModePtr();
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
	// ^ same as this:
	//return interference - omniAttenuation;
}

const wns::Power
PowerMeasurement::getNoisePower() const
{
	assure(false,"not yet implemented");
	return noise;
}

const wns::Ratio
PowerMeasurement::getSINR() const
{
	return rxPower / interference; // interference contains noise
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
	return txPower;
}

const wns::Ratio
PowerMeasurement::getPathLoss() const
{
	return txPower/rxPower;
}

const wns::Power
PowerMeasurement::getRSS() const // received signal strength (S+I)
{
	return rxPower+interference;
}

const wns::service::phy::phymode::PhyModeInterfacePtr
PowerMeasurement::getPhyMode() const
{
	assure(phyModePtr,"phyModePtr==NULL");
	return phyModePtr;
}

/*
// meters between sender and receiver [m]
const double PowerMeasurement::getDistance() const
{
	assure(false,"not yet implemented");
	//double Station::getDistance(PositionableInterface* p)
	// rise::Transmitter* transmitter    = transmissionObjectPtr->getTransmitter();
	// rise::Station* transmitterStation = transmitter->getStation();
	assure(transmitterStation != NULL,"transmitterStation==NULL");
	double distance = transmitterStation->getDistance(receiverStation);
	// other way:
	// wns::PositionableInterface* mobility via Layer2 class:
	// mobility = getNode()->getService<wns::PositionableInterface*>("mobility");
	// distance = mobility->getDistance( source->getService<wns::PositionableInterface*>("mobility") );
	return distance;
}
*/

// OFDMA subchannel used
const int PowerMeasurement::getSubChannel() const
{
	assure(false,"not yet implemented");
	return subChannel;
}

 // MIMO/beamforming beam
const int PowerMeasurement::getBeam() const
{
	assure(false,"not yet implemented");
	return beam;
}

const
rise::Station* PowerMeasurement::getSourceStation() const
{
	// rise::Transmitter* transmitter    = transmissionObjectPtr->getTransmitter();
	// rise::Station* transmitterStation = transmitter->getStation();
	assure(transmitterStation != NULL,"transmitterStation==NULL");
	return transmitterStation;
}

//const
wns::node::Interface* PowerMeasurement::getSourceNode() const
{
	// rise::Transmitter* transmitter    = transmissionObjectPtr->getTransmitter();
	// rise::Station* transmitterStation = transmitter->getStation();
	//wns::node::Interface* sourceNode = transmitterStation->getNode(); // only possible for OFDMAPhy
	// only OFDMAPhy::Station knows its node.
	assure(sourceNode != NULL,"sourceNode==NULL");
	return sourceNode;
}

/*
void PowerMeasurement::setSystemManager(rise::SystemManager* _systemManager) {
	systemManager=_systemManager;
}
*/

std::string PowerMeasurement::getString() const
{
	std::stringstream s;
	s << "[RxP="<<getRxPower()<<", I="<<getInterferencePower()<<", SINR="<<getSINR()<<"]";
	return s.str();
}
