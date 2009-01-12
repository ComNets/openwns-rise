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

#ifndef _DEMODULATORGMSK_HPP
#define _DEMODULATORGMSK_HPP

#include <RISE/demodulator/Demodulator.hpp>

#include <WNS/PowerRatio.hpp>

namespace rise {
	/**
	 * @brief Demodulator for GMSK
	 * @ingroup DEMODULATOR
	 */
	class DemodulatorGMSK :
		public Demodulator
	{
	public:
		DemodulatorGMSK()
		{}
		/* Taken from: K. Murota, K. Hirade: GMSK Modulation for Digital
		   Mobile Radio Telephony. IEEE Transactions on Communications, No. 7,
		   pp. 1040-1050, Jul. 1981.
		   Warning: The formula in the paper uses GMSK with BT = 0.25, but in
		   GSM and TETRAPOL we have BT=0.3*/

		virtual double getBER(const wns::Ratio &aRatio);
		virtual ~DemodulatorGMSK();
	};
}
#endif //DEMODULATORGMSK_HPP



