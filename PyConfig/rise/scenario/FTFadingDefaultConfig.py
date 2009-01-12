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

# default parameterization for FTFading classes. Some useful initial values.
from FTFading import *

class FTFadingOff_defaultConfig(FTFadingOff):
    def __init__(self):
           super(FTFadingOff_defaultConfig, self).__init__()

class FTFadingFflat_defaultConfig(FTFadingFflat):
    def __init__(self,numSubCarriers):
           super(FTFadingFflat_defaultConfig, self).__init__(samplingTime="0.0005",dopFreq = "10",numWaves= "100", numSubCarriers = numSubCarriers )

class FTFadingFuncorrelated_defaultConfig(FTFadingFuncorrelated):
    def __init__(self,numSubCarriers):
           super(FTFadingFuncorrelated_defaultConfig, self).__init__(samplingTime="0.0005",dopFreq = "10",numWaves= "100", numSubCarriers = numSubCarriers)


class FTFadingFneighbourCorrelation_defaultConfig(FTFadingFneighbourCorrelation):
    def __init__(self,numSubCarriers):
        super(FTFadingFneighbourCorrelation_defaultConfig, self).__init__(samplingTime="0.0005",neighbourCorrelationFactor = "0.8",dopFreq = "10",numWaves= "100",numSubCarriers = numSubCarriers)

## These are the some of the parameters which can be applied in the FTFading strategies ##
#numSubCarriers = 100 -> we are working with 100 subcarriers
#dopfreq = 167 Hz -> which is the doppler frequency for a speed of 20 m/s
#sampFreq = 2000 Hz -> accoring to the TTI of the ScaleNet simulations (0.0005 seconds)

