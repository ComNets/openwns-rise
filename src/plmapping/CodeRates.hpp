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

#ifndef RISE_PLMAPPING_CODERSPECIFICATION_HPP
#define RISE_PLMAPPING_CODERSPECIFICATION_HPP

#include <WNS/service/phy/phymode/PhyModeInterface.hpp>
#include <WNS/service/phy/phymode/MI2PERInterface.hpp>
#include <WNS/container/Registry.hpp>
#include <WNS/Singleton.hpp>
#include <WNS/pyconfig/View.hpp>
#include <string>
#include <vector>

namespace rise { namespace plmapping {

	/** @brief int values for Coding: see my constuctor in .cpp and LLMapping.py */
	typedef wns::service::phy::phymode::Coding Coding;
	const Coding UNDEFINED_CODING = 0u; // global constant

	/** @brief Basic Specification of ONE Coder
	 corresponds to Python class rise.plmapping.CoderSpecification */
	class CoderSpecification :
		virtual public wns::service::phy::phymode::CoderSpecInterface
	{
	public:
		CoderSpecification(Coding _coding, double _rate, std::string& _name) :
			coding(_coding),
			rate(_rate),
			name(_name)
		{}
		virtual ~CoderSpecification(){}
		Coding getCoding() const { return coding; }
		double getRate() const { return rate; }
		bool hasNoCoding() const { return (rate==1.0); }
		const std::string& getName() const { return name; } // by reference
	protected:
		CoderSpecification() {}; // empty constructor needed for inherited class
		Coding coding; // integer index
		double rate;
		std::string name;
	};

	/** @brief Class to match std::string representation of Code Rates to Integers,
	 * code rates and vice versa.
	 * corresponds to Python class rise.plmapping.CoderMapping */
	class CoderMapping :
		virtual public wns::service::phy::phymode::CoderMappingInterface	{
	protected:
		unsigned int maxCodeIndex;
		std::vector<CoderSpecification>                coderSpecificationVector;
		wns::container::Registry<std::string, Coding>  codename2int;
	public:
		/* @brief Constructor */
		CoderMapping(const wns::pyconfig::View& config);
		/* @brief Destructor */
		virtual ~CoderMapping(){};

		/** @brief true, if the object is ready to use */
		virtual bool isConfigured() const;

		/** @brief number of elements in table (plus one more) */
		unsigned int
		getMaxCodeIndex() const { return maxCodeIndex; };

		/* @brief get coding rate for coder given by name */
		virtual double
		getRate(const std::string& s) const;

		/* @brief get coding rate */
		virtual double
		getRate(Coding coding) const;

		/* @brief get coding (int) from name */
		virtual Coding
		fromString(const std::string& s) const;

		/* @brief get coder name */
		virtual const std::string&
		getString(Coding c) const;

		/* @brief get coder specification */
		virtual const CoderSpecification&
		getCoderSpecification(Coding c) const;
	};

}}

#endif // RISE_PLMAPPING_CODERSPECIFICATION_HPP
