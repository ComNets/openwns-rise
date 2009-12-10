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

#include <RISE/transceiver/transmitter.hpp>
#include <RISE/medium/PhysicalResource.hpp>
#include <RISE/medium/Medium.hpp>
#include <RISE/RISE.hpp>
#include <RISE/stations/station.hpp>

#include <WNS/Functor.hpp>
#include <WNS/pyconfig/View.hpp>

#include <sstream>

using namespace rise;

Transmitter::Transmitter(const wns::pyconfig::View& config, Station* s, antenna::Antenna* a)
	: PropagationCharacteristic(config),
	  txPwr(),
	  transmitterId(nextid++),
 	  maxTxPwr(),
	  prc(),
	  antenna(a),
	  midFrequency(0),
	  log(config.getView("logger")),
	  active(false)
{
	MESSAGE_SINGLE(NORMAL, log, "rise::Transmitter created");
	this->startObserving(s);
}

Transmitter::~Transmitter() {
 	MESSAGE_BEGIN(NORMAL, log, m, "Shutting down ...");
 	MESSAGE_END();
}

void Transmitter::configureLogger()
{
 	if (RISE::getPyConfigView().get<bool>("debug.transmitter"))
	{
		log.switchOn();
 		pd_debugFlag = true;
		//MESSAGE_SINGLE(NORMAL, log, "configureLogger(): switchOn. transmitterId="<<transmitterId);
 	} else {
 		log.switchOff();
 		pd_debugFlag = false;
 	}
}

void Transmitter::startTransmitting(TransmissionObjectPtr transmissionObject, int32_t subCarrier)
{
    assure(prc.at(subCarrier), "SubCarrier with id " << subCarrier << " does not exist");
	prc[subCarrier]->startTransmission(transmissionObject);

 	MESSAGE_BEGIN(NORMAL, log, m, "Starting transmission");
 	m << " on subBand=" << subCarrier
	  << ", f=" << transmissionObject->getPhysicalResource()->getFrequency()
	  << ", b=" << transmissionObject->getPhysicalResource()->getBandwidth();
	if (transmissionObject->getPhyModePtr()) // != NULL
		m << ", m&c=" << transmissionObject->getPhyModePtr()->getString();
	MESSAGE_END();
}

void Transmitter::stopTransmitting(TransmissionObjectPtr transmissionObject)
{
	PhysicalResourceIterator itr, itrEND;
	itr = prc.begin();
	itrEND = prc.end();
	int subCarrier= 0;
	while( (!(*itr)->contains(transmissionObject)) && itr!=itrEND)
	{
		++itr;
		++subCarrier;
	}
    // assert that we have found the matching physicalResource
    assure(itr!=itrEND, "Cannot find physicalResource with given transmissionObject");

 	MESSAGE_BEGIN(NORMAL, log, m, "Stopping transmission. PhysicalResource: ");
	m << "f=" << transmissionObject->getPhysicalResource()->getFrequency()
	  << ", b=" << transmissionObject->getPhysicalResource()->getBandwidth()
	  << ", sc=" << subCarrier;
	if (transmissionObject->getPhyModePtr()) // != NULL
		m << ", m&c=" << transmissionObject->getPhyModePtr()->getString();
		//<< " " << transmissionObject.getPtr();
 	MESSAGE_END();
	// here it goes on with the transmission
	// see PhysicalResource::stopTransmission()
	// this calls PhysicalResource::notifyReceivers()
	(*itr)->stopTransmission(transmissionObject);
}

void Transmitter::tune(double f,double b,int32_t numberOfSubCarriers)
{
	MESSAGE_SINGLE(NORMAL, log, "Transmitter::tune(f="<<f<<",b="<<b<<",#SC="<<numberOfSubCarriers<<")");

	assure(!active, "Tune not possible. Transmitter still active.");
	assure(f>0, "No negative frequencies allowed.");
	assure(b>0, "No negative bandwidth allowed.");
	assure(numberOfSubCarriers>=0, "No negative number of sub-carriers allowed.");

	// complete retuning is only supported by now
	prc.clear();

	double carrierBandwidth = b/numberOfSubCarriers;
	double lowestFrequency = f - b/2;
	double firstCarrierMidFrequency = lowestFrequency + carrierBandwidth/2;

	for(int32_t i=0; i<numberOfSubCarriers; ++i) {
		double midFrequency = firstCarrierMidFrequency + i*carrierBandwidth;
		medium::Medium* m = medium::Medium::getInstance();
		medium::PhysicalResource* p = m->getPhysicalResource(midFrequency, carrierBandwidth);
		prc.push_back(p);
	}
}

std::string Transmitter::getDebugInfo()
{
	std::stringstream ss;
	// this throws "pure virtual method called - Segmentation fault" when called in the constructor
	assure(getStation() != NULL, "unknown station");
	ss << "Transmitter (" << transmitterId << ") belongs to Station (" << getStation()->getStationId() << ")";
	return ss.str();
}

void Transmitter::positionChanged()
{
	/* do nothing */
}

void Transmitter::positionWillChange()
{
	std::for_each(
		prc.begin(),
		prc.end(),
		std::bind2nd(
			std::mem_fun(
				&medium::PhysicalResource::mobilityUpdate),
			this));
}

int32_t Transmitter::nextid = 0;


