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

#include <RISE/scenario/mobility/Component.hpp>
#include <RISE/scenario/mobility/Mobility.hpp>

#include <WNS/probe/bus/ContextProvider.hpp>
#include <boost/bind.hpp>
#include <iomanip>

using namespace rise::scenario::mobility;

STATIC_FACTORY_REGISTER_WITH_CREATOR(
	rise::scenario::mobility::Component,
	wns::node::component::Interface,
	"rise.mobility.Component",
	wns::node::component::ConfigCreator
	);

Component::Component(
	wns::node::Interface* node,
	const wns::pyconfig::View& pyConfigView) :

	wns::node::component::Component(node, pyConfigView),
	mobility(NULL),
	logger(pyConfigView.get("logger"))
{
	// create mobility
	wns::pyconfig::View mobilityView = pyConfigView.get("mobility");
	std::string plugin = mobilityView.get<std::string>("nameInMobilityFactory");
	mobility = rise::scenario::mobility::MobilityFactory::creator(plugin)->create(mobilityView);

	MESSAGE_SINGLE(NORMAL, logger, "Done creating rise::scenario::mobility::Component of type " << plugin);
}

void
Component::doStartup()
{
	wns::probe::bus::ContextProviderCollection& cpc = getContextProviderCollection();
	cpc.addProvider(wns::probe::bus::contextprovider::Callback(
				"rise.scenario.mobility.x",
				boost::bind(&Component::getX, this)));

	cpc.addProvider(wns::probe::bus::contextprovider::Callback(
				"rise.scenario.mobility.y",
				boost::bind(&Component::getY, this)));

	cpc.addProvider(wns::probe::bus::contextprovider::Callback(
				"rise.scenario.mobility.z",
				boost::bind(&Component::getZ, this)));

	// register station as a PHY DataTransmissionService
	this->addService("mobility", mobility);

	// register myself to Mobility
	this->startObserving(this->mobility);

    positionContextCollectorX = wns::probe::bus::ContextCollectorPtr(
        new wns::probe::bus::ContextCollector(cpc,
                              "rise.scenario.mobility.PositionX"));
    positionContextCollectorY = wns::probe::bus::ContextCollectorPtr(
        new wns::probe::bus::ContextCollector(cpc,
                              "rise.scenario.mobility.PositionY"));
    positionContextCollectorZ = wns::probe::bus::ContextCollectorPtr(
        new wns::probe::bus::ContextCollector(cpc,
                              "rise.scenario.mobility.PositionZ"));

	MESSAGE_SINGLE(NORMAL, logger, "Done rise::scenario::mobility::Component::startup()");
}

Component::~Component()
{
	delete mobility;
}

void
Component::onNodeCreated()
{
}

void
Component::onWorldCreated()
{
}

void
Component::onShutdown()
{
}

void
Component::positionWillChange()
{
}

void
Component::positionChanged()
{
    positionContextCollectorX->put(mobility->getPosition().getX());
    positionContextCollectorY->put(mobility->getPosition().getY());
    positionContextCollectorZ->put(mobility->getPosition().getZ());
}

int
Component::getX()
{
	return (int)mobility->getPosition().getX();
}

int
Component::getY()
{
	return (int)mobility->getPosition().getY();
}

int
Component::getZ()
{
	return (int)mobility->getPosition().getZ();
}


