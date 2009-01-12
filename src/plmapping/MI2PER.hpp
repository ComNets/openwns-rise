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

#ifndef _MI2PER_HPP
#define _MI2PER_HPP

#include <limits.h>
#include <WNS/PyConfigViewCreator.hpp>
#include <WNS/Enum.hpp>
#include <WNS/logger/Logger.hpp>
#include <WNS/container/Registry.hpp>
#include <WNS/container/Mapping.hpp>
#include <WNS/Singleton.hpp>
#include <WNS/pyconfig/View.hpp>

#include <RISE/plmapping/PhyMode.hpp>
#include <RISE/plmapping/CodeRates.hpp>

namespace rise { namespace plmapping {

	namespace tests {
		class MI2PERTest;
		class CoderFullTests;
	}

	/** @brief vector of known blocklengths */
	typedef std::vector< int > CodeWordLengths;

	/**
	 * @brief Bass Class for Full Channel Coder Specification with MI2PER conversion
	 */
	class CoderFullSpecification :
		virtual public wns::service::phy::phymode::CoderFullSpecInterface,
		public CoderSpecification // base class
	{
  	public:
		CoderFullSpecification(const wns::pyconfig::View&){}
		virtual ~CoderFullSpecification(){}

		/** @brief get PER for given MI and blockLength (=payload[net], not CWL[gross]) */
		virtual double mapMI2PER(double mi, unsigned int blockLength) const = 0;
		/** @brief get MI for a given PER target ratio and blockLength (=payload[net], not CWL[gross]) */
		/** this iterative method is not cheap: O(log(tableSize)) */
		virtual double PER2MIB(double per, unsigned int blockLength) const = 0;
	};

	typedef wns::PyConfigViewCreator<CoderFullSpecification> CoderSpecCreator;
	typedef wns::StaticFactory<CoderSpecCreator> CoderSpecFactory;

	/**
	 * @brief Implementation with MI2PER conversion table
	 */
	class TableCoder :
		public CoderFullSpecification
	{
		friend class tests::MI2PERTest;
		friend class tests::CoderFullTests;

		/** @brief lookup table from MI to PER, each index step represents a MI-step of 10^-3 */
		typedef std::vector< double > LookupTable;
		/** @brief maps from a codewordlength(int) to the appropriate Lookup Table */
		//typedef wns::container::Registry< int, LookupTable > SizeRegistry;
		/** @brief vector of tables for known blocklengths */
		typedef std::vector< LookupTable > VectorOfTablesByCWLSize;
	public:
		TableCoder(const wns::pyconfig::View& config);
		virtual ~TableCoder(){}
		/** @brief get PER for given MI and blockLength (=payload[net], not CWL[gross]) */
		virtual double mapMI2PER(double mi, unsigned int blockLength) const;
		/** @brief get MI for a given PER target ratio and blockLength (=payload[net], not CWL[gross]) */
		/** this iterative method is not cheap: O(log(tableSize)) */
		virtual double PER2MIB(double per, unsigned int blockLength) const;
	private:
		/**
		 * @brief get nearest matching CWL from table
		 * @todo msg@comnets.rwth-aachen.de 23.10.2007: blockLenght, CWL and all
		 * the other stuff should be "int" instead of "unsigned int" to be able
		 * to say assure(blockLength > 0, ...). This is useless with "unsigned
		 * int" since this is always true.
		 */
		virtual unsigned int suitableCWL(unsigned int CWL) const;
		/** @brief get vector of all known CWLs in table */
		virtual const CodeWordLengths& getCWLvector() const;
		/** @brief get nearest matching CWL (index) from table */
		virtual unsigned int suitableCWLposition(unsigned int CWL) const;

		unsigned int numberOfCWLs;
		/** @brief Lookup table = vectorOfTablesByCWLSize[0] */
		VectorOfTablesByCWLSize vectorOfTablesByCWLSize;
		/** @brief e.g. codeWordLengths[0..5]={300,600,...} */
		CodeWordLengths codeWordLengths;
		/** @brief the Logger */
		wns::logger::Logger logger;
	};

	/**
	 * @brief Implementation with MI2PER conversion formula
	 */
	class FormulaCoder :
		public CoderFullSpecification
	{
	public:
		FormulaCoder(const wns::pyconfig::View& config) :
			CoderFullSpecification(config)
		{}

		virtual ~FormulaCoder(){}

		/** @brief get PER for given MI and blockLength (=payload[net], not CWL[gross]) */
		virtual double mapMI2PER(double mi, unsigned int blockLength) const;
		/** @brief get MI for a given PER target ratio and blockLength (=payload[net], not CWL[gross]) */
		/** this iterative method is not cheap: O(log(tableSize)) */
		virtual double PER2MIB(double per, unsigned int blockLength) const;
	};

	/**
	 * @brief Full Channel Coder Mapping object (Singleton).
	 * This object contains a huge amount of data for the MI2PER tables.
	 * So keep this only one time in memory.
	 */
	class CoderFullMapping :
		virtual public wns::service::phy::phymode::CoderFullMappingInterface,
		public CoderMapping
	{
		friend class tests::MI2PERTest;
		friend class tests::CoderFullTests;
	public:
		CoderFullMapping(const wns::pyconfig::View& config);
		virtual ~CoderFullMapping();

		/** @brief get PER for given MI and blockLength (=payload[net], not CWL[gross]) */
		double mapMI2PER(double mi,
						 unsigned int blockLength,
						 Coding coding) const;

		/** @brief get MI for a given PER target ratio and blockLength (=payload[net], not CWL[gross]) */
		/** this iterative method is not cheap: O(log(tableSize)) */
		double PER2MIB(double per, unsigned int blockLength, Coding coding) const;

	private:
		/** @brief true, if the object is ready to use */
		virtual bool isConfigured() const;

		/* @brief get coder full specification */
		virtual const CoderFullSpecification&
		getCoderFullSpecification(Coding c) const;

		/** @brief maps from the Coding ID to the Sub-Map for the different block lengths */
		typedef std::vector< CoderFullSpecification* > CoderFullSpecVector;
		CoderFullSpecVector coderFullSpecVector;

		/** @brief logger */
		wns::logger::Logger logger;
	};


	/**
	 * @brief implementation the MI2PER mapper
	 */
	class GENERICM2P :
		virtual public wns::service::phy::phymode::MI2PERMappingInterface
	{
	public:
		GENERICM2P(const wns::pyconfig::View& config);
		virtual ~GENERICM2P(){}

		/** @brief PER=f(mi,BL,coding), blockLength=payload, not CWL */
		virtual double mapMI2PER(double mi,
								 unsigned int blockLength,
								 Coding coding) const;

	private:
		wns::service::phy::phymode::MI2PERMappingInterface* coderFullMapping;
	};

	/**
	 * @brief implementation of the WINNER L2S Interface
	 */
	typedef GENERICM2P WINNERM2P;

	/**
	 * @brief return a Fixed PER for debugging purposes
	 */
	class FixedM2P :
		virtual public wns::service::phy::phymode::MI2PERMappingInterface
	{
	public:
		explicit FixedM2P(const wns::pyconfig::View& config) :
			fixedPER(config.get<double>("PER"))
		{}
		virtual ~FixedM2P(){}
		virtual double mapMI2PER(double, unsigned int, Coding) const
			{
				return fixedPER;
			}
	private:
		double fixedPER;
	};

	/**
	 * @brief return PER for MI with an analytic calculation
	 */
	class FormulaM2P :
		virtual public wns::service::phy::phymode::MI2PERMappingInterface
	{
	public:
		explicit FormulaM2P(const wns::pyconfig::View&){}
		virtual ~FormulaM2P(){}
		virtual double mapMI2PER(double, unsigned int, Coding) const
			{
				return 0.0; // TODO
			}
	};

}} // namespaces

#endif //MI2PER_HPP


