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



class FTFading(object):
    ftfadingName = None
    samplingTime = None # Time interval between changes of state  (Event time)
    numSubCarriers = None # Number of subchannels that will be taken into account
    logger = None
    def __init__(self,samplingTime,parentLogger = None):
        self.samplingTime = samplingTime
        self.logger = openwns.logger.Logger("RISE", "PHY.FTFading", True, parentLogger)

class FTFadingJakes(FTFading):
    """ intermediate class for fading objects based on Jakes' Model """
    dopFreq  =   None #  [Hz] Max Doppler frequency variation.
    numWaves =   None # Number of waves which generate every reflection

    def __init__(self,samplingTime,dopFreq,numWaves,numSubCarriers,parentLogger = None):
        super(FTFadingJakes, self).__init__(samplingTime,parentLogger)
        self.dopFreq = dopFreq
        self.samplingTime = samplingTime
        self.numWaves = numWaves
        self.numSubCarriers = numSubCarriers


class FTFadingOff(FTFading):
    """ No strategy is applied (absolutely flat channel) """

    ftfadingName = "rise.scenario.ftfading.FTFadingOff"

    def __init__(self,parentLogger = None):
        self.samplingTime = 0.0 # off
        self.numSubCarriers = 0
        super(FTFadingOff, self).__init__(self.samplingTime,parentLogger)

class FTFadingFflat(FTFadingJakes):
    """ Time Correlated and flat Frequency """

    ftfadingName = "rise.scenario.ftfading.FTFadingFflat"

    def __init__(self,samplingTime, dopFreq, numWaves, numSubCarriers, parentLogger = None):
        super(FTFadingFflat, self).__init__(samplingTime,dopFreq,numWaves,numSubCarriers,parentLogger)


class FTFadingFuncorrelated(FTFadingJakes):
    """ Time and Frequency Correlated uncorrelated """

    ftfadingName = "rise.scenario.ftfading.FTFadingFuncorrelated"

    def __init__(self, samplingTime, dopFreq, numWaves,numSubCarriers, parentLogger = None):
        super(FTFadingFuncorrelated, self).__init__(samplingTime,dopFreq,numWaves,numSubCarriers,parentLogger)


class FTFadingFneighbourCorrelation(FTFadingJakes):
    """ Time and Frequency Correlated by neighbour correlation method """

    ftfadingName  = "rise.scenario.ftfading.FTFadingFneighbourCorrelation"
    neighbourCorrelationFactor = None

    def __init__(self,samplingTime,neighbourCorrelationFactor,dopFreq, numWaves, numSubCarriers, parentLogger = None):
        super( FTFadingFneighbourCorrelation, self).__init__(samplingTime,dopFreq,numWaves,numSubCarriers,parentLogger)
        self.neighbourCorrelationFactor = neighbourCorrelationFactor
