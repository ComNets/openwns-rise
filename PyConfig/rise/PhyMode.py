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
from openwns.interval import Interval
import rise.SNR2MI
import rise.CoderSpecification

# Info on MI: http://en.wikipedia.org/wiki/Mutual_information

class PhyMode(object):
    nameInPhyModeFactory = "rise.PhyMode.PhyMode"
    modulation = None # string
    coding     = None # string
    # the following values are needed for dataRate calculations:
    symbolDuration = None
    subCarriersPerSubChannel = None
    # see: LLMapping.py for SINR-2-PhyMode mapping
    snr2miMapping = None # mapping class object
    mi2perMapper  = None # mapping class object

    # can be called like PhyMode(modulation = "QPSK",  coding = "Turbo_UMTS-1/3")
    # can be called like PhyMode("QPSK-Turbo_UMTS-1/3")
    def __init__(self, modulation, coding=None, **kw):
        if (coding==None): # support format PhyMode("QPSK-Turbo_UMTS-1/3")
            parts=modulation.split("-")
            modulation=parts[0]
            coding="-".join(parts[1:])
            #print "PhyMode(",modulation,",",coding,")"
        self.modulation = modulation
        self.coding = coding
        self.snr2miMapping = rise.SNR2MI.default # using table, not formula
        #self.coderMapping = rise.CoderSpecification.defaultCoderMapping # using table, not formula
        self.mi2perMapper = rise.CoderSpecification.defaultCoderMapping # using table, not formula
        attrsetter(self, kw)

    def setSymbolDuration(self, symbolDuration):
        self.symbolDuration = symbolDuration
    def setSubCarriersPerSubChannel(self, subCarriersPerSubChannel):
        self.subCarriersPerSubChannel = subCarriersPerSubChannel
    def getString(self):
        return "-".join((self.modulation,self.coding))

# not usable because of PhyMode constructor using PyConfig and strict string2int
class NoPhyMode(PhyMode):
    def __init__(self):
        self.modulation = ""
        self.coding = ""

# dimension/size of smallest Phy symbol
class PhyModeSupport(object):
    symbolDuration = None
    subCarriersPerSubChannel = None
    def __init__(self, **kw):
        attrsetter(self, kw)

#class PhyModeMapper(HasModeName):
class PhyModeMapper(object):
    nameInPhyModeMapperFactory = "rise.plmapping.PhyModeMapper"

    mapEntries = None
    symbolDuration = None
    subCarriersPerSubChannel = None
    minimumSINR = None # below this SINR there will be no useful transmission (PER too high)
    # ^ in practice this can depend on the codeWordLength
    snr2miMapper = None # mapper objects
    mi2perMapper = None # mapper objects
    dynamicTable = False # True: calculate PhyModes=f(bl,PER)

    class MapEntry:
        sinrInterval = None
        phyMode = None

    def __init__(self, symbolDuration, subCarriersPerSubChannel, **kw):
        self.mapEntries = []
        self.symbolDuration = symbolDuration
        self.subCarriersPerSubChannel = subCarriersPerSubChannel
        # the next two mappers can be overwritten by extra arguments of the constructor:
        self.snr2miMapper = rise.SNR2MI.default # The default is currently specified there
        self.mi2perMapper = rise.CoderSpecification.defaultCoderMapping # The default is currently specified there
        attrsetter(self, kw)

    def setMinimumSINR(self, sinr):
        self.minimumSINR = sinr

    def addPhyMode(self, sinrInterval, phyMode):
        # items must be added in order of ascending sinr ranges
        tmp = self.MapEntry()
        tmp.sinrInterval = sinrInterval
        tmp.phyMode = phyMode
        tmp.phyMode.symbolDuration = self.symbolDuration
        tmp.phyMode.subCarriersPerSubChannel = self.subCarriersPerSubChannel
        # ensure that the PhyMode class uses the same mappers:
        #tmp.phyMode.snr2miMapping = self.snr2miMapper
        #tmp.phyMode.coderMapping = self.mi2perMapper
        self.mapEntries.append(tmp)
