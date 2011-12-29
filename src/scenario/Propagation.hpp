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

#ifndef _RISE_SCENARIO_PROPAGATION_HPP
#define _RISE_SCENARIO_PROPAGATION_HPP

#include <RISE/scenario/pathloss/Pathloss.hpp>
#include <RISE/scenario/shadowing/Shadowing.hpp>
#include <RISE/scenario/fastfading/FastFading.hpp>

#include <WNS/container/Matrix.hpp>
#include <WNS/StaticFactoryBroker.hpp>
#include <WNS/Broker.hpp>
#include <WNS/Singleton.hpp>
#include <WNS/pyconfig/View.hpp>
#include <WNS/Ttos.hpp>
#include <WNS/Exception.hpp>

#include <string>

namespace rise { namespace scenario { 

    /**
     * @brief Propagation matrix.
     *
     * This class holds matrices containing the configured propagation models for each propagation characteristic pair.
     */
    class Propagation
    {
	typedef wns::container::Matrix<pathloss::Pathloss*, 2> PathlossMatrix;
	typedef wns::container::Matrix<shadowing::Shadowing*, 2> ShadowingMatrix;
	typedef wns::container::Matrix<fastfading::FastFading*, 2> FastFadingMatrix;
    public:

	typedef PathlossMatrix::SizeType IdType;

	/**
	 * @brief Thrown, if a model for a not configured pair was requested.
	 */
	class NoModelError :
	    public wns::Exception
	{
	public:
	    NoModelError(const std::string& modelType,
			 const std::string& transmitterName,
			 const std::string& receiverName);
	
        virtual ~NoModelError() throw() {};
    };

	Propagation(const wns::pyconfig::View& config);

	virtual
	~Propagation();

	/**
	 * @brief Return model for (transmitterid, receiverId).
	 *
	 * Throws NoModelError, if no model was configured.
	 */
	const pathloss::Pathloss&
	getPathlossModel(const IdType& transmitterId, const IdType& receiverId) const;

	/**
	 * @brief Return model for (transmitterid, receiverId).
	 *
	 * Throws NoModelError, if no model was configured.
	 */
	const shadowing::Shadowing&
	getShadowingModel(const IdType& transmitterId, const IdType& receiverId) const;

	/**
	 * @brief Return model for (transmitterid, receiverId).
	 *
	 * Throws NoModelError, if no model was configured.
	 */
	const fastfading::FastFading&
	getFastFadingModel(const IdType& transmitterId, const IdType& receiverId) const;

	/**
	 * @brief Create a propagation model using a StaticFactoryBroker.
	 */
	template<class C>
	static C*
	create(const wns::pyconfig::View& config)
	{
	    const std::string model = config.get<std::string>("__plugin__");
 	    return getBroker<C>().procure(model, config);
	}

	std::string
	getName(const IdType& id) const;

    private:
	PathlossMatrix pathlossMatrix;
	ShadowingMatrix shadowingMatrix;
	FastFadingMatrix fastFadingMatrix;

	const wns::pyconfig::View config;

 	template<class C>
 	static wns::StaticFactoryBroker<C, wns::PyConfigViewCreator<C> >&
 	getBroker()
 	{
 	    static wns::StaticFactoryBroker<C, wns::PyConfigViewCreator<C> > broker;
 	    return broker;
 	}

    };

    typedef wns::Broker<Propagation> PropagationBroker;
    typedef wns::SingletonHolder<PropagationBroker> GlobalPropagationBroker;

} // scenario
} // rise

#endif // NOT defined _RISE_SCENARIO_PROPAGATION_HPP
