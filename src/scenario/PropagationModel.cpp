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

#include <RISE/scenario/PropagationModel.hpp>

#include <sstream>

using namespace rise::scenario;

PropagationModel::PropagationModel() : 
    parent_(NULL)
{
}

PropagationModel::~PropagationModel()
{
}

void
PropagationModel::setParentAndTypeIDs(Propagation* parent, 
                            Propagation::IdType transmitterType, 
                            Propagation::IdType receiverType)
{
    assure(parent != NULL, "Propagation cannot be NULL");
    parent_ = parent;
    transmitterType_ = transmitterType;
    receiverType_ = receiverType;
}

Propagation*
PropagationModel::getParent() const
{
    return parent_;
}

Propagation::IdType
PropagationModel::getTransmitterType() const
{
    return transmitterType_;
}

Propagation::IdType
PropagationModel::getReceiverType() const
{
    return receiverType_;
}

std::string
PropagationModel::getLinkTypeString() const
{
    assure(getParent() != NULL, "Parent not set");

    std::stringstream s;
    s << getParent()->getName(transmitterType_);
    s << "->" << getParent()->getName(receiverType_);
    return s.str();
}



