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

#ifndef _STATIPATTERNKERNEL_HPP
#define _STATIPATTERNKERNEL_HPP

#include <RISE/misc/RISELogger.hpp>
#include <WNS/PowerRatio.hpp>
#include <WNS/Direction.hpp>
#include <vector>

namespace rise { namespace antenna { namespace pattern { namespace kernel {
	//! A pattern kernel for static antenna patterns
	class Kernel
	{
	public:
		typedef std::map<std::string, Kernel*> PKContainer;
		typedef std::vector<double> Double64Vector;

		Kernel();
		Kernel(const std::string t, const std::string p);
		virtual ~Kernel();

		static Kernel* createPatternKernel(const std::string type,
										   const std::string pattern);
		static Kernel* getPatternKernel(const wns::pyconfig::View& antennaView);
		virtual wns::Ratio getGain(const wns::Direction& pOffset) = 0;

	protected:
		virtual void pd_startUp() = 0;
		std::string type;
		std::string pattern;
		/**
		 * @brief Local Logger
		 */
		RISELogger log;
		double pd_gain;
	private:
		static PKContainer p_availablePatterns;
	};
}}}}
#endif //_PATTERNKERNELSTATIC_HPP

