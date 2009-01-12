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

#ifndef RISE_ANTENNA_BEAMFORMING_HPP
#define RISE_ANTENNA_BEAMFORMING_HPP

#include <map>
#include <vector>
#include <boost/numeric/ublas/vector.hpp>
#include <boost/numeric/ublas/matrix.hpp>
#include <complex>

#include <WNS/distribution/Norm.hpp>
#include <WNS/CandI.hpp>
#include <RISE/antenna/Antenna.hpp>
#include <RISE/misc/pointer.hpp>

namespace rise { namespace antenna {
	/**
	 * @brief Beamforming antenna
	 * Optimal Beamformer,
	 * L. C. Godara. Application of antenna arrays to mobile communications,
	 * part II: Beamforming and direction of arrival considerations. Proceedings of
	 * the IEEE, 85(8):1193-1245, 1997.
	 *
	 */
	typedef std::vector<PatternPtr> PatternContainer;
	typedef std::map<TransmissionObjectPtr, PatternPtr> TO2PatternMap;
	class Beamforming :
		public Antenna
	{
	public:
		Beamforming(const wns::pyconfig::View& pyConfigView,
					Station* const station);

		virtual ~Beamforming();

		virtual wns::Ratio getGain(const wns::Position& pos,
								   const PatternPtr pattern) const;

		virtual wns::Ratio getGain(const wns::Position& pos,
								   const TransmissionObjectPtr& TO) const;

		virtual std::map<rise::Station*, wns::CandI >
		calculateCandIsRx(const std::vector<Station*>& combination,
						  wns::Power iInterPlusNoise);

		virtual std::map<rise::Station*, wns::CandI>
		calculateCandIsTx(const std::map<Station*, wns::Power>& station2iIntercellPlusNoise,
						  wns::Power xFriendlyness,
						  wns::Power txPower,
						  bool eirpLimited);

		virtual double
		estimateDoA(Station* id);

		virtual PatternPtr calculateAndSetBeam(Station* id,
											  const std::vector<Station*>& undesired,
											  wns::Power IinterPlusNoise);

		/**
		 * @brief Set the power level that is received from station 'id'
		 *
		 * This value is the power level actually received at the Beamforming
		 * antenna. That is, TX-power of station - pathloss to Beamforming antenna.
		 *
		 * It is mandatory to set this value before using any Beamforming functions.
		 *
		 */
		virtual void setPowerReceivedForStation(Station* const id,
												const wns::Power power);
		/**
		 * @brief Set the power level that the station 'id' uses to transmit
		 *
		 * This value is the power level actually used by the station 'id' to transmit.
		 * It is mandatory to set this value before using calculateSINRsTx because from
		 * this value and the value set with setPowerReceivedForStation the pathloss
		 * between the Beamforming antenna and the station is calculated. The pathloss
		 * is required for the correct calculation of the SINRs in the TX-case.
		 */
		virtual void setTxPowerForStation(Station* const id,
										  const wns::Power power);

		virtual void startReceiving(const TransmissionObjectPtr& TO,
									PatternPtr pattern);

		virtual void stopReceiving(const TransmissionObjectPtr& TO);

		enum layoutType
		{
			linear = 0,
			circular
		};

		virtual void drawRadiationPattern(std::string file,
										  PatternPtr pattern);

	protected:
		virtual wns::Ratio pd_getGainEntry(uint32_t index,
										   PatternPtr patternId) const;
		uint32_t pd_noAntennaElements;
		std::map <Station*, wns::Power> pd_lastPowerReceived;
		std::map <Station*, wns::Power> pd_lastTxPower;
		std::map <Station*, double> pd_azimuthAngles;
		TO2PatternMap pd_receivedTO2PatternMap;
		layoutType arrayLayout;

		virtual void drawRadiationPattern() const;

	private:

		std::vector<boost::numeric::ublas::vector<std::complex<double> >*> steervectors;
		void p_herm_rest(boost::numeric::ublas::matrix<std::complex<double> >& c, const uint32_t m) const;

		PatternPtr p_calculateBeam(Station* id,
								  const std::vector<Station*>& undesired,
								  wns::Power IinterPlusNoise);
		double calculatePhaseDelta(int antennaNo, double stationPhi) const;

		/**
		 * @brief variance of the Gaussian Distribution
		 */
		double positionErrorVariance;

		/**
		 * @brief pointer to the used Gaussian Normal Distribution
		 */
		wns::distribution::Norm dis;
	};
}}
#endif  // NOT defined RISE_ANTENNA_BEAMFORMING_HPP


