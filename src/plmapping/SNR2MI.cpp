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

#include <RISE/plmapping/SNR2MI.hpp>
#include <RISE/plmapping/PhyMode.hpp>
#include <WNS/StaticFactoryBroker.hpp>

using namespace rise;
using namespace rise::plmapping;
using namespace rise::plmapping::Modulations;

STATIC_FACTORY_BROKER_REGISTER_PLAIN(SNR2MIMap,
									 wns::service::phy::phymode::SNR2MIInterface,
									 "rise.SNR2MI.Table");

STATIC_FACTORY_BROKER_REGISTER_PLAIN(SNR2MIFormula,
									 wns::service::phy::phymode::SNR2MIInterface,
									 "rise.SNR2MI.Formula");

/************************* Base Class ***********************/

double SNR2MI::convertSNR2MIB(const wns::Ratio& snr, const wns::service::phy::phymode::PhyModeInterface& phyMode) const
{
	return convertSNR2MI(snr, phyMode) / (double) phyMode.getModulation(); // MI per bit
}

double SNR2MI::convertSNR2MI(const wns::Ratio& snr, const wns::service::phy::phymode::PhyModeInterface& phyMode) const
{
	wns::service::phy::phymode::Modulation modTYPE = phyMode.getModulation();
	return mapSNR2MI(snr, modTYPE); // MI
}

double SNR2MI::convertSNR2MIB(const wns::Ratio& snr, wns::service::phy::phymode::Modulation modTYPE) const
{
	return mapSNR2MI(snr, modTYPE) / (double) modTYPE; // MI per bit
}


double SNR2MI::MIB2SNR(double mib, wns::service::phy::phymode::Modulation modTYPE) const
{
	// nullstellenproblem: find x satisfying f(x)=0 with f(x)=SNR2MI(snr)-mib
	// solution: see rootFindByBisection() function in .hpp
	const unsigned int maxIterations = 100;
	const double accuracy = 1e-6; // determines (runtime) cost of this function
	double &offset = mib; // alias
	unsigned int iteration=0;
	// initial bracket: [-inf;+inf] would be too much
	double x1       = -10.0;
	double x2       = 5.0+4.0*modTYPE;
	// convertSNR2MIB() should be switched to "formula" mode for best results
	double f    = convertSNR2MIB(wns::Ratio().from_dB(x1),modTYPE) - offset;
	double fmid = convertSNR2MIB(wns::Ratio().from_dB(x2),modTYPE) - offset;
	if (f==0.0)    return x1;
	if (fmid==0.0) return x2;
	// expand bracket if necessary:
	//const double expandFactor = 1.6; // numerical recipes recommendation
	while((f*fmid>=0.0) && (++iteration<maxIterations)) {
		if (f>=0.0) {
			//x1+=expandFactor*(x1-x2); // move left
			x1-=5.0; // 5dB to the left
			f = convertSNR2MIB(wns::Ratio().from_dB(x1),modTYPE) - offset;
			if (f==0.0)    return x1;
		}
		if (fmid<=0.0) {
			//x2+=expandFactor*(x2-x1); // move right
			x2+=5.0; // 5dB to the right
			fmid = convertSNR2MIB(wns::Ratio().from_dB(x2),modTYPE) - offset;
			if (fmid==0.0) return x2;
		}
	}
	// now bracket should be ok (negative@x1, positive@x2):
	assure(f*fmid<0.0, "rootFindByBisection(): Root must be bracketed within ["<<x1<<","<<x2<<"]: f="<<f<<", fmid="<<fmid);
	double rtb = (f<0.0) ? x1 : x2;
	double dx  = (f<0.0) ? (x2-x1) : (x1-x2);
	for(iteration=1; iteration<=maxIterations; iteration++) {
		double xmid=rtb+(dx *= 0.5);
		fmid = convertSNR2MIB(wns::Ratio().from_dB(xmid),modTYPE) - offset;
		if (fmid <= 0.0) rtb=xmid;
		if ((fabs(dx) < accuracy) || (fmid == 0.0)) return rtb;
	}
	assure(false,"rootFindByBisection(): too many iterations: "<<maxIterations);
	return rtb;
}

double SNR2MI::BER2MIB(double ber)
{
	assure(ber<=1.0,"BER2MIB(): BER must be <= 1.0");
	//assure(ber<=0.5+1e-6,"BER2MIB(): BER must be <= 0.5");
	assure(ber>=0.0,"BER2MIB(): BER must be >= 0.0");
	// http://de.wikipedia.org/wiki/Kanalkapazit%C3%A4t
    // result = 1 + ( (1-e)*log2(1-e) + e*log2(e) ); % Matlab code
	if (ber<=1e-12) return 1.0; // border value
	if (ber>=1.0) return 1.0; // border value
	return 1.0 + ( (1.0-ber)*log2(1.0-ber) + ber*log2(ber) );
}

double SNR2MI::MIB2BER(double mib)
{
	//assure(false,"MIB2BER not yet implemented");
	if (mib<=0.0) return 0.5;
	if (mib>=1.0) return 0.0;
	return rootFindByBisection(&BER2MIB, /*offset*/mib, 0.0,0.5, /*accuracy*/1e-6);
}



/************************* Mapping by Formula ***********************/

double SNR2MIFormula::mapSNR2MI(const wns::Ratio& snr, wns::service::phy::phymode::Modulation modTYPE) const
{
	assure(modTYPE > UNDEFINED_MODULATION, "invalid Modulation");
	//assure(modTYPE <= MAX_MODULATIONS, "unknown Modulation"); // the formula should be valid for all, even QAM1024...
	double shannon = log2(1.0+pow(10.0,(snr.get_dB()/10.0)));
	double sf = 0.95 - (modTYPE % 2)*0.08;
	double w = 2*modTYPE+1;
	double sh = sf * shannon;
	// mi = 1./(sh.^(-w) + modIndex^(-w)).^(1/w)
	double mi = 1/pow((pow(sh,-w) + pow(modTYPE,-w)),1.0/w);

	return mi; // full MI (not per bit)
}


/************************* Mapping by Table ***********************/


SNR2MIMap::SNR2MIMap() :
	wns::container::Mapping<Modulation, double>(30)
{
    initBPSK();
	initQPSK();
	initQAM8();
	initQAM16();
	initQAM32();
	initQAM64();
	//std::cout << "SNR2MI::SNR2MI()" << std::endl;
}



double SNR2MIMap::mapSNR2MI(const wns::Ratio& snr, wns::service::phy::phymode::Modulation modTYPE) const
{
	assure(modTYPE > UNDEFINED_MODULATION, "invalid Modulation");
	assure(modTYPE <= MAX_MODULATIONS, "unknown Modulation");
	assure(modTYPE <= MAX_MODULATIONS_FOR_TABLE, "No table for Modulation");
	// handle out-of-range
	if (snr >=  wns::Ratio::from_factor(1000))
		return ( (double)modTYPE );
	if (snr <=  wns::Ratio::from_factor(0.001))
		return ( 0.0 );
	double snr_dB  = snr.get_dB();
	int snr_dB_int = int(floor(snr_dB));
	// else do linear interpolation
	double lower = map(snr_dB_int   , modTYPE);
	double upper = map(snr_dB_int+1 , modTYPE);
	double mi  = lower +  ((upper - lower) * (snr_dB - snr_dB_int )); // linear interpolation

	return mi; // full MI (not per bit)
}


void SNR2MIMap::initBPSK()
{
	mf[BPSK()].resize(61);
    mf[BPSK()][  0] = 0	 ;
    mf[BPSK()][  1] = 0	 ;
    mf[BPSK()][  2] = 0	 ;
    mf[BPSK()][  3] = 0	 ;
    mf[BPSK()][  4] = 0	 ;
    mf[BPSK()][  5] = 0	 ;
    mf[BPSK()][  6] = 0	 ;
    mf[BPSK()][  7] = 0	 ;
    mf[BPSK()][  8] = 0	 ;
    mf[BPSK()][  9] = 0	 ;
    mf[BPSK()][ 10] = 0.014284 ;
    mf[BPSK()][ 11] = 0.017937 ;
    mf[BPSK()][ 12] = 0.02251	 ;
    mf[BPSK()][ 13] = 0.028226 ;
    mf[BPSK()][ 14] = 0.035357 ;
    mf[BPSK()][ 15] = 0.044236 ;
    mf[BPSK()][ 16] = 0.055259 ;
    mf[BPSK()][ 17] = 0.068899 ;
    mf[BPSK()][ 18] = 0.085706 ;
    mf[BPSK()][ 19] = 0.10631	 ;
    mf[BPSK()][ 20] = 0.13142	 ;
    mf[BPSK()][ 21] = 0.16177	 ;
    mf[BPSK()][ 22] = 0.19814	 ;
    mf[BPSK()][ 23] = 0.24123	 ;
    mf[BPSK()][ 24] = 0.29159	 ;
    mf[BPSK()][ 25] = 0.34951	 ;
    mf[BPSK()][ 26] = 0.41482	 ;
    mf[BPSK()][ 27] = 0.48671	 ;
    mf[BPSK()][ 28] = 0.5636	 ;
    mf[BPSK()][ 29] = 0.64297	 ;
    mf[BPSK()][ 30] = 0.72145	 ;
    mf[BPSK()][ 31] = 0.79507	 ;
    mf[BPSK()][ 32] = 0.8598	 ;
    mf[BPSK()][ 33] = 0.91235	 ;
    mf[BPSK()][ 34] = 0.95101	 ;
    mf[BPSK()][ 35] = 0.97618	 ;
    mf[BPSK()][ 36] = 0.99026	 ;
    mf[BPSK()][ 37] = 0.9968	 ;
    mf[BPSK()][ 38] = 0.9992	 ;
    mf[BPSK()][ 39] = 0.99986	 ;
    mf[BPSK()][ 40] = 0.99998	 ;
    mf[BPSK()][ 41] = 1	 ;
    mf[BPSK()][ 42] = 1	 ;
    mf[BPSK()][ 43] = 1	 ;
    mf[BPSK()][ 44] = 1	 ;
    mf[BPSK()][ 45] = 1	 ;
    mf[BPSK()][ 46] = 1	 ;
    mf[BPSK()][ 47] = 1	 ;
    mf[BPSK()][ 48] = 1	 ;
    mf[BPSK()][ 49] = 1	 ;
    mf[BPSK()][ 50] = 1	 ;
    mf[BPSK()][ 51] = 1	 ;
    mf[BPSK()][ 52] = 1	 ;
    mf[BPSK()][ 53] = 1	 ;
    mf[BPSK()][ 54] = 1	 ;
    mf[BPSK()][ 55] = 1	 ;
    mf[BPSK()][ 56] = 1	 ;
    mf[BPSK()][ 57] = 1	 ;
    mf[BPSK()][ 58] = 1	 ;
    mf[BPSK()][ 59] = 1	 ;
    mf[BPSK()][ 60] = 1  ;
}
void SNR2MIMap::initQPSK()
{
	mf[QPSK()].resize(61);
    mf[QPSK()][  0] = 0	 ;
    mf[QPSK()][  1] = 0	 ;
    mf[QPSK()][  2] = 0	 ;
    mf[QPSK()][  3] = 0	 ;
    mf[QPSK()][  4] = 0	 ;
    mf[QPSK()][  5] = 0	 ;
    mf[QPSK()][  6] = 0	 ;
    mf[QPSK()][  7] = 0	 ;
    mf[QPSK()][  8] = 0	 ;
    mf[QPSK()][  9] = 0	 ;
    mf[QPSK()][ 10] = 0.014355 ;
    mf[QPSK()][ 11] = 0.018049 ;
    mf[QPSK()][ 12] = 0.022686 ;
    mf[QPSK()][ 13] = 0.028502 ;
    mf[QPSK()][ 14] = 0.035791 ;
    mf[QPSK()][ 15] = 0.044915 ;
    mf[QPSK()][ 16] = 0.05632	 ;
    mf[QPSK()][ 17] = 0.070551 ;
    mf[QPSK()][ 18] = 0.088268 ;
    mf[QPSK()][ 19] = 0.11027	 ;
    mf[QPSK()][ 20] = 0.13749	 ;
    mf[QPSK()][ 21] = 0.17103	 ;
    mf[QPSK()][ 22] = 0.21215	 ;
    mf[QPSK()][ 23] = 0.26226	 ;
    mf[QPSK()][ 24] = 0.32286	 ;
    mf[QPSK()][ 25] = 0.39546	 ;
    mf[QPSK()][ 26] = 0.48149	 ;
    mf[QPSK()][ 27] = 0.58207	 ;
    mf[QPSK()][ 28] = 0.69776	 ;
    mf[QPSK()][ 29] = 0.82822	 ;
    mf[QPSK()][ 30] = 0.97189	 ;
    mf[QPSK()][ 31] = 1.1256	 ;
    mf[QPSK()][ 32] = 1.2843	 ;
    mf[QPSK()][ 33] = 1.4413	 ;
    mf[QPSK()][ 34] = 1.5887	 ;
    mf[QPSK()][ 35] = 1.7184	 ;
    mf[QPSK()][ 36] = 1.8238	 ;
    mf[QPSK()][ 37] = 1.9014	 ;
    mf[QPSK()][ 38] = 1.952	 ;
    mf[QPSK()][ 39] = 1.9803	 ;
    mf[QPSK()][ 40] = 1.9935	 ;
    mf[QPSK()][ 41] = 1.9984	 ;
    mf[QPSK()][ 42] = 1.9997	 ;
    mf[QPSK()][ 43] = 2	 ;
    mf[QPSK()][ 44] = 2	 ;
    mf[QPSK()][ 45] = 2	 ;
    mf[QPSK()][ 46] = 2	 ;
    mf[QPSK()][ 47] = 2	 ;
    mf[QPSK()][ 48] = 2	 ;
    mf[QPSK()][ 49] = 2	 ;
    mf[QPSK()][ 50] = 2	 ;
    mf[QPSK()][ 51] = 2	 ;
    mf[QPSK()][ 52] = 2	 ;
    mf[QPSK()][ 53] = 2	 ;
    mf[QPSK()][ 54] = 2	 ;
    mf[QPSK()][ 55] = 2	 ;
    mf[QPSK()][ 56] = 2	 ;
    mf[QPSK()][ 57] = 2	 ;
    mf[QPSK()][ 58] = 2	 ;
    mf[QPSK()][ 59] = 2	 ;
    mf[QPSK()][ 60] = 2  ;
}

void SNR2MIMap::initQAM8()
{
	mf[QAM8()].resize(61);
    mf[QAM8()][  0] = 0	 ;
    mf[QAM8()][  1] = 0	 ;
    mf[QAM8()][  2] = 0	 ;
    mf[QAM8()][  3] = 0	 ;
    mf[QAM8()][  4] = 0	 ;
    mf[QAM8()][  5] = 0	 ;
    mf[QAM8()][  6] = 0	 ;
    mf[QAM8()][  7] = 0	 ;
    mf[QAM8()][  8] = 0	 ;
    mf[QAM8()][  9] = 0	 ;
    mf[QAM8()][ 10] = 0.014324 ;
    mf[QAM8()][ 11] = 0.017999 ;
    mf[QAM8()][ 12] = 0.022608 ;
    mf[QAM8()][ 13] = 0.028379 ;
    mf[QAM8()][ 14] = 0.035598 ;
    mf[QAM8()][ 15] = 0.044614 ;
    mf[QAM8()][ 16] = 0.05585	 ;
    mf[QAM8()][ 17] = 0.069819 ;
    mf[QAM8()][ 18] = 0.087136 ;
    mf[QAM8()][ 19] = 0.10852	 ;
    mf[QAM8()][ 20] = 0.13482	 ;
    mf[QAM8()][ 21] = 0.167	 ;
    mf[QAM8()][ 22] = 0.2061	 ;
    mf[QAM8()][ 23] = 0.25329	 ;
    mf[QAM8()][ 24] = 0.30976	 ;
    mf[QAM8()][ 25] = 0.37668	 ;
    mf[QAM8()][ 26] = 0.45515	 ;
    mf[QAM8()][ 27] = 0.54608	 ;
    mf[QAM8()][ 28] = 0.65018	 ;
    mf[QAM8()][ 29] = 0.7679	 ;
    mf[QAM8()][ 30] = 0.8994	 ;
    mf[QAM8()][ 31] = 1.0446	 ;
    mf[QAM8()][ 32] = 1.2032	 ;
    mf[QAM8()][ 33] = 1.3746	 ;
    mf[QAM8()][ 34] = 1.5575	 ;
    mf[QAM8()][ 35] = 1.7501	 ;
    mf[QAM8()][ 36] = 1.9493	 ;
    mf[QAM8()][ 37] = 2.1501	 ;
    mf[QAM8()][ 38] = 2.3451	 ;
    mf[QAM8()][ 39] = 2.5252	 ;
    mf[QAM8()][ 40] = 2.681	 ;
    mf[QAM8()][ 41] = 2.805	 ;
    mf[QAM8()][ 42] = 2.894	 ;
    mf[QAM8()][ 43] = 2.9502	 ;
    mf[QAM8()][ 44] = 2.9805	 ;
    mf[QAM8()][ 45] = 2.9939	 ;
    mf[QAM8()][ 46] = 2.9986	 ;
    mf[QAM8()][ 47] = 2.9998	 ;
    mf[QAM8()][ 48] = 3	 ;
    mf[QAM8()][ 49] = 3	 ;
    mf[QAM8()][ 50] = 3	 ;
    mf[QAM8()][ 51] = 3	 ;
    mf[QAM8()][ 52] = 3	 ;
    mf[QAM8()][ 53] = 3	 ;
    mf[QAM8()][ 54] = 3	 ;
    mf[QAM8()][ 55] = 3	 ;
    mf[QAM8()][ 56] = 3	 ;
    mf[QAM8()][ 57] = 3	 ;
    mf[QAM8()][ 58] = 3	 ;
    mf[QAM8()][ 59] = 3	 ;
    mf[QAM8()][ 60] = 3  ;

}

void SNR2MIMap::initQAM16()
{
	mf[QAM16()].resize(61);
    mf[QAM16()][  0] = 0	 ;
    mf[QAM16()][  1] = 0	 ;
    mf[QAM16()][  2] = 0	 ;
    mf[QAM16()][  3] = 0	 ;
    mf[QAM16()][  4] = 0	 ;
    mf[QAM16()][  5] = 0	 ;
    mf[QAM16()][  6] = 0	 ;
    mf[QAM16()][  7] = 0	 ;
    mf[QAM16()][  8] = 0	 ;
    mf[QAM16()][  9] = 0	 ;
    mf[QAM16()][ 10] = 0.014355 ;
    mf[QAM16()][ 11] = 0.018049 ;
    mf[QAM16()][ 12] = 0.022686 ;
    mf[QAM16()][ 13] = 0.028502 ;
    mf[QAM16()][ 14] = 0.035791 ;
    mf[QAM16()][ 15] = 0.044915 ;
    mf[QAM16()][ 16] = 0.05632	 ;
    mf[QAM16()][ 17] = 0.070551 ;
    mf[QAM16()][ 18] = 0.08827	 ;
    mf[QAM16()][ 19] = 0.11027	 ;
    mf[QAM16()][ 20] = 0.1375	 ;
    mf[QAM16()][ 21] = 0.17105	 ;
    mf[QAM16()][ 22] = 0.2122	 ;
    mf[QAM16()][ 23] = 0.26237	 ;
    mf[QAM16()][ 24] = 0.3231	 ;
    mf[QAM16()][ 25] = 0.396	 ;
    mf[QAM16()][ 26] = 0.48263	 ;
    mf[QAM16()][ 27] = 0.58443	 ;
    mf[QAM16()][ 28] = 0.70252	 ;
    mf[QAM16()][ 29] = 0.83759	 ;
    mf[QAM16()][ 30] = 0.98974	 ;
    mf[QAM16()][ 31] = 1.1585	 ;
    mf[QAM16()][ 32] = 1.3427	 ;
    mf[QAM16()][ 33] = 1.541	 ;
    mf[QAM16()][ 34] = 1.7517	 ;
    mf[QAM16()][ 35] = 1.9732	 ;
    mf[QAM16()][ 36] = 2.2036	 ;
    mf[QAM16()][ 37] = 2.4413	 ;
    mf[QAM16()][ 38] = 2.6837	 ;
    mf[QAM16()][ 39] = 2.9269	 ;
    mf[QAM16()][ 40] = 3.1639	 ;
    mf[QAM16()][ 41] = 3.3852	 ;
    mf[QAM16()][ 42] = 3.5794	 ;
    mf[QAM16()][ 43] = 3.7371	 ;
    mf[QAM16()][ 44] = 3.853	 ;
    mf[QAM16()][ 45] = 3.9285	 ;
    mf[QAM16()][ 46] = 3.9708	 ;
    mf[QAM16()][ 47] = 3.9904	 ;
    mf[QAM16()][ 48] = 3.9976	 ;
    mf[QAM16()][ 49] = 3.9996	 ;
    mf[QAM16()][ 50] = 3.9999	 ;
    mf[QAM16()][ 51] = 4	 ;
    mf[QAM16()][ 52] = 4	 ;
    mf[QAM16()][ 53] = 4	 ;
    mf[QAM16()][ 54] = 4	 ;
    mf[QAM16()][ 55] = 4	 ;
    mf[QAM16()][ 56] = 4	 ;
    mf[QAM16()][ 57] = 4	 ;
    mf[QAM16()][ 58] = 4	 ;
    mf[QAM16()][ 59] = 4	 ;
    mf[QAM16()][ 60] = 4     ;

}

void SNR2MIMap::initQAM32()
{
	mf[QAM32()].resize(61);
    mf[QAM32()][  0] = 0	 ;
    mf[QAM32()][  1] = 0	 ;
    mf[QAM32()][  2] = 0	 ;
    mf[QAM32()][  3] = 0	 ;
    mf[QAM32()][  4] = 0	 ;
    mf[QAM32()][  5] = 0	 ;
    mf[QAM32()][  6] = 0	 ;
    mf[QAM32()][  7] = 0	 ;
    mf[QAM32()][  8] = 0	 ;
    mf[QAM32()][  9] = 0	 ;
    mf[QAM32()][ 10] = 0.014328 ;
    mf[QAM32()][ 11] = 0.018007 ;
    mf[QAM32()][ 12] = 0.022619 ;
    mf[QAM32()][ 13] = 0.028397 ;
    mf[QAM32()][ 14] = 0.035627 ;
    mf[QAM32()][ 15] = 0.044658 ;
    mf[QAM32()][ 16] = 0.055919 ;
    mf[QAM32()][ 17] = 0.069928 ;
    mf[QAM32()][ 18] = 0.087305 ;
    mf[QAM32()][ 19] = 0.10878	 ;
    mf[QAM32()][ 20] = 0.13522	 ;
    mf[QAM32()][ 21] = 0.16761	 ;
    mf[QAM32()][ 22] = 0.20703	 ;
    mf[QAM32()][ 23] = 0.25468	 ;
    mf[QAM32()][ 24] = 0.31183	 ;
    mf[QAM32()][ 25] = 0.37974	 ;
    mf[QAM32()][ 26] = 0.45959	 ;
    mf[QAM32()][ 27] = 0.55248	 ;
    mf[QAM32()][ 28] = 0.65924	 ;
    mf[QAM32()][ 29] = 0.78052	 ;
    mf[QAM32()][ 30] = 0.91665	 ;
    mf[QAM32()][ 31] = 1.0677	 ;
    mf[QAM32()][ 32] = 1.2337	 ;
    mf[QAM32()][ 33] = 1.4141	 ;
    mf[QAM32()][ 34] = 1.6084	 ;
    mf[QAM32()][ 35] = 1.8159	 ;
    mf[QAM32()][ 36] = 2.0353	 ;
    mf[QAM32()][ 37] = 2.2656	 ;
    mf[QAM32()][ 38] = 2.5053	 ;
    mf[QAM32()][ 39] = 2.7533	 ;
    mf[QAM32()][ 40] = 3.0083	 ;
    mf[QAM32()][ 41] = 3.2688	 ;
    mf[QAM32()][ 42] = 3.5331	 ;
    mf[QAM32()][ 43] = 3.7978	 ;
    mf[QAM32()][ 44] = 4.0565	 ;
    mf[QAM32()][ 45] = 4.2996	 ;
    mf[QAM32()][ 46] = 4.5152	 ;
    mf[QAM32()][ 47] = 4.6924	 ;
    mf[QAM32()][ 48] = 4.8249	 ;
    mf[QAM32()][ 49] = 4.9129	 ;
    mf[QAM32()][ 50] = 4.9634	 ;
    mf[QAM32()][ 51] = 4.9875	 ;
    mf[QAM32()][ 52] = 4.9967	 ;
    mf[QAM32()][ 53] = 4.9994	 ;
    mf[QAM32()][ 54] = 4.9999	 ;
    mf[QAM32()][ 55] = 5	 ;
    mf[QAM32()][ 56] = 5	 ;
    mf[QAM32()][ 57] = 5	 ;
    mf[QAM32()][ 58] = 5	 ;
    mf[QAM32()][ 59] = 5	 ;
    mf[QAM32()][ 60] = 5     ;

}

void SNR2MIMap::initQAM64()
{
	mf[QAM64()].resize(61);
    mf[QAM64()][  0] = 0	 ;
    mf[QAM64()][  1] = 0	 ;
    mf[QAM64()][  2] = 0	 ;
    mf[QAM64()][  3] = 0	 ;
    mf[QAM64()][  4] = 0	 ;
    mf[QAM64()][  5] = 0	 ;
    mf[QAM64()][  6] = 0	 ;
    mf[QAM64()][  7] = 0	 ;
    mf[QAM64()][  8] = 0	 ;
    mf[QAM64()][  9] = 0	 ;
    mf[QAM64()][ 10] = 0.014355 ;
    mf[QAM64()][ 11] = 0.018049 ;
    mf[QAM64()][ 12] = 0.022686 ;
    mf[QAM64()][ 13] = 0.028502 ;
    mf[QAM64()][ 14] = 0.035791 ;
    mf[QAM64()][ 15] = 0.044915 ;
    mf[QAM64()][ 16] = 0.05632	 ;
    mf[QAM64()][ 17] = 0.070552 ;
    mf[QAM64()][ 18] = 0.08827	 ;
    mf[QAM64()][ 19] = 0.11027	 ;
    mf[QAM64()][ 20] = 0.1375	 ;
    mf[QAM64()][ 21] = 0.17105	 ;
    mf[QAM64()][ 22] = 0.21221	 ;
    mf[QAM64()][ 23] = 0.26239	 ;
    mf[QAM64()][ 24] = 0.32314	 ;
    mf[QAM64()][ 25] = 0.39607	 ;
    mf[QAM64()][ 26] = 0.48278	 ;
    mf[QAM64()][ 27] = 0.58474	 ;
    mf[QAM64()][ 28] = 0.70312	 ;
    mf[QAM64()][ 29] = 0.83872	 ;
    mf[QAM64()][ 30] = 0.99177	 ;
    mf[QAM64()][ 31] = 1.162	 ;
    mf[QAM64()][ 32] = 1.3484	 ;
    mf[QAM64()][ 33] = 1.55	 ;
    mf[QAM64()][ 34] = 1.7652	 ;
    mf[QAM64()][ 35] = 1.9926	 ;
    mf[QAM64()][ 36] = 2.2308	 ;
    mf[QAM64()][ 37] = 2.4786	 ;
    mf[QAM64()][ 38] = 2.7349	 ;
    mf[QAM64()][ 39] = 2.9985	 ;
    mf[QAM64()][ 40] = 3.2686	 ;
    mf[QAM64()][ 41] = 3.5442	 ;
    mf[QAM64()][ 42] = 3.8246	 ;
    mf[QAM64()][ 43] = 4.1087	 ;
    mf[QAM64()][ 44] = 4.3953	 ;
    mf[QAM64()][ 45] = 4.6814	 ;
    mf[QAM64()][ 46] = 4.9613	 ;
    mf[QAM64()][ 47] = 5.225	 ;
    mf[QAM64()][ 48] = 5.4601	 ;
    mf[QAM64()][ 49] = 5.6547	 ;
    mf[QAM64()][ 50] = 5.8015	 ;
    mf[QAM64()][ 51] = 5.9	 ;
    mf[QAM64()][ 52] = 5.9573	 ;
    mf[QAM64()][ 53] = 5.9852	 ;
    mf[QAM64()][ 54] = 5.996	 ;
    mf[QAM64()][ 55] = 5.9992	 ;
    mf[QAM64()][ 56] = 5.9999	 ;
    mf[QAM64()][ 57] = 6	 ;
    mf[QAM64()][ 58] = 6	 ;
    mf[QAM64()][ 59] = 6	 ;
    mf[QAM64()][ 60] = 6        ;

}



