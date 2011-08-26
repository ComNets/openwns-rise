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

#include <RISE/scenario/fastfading/Jakes.hpp>

using namespace rise::scenario::fastfading;

STATIC_FACTORY_BROKER_REGISTER(Jakes, FastFading, "Jakes");

Jakes::Jakes(const wns::pyconfig::View& config) : 
    PerLinkAndSC(config),
    genConfig_(config.getView("ftFadingJakes")),
    logger_(config.get("logger"))
{
}

Jakes::~Jakes()
{
    for(int i = 0; i < generators_.size(); i++)
    {
        if(generators_[i] != NULL)
        {
            delete generators_[i];
        }
    }
}

wns::Ratio
Jakes::doGetFastFading(const unsigned int link, const unsigned int sc, 
    const unsigned int servingLink) const
{
    assure(link < generators_.size(), "LinkId out of range");

    if(generators_[link] == NULL)
    {
        std::string ftfadingName = genConfig_.get<std::string>("ftfadingName");

        MESSAGE_SINGLE(NORMAL, logger_, "Creating FTFadingJakes strategy " 
            << ftfadingName << " for link " << link);

        rise::scenario::fastfading::jakes::FTFadingJakes::FTFadingCreator* FTFadingCreator =
            rise::scenario::fastfading::jakes::FTFadingJakes::FTFadingFactory::creator(ftfadingName);

        generators_[link] = FTFadingCreator->create(genConfig_);
    }

    assure(generators_[link] != NULL, "No Jakes generator for link " << link);
    return generators_[link]->getFTFading(sc);
}

void
Jakes::setLinks(const PerLinkAndSC::LinkMap& links, 
    const std::set<unsigned int>& losSet)
{
    generators_.resize(links.size());
    generators_.assign(links.size(), NULL);

}

