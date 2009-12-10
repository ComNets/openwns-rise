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

#include <RISE/stations/station.hpp>
#include <RISE/manager/systemmanager.hpp>
#include <RISE/RISE.hpp>
#include <RISE/scenario/scenerymap/SceneryMap.hpp>
#include <RISE/antenna/Antenna.hpp>
#include <RISE/antenna/Static.hpp>
#include <RISE/antenna/Beamforming.hpp>
#include <RISE/scenario/mobility/Mobility.hpp>
#include <RISE/scenario/sceneryfile/SceneryFile.hpp>

#include <WNS/Assure.hpp>

#include <stdexcept>

using namespace rise;

Station::Station(const wns::pyconfig::View& _pyConfigView) :
	pd_antenna(0),
	pyConfigView(_pyConfigView),
	log(_pyConfigView.getView("logger")),
	initialized(false),
	stationId(-1), // set later by systemmanager
	pathlossMap(NULL),
	interpolatedPathlossMap(NULL),
	shadowingMap(NULL),
	interpolatedShadowingMap(NULL),
	mobility(NULL)
{
	createAntenna();
}

Station::~Station()
{
	delete pd_antenna;
}

void
Station::createAntenna()
{
	wns::pyconfig::View antennaView = pyConfigView.getView("antennas", 0);
	assure(pyConfigView.len("antennas") == 1, "Only one antenna supported at the moment!");
	std::string s = antennaView.get<std::string>("__plugin__");
	if (s=="Internal"|| s=="Antenna3D"|| s=="Planet")
		pd_antenna = new antenna::Static(antennaView, this);
	else {
		wns::Exception e;
		e << "No such antenna type";
		throw e;
	}
}


void
Station::setMobility(rise::scenario::mobility::MobilityInterface* _mobility)
{
	assure(!mobility, "Mobility Already set.");
	mobility = _mobility;
	this->startObserving(this->mobility);

	this->positionChanged();

	// Now everything is complete, so we can say:
	MESSAGE_BEGIN(NORMAL, log, m, "Created at position ");
	m << this->getPosition();
	MESSAGE_END();
}

scenario::mobility::MobilityInterface*
Station::getMobility() const
{
	assure(mobility, "Mobility Not set.");
	return mobility;
}

const wns::Position& Station::getPosition() const
{
	// forward from mobility
	assure(mobility, "Mobility Not set.");
	return mobility->getPosition();
}

double Station::getDistance(PositionableInterface* p)
{
	// forward from mobility
	assure(mobility, "Mobility Not set.");
	return mobility->getDistance(p);
}

double Station::getDistance(const PositionableInterface& p)
{
	// forward from mobility
	assure(mobility, "Mobility Not set.");
	return mobility->getDistance(p);
}

double Station::getAngle(const PositionableInterface& p)
{
    // forward from mobility
	assure(mobility, "Mobility Not set.");
	return mobility->getAngle(p);
}


void Station::positionWillChange()
{
	// forward from mobility
	this->sendNotifies(&PositionObserver::positionWillChange);
}

void Station::positionChanged()
{
	// forward from mobility
	this->sendNotifies(&PositionObserver::positionChanged);
}

void Station::initialize()
{
	initializePathlossMap();
	initializeShadowingMap();
	initialized = true;
}

int32_t Station::getStationId() const
{
	assure(stationId >= 0, "Station ID not set yet!");
	return stationId;
}

void Station::setStationId(int32_t id)
{
	assure(id >= 0, "Only station IDs >= 0 allowed!");
	stationId = id;
}

antenna::Antenna* Station::getAntenna() const
{
	assure(pd_antenna, "Antenna not set so far!");
	return pd_antenna;
}

bool Station::isInitialized() const
{
	return initialized;
}

void
Station::moveTo(const wns::Position& p)
{
	assure(mobility, "Mobility Not set.");
	mobility->moveTo(p);
}

void
Station::moveTo(const wns::PositionOffset& p)
{
	assure(mobility, "Mobility Not set.");
	mobility->moveTo(p);
}

bool
Station::hasPathlossMap() const
{
	return (pathlossMap != NULL);
}

const scenario::scenerymap::PathlossMap2D&
Station::getPathlossMap() const
{
	assure(isInitialized(), "You must initialize() your station!");
	if (pathlossMap != NULL) std::logic_error("Cannot access non-existent pathloss map.");
	return *pathlossMap;
}

const scenario::scenerymap::Interpolation2D&
Station::getInterpolatedPathlossMap() const
{
	assure(isInitialized(), "You must initialize() your station!");
	if (interpolatedPathlossMap != NULL) std::logic_error("Cannot access non-existent pathloss map interpolation.");
	return *interpolatedPathlossMap;
}

bool
Station::hasShadowingMap() const
{
	return (shadowingMap != NULL);
}

const scenario::scenerymap::ShadowingMap2D&
Station::getShadowingMap() const
{
	assure(isInitialized(), "You must Initialize() your station!");
	if (shadowingMap != NULL) std::logic_error("Cannot access non-existent shadowing map.");
	return *shadowingMap;
}

const scenario::scenerymap::Interpolation2D&
Station::getInterpolatedShadowingMap() const
{
	assure(isInitialized(), "You must Initialize() your station!");
	if (interpolatedShadowingMap != NULL) std::logic_error("Cannot access non-existent shadowing map interpolation.");
	return *interpolatedShadowingMap;
}

void Station::initializePathlossMap()
{
	if (!pyConfigView.isNone("pathlossMap")) {
		scenario::Scenario& scenario = *(getSystemManager()->getScenario());
		const std::string fn = pyConfigView.get<std::string>("pathlossMap");
		scenario.openSceneryFile(fn, "SC");
		pathlossMap = &(scenario.getSceneryFile(fn).getPathlossMap()[stationId]);

		const std::string interpolationType = pyConfigView.get<std::string>("pathlossInterpolation");
		if (interpolationType == "NearestNeighbour")
			interpolatedPathlossMap = new scenario::scenerymap::NearestNeighbour2D(*pathlossMap);
		else if (interpolationType == "Bilinear")
			interpolatedPathlossMap = new scenario::scenerymap::Bilinear(*pathlossMap);
		else std::logic_error("Unsupported 'pathlossInterpolation'");
	}
}

void Station::initializeShadowingMap()
{
	if (!pyConfigView.isNone("shadowingMap")) {
		scenario::Scenario& scenario = *(getSystemManager()->getScenario());
		const std::string fn = pyConfigView.get<std::string>("shadowingMap");
		scenario.openSceneryFile(fn, "SC");
		shadowingMap = &(scenario.getSceneryFile(fn).getShadowingMap());

		const std::string interpolationType = pyConfigView.get<std::string>("shadowingInterpolation");
		if (interpolationType == "NearestNeighbour")
			interpolatedShadowingMap = new scenario::scenerymap::NearestNeighbour2D(*shadowingMap);
		else if (interpolationType == "Bilinear")
			interpolatedShadowingMap = new scenario::scenerymap::Bilinear(*shadowingMap);
		else std::logic_error("Unsupported 'shadowingInterpolation'");
	}
}

