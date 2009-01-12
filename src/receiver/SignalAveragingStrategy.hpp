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

#ifndef __SIGNALAVERAGINGSTRATEGY_HPP
#define __SIGNALAVERAGINGSTRATEGY_HPP


#include <RISE/transceiver/transmitter.hpp>

#include <WNS/simulator/ISimulator.hpp>
#include <WNS/events/scheduler/Interface.hpp>

#include <WNS/Assure.hpp>
#include <WNS/PowerRatio.hpp>
#include <WNS/TimeWeightedAverage.hpp>

#include <map>

namespace rise { namespace receiver {
	class PathlossCalculationInterface
	{
	public:
		virtual ~PathlossCalculationInterface() {}

	protected:
		virtual wns::Ratio getLoss(Transmitter* t, double frequency) = 0;
	};

	class SignalCalculationInterface
	{
	public:
		/**
		 * @brief Current signal level of Transmission t
		 */
		virtual wns::Power getRxPower(const TransmissionObjectPtr& t) = 0; // implemented in rise::Receiver
		virtual ~SignalCalculationInterface() {}
	};


	class InterferenceCalculationInterface
	{
	public:
		/**
		 * @brief Current interference level of Transmission t
		 */
		virtual wns::Power getInterference(const TransmissionObjectPtr& t) = 0;
		virtual ~InterferenceCalculationInterface() {}
	};

	/**
	 * @brief Allows averaging of signals
	 */
	class TransmissionAveragingStrategy :
		virtual public SignalCalculationInterface,
		virtual public InterferenceCalculationInterface
	{
	public:
		virtual ~TransmissionAveragingStrategy()
		{}

		/**
		 * @brief Call this if the signal levels have changed (due to movement
		 * or something else)
		 *
		 * This will update the averages
		 */
		virtual void signalLevelsChange() = 0;

		/**
		 * @brief This has to be called if a transmission has ended
		 *
		 * After this method has been called the averaging is stopped. Calling
		 * signalLevelsChanged() will have no effect on Transmission "t".
		 */
		virtual void endOfTransmission(const TransmissionObjectPtr& t) = 0;

		/**
		 * @brief Calling this results in Transmission "t" being consindered for
		 * averaging.
		 */
		virtual void add(const TransmissionObjectPtr& t) = 0;

		/**
		 * @brief Remove a Transmission
		 */
		virtual void remove(const TransmissionObjectPtr& t) = 0;

		/**
		 * @brief Remove all Transmission
		 */
		virtual void removeAll() = 0;

		/**
		 * @brief Returns average signal level of Transmission t (so far)
		 *
		 * @note In case the Transmission is ongoing the signal level is
		 * averaged up to the moment the method is called.
		 */
		virtual wns::Power getAveragedRxPower(const TransmissionObjectPtr& t) = 0;

		/**
		 * @brief Returns average interference level of Transmission t (so far)
		 *
		 * @note In case the Transmission is ongoing the interference level is
		 * averaged up to the moment the method is called.
		 */
		virtual wns::Power getAveragedInterference(const TransmissionObjectPtr& t) = 0;

		/**
		 * @brief Returns the averaged CIR level of Transmission t (so far)
		 *
		 * @note In case the Transmission is ongoing the CIR level is
		 * averaged up to the moment the method is called.
		 */
		virtual wns::Ratio getAveragedCIR(const TransmissionObjectPtr& t) = 0;
	};

	/**
	 * @brief This may be used if no averaging is needed (e.g. the system is
	 * synchronized)
	 */
	class NoTransmissionAveraging :
		virtual public TransmissionAveragingStrategy
	{
		typedef std::map<TransmissionObjectPtr, wns::Power> CurrentSignalMap;
		typedef std::map<TransmissionObjectPtr, bool> CalledEndOfTransmission;
	public:
		NoTransmissionAveraging();
		virtual ~NoTransmissionAveraging();

		// TransmissionAveragingStrategy interface
		virtual void signalLevelsChange();
		virtual void endOfTransmission(const TransmissionObjectPtr& t);
		virtual void add(const TransmissionObjectPtr& t);
		virtual void remove(const TransmissionObjectPtr& t);
		virtual void removeAll();
		virtual wns::Power getAveragedRxPower(const TransmissionObjectPtr& t);
		virtual wns::Power getAveragedInterference(const TransmissionObjectPtr& t);
		virtual wns::Ratio getAveragedCIR(const TransmissionObjectPtr& t);
	private:
		CurrentSignalMap carrier;
		CurrentSignalMap interference;
		CalledEndOfTransmission calledEOT;
		simTimeType lastAveraging;
	};

	/**
	 * @brief This may be used if a time-weighted averaging is needed (e.g. the
	 * system is not synchronized)
	 */
	class TimeWeightedTransmissionAveraging :
		virtual public TransmissionAveragingStrategy
	{
		typedef wns::TimeWeightedAverage<wns::Power, wns::events::scheduler::Interface> SignalAverage;
		typedef std::map<TransmissionObjectPtr, SignalAverage> AveragedSignalMap;
		typedef std::map<TransmissionObjectPtr, bool> CalledEndOfTransmission;
	public:
		TimeWeightedTransmissionAveraging();
		virtual ~TimeWeightedTransmissionAveraging();

		// TransmissionAveragingStrategy interface
		virtual void signalLevelsChange();
		virtual void endOfTransmission(const TransmissionObjectPtr& t);
		virtual void add(const TransmissionObjectPtr& t);
		virtual void remove(const TransmissionObjectPtr& t);
		virtual void removeAll();
		virtual wns::Power getAveragedRxPower(const TransmissionObjectPtr& t);
		virtual wns::Power getAveragedInterference(const TransmissionObjectPtr& t);
		virtual wns::Ratio getAveragedCIR(const TransmissionObjectPtr& t);

	private:
		AveragedSignalMap carrier;
		AveragedSignalMap interference;
		CalledEndOfTransmission calledEOT;
		simTimeType lastAveraging;
	};
}}
#endif // not defined __SIGNALAVERAGINGSTRATEGY_HPP



