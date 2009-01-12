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

#ifndef _PATTERNKERNEL2D_HPP
#define _PATTERNKERNEL2D_HPP

#include <RISE/antenna/pattern/kernel/Kernel.hpp>
#include <WNS/PowerRatio.hpp>

namespace rise { namespace antenna { namespace pattern { namespace kernel {
	//! A pattern kernel for static 2D patterns
	class TwoDimensional : public Kernel
	{
	public:
		TwoDimensional();
		TwoDimensional(const std::string type,
						const std::string pattern);
		static Kernel* createPatternKernel(const std::string,
										   const std::string pattern);
		virtual wns::Ratio getGain(const wns::Direction& direction);

	protected:

		virtual void pd_startUp();
		//! resolution (rad per step) for elevation
		double                pd_elevationAngleStep;
		//! resolution (rad per step) for azimuth
		double                pd_azimuthAngleStep;
		//! pointer to array of directivity (elevation)
		Double64Vector          pd_elevationVector;
		//! pointer to array of directivity (azimuth)
		Double64Vector          pd_azimuthVector;
	};
}}}}
#endif


