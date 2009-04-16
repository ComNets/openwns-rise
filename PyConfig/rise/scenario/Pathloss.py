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

from openwns.interval import Interval
from openwns import dB, fromdB

import math

class Pathloss(object):
    minPathloss = None

    def __init__(self, minPathloss = "0.0 dB"):
        self.minPathloss = minPathloss

class Constant(Pathloss):
    __plugin__ = 'Constant'
    value = None
    def __init__(self, value):
        super(Constant, self).__init__()
        self.value = value

class Deny(Pathloss):
    __plugin__ = 'Deny'
    message = None
    def __init__(self, message = "Error: Calculation of pathloss was configured to be denied. Additionally an informative deny message was not provided."):
        super(Deny, self).__init__()
        self.message = message

class FreeSpace(Pathloss):
    __plugin__ = 'FreeSpace'
    def __init__(self, minPathloss = "42.0 dB"):
        super(FreeSpace, self).__init__(minPathloss)

class Map(Pathloss):
    __plugin__ = 'Map'
    sceneryMapName = None
    interpolationType = None

    def __init__(self, sceneryMapName, 
         interpolationType = 'NearestNeighbour',
                 minPathloss = "0.0 dB"):
        super(Map, self).__init__(minPathloss)
        self.sceneryMapName = sceneryMapName
        self.interpolationType = interpolationType

class UserDef(Pathloss):
    validFrequencies = None
    validDistances = None
    distNormFactor = None
    outOfMinRange = None
    outOfMaxRange = None

    """Base configuration for a pathloss model that can be tweaked in many ways"""
    def __init__(self,
                 validFrequencies,
                 validDistances,
                 distanceUnit,
                 minPathloss,
                 outOfMinRange,
                 outOfMaxRange):
        """validFrequncies = wns.Interval(a,b)
           distanceUnit = 'km' | 'm'
        """
        super(UserDef, self).__init__(minPathloss)
        self.validFrequencies = validFrequencies
        self.validDistances = validDistances
        if distanceUnit == "m":
            self.distNormFactor = 1
        elif distanceUnit == "km":
            self.distNormFactor = 1.0/1000.0
        else:
            raise Exception("Invalid distance unit (not 'm' or 'km') in UserDef pathloss configuration")
        self.outOfMinRange = outOfMinRange
        self.outOfMaxRange = outOfMaxRange


class SingleSlope(UserDef):
    __plugin__ = 'SingleSlope'
    offset = None
    freqFactor = None
    distFactor = None

    """A pathloss model that can be tweaked in many ways"""
    def __init__(self, validFrequencies,
                 validDistances,
                 offset,
                 freqFactor,
                 distFactor,
                 distanceUnit,
                 minPathloss = "0.0 dB",
         outOfMinRange = Deny,
         outOfMaxRange = Deny):
        """validFrequncies = wns.Interval(a,b)
           distanceUnit = 'km' | 'm'
        """
        super(SingleSlope, self).__init__(validFrequencies,
                                          validDistances,
                                          distanceUnit,
                                          minPathloss,
                                          outOfMinRange,
                                          outOfMaxRange)

        self.offset = offset
        self.freqFactor = freqFactor
        self.distFactor = distFactor

class PyFunction(SingleSlope):
    __plugin__ = 'PyFunction'
    offset = None
    scenarioWrap = None
    sizeX = None
    sizeY = None

    def __init__(self,validFrequencies,
                validDistances,
                offset,
                freqFactor,
                distFactor,
                distanceUnit,
                minPathloss = "0.0 dB",
                outOfMinRange = Deny,
                outOfMaxRange = Deny,
                scenarioWrap = False,
                sizeX = 0,
                sizeY = 0):
        super(PyFunction, self).__init__(validFrequencies,
                    validDistances,
                    offset,
                    freqFactor,
                    distFactor,
                    distanceUnit,
                    minPathloss,
                    outOfMinRange,
                    outOfMaxRange)
        self.offset = offset
        self.scenarioWrap = scenarioWrap
        self.sizeX = sizeX
        self.sizeY = sizeY

    def calculateDistance(self, sourceX, sourceY, targetX, targetY):
        if(self.scenarioWrap):
        #print "calculateDistance: dx1: ", math.fabs(sourceX - targetX),
        #print " dx2: ", self.sizeX - math.fabs(sourceX - targetX),
        #print " dy1: ", math.fabs(sourceY - targetY),
        #print " dy2: ", self.sizeY - math.fabs(sourceY - targetY)
            d_x = min(math.fabs(sourceX - targetX), self.sizeX - math.fabs(sourceX - targetX))
            d_y = min(math.fabs(sourceY - targetY), self.sizeY - math.fabs(sourceY - targetY))
        else:
            d_x = sourceX-targetX
            d_y = sourceY-targetY
        return(math.sqrt(d_x**2 + d_y**2))

    def calculatePathloss(self, sourceX, sourceY, targetX, targetY, frequency, baseHeight):
        distance = self.calculateDistance(sourceX, sourceY, targetX, targetY)
        if(distance == 0):
            distance = 0.1
        pl = fromdB(self.offset) + self.freqFactor * math.log(frequency,10) + self.distFactor * math.log(distance,10)
        return(dB(pl))

    def getFreqFactor(self):
        return(self.freqFactor)
    
    def getDistFactor(self):
        return(self.distFactor)

class MultiSlope(UserDef):

    class Range:
        offset = None
        freqFactor = None
        distFactor = None
        distRange = None

        __plugin__ = 'MultiSlope'
        validFrequencies = None
        validDistances = None
        outOfMinRange = None
        outOfMaxRange = None

        ranges = None

    def __init__(self, validFrequencies,
                 validDistances,
                 ranges,
                 distanceUnit,
                 minPathloss = "0.0 dB",
        outOfMinRange = Deny,
        outOfMaxRange = Deny):
        super(MultiSlope, self).__init__(validFrequencies,
                                         validDistances,
                                         distanceUnit,
                                         minPathloss,
                                         outOfMinRange,
                                         outOfMaxRange)
        self.ranges = []
        self.ranges += ranges

    def addRange(self,
                 offset,
                 freqFactor,
                 distFactor,
                 minDist,
                 maxDist):
        tmp = MultiSlope.Range()
        tmp.offset = offset
        tmp.freqFactor = freqFactor
        tmp.distFactor = distFactor
        tmp.distRange = Interval(minDist, maxDist, "(]")
        self.ranges.append(tmp)

    def addSlope(self,
                 slope,
                 minDist,
                 maxDist):
        tmp = MultiSlope.Range()
        tmp.offset = slope.offset
        tmp.freqFactor = slope.freqFactor
        tmp.distFactor = slope.distFactor
        tmp.distRange = Interval(minDist, maxDist, "(]")
        self.ranges.append(tmp)

class MultiModel(Pathloss):

    class Range:
        offset = None
        modelConfig = None

        __plugin__ = 'MultiModel'

        ranges = None

    def __init__(self, ranges):
        self.ranges = []
        self.ranges += ranges

    def addRange(self,
                 minDist,
                 maxDist,
                 modelConfig):
        tmp = MultiModel.Range()
        tmp.distRange = Interval(minDist, maxDist, "(]")
        tmp.modelConfig = modelConfig
        self.ranges.append(tmp)
        self.minPathloss = self.ranges[0].modelConfig.minPathloss # WHATIF the first element in the list is not the "first" range?


class Umts3003Outdoor(SingleSlope):

    def __init__(self, minPathloss = "0.0 dB"):
        super(Umts3003Outdoor, self).__init__(validFrequencies = Interval(150, 2100),
                                              validDistances = Interval(0, 20000),
                                              offset = "49 dB",
                                              freqFactor = 30,
                                              distFactor = 40,
                                              distanceUnit = "km",
                                              minPathloss = minPathloss,
                                              outOfMinRange = Deny(),
                                              outOfMaxRange = Deny())

class Umts3003Vehicular(Pathloss):
    __plugin__ = 'Umts3003Vehicular'

    def __init__(self, minPathloss = "0.0 dB"):
        super(Umts3003Vehicular, self).__init__(minPathloss)

# The following classes are for unit tests

class ConstantTestPos(Constant):
    def __init__(self):
        super(ConstantTestPos, self).__init__("23.11 dB")

class ConstantTestNeg(Constant):
    def __init__(self):
        super(ConstantTestNeg, self).__init__("-23.11 dB")

class DenyTest(Deny):
    pass

class DistanceDependentTest(Pathloss):
    def __init__(self):
        super(DistanceDependentTest, self).__init__("1.0 dB")

class DistanceIndependentTest(Pathloss):
    def __init__(self):
        super(DistanceIndependentTest, self).__init__("0.0 dB")

class FreeSpaceTest(FreeSpace):
    def __init__(self):
        super(FreeSpaceTest, self).__init__("42.00 dB")

class RangeCheckedTest(Pathloss):
    constant = None
    def __init__(self):
        super(RangeCheckedTest, self).__init__("0.0 dB")
        self.constant = Constant("2.0 dB")

class Umts3003VehicularTest(Umts3003Vehicular):
    def __init__(self):
        super(Umts3003VehicularTest, self).__init__("42.0 dB")

class SingleSlopeTestFreeSpace(SingleSlope):
    def __init__(self):
        validFrequencies = Interval(1, 9999)
        validDistances = Interval(0, 99999)
        super(SingleSlopeTestFreeSpace, self).__init__(validFrequencies = Interval(1, 9999),
                                                   validDistances = Interval(0, 99999),
                                                   offset = "32.44 dB",
                                                   freqFactor = 20,
                                                   distFactor = 20,
                                                   distanceUnit = "km",
                                                   minPathloss = "42 dB",
                                                   outOfMinRange = Deny(),
                                                   outOfMaxRange = Deny())

from math import log10

class WINNERC1LOSTest(MultiSlope):
    """ WINNER Pathloss Model C1 LOS: sub-urban marco cell LOS, all Values according to D1.1.1 v1.2"""
    def __init__(self):
        c0 = 299792458 # Speed of light in [m/s]
        hBS = 25.0 # m
        hUT = 1.5 # m
        f = 3950 # MHz
        dBP = 4 * hBS*hUT*f*1e6/c0   # breakpoint between slopes
        minDistance = 30 # m
        maxDistance = 5000 # m

        super(WINNERC1LOSTest, self).__init__(validFrequencies = Interval(2000, 6000),
                                             validDistances = Interval(minDistance, maxDistance), # [m]
                                             ranges = [], # ranges defined below
                                             distanceUnit = "m", # nur fuer die Formel, nicht fuer validDistances
                                             minPathloss = "64.0 dB", # pathloss at 8m distance
                                             outOfMinRange = FreeSpace("44.35 dB"), # Pathloss
                                             outOfMaxRange = Deny())

        self.addRange(offset = "%.5f dB" % (41.2 + 20*log10(f*1e-3/5.0)),
                      freqFactor = "0 dB",
                      distFactor = "23.8 dB",
                      minDist = minDistance,
                      maxDist = dBP)

        self.addRange(offset = "%.5f dB" % (11.65-16.2*log10(hBS)-16.2*log10(hUT)+ 3.8*log10(f*1e-3/5.0) ),
                      freqFactor = 0,
                      distFactor = "40.0 dB",
                      minDist = dBP,
                      maxDist = maxDistance)

