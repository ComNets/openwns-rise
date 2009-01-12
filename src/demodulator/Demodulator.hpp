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

#ifndef __DEMODULATOR_HPP
#define __DEMODULATOR_HPP

#include <WNS/PowerRatio.hpp>

namespace rise {
	/**
	 * @brief Provides an interface that take Eb/N0 and computes a raw BER
	 * (after demodulation)
	 * @ingroup DEMODULATOR
	 */
	class Demodulator
	{
	public:
		virtual double getBER(const wns::Ratio &aRatio) = 0;
		virtual ~Demodulator();
	protected:
		//! Computes the Q function as found in
		// Proakis (3rd ed, 1995, eq. 2-1-98)
		/** Use this for BER calcualtion with linear Eb/N0 ratios */
		double Q(double x);
		// If someone should need a generalized Q function, we need an
		// algorithm to compute
		// double Q(double x, double y);
		Demodulator();
	};
}
#endif // __DEMODULATOR_HPP


