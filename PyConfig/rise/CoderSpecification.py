###############################################################################
# This file is part of openWNS (open Wireless Network Simulator)
# _____________________________________________________________________________
#
# Copyright (C) 2004-2007
# Chair of Communication Networks (ComNets)
# Kopernikusstr. 5, D-52074 Aachen, Germany
# phone: ++49-241-80-27910,
# fax: ++49-241-80-22242
# email: info@openwns.org
# www: http://www.openwns.org
# _____________________________________________________________________________
#
# openWNS is free software; you can redistribute it and/or modify it under the
# terms of the GNU Lesser General Public License version 2 as published by the
# Free Software Foundation;
#
# openWNS is distributed in the hope that it will be useful, but WITHOUT ANY
# WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR
# A PARTICULAR PURPOSE.  See the GNU Lesser General Public License for more
# details.
#
# You should have received a copy of the GNU Lesser General Public License
# along with this program.  If not, see <http://www.gnu.org/licenses/>.
#
###############################################################################

from openwns.pyconfig import attrsetter
import openwns.logger

# Class for keeping information about ONE specific coder
class CoderSpecification(object):
    nameInCoderSpecFactory = None
    parentLogger = None
    logger    = None
    coderName = None # name(string)
    rate      = None # fraction(double)
    index     = None # int
    def __init__(self, coderName, rate, index, **kw):
        self.coderName = coderName
        self.rate   = rate
        self.index  = index
        attrsetter(self, kw)
        self.logger = openwns.logger.Logger("CoderSpecification", True, self.parentLogger)
        #self.logger = wns.Logger.Logger("CoderSpecification", True, None)

class TableCoder(CoderSpecification):
    cwlTable     = None
    sizeRegistry = None

    def __init__(self, coderName, rate, index, cwlTable, sizeRegistry, **kw):
        super(TableCoder,self).__init__(coderName, rate, index)
        self.nameInCoderSpecFactory = "rise.CoderSpecification.Table"
        self.cwlTable = cwlTable
        self.sizeRegistry = sizeRegistry
        attrsetter(self, kw)

class FormulaCoder(CoderSpecification):
    def __init__(self, coderName, rate, index):
        super(FormulaCoder,self).__init__(coderName, rate, index, **kw)
        self.nameInCoderSpecFactory = "rise.CoderSpecification.Formula"
        attrsetter(self, kw)

# Class for keeping information about all coders
class CoderMapping(object):
    nameInCoderFullMappingFactory = 'rise.plmapping.CoderFullMapping'
    coderMap = None

    def __init__(self, mapping = "Table"):
        #print "CoderMapping() called (huge datastructure, 1x only)"
        self.coderMap = [] # list of CoderSpecification's
        ii=1
        for codeSpec in [(  "UNDEFINED_CODING", 1.0     ),
                         (  "No",               1.0     ),
                         (  "Turbo_UMTS-1/3",   1.0/3.0 ),
                         (  "Turbo_UMTS-1/2",   1.0/2.0 ),
                         (  "Turbo_UMTS-2/3",   2.0/3.0 ),
                         (  "Turbo_UMTS-5/6",   5.0/6.0 ),
                         (  "RCPC_BENQ-1/3",    1.0/3.0 ),
                         (  "RCPC_BENQ-1/2",    1.0/2.0 ),
                         (  "RCPC_BENQ-2/3",    2.0/3.0 ),
                         (  "LDPC_SEUK-1/2",    1.0/2.0 ),
                         (  "LDPC_SEUK-2/3",    2.0/3.0 ),
                         (  "LDPC_SEUK-3/4",    3.0/4.0 ),
                         (  "Turbo_EAB-1/3",    1.0/3.0 ),
                         (  "Turbo_EAB-1/2",    1.0/2.0 ),
                         (  "Turbo_EAB-2/3",    2.0/3.0 ),
                         (  "WIMAX-1/2",        1.0/2.0 ),
                         (  "WIMAX-2/3",        2.0/3.0 ),
                         (  "WIMAX-3/4",        3.0/4.0 ),
                         (  "WiMAX-320",	0.117 ),
                         (  "WiMAX-720",	0.189 ),
                         (  "WiMAX-1020",	0.267 ),
                         (  "WiMAX-1320",	0.379 ),
                         (  "WiMAX-1520",	0.479 ),
                         (  "WiMAX-1720",	0.615 ),
                         (  "WiMAX-1920",	0.391 ),
                         (  "WiMAX-2120",	0.479 ),
                         (  "WiMAX-2320",	0.615 ),
                         (  "WiMAX-2420",	0.458 ),
                         (  "WiMAX-2520",	0.521 ),
                         (  "WiMAX-2620",	0.615 ),
                         (  "WiMAX-2720",	0.688 ),
                         (  "WiMAX-2820",	0.781 ),
                         (  "WiMAX-2920",	0.819 ),
                         (  "WiMAX-3020",	0.917 ),
                         (  "lte_m_2_tbs_1384",  0.105000),
                         (  "lte_m_2_tbs_2216",  0.168000),
                         (  "lte_m_2_tbs_3624",  0.275000),
                         (  "lte_m_2_tbs_5160",  0.391000),
                         (  "lte_m_2_tbs_6968",  0.528000),
                         (  "lte_m_4_tbs_11448", 0.434000),
                         (  "lte_m_4_tbs_15264", 0.578000),
                         (  "lte_m_6_tbs_16416", 0.415000),
                         (  "lte_m_4_tbs_8760",  0.332000),
                         (  "lte_m_6_tbs_21384", 0.540000),
                         (  "lte_m_6_tbs_25456", 0.643000),
                         (  "lte_m_6_tbs_28336", 0.716000),
                         (  "lte_m_6_tbs_31704", 0.801000),
]:

            coderName = codeSpec[0];
            rate = codeSpec[1]
            # choose one of the methods for MI2PER mapping
            # TODO: choose formula if it exists, table else [rs].
            if mapping == "Table":
                
                # Workaround for open source publication (mue)
                try:
                    import rise.AllMI2PERTables
                    # full table for all CWLs
                    sizeregistrytmp = rise.AllMI2PERTables.allMI2PERTables.MI2PER[coderName] 
                    # vector of all CWLs
                    CWLTableTmp = rise.AllMI2PERTables.allMI2PERTables.MI2PER[coderName].keys() 
                except ImportError:
                    import rise.allmi2pertables
                    # full table for all CWLs
                    sizeregistrytmp = rise.allmi2pertables.allMI2PERTables.MI2PER[coderName] 
                    # vector of all CWLs
                    CWLTableTmp = rise.allmi2pertables.allMI2PERTables.MI2PER[coderName].keys()

                CWLTableTmp.sort()
                self.coderMap.append(TableCoder( coderName = coderName,
                                                 rate = rate,
                                                 index = ii,
                                                 cwlTable = CWLTableTmp,
                                                 sizeRegistry = sizeregistrytmp ))
            elif mapping == "Formula":
                self.coderMap.append(FormulaCoder( coderName = coderName,
                                                   rate = rate,
                                                   index = ii))
            else:
                raise "Unknown MI2PER Mapping '%s' specified" % mapping
            ii = ii + 1
        # loop end

    def getName(self,INDEX):
        return self.coderMap[INDEX].coderName
    def getRate(self,INDEX):
        return self.coderMap[INDEX].rate
    def getIndex(self,name):
        for codeSpec in self.coderMap:
            if (codeSpec.coderName == name):
                return codeSpec.index

default = CoderMapping(mapping = "Table")
defaultCoderMapping = default # better name
