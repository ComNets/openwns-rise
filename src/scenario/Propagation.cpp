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

#include <RISE/scenario/Propagation.hpp>
#include <RISE/scenario/pathloss/Pathloss.hpp>
#include <RISE/scenario/shadowing/Shadowing.hpp>
#include <RISE/scenario/fastfading/FastFading.hpp>

#include <WNS/pyconfig/helper/Functions.hpp>
#include <WNS/Ttos.hpp>
#include <WNS/Assure.hpp>

using namespace rise::scenario;

Propagation::NoModelError::NoModelError(const std::string& modelType,
					const std::string& transmitterName,
					const std::string& receiverName)
    : wns::Exception("No " + modelType + "-model configured for " + transmitterName + " -> " + receiverName)
{
}

Propagation::Propagation(const wns::pyconfig::View& config)
    : config(config),
    initialized_(false),
    logger_(wns::logger::Logger("RISE", "Propagation"))
{
    PathlossMatrix::SizeType sizes[2];
    sizes[0] = sizes[1] = config.get<std::size_t>("maxId()");
    pathlossMatrix = PathlossMatrix(sizes, NULL);
    shadowingMatrix = ShadowingMatrix(sizes, NULL);
    fastFadingMatrix = FastFadingMatrix(sizes, NULL);

    for (PathlossMatrix::SizeType i = 0; i < sizes[0]; ++i)
	{
		for (PathlossMatrix::SizeType j = 0; j < sizes[1]; ++j) 
		{
			if (config.get<bool>("knowsPairById(" + wns::Ttos(i) + ", " + wns::Ttos(j) + ")")) 
			{
				wns::pyconfig::View pairView = config.getView("getPair(" + wns::Ttos(i) + ", " + wns::Ttos(j) + ")");
				pathlossMatrix[i][j] = create<pathloss::Pathloss>(pairView.getView("pathloss"));
				shadowingMatrix[i][j] = create<shadowing::Shadowing>(pairView.getView("shadowing"));
				fastFadingMatrix[i][j] = create<fastfading::FastFading>(pairView.getView("fastFading"));

                pathlossMatrix[i][j]->setParentAndTypeIDs(this, i, j);
                shadowingMatrix[i][j]->setParentAndTypeIDs(this, i, j);
                fastFadingMatrix[i][j]->setParentAndTypeIDs(this, i, j);
			}
	    }
	}
}

Propagation::~Propagation()
{
}

void
Propagation::onWorldCreated()
{    
    if(initialized_)
        return;

    for (PathlossMatrix::SizeType i = 0; i < config.get<std::size_t>("maxId()"); ++i)
    {
        for (PathlossMatrix::SizeType j = 0; j < config.get<std::size_t>("maxId()"); ++j) 
    	{
			if (config.get<bool>("knowsPairById(" + wns::Ttos(i) + ", " + wns::Ttos(j) + ")")) 
			{
                MESSAGE_SINGLE(NORMAL, logger_, "Initializing models for [" 
                    << getName(i) << ";" << getName(j) << "]");

                pathlossMatrix[i][j]->onWorldCreated();
                shadowingMatrix[i][j]->onWorldCreated();            
                fastFadingMatrix[i][j]->onWorldCreated();
            }
        }
    }
    initialized_ = true;	
}

const pathloss::Pathloss&
Propagation::getPathlossModel(const IdType& transmitterId, const IdType& receiverId) const
{
    pathloss::Pathloss* model = pathlossMatrix[transmitterId][receiverId];
    if (model == NULL)
	throw NoModelError("pathloss", getName(transmitterId), getName(receiverId));
    return *model;
}

const shadowing::Shadowing&
Propagation::getShadowingModel(const IdType& transmitterId, const IdType& receiverId) const
{
    shadowing::Shadowing* model = shadowingMatrix[transmitterId][receiverId];
    if (model == NULL)
	throw NoModelError("shadowing", getName(transmitterId), getName(receiverId));
    return *model;
}

const fastfading::FastFading&
Propagation::getFastFadingModel(const IdType& transmitterId, const IdType& receiverId) const
{
    fastfading::FastFading* model = fastFadingMatrix[transmitterId][receiverId];
    if (model == NULL)
	throw NoModelError("shadowing", getName(transmitterId), getName(receiverId));
    return *model;
}

std::string
Propagation::getName(const IdType& id) const
{
    return config.get<std::string>("findName(" + wns::Ttos(id) + ")");
}

