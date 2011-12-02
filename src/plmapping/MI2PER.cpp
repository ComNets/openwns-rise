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

#include <RISE/plmapping/MI2PER.hpp>
#include <RISE/plmapping/CodeRates.hpp>

#include <cmath>
#include <sstream>
#include <vector>
#include <WNS/StaticFactoryBroker.hpp>
#include <WNS/PyConfigViewCreator.hpp>
#include <WNS/Assure.hpp>
#include <WNS/Ttos.hpp>

using namespace rise;
using namespace rise::plmapping;

STATIC_FACTORY_BROKER_REGISTER(CoderFullMapping,
							   wns::service::phy::phymode::CoderFullMappingInterface,
							   "rise.plmapping.CoderFullMapping");

STATIC_FACTORY_REGISTER_WITH_CREATOR( TableCoder, CoderFullSpecification, "rise.CoderSpecification.Table", wns::PyConfigViewCreator);
STATIC_FACTORY_REGISTER_WITH_CREATOR( FormulaCoder, CoderFullSpecification, "rise.CoderSpecification.Formula", wns::PyConfigViewCreator);

STATIC_FACTORY_REGISTER_WITH_CREATOR( GENERICM2P, wns::service::phy::phymode::MI2PERMappingInterface, "rise.plmapping.GENERIC", wns::PyConfigViewCreator);
STATIC_FACTORY_REGISTER_WITH_CREATOR( WINNERM2P,  wns::service::phy::phymode::MI2PERMappingInterface, "rise.plmapping.WINNER",  wns::PyConfigViewCreator);
STATIC_FACTORY_REGISTER_WITH_CREATOR( FixedM2P,   wns::service::phy::phymode::MI2PERMappingInterface, "rise.plmapping.Fixed",   wns::PyConfigViewCreator);
STATIC_FACTORY_REGISTER_WITH_CREATOR( FormulaM2P, wns::service::phy::phymode::MI2PERMappingInterface, "rise.plmapping.Formula", wns::PyConfigViewCreator);


TableCoder::TableCoder(const wns::pyconfig::View& config)
	: CoderFullSpecification(config),
	  numberOfCWLs(0),
	  logger(std::string("RISE"), std::string("MI2PERmapper"))
	  //logger(config.get("logger")) // TODO
{
	MESSAGE_SINGLE(VERBOSE, logger, "TableCoder::Constructor:");
	// ugly, but we have to set some base class members here
	assure(config.knows("coderName"),"PyConfig: missing: coderName");
	name   = config.get<std::string>("coderName"); // inherited member name
	coding = config.get<Coding>("index"); // inherited
	rate   = config.get<double>("rate"); // inherited
	codeWordLengths.clear(); // empty vector
	vectorOfTablesByCWLSize.clear(); // empty vector

	assure(config.knows("cwlTable"),"PyConfig: missing: cwlTable");
	numberOfCWLs = config.len("cwlTable"); // member
	if (numberOfCWLs==0) {
		MESSAGE_SINGLE(NORMAL, logger, "Coder["<<coding<<"]="<<name<<": empty table.");
	} else {
		MESSAGE_BEGIN(NORMAL, logger, m, "Coder["<<coding<<"]="<<name<<": cwlTable[0.."<<numberOfCWLs-1<<"]=(");
		    //m << "brutto CWL[bits]=(";
			for (unsigned int cwlIndex=0; cwlIndex<numberOfCWLs; ++cwlIndex){ m << config.get<int>("cwlTable",cwlIndex) << ",";	}
			m << ")";
		MESSAGE_END();

		for (unsigned int cwlIndex=0; cwlIndex<numberOfCWLs; ++cwlIndex) {
			int CWL = config.get<int>("cwlTable",cwlIndex);
			//MESSAGE_SINGLE(NORMAL, logger, "Working on CWL:  "<<CWL);
			codeWordLengths.push_back(CWL); //codeWordLengths[cwlIndex] = CWL;
			assure(codeWordLengths[cwlIndex] == CWL, "CWL problem");
		}
		assure(codeWordLengths.size() == numberOfCWLs, "invalid codeWordLengths");
		//CodeWordLengths codeWordLengths; // codeWordLengths[0..5]={300,600,...}
		std::sort(codeWordLengths.begin(),codeWordLengths.end());
		//MESSAGE_BEGIN(NORMAL, logger, m, " - codeWordLengths=(");
		//    for (unsigned int cwlIndex=0; cwlIndex<numberOfCWLs; ++cwlIndex){ m << codeWordLengths[cwlIndex] << ",";	}
		//    m << ")";
		//MESSAGE_END();

		//SizeRegistry sizeRegistry; // member
		for (unsigned int cwlIndex=0; cwlIndex<numberOfCWLs; ++cwlIndex){
			int CWL = codeWordLengths[cwlIndex];
			//MESSAGE_SINGLE(VERBOSE, logger, " - Reading table for CWL=" << CWL );
			assure(config.knows("sizeRegistry["+wns::Ttos(CWL)+"]"),"PyConfig: missing: "<<"sizeRegistry["<<CWL<<"]");
			unsigned int tableSize = config.len("sizeRegistry["+wns::Ttos(CWL)+"]");
			MESSAGE_SINGLE(VERBOSE, logger, " - Reading table for CWL="<<CWL<<": "<<tableSize<<" elements.");
			LookupTable table(tableSize); // temporary object
			for (unsigned int miIndex=0; miIndex<tableSize; ++miIndex) { // forall mi values
				double perValue = config.get<double>("sizeRegistry["+wns::Ttos(CWL)+"]["+wns::Ttos(miIndex)+"][1]");
				// the original tables often have ripple near per=1.0. Bad thing.
				// mathematical correction to make function monotonously decreasing:
				if (perValue>1.0) perValue=1.0;
				int past=1;
				while(((int)miIndex-past>=0) && (table[miIndex-past] < perValue)) {
					table[miIndex-past]=perValue; past++;
				}
				table[miIndex] = perValue;
			} // forall mi values
			assure(table.size() == tableSize, "wrong size of table");
			//sizeRegistry.insert(CWL, table); // table is copied
			// better:
			vectorOfTablesByCWLSize.push_back(table); // table is copied
		} // forall CWLs
		//assure(sizeRegistry.size() == numberOfCWLs, "wrong size of sizeRegistry");
		assure(vectorOfTablesByCWLSize.size() == numberOfCWLs, "wrong size of vectorOfTablesByCWLSize");
	} // if not empty
} // CoderFullSpecification::configure()

unsigned int
TableCoder::suitableCWL(unsigned int blockLength /* gross */) const
{
	unsigned int distance = 0;
	unsigned int minDistance = INT_MAX;
	unsigned int result = 0;
	//MESSAGE_SINGLE(NORMAL, logger, "CoderFullSpecification::suitableCWL("<<blockLength<<")");
	assure(numberOfCWLs == codeWordLengths.size(), "invalid codeWordLengths");
	for (unsigned int cwlIndex=0; cwlIndex<numberOfCWLs; ++cwlIndex){
		unsigned int cwl = codeWordLengths.at(cwlIndex); // =codeWordLengths[cwlIndex]
		distance = cwl > blockLength ? cwl - blockLength : blockLength - cwl;
		if (distance<minDistance) {
			minDistance = distance;
			result = cwl;
		} else if (distance==minDistance) {
			// Use the smaller of two CWLs in case of doubt
			result = std::min( result, cwl );
		}
	}
	MESSAGE_SINGLE(NORMAL, logger, "CoderFullSpecification::suitableCWL("<<blockLength<<") = "<<result);
	return result;
}

unsigned int
TableCoder::suitableCWLposition(unsigned int blockLength /* gross */) const
{
	unsigned int distance = 0;
	unsigned int minDistance = INT_MAX;
	unsigned int resultCWL = 0;
	unsigned int resultCWLposition = 0;
	//MESSAGE_SINGLE(NORMAL, logger, "CoderFullSpecification::suitableCWL("<<blockLength<<")");
	assure(numberOfCWLs == codeWordLengths.size(), "invalid codeWordLengths");
	for (unsigned int cwlIndex=0; cwlIndex<numberOfCWLs; ++cwlIndex){
		unsigned int cwl = codeWordLengths.at(cwlIndex); // =codeWordLengths[cwlIndex]
		distance = cwl > blockLength ? cwl - blockLength : blockLength - cwl;
		//MESSAGE_SINGLE(NORMAL, logger, "CoderFullSpecification::suitableCWL("<<blockLength<<"): compare["<<cwlIndex<<"] with cwl="<<cwl);
		if (distance<minDistance) {
			minDistance = distance;
			resultCWL = cwl; resultCWLposition = cwlIndex;
		} else if (distance==minDistance) {
			// Use the smaller of two CWLs in case of doubt
			resultCWL = std::min( resultCWL, cwl );
		}
	}
	MESSAGE_SINGLE(NORMAL, logger, "CoderFullSpecification::suitableCWL("<<blockLength<<") = "<<resultCWL);
	return resultCWLposition;
}

const CodeWordLengths&
TableCoder::getCWLvector() const
{
	return codeWordLengths;
}

double
TableCoder::mapMI2PER(double mi,
					  /* blockLength=payload[net], not CWL[gross] */
					  unsigned int blockLength) const
{
	MESSAGE_SINGLE(NORMAL,logger,"CoderFullSpecification::mapMI2PER(mi="<<mi<<",bl="<<blockLength<<")");
	if (mi > (1.0 - 1e-6)) { return 0.0; } // mi=1.0 => no PER

	//unsigned int bruttobits = (unsigned int) ((1.0*blockLength) / getRate());
	unsigned int bruttobits = (unsigned int) ((1.0*blockLength) / rate); // member

	// We only have tables for discrete CWLs, so
	// method 1 is, to choose the next matching CWL in table:
	// try to find the best Matching Codeword Length
	//unsigned int CWL = suitableCWL(bruttobits);
	unsigned int CWLposition = suitableCWLposition(bruttobits);
	// method 2 would be a kind of interpolation (but never average PER values!!!)

	MESSAGE_BEGIN(NORMAL, logger, m, "Mapping for block: netto=");
	m << blockLength << " Bits, brutto="
	  << bruttobits << " Bits, using table for CWL=" << codeWordLengths.at(CWLposition) << " Bits.";
	MESSAGE_END();

	// obtain lookup table for the best matching code word length
	//const LookupTable &table = sizeRegistry.find(CWL); // copy???
	assure(vectorOfTablesByCWLSize.size()>CWLposition,"invalid size");
	const LookupTable& table = vectorOfTablesByCWLSize[CWLposition];
	// ^ reference (faster, without copy)

	// round mi for lookup
	//int MI = int(1000.0*mi + 0.5); // implicit policy for the table
	//return table[MI];
	// NEW: interpolation:
	int MI1 = int(1000.0*mi);
	int MI2 = int(1000.0*mi)+1;
    if(MI2 > table.size() - 1)
        return 0.0;

	double PER1= table[MI1];
	double PER2= table[MI2];
	double PER = PER1 + (PER2-PER1)*(mi*1000.0-(double)MI1); // linear interpolation
	return PER;
} // CoderFullSpecification::mapMI2PER()

double
TableCoder::PER2MIB(double per, unsigned int blockLength /* smaller number (payload) */) const
{
	if (per <= 0.0) { return 1.0; }
	if (per >= 1.0) { return 0.0; }
	unsigned int bruttobits = (unsigned int) ((1.0*blockLength) / rate); // member
	unsigned int CWLposition = suitableCWLposition(bruttobits);
	assure(vectorOfTablesByCWLSize.size()>CWLposition,"CWLposition out of bounds");
	const LookupTable& table = vectorOfTablesByCWLSize[CWLposition];
	const unsigned int tableSize=table.size();
	assure(tableSize>0,"empty table");
	unsigned int leftBracket=0;
	unsigned int rightBracket=tableSize-1;
	unsigned int currentPosition=tableSize*3/4; // good guess for mi tables
	double mi=1.0; // best case
	double leftPER  = table[leftBracket];
	double rightPER = table[rightBracket];
	double slope    = rightPER - leftPER; // always negative for MI2PER
	double riseOrFall = (slope>=0) ? 1.0 : -1.0;
	// successive approximation:
	while ( (rightBracket-leftBracket)>1 ) {
		double PER = table[currentPosition];
		//std::cout << "PER2MIB(): ["<<leftBracket<<","<<currentPosition<<","<<rightBracket<<"]: per=["<<leftPER<<","<<PER<<","<<rightPER<<"]" <<std::endl;
		if ( riseOrFall*(per-PER)>0 ) { // go right
			leftBracket = currentPosition;
			leftPER  = table[leftBracket];
		} else { // go left
			rightBracket = currentPosition;
			rightPER = table[rightBracket];
		}
		currentPosition = (rightBracket + leftBracket)/2; // middle
	}
	// linear interpolation:
	double  leftMI =  leftBracket/1000.0;
	double rightMI = rightBracket/1000.0;
	if (rightBracket==1000) { rightPER=0.0; } // few tables have >0 at the end which leads to errors
	double deltaPER = (per-leftPER)/(rightPER-leftPER);
	//MESSAGE_SINGLE(NORMAL, logger, "PER2MIB(per="<<per<<", blockLength="<<blockLength<<"): "<<leftMI<<"<=MI<="<<rightMI<<", leftPER="<<leftPER<<", rightPER="<<rightPER<<", deltaPER="<<deltaPER);
	assure(deltaPER>=0.0,"PER2MIB(per="<<per<<", blockLength="<<blockLength<<"): "<<leftMI<<"<=MI<="<<rightMI<<", leftPER="<<leftPER<<", rightPER="<<rightPER<<", deltaPER="<<deltaPER);
	assure(deltaPER<=1.0,"PER2MIB(per="<<per<<", blockLength="<<blockLength<<"): "<<leftMI<<"<=MI<="<<rightMI<<", leftPER="<<leftPER<<", rightPER="<<rightPER<<", deltaPER="<<deltaPER);
	mi = leftMI + (rightMI-leftMI)*deltaPER;
	return mi;
}


double
FormulaCoder::mapMI2PER(double /*mi*/,
						unsigned int /*blockLength*/) const
{
	assure(false, "Currently No working implementation here");
	/*
	// by formula:
	ser = pow(10.0, serlog);
	spp = double(compound->getLengthInBits())
		/double(PHYTools::getBitsPerSymbol(phyMode));
    // FIXME: Wrongness because of truncation of the Symbols per Packet
	per=1-pow((1-ser),spp);
	*/
	return 0.0;
}

double
FormulaCoder::PER2MIB(double /*per*/, unsigned int /*blockLength*/) const
{
	assure(false, "Currently No working implementation here");
	return 0.0;
}

/**************************************************************/

/** @brief Helper Function to create CoderFullSpecification from StaticFactory */
CoderFullSpecification*
getCoderSpec(const wns::pyconfig::View& config)
{
	std::string coderSpecType = config.get<std::string>("nameInCoderSpecFactory");
	return CoderSpecFactory::creator(coderSpecType)->create(config);
}

CoderFullMapping::CoderFullMapping(const wns::pyconfig::View& config)
	: CoderMapping(config),
	  logger(std::string("RISE"), std::string("CoderFullMapping"))
{
	const std::string coderFullMapName("coderMap");
	//MESSAGE_SINGLE(NORMAL, logger, "CoderFullMapping::configure()");
	coderFullSpecVector.clear(); // empty vector
	//logger = config.get("logger");

	// read in all the tables for the different codeRates and codeWordSizes
	//assure(config.knows(""),"PyConfig: missing: ");
	//assure(config.knows("coderFullMapping"),"PyConfig: missing: coderFullMapping");
	// at this point, the config points to the contents of CoderFullMapping()
	assure(config.knows(coderFullMapName),"PyConfig: missing:"<<coderFullMapName);
	unsigned int numberOfCoders = config.len(coderFullMapName);
	MESSAGE_SINGLE(NORMAL, logger, "CoderFullMapping::configure(): Storing Mapping for "<<numberOfCoders<<" CodingSpecifications ");

	for (Coding coding=0u; coding<numberOfCoders; ++coding) {
	//for (int ii=0; ii<numberOfCoders; ++ii){
		//Coding coding = (Coding) ii; // both int
		//MESSAGE_SINGLE(NORMAL, logger, " - Index of CoderSpecification = "<<coding);
		assure(config.knows(coderFullMapName+"["+ wns::Ttos(coding) +"]"),"PyConfig: missing:"<<coderFullMapName<<"["<<wns::Ttos(coding)<<"]");
		wns::pyconfig::View coderSpecConfig = config.getView(coderFullMapName+"["+wns::Ttos(coding)+"]");
		// temporary object, copied into the container:
		CoderFullSpecification* coderFullSpecification = getCoderSpec(coderSpecConfig);
		//codeNameRegistry.insert(coding, coderFullSpecification); // copy object
		// better, new way:
		coderFullSpecVector.push_back(coderFullSpecification); // copy object
		// ^ same as coderFullSpecVector[coding] = coderFullSpecification;
		// inherited structure:
		//coderSpecificationVector.push_back(CoderSpecification(coderNum,rate,coderName));
	} // forall coders
	maxCodeIndex = numberOfCoders-1; // inherited
	MESSAGE_SINGLE(NORMAL, logger, "CoderFullMapping::configure(): Stored Mapping for "<<numberOfCoders<<" CodingSpecifications ");
}

CoderFullMapping::~CoderFullMapping()
{
	while (!coderFullSpecVector.empty())
	{
		delete *(coderFullSpecVector.begin());
		coderFullSpecVector.erase(coderFullSpecVector.begin());
	}
}


bool
CoderFullMapping::isConfigured() const
{
	return !coderFullSpecVector.empty(); // true if not empty
}

const CoderFullSpecification&
CoderFullMapping::getCoderFullSpecification(Coding c) const
{
	assure(!coderFullSpecVector.empty(), "empty vector");
	assure(coderFullSpecVector.size()>c, "bad vector size");
	return *(coderFullSpecVector[c]);
}


double
CoderFullMapping::mapMI2PER(double mi,
							unsigned int blockLength, /* blockLength=payload, not CWL */
							Coding coding) const
{
	if (mi > (1.0 - 1e-6)) { return 0.0; } // mi=1.0 => no PER
	assure(!coderFullSpecVector.empty(), "empty vector");
	assure(isConfigured(), "object is not yet configured");
	assure(coderFullSpecVector.size()>coding, "bad vector size");
	//CoderFullSpecification coderFullSpecification = codeNameRegistry.find(coding);
	const CoderFullSpecification &coderFullSpecification = *(coderFullSpecVector[coding]);
	return coderFullSpecification.mapMI2PER(mi, blockLength);
}

double
CoderFullMapping::PER2MIB(double per, unsigned int blockLength, Coding coding) const
{
	assure(!coderFullSpecVector.empty(), "empty vector");
	assure(isConfigured(), "object is not yet configured");
	assure(coderFullSpecVector.size()>coding, "bad vector size");
	const CoderFullSpecification &coderFullSpecification = *(coderFullSpecVector[coding]);
	return coderFullSpecification.PER2MIB(per, blockLength);
}


/**************************************************************/

// constructor
GENERICM2P::GENERICM2P(const wns::pyconfig::View& config)
	: coderFullMapping(wns::service::phy::phymode::CoderFullMappingInterface::getCoderFullMapping(config.get("mi2per")))
{
	assure(coderFullMapping,"undefined coderFullMapping");
}

double
GENERICM2P::mapMI2PER(double mi,
					  unsigned int blockLength, /* blockLength=payload, not CWL */
					  Coding coding) const
{
	assure(coderFullMapping,"undefined coderFullMapping");
	return coderFullMapping->mapMI2PER(mi, blockLength, coding);
}


