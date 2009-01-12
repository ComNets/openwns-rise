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

#ifndef RISE_PLMAPPING_SNR2MI_HPP
#define RISE_PLMAPPING_SNR2MI_HPP

#include <WNS/PowerRatio.hpp>
#include <WNS/container/Mapping.hpp>
#include <WNS/Enum.hpp>
#include <WNS/Singleton.hpp>
#include <WNS/service/phy/phymode/SNR2MIInterface.hpp>

namespace rise { namespace plmapping { 

	namespace tests {
		class SNR2MITest;
		class CoderFullTests;
	}

	const unsigned int MAX_MODULATIONS_FOR_TABLE = 6; // only up to QAM64

	/**
	 * @brief returns the Mutual Information (MI) value per Bit
	 * corresponding to the SNR and Modulation value
     * This is independent of coding and other parameters
     * Valid for WINNER, ScaleNet, WiMac, WigWam, ...
	 * @see http://en.wikipedia.org/wiki/Mutual_information
	 * @author Rainer Schoenen (rs@comnets.rwth-aachen.de)
	 */
	class SNR2MI :
		virtual public wns::service::phy::phymode::SNR2MIInterface
	{
		friend class tests::SNR2MITest;
		friend class tests::CoderFullTests;
	public:
		virtual ~SNR2MI(){}
		/** @brief returns the Mutual Information value per Bit (MIB) */
	    virtual double convertSNR2MIB(const wns::Ratio& snr, const wns::service::phy::phymode::PhyModeInterface& phyMode) const;
		/** @brief returns the Mutual Information value (MI) (not per Bit) */
	    virtual double convertSNR2MI(const wns::Ratio& snr, const wns::service::phy::phymode::PhyModeInterface& phyMode) const;
		/** @brief invert the SNR2MIB function */
        /** @author Rainer Schoenen (rs@comnets.rwth-aachen.de) */
		double MIB2SNR(double mib, wns::service::phy::phymode::Modulation modTYPE) const;
	private:
		/** @brief the actual conversion method, has to be overloaded */
		virtual double mapSNR2MI(const wns::Ratio& snr, wns::service::phy::phymode::Modulation modTYPE) const = 0;
		/** @brief returns the Mutual Information value per Bit (MIB) */
	    double convertSNR2MIB(const wns::Ratio& snr, wns::service::phy::phymode::Modulation modTYPE) const;
		/** @brief functional dependency between BER (bit error ratio) and MIB (mutual information) */
		/** @see http://de.wikipedia.org/wiki/Kanalkapazit%C3%A4t */
        /** @author Rainer Schoenen (rs@comnets.rwth-aachen.de) */
		static double BER2MIB(double ber);
		/** @brief functional dependency between BER (bit error ratio) and MIB (mutual information) */
		/** this iterative method is not cheap: O(log(1/precision)) */
        /** @author Rainer Schoenen (rs@comnets.rwth-aachen.de) */
		static double MIB2BER(double mib);
	};


	/** @brief Implementation of the SNR2MI Mapping using a (slower, but less
	 * memory-consuming) mapping formula */
	class SNR2MIFormula :
		public SNR2MI
	{
	public:
		SNR2MIFormula(){};
		virtual ~SNR2MIFormula(){};

	private:
		/** @brief returns the Mutual Information (MI) value per Bit */
	    virtual double mapSNR2MI(const wns::Ratio& snr, wns::service::phy::phymode::Modulation modTYPE) const;
	};


	/** @brief Implementation of the SNR2MI Mapping using static mapping tables */
	class SNR2MIMap :
		public SNR2MI,
		public wns::container::Mapping<wns::service::phy::phymode::Modulation, double>
	{
	public:
		SNR2MIMap();
		virtual ~SNR2MIMap(){};

	private:
		/** @brief returns the Mutual Information (MI) value per Bit (read from table)  */
	    virtual double mapSNR2MI(const wns::Ratio& snr, wns::service::phy::phymode::Modulation modTYPE) const;

		/** @brief the init* methods contain the hard-coded table data */
	    void initBPSK();
	    void initQPSK();
	    void initQAM8();
	    void initQAM16();
	    void initQAM32();
	    void initQAM64();

	};


	inline double log2(double x) { return log(x)/log(2); }

	/** @brief find the root (Nullstelle) of a function */
	/** function must be static, if it is a member of a class */
	/** @author Rainer Schoenen (rs@comnets.rwth-aachen.de) */
	inline double rootFindByBisection(double (*func)(double), double offset, double x1, double x2, double accuracy)
	{
		const unsigned int maxIterations = 100;
		unsigned int iteration;
		double f    = (*func)(x1) - offset;
		double fmid = (*func)(x2) - offset;
		assure(f*fmid<0.0, "rootFindByBisection(): Root must be bracketed within ["<<x1<<","<<x2<<"]: f="<<f<<", fmid="<<fmid);
		double rtb = (f<0.0) ? x1 : x2;
		double dx  = (f<0.0) ? (x2-x1) : (x1-x2);
		for(iteration=1; iteration<=maxIterations; iteration++) {
			double xmid=rtb+(dx *= 0.5);
			fmid = (*func)(xmid) - offset;
			if (fmid <= 0.0) rtb=xmid;
			if ((fabs(dx) < accuracy) || (fmid == 0.0)) return rtb;
		}
		assure(false,"rootFindByBisection(): too many iterations: "<<maxIterations);
		return rtb;
	}

}}
#endif // RISE_PLMAPPING_SNR2MI_HPP


