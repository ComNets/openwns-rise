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

#ifndef _DEMODULATORBPSK_HPP
#define _DEMODULATORBPSK_HPP

#include <RISE/demodulator/Demodulator.hpp>
#include <WNS/PowerRatio.hpp>

namespace rise {
	/**
	 * @brief Demodulator for Binary Phase Shift Keying (BPSK)
	 * @ingroup DEMODULATOR
	 */
	class DemodulatorBPSK :
		public Demodulator
	{
	public:
		DemodulatorBPSK() {};
		virtual double getBER(const wns::Ratio &aRatio);
		virtual ~DemodulatorBPSK();
	};
}
#endif //DEMODULATORBPSK_HPP



