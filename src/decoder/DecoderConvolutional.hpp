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

#ifndef DECODERCONVOLUTIONAL_HPP
#define DECODERCONVOLUTIONAL_HPP

#include <RISE/decoder/Decoder.hpp>

namespace rise {
	/**
	 * @briefThis decoder provides the BER of a convolutional coder. This is an
	 * analytic approximation for hard decision viterbi decoders
	 *
	 * @ingroup DECODER
	 */
	class DecoderConvolutional :
		public Decoder
	{
		public:
				DecoderConvolutional(int k,
									 int bdfree,
									 int dfree,
									 double punctuation = 1);

				virtual double getBER(double p_raw);

				virtual double getBER(double p_raw, double aPuncturingFactor);

				virtual ~DecoderConvolutional();
		private:
				//! number of info bits per time unit
				int K;
				//! Total number of nonzero information bits on all weight d_free paths
				int B_dfree;
				//! minimum free distance of the convolutional code
				int D_free;
				//! factor for puncturing
				double Punctuation;
		};
}

#endif //DECODERCONVOLUTIONAL_HPP


