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

#ifndef RISE_SCENARIO_PROPAGATIONMODEL_HPP
#define RISE_SCENARIO_PROPAGATIONMODEL_HPP

#include <RISE/scenario/Propagation.hpp>

namespace rise { namespace scenario {

    /**
     * @brief Class with common info for Pathloss, Shadowing and FastFading
     */
    class IPropagationModel
    {
    public:
        virtual void
        setParentAndTypeIDs(Propagation* parent, 
                            Propagation::IdType transmitterType, 
                            Propagation::IdType receiverType) = 0;

        virtual Propagation*
        getParent() const = 0;

        virtual Propagation::IdType
        getTransmitterType() const = 0;

        virtual Propagation::IdType
        getReceiverType() const = 0;

        virtual std::string
        getLinkTypeString() const = 0;
    };

    class PropagationModel :
        public IPropagationModel
    {
    public:
        PropagationModel();
        
        ~PropagationModel();

        virtual void
        setParentAndTypeIDs(Propagation* parent, 
                            Propagation::IdType transmitterType, 
                            Propagation::IdType receiverType);

        virtual Propagation*
        getParent() const;

        virtual Propagation::IdType
        getTransmitterType() const;

        virtual Propagation::IdType
        getReceiverType() const;

        virtual std::string
        getLinkTypeString() const;

    private:
        Propagation* parent_;
        Propagation::IdType transmitterType_;
        Propagation::IdType receiverType_;        
    };
} // scenario
} // rise

#endif // NOT defined RISE_SCENARIO_PROPAGATIONMODEL_HPP
