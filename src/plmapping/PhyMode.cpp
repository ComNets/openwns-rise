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

#include <WNS/service/phy/phymode/PhyModeInterface.hpp>
#include <WNS/service/phy/phymode/SNR2MIInterface.hpp>
#include <WNS/service/phy/phymode/MI2PERInterface.hpp>
#include <RISE/plmapping/PhyMode.hpp>
#include <WNS/StaticFactory.hpp>

using namespace rise::plmapping;

STATIC_FACTORY_REGISTER_WITH_CREATOR(PhyMode,
                                     wns::service::phy::phymode::PhyModeInterface,
                                     "rise.PhyMode.PhyMode",
                                     wns::PyConfigViewCreator);

STATIC_FACTORY_REGISTER(PhyMode,
                        wns::service::phy::phymode::PhyModeInterface,
                        "rise.PhyMode.Empty");

PhyMode::PhyMode() :
    snr2miMapper(NULL),
    coderMapper(NULL),
    subCarriersPerSubChannelKnown(false),
    subCarriersPerSubChannel(1),
    symbolDurationKnown(false),
    symbolDuration(0.0),
    dataRateKnown(false),
    dataRate(0.0)
{
    coding = UNDEFINED_CODING;
    modulation = UNDEFINED_MODULATION;
    //std::cout << "PhyMode::PhyMode(UNDEFINED)"<< std::endl;
}

/** @brief Construct PHY Mode from PyConfig	*/
PhyMode::PhyMode(const wns::pyconfig::View& config) :
    snr2miMapper(wns::service::phy::phymode::SNR2MIInterface::getSNR2MImapper(config.get("snr2miMapping"))), // unique object pointer
    coderMapper(wns::service::phy::phymode::CoderFullMappingInterface::getCoderFullMapping(config.get("mi2perMapper"))), // unique object pointer
    modulation(rise::plmapping::Modulations::fromString(config.get<std::string>("modulation"))),
    coding(coderMapper->fromString(config.get<std::string>("coding"))),
    subCarriersPerSubChannelKnown(false),
    subCarriersPerSubChannel(1),
    symbolDurationKnown(false),
    symbolDuration(0.0),
    dataRateKnown(false),
    dataRate(0.0)
{
    assure(modulation > UNDEFINED_MODULATION, "UNDEFINED_MODULATION");
    assure(modulation <= MAX_MODULATIONS, "MAX_MODULATIONS exceeded");
    assure(coderMapper != NULL, "invalid coderMapper");
    assure(coding <= coderMapper->getMaxCodeIndex(), "MaxCodeIndex exceeded");
    if (config.knows("symbolDuration") && !config.isNone("symbolDuration")) {
        symbolDuration = config.get<simTimeType>("symbolDuration");
        symbolDurationKnown = true;
    }
    if (config.knows("subCarriersPerSubChannel") && !config.isNone("subCarriersPerSubChannel")) {
        subCarriersPerSubChannel = config.get<unsigned int>("subCarriersPerSubChannel");
        subCarriersPerSubChannelKnown = true;
    }
    calculateDataRate();
    //std::cout << "PhyMode::PhyMode("<<getstd::string()<<") = " << dataRate << std::endl;
}


/** @brief Construct PHY Mode from two ints. Also needs the coderMapper pointer (Used for conversion). */
PhyMode::PhyMode(Modulation _modulation, rise::plmapping::Coding _coding, wns::service::phy::phymode::SNR2MIInterface* _snr2miMapper, wns::service::phy::phymode::CoderFullMappingInterface* _coderMapper) :
    snr2miMapper(_snr2miMapper),
    coderMapper(_coderMapper),
    subCarriersPerSubChannelKnown(false),
    subCarriersPerSubChannel(1),
    symbolDurationKnown(false),
    symbolDuration(0.0),
    dataRateKnown(false),
    dataRate(0.0)
{
    assure(_modulation <=MAX_MODULATIONS, "MAX_MODULATIONS exceeded");
    assure(snr2miMapper != NULL, "invalid sinr2miMapper");
    assure(coderMapper != NULL, "invalid coderMapper");
    assure(_coding <= coderMapper->getMaxCodeIndex(), "MaxCodeIndex exceeded: "<<coding<<" vs "<<coderMapper->getMaxCodeIndex());
    coding = _coding; modulation = _modulation;
    //std::cout << "PhyMode::PhyMode("<<modulation<<","<<coding<<":"<<getstd::string()<<") = " << dataRate << std::endl;
}

PhyMode::~PhyMode() {}

void
PhyMode::setSubCarriersPerSubChannel(unsigned int _subCarriersPerSubChannel)
{
    subCarriersPerSubChannel = _subCarriersPerSubChannel;
    subCarriersPerSubChannelKnown = true;
    //std::cout << "PhyMode::setSubCarriersPerSubChannel("<<getstd::string()<<","<<subCarriersPerSubChannel<<")"<< std::endl;
    calculateDataRate();
}

void
PhyMode::setSymbolDuration(simTimeType _symbolDuration)
{
    symbolDuration = _symbolDuration;
    symbolDurationKnown = true;
    //std::cout << "PhyMode::setSymbolDuration("<<getstd::string()<<","<<symbolDuration<<")"<< std::endl;
    calculateDataRate();
}

void
PhyMode::cloneParameters(const PhyMode& other)
{
    assure(other.dataRateIsValid(),"cloneParameters() requires fully parameterized PhyMode");
    subCarriersPerSubChannel = other.subCarriersPerSubChannel;
    subCarriersPerSubChannelKnown = true;
    symbolDuration = other.symbolDuration;
    symbolDurationKnown = true;
    calculateDataRate();
}

bool
PhyMode::isValid() const
{
    return (modulation>UNDEFINED_MODULATION)
        && (modulation<=MAX_MODULATIONS)
        && (coding>UNDEFINED_CODING)
        && (coderMapper!=NULL);
}

/** @brief this PhyMode can offer a data Rate */
bool
PhyMode::dataRateIsValid() const
{
    return isValid() && dataRateKnown;
}

rise::plmapping::Modulation
PhyMode::getModulation() const {
    //if (this->isValid() == false) throw (UndefinedException());
    return modulation;
}

rise::plmapping::Coding
PhyMode::getCoding() const {
    //if (this->isValid() == false) throw (UndefinedException());
    return coding;
}

/** @brief access method for the code rate (double) */
double
PhyMode::getCodeRate() const
{
    assure(coderMapper != NULL, "invalid coderMapper");
    if (this->isValid() == false) throw (UndefinedException());
    return coderMapper->getRate(coding);
}

/** @brief bit rate per OFDM symbol */
double
PhyMode::getBitsPerSymbol() const
{
    assure(coderMapper != NULL, "invalid coderMapper");
    return coderMapper->getRate(coding) * modulation;
}

/** @brief number of bits that fit into certain number of OFDM symbols */
/*
unsigned int
PhyMode::getBitCapacity(double numberOfOFDMSymbols) const
{
    assure(coderMapper != NULL, "invalid coderMapper");
    return (unsigned int) (coderMapper->getRate(coding) * modulation * numberOfOFDMSymbols + 0.5);
};
*/

double
PhyMode::getDataRate(double _symbolRate, unsigned int _subCarriersPerSubChannel) const
{
    return getBitsPerSymbol() * _symbolRate * _subCarriersPerSubChannel;
}

double
PhyMode::getDataRate() const
{
    assure(dataRateKnown,"getDataRate(): cannot get dataRate for "<<getString());
    return dataRate;
}

double PhyMode::getSINR2MI(const wns::Ratio& sinr) const
{
    assure(snr2miMapper != NULL, "invalid snr2miMapper");
    return snr2miMapper->convertSNR2MI(sinr,*this);
}
double PhyMode::getSINR2MIB(const wns::Ratio& sinr) const
{
    assure(snr2miMapper != NULL, "invalid snr2miMapper");
    return snr2miMapper->convertSNR2MIB(sinr,*this);
}
double PhyMode::getMI2PER(const double mib, unsigned int bits) const
{
    assure(coderMapper != NULL, "invalid coderMapper");
    return coderMapper->mapMI2PER(mib,bits,coding);
}
double PhyMode::getSINR2PER(const wns::Ratio& sinr, unsigned int bits) const
{
    assure(coderMapper != NULL, "invalid coderMapper");
    return coderMapper->mapMI2PER(snr2miMapper->convertSNR2MIB(sinr,*this),bits,coding);
}

wns::Ratio
PhyMode::getMIB2SINR(const double& mib) const
{
    assure(coderMapper != NULL, "invalid coderMapper");
    return snr2miMapper->convertMIB2SNR(mib, getModulation());
}

unsigned int
PhyMode::getBitCapacityFractional(simTimeType duration) const
{
    // calculate the capacity of this burst [bits]
    //static double epsilon = 1e-6; // to combat precision errors
    assure(dataRateKnown,"unknown dataRate");
    double capacity = dataRate * duration;
    return int(capacity+0.5);
}

// attention: symbolDuration here means fullSymbolDuration = 1/symbolRateOFDM = OFDMsymbolDuration+cyclicPrefixDuration
// duration is also counted INCLUDING cyclic prefic
unsigned int
PhyMode::getBitCapacity(simTimeType duration) const
{
    // calculate the capacity of this burst [bits]
    static double epsilon = 1e-6;
    assure(subCarriersPerSubChannelKnown,"unknown subCarriersPerSubChannel");
    assure(symbolDurationKnown,"unknown symbolDuration");
    int OFDMsymbols = (int)(duration / symbolDuration + epsilon);
    // the following "if" is a quickhack to avoid assert failure in winprost::PhyUser
    // because duration = (phyCommand->local.stop - phyCommand->local.start) is too short
    if ((OFDMsymbols == 0) // very_short_duration, bad idea of caller!
        && (duration > 0.0)) // there is at least some small time
        OFDMsymbols = 1; // return at least the capacity of one OFDM symbol length
    double capacity = OFDMsymbols
        * subCarriersPerSubChannel
        * getBitsPerSymbol();
    return int(capacity+epsilon); // to combat precision errors
}

std::string
PhyMode::getModulationString() const
{
    if (modulation != UNDEFINED_MODULATION) {
        return rise::plmapping::Modulations::toString(modulation);
    } else {
        return std::string("UNDEFINED_MODULATION");
    }
}

std::string
PhyMode::getCodingString() const
{
    if (!this->isValid())
        return std::string("UNDEFINED_CODING");

    assure(coderMapper != NULL, "invalid coderMapper");
    return coderMapper->getString(coding);
}

std::string
PhyMode::getString() const
{
    return getModulationString()+"-"+getCodingString();
}

bool
PhyMode::nameMatches(const std::string& name) const
{
    return (getString().compare(name) == 0);
}

unsigned int
PhyMode::toInt() const
{
    return ((unsigned int)coding << MOD_BITS) + modulation;
}

void
PhyMode::calculateDataRate()
{
    if (subCarriersPerSubChannelKnown && symbolDurationKnown) {
        // attention: symbolDuration here means fullSymbolDuration = 1/symbolRateOFDM = OFDMsymbolDuration+cyclicPrefixDuration
        dataRate = getBitsPerSymbol() * subCarriersPerSubChannel / symbolDuration;
        dataRateKnown = true;
    }
}

