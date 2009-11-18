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

#include <RISE/plmapping/CodeRates.hpp>

using namespace rise::plmapping;


CoderMapping::CoderMapping(const wns::pyconfig::View& config) :
    maxCodeIndex(0),
    coderSpecificationVector(),
    codename2int()
{
    unsigned int numberOfCoders = 0u;
    const std::string coderMapName("coderMap");
    //std::cout << "CoderMapping::configure() called" << std::endl;
    coderSpecificationVector.clear(); // empty vector
    assure(config.knows(coderMapName),"missing "<<coderMapName);
    if (!config.isNone(coderMapName))
    {
        numberOfCoders = config.len(coderMapName);
        //std::cout << "  numberOfCoders="<<numberOfCoders << std::endl;
        for (Coding coderNum=0u; coderNum<numberOfCoders; ++coderNum) {
            wns::pyconfig::View coderConfig = config.getView(coderMapName,coderNum);
            double rate = coderConfig.get<double>("rate");
            std::string coderName = coderConfig.get<std::string>("coderName");
            //std::cout << "  coder["<<coderNum<<"]="<<coderName<<", "<<rate << std::endl;
            codename2int.insert(coderName, coderNum);
            coderSpecificationVector.push_back(CoderSpecification(coderNum,rate,coderName));
        }
        assure(codename2int.size() == numberOfCoders, "size not ok");
        assure(coderSpecificationVector.size() == numberOfCoders, "size not ok");
    }
    maxCodeIndex = numberOfCoders-1;
    //std::cout << "CoderMapping::CoderMapping()"<< std::endl;
}


bool
CoderMapping::isConfigured() const
{
    return !coderSpecificationVector.empty(); // true if not empty
}

double
CoderMapping::getRate(Coding coding) const
{
    assure(!coderSpecificationVector.empty(), "empty vector");
    assure(coderSpecificationVector.size()>coding, "bad vector size");
    return coderSpecificationVector[coding].getRate();
}

double
CoderMapping::getRate(const std::string& s) const
{
    assure(!codename2int.empty(), "empty vector");
    Coding coderIndex = codename2int.find(s); // throws exception if not found
    assure(coderIndex<= maxCodeIndex, "invalid coder index " << coderIndex);
    assure(!coderSpecificationVector.empty(), "empty vector");
    assure(coderSpecificationVector.size()>coderIndex, "bad vector size");
    return coderSpecificationVector[coderIndex].getRate();
}

Coding
CoderMapping::fromString(const std::string& s) const
{
    assure(!codename2int.empty(), "empty vector");
    return codename2int.find(s); // throws exception if not found
}

const std::string&
CoderMapping::getString(Coding c) const
{
    assure(!coderSpecificationVector.empty(), "empty vector");
    assure(coderSpecificationVector.size()>c, "bad vector size");
    //std::cout << "CoderMapping::getstd::string("<<c<<", MAX="<< coderSpecificationVector.size() <<")"<< std::endl;
    assure(c<=maxCodeIndex, "invalid coder index " << c);
    return coderSpecificationVector[c].getName();
}

const CoderSpecification&
CoderMapping::getCoderSpecification(Coding c) const
{
    assure(!coderSpecificationVector.empty(), "empty vector");
    assure(coderSpecificationVector.size()>c, "bad vector size");
    return coderSpecificationVector[c];
}
