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

import openwns.logger
import string

class No:
    __plugin__ = 'None'

class Rice:
    def __init__(self, variance = 3.0):
        self.variance = variance
        self.__plugin__ = 'Rice'

class PerLinkAndSubchannel:
    def __init__(self, numberOfSubchannels, systemName = "ofdma"):
        self.numberOfSubchannels = numberOfSubchannels
        self.systemName = systemName
        self.baseLogger = openwns.logger.Logger("RISE", "FastFading.PerLinkAndSC", True)

# Manages different Jakes FTFading generators. Only creates instances 
# when links are really active
class Jakes(PerLinkAndSubchannel):
    def __init__(self, ftFadingJakes):
        PerLinkAndSubchannel.__init__(self, ftFadingJakes.numSubCarriers)
        self.logger = openwns.logger.Logger("RISE", "Jakes", True, self.baseLogger)
        self.ftFadingJakes = ftFadingJakes
        loggerName = ftFadingJakes.ftfadingName
        loggerName = string.split(loggerName, ".")
        self.ftFadingJakes.logger = openwns.logger.Logger(
            "RISE", loggerName[-1], True, self.logger)
        self.__plugin__ = 'Jakes'

class FTFadingJakes(object):
    """ intermediate class for fading objects based on Jakes' Model """
    dopFreq  =   None #  [Hz] Max Doppler frequency variation.
    numWaves =   None # Number of waves which generate every reflection
    ftfadingName = None
    samplingTime = None # Time interval between changes of state  (Event time)
    numSubCarriers = None # Number of subchannels that will be taken into account
    logger = None

    def __init__(self,samplingTime,dopFreq,numWaves,numSubCarriers):
        self.dopFreq = dopFreq
        self.samplingTime = samplingTime
        self.numWaves = numWaves
        self.numSubCarriers = numSubCarriers
        self.samplingTime = samplingTime
	self.logger = openwns.logger.Logger("RISE", "FTFadingJakes", True)


class FTFadingFflat(FTFadingJakes):
    """ Time Correlated and flat Frequency """

    ftfadingName = "rise.scenario.ftfading.FTFadingFflat"

    def __init__(self,samplingTime, dopFreq, numWaves, numSubCarriers):
        super(FTFadingFflat, self).__init__(samplingTime,dopFreq,numWaves,numSubCarriers)

class FTFadingFuncorrelated(FTFadingJakes):
    """ Time and Frequency Correlated uncorrelated """

    ftfadingName = "rise.scenario.ftfading.FTFadingFuncorrelated"

    def __init__(self, samplingTime, dopFreq, numWaves,numSubCarriers):
        super(FTFadingFuncorrelated, self).__init__(samplingTime,dopFreq,numWaves,numSubCarriers)

class FTFadingFneighbourCorrelation(FTFadingJakes):
    """ Time and Frequency Correlated by neighbour correlation method """

    ftfadingName  = "rise.scenario.ftfading.FTFadingFNCorr"
    neighbourCorrelationFactor = None

    def __init__(self,samplingTime,neighbourCorrelationFactor,dopFreq, numWaves, numSubCarriers):
        super( FTFadingFneighbourCorrelation, self).__init__(samplingTime,dopFreq,numWaves,numSubCarriers)
        self.neighbourCorrelationFactor = neighbourCorrelationFactor

## These are the some of the parameters which can be applied in the FTFading strategies ##
#numSubCarriers = 100 -> we are working with 100 subcarriers
#dopfreq = 167 Hz -> which is the doppler frequency for a speed of 20 m/s
#sampFreq = 2000 Hz -> accoring to the TTI of the ScaleNet simulations (0.0005 seconds)

class FTFadingFflat_defaultConfig(FTFadingFflat):
    def __init__(self,numSubCarriers):
        super(FTFadingFflat_defaultConfig, self).__init__(samplingTime="0.0005", dopFreq = "10",
            numWaves= "100", numSubCarriers = numSubCarriers)

class FTFadingFuncorrelated_defaultConfig(FTFadingFuncorrelated):
    def __init__(self,numSubCarriers, parentLogger = None):
           super(FTFadingFuncorrelated_defaultConfig, self).__init__(samplingTime="0.0005",dopFreq = "10",
            numWaves= "100", numSubCarriers = numSubCarriers)


class FTFadingFneighbourCorrelation_defaultConfig(FTFadingFneighbourCorrelation):
    def __init__(self,numSubCarriers):
        super(FTFadingFneighbourCorrelation_defaultConfig, self).__init__(samplingTime="0.0005",
            neighbourCorrelationFactor = "0.8",dopFreq = "10",numWaves= "100",numSubCarriers = numSubCarriers)



