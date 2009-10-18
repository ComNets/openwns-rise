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

class LOSUrbanMicro(MultiSlope):
    #""" IMT-Advanced Pathloss Model, urban micro cell LOS, all Values according to Rep. ITU-R M.2135"""
        c0 = 299792458 # Speed of light in [m/s]
        hBS = None # m
        hUT = None # m
        f = None # MHz
        dBP = None  # breakpoint between slopes
	dBPe = None   # breakpoint distance with effective antenna hights
        minDistance = None # m
        maxDistance = None # m
        def __init__(self, minDistance, maxDistance, hBS=10, hUT=1.5, f=2500):
        	self.hBS = hBS
        	self.hUT = hUT
        	self.f = f
        	self.dBP = 2*math.pi*(self.hBS)*(self.hUT)*f*1e6/self.c0   # breakpoint between slopes
        	self.dBPe = 4*(self.hBS-1)*(self.hUT-1)*f*1e6/self.c0   # breakpoint distance with effective antenna hights
        	self.minDistance = minDistance
        	self.maxDistance = maxDistance
        	super(LOSUrbanMicro, self).__init__(validFrequencies = Interval(800, 6000),
                                             validDistances = Interval(minDistance, maxDistance), # [m]
                                             ranges = [], # ranges defined below
                                             distanceUnit = "m", # nur fuer die Formel, nicht fuer validDistances
					     minPathloss = "%.5f dB" % (22.0*log10(self.minDistance)+ 28.0+ 20.0*log10(self.f*1e-3)),
                                             #minPathloss = "57.95 dB", # pathloss at 10m distance
                                             outOfMinRange = FreeSpace("44.35 dB"), # Pathloss
                                             outOfMaxRange = Deny())

        	self.addRange(offset = "%.5f dB" % (28.0+ 20.0*log10(1e-3)),
                      freqFactor = "20 dB",
                      distFactor = "22.0 dB",
                      minDist = self.minDistance,
                      maxDist = self.dBPe)

        	self.addRange(offset = "%.5f dB" % (7.8-18.0*log10(self.hBS-1)-18.0*log10(self.hUT-1)+ 2.0*log10(1e-3) ),
                      freqFactor = "2 db",
                      distFactor = "40.0 dB",
                      minDist = self.dBPe,
                      maxDist = self.maxDistance)

class LOSUrbanMacro(MultiSlope):
    #""" IMT-Advanced Pathloss Model, urban macro cell LOS, all Values according to Rep. ITU-R M.2135"""
        c0 = 299792458 # Speed of light in [m/s]
        hBS = None # m
        hUT = None # m
        f = None # MHz
        dBP = None  # breakpoint between slopes
        dBPe = None   # breakpoint distance with effective antenna hights
        minDistance = None # m
        maxDistance = None # m
	def __init__(self, minDistance, maxDistance, hBS=25, hUT=1.5, f=2000):
        	self.hBS = hBS
        	self.hUT = hUT
        	self.f = f
        	self.dBP = 2*math.pi*(self.hBS)*(self.hUT)*f*1e6/self.c0   # breakpoint between slopes
        	self.dBPe = 4*(self.hBS-1)*(self.hUT-1)*f*1e6/self.c0   # breakpoint distance with effective antenna hights
        	self.minDistance = minDistance
        	self.maxDistance = maxDistance
	        super(LOSUrbanMacro, self).__init__(validFrequencies = Interval(800, 6000),
                                             validDistances = Interval(minDistance, maxDistance), # [m]
                                             ranges = [], # ranges defined below
                                             distanceUnit = "m", # nur fuer die Formel, nicht fuer validDistances
					     					 minPathloss = "%.5f dB" % (22.0*log10(self.minDistance)+ 28.0+ 20.0*log10(self.f*1e-3)),
                                             #minPathloss = "56.02 dB", # pathloss at 8m distance
                                             outOfMinRange = FreeSpace("44.35 dB"), # Pathloss
                                             outOfMaxRange = Deny())
        	self.addRange(offset = "%.5f dB" % (28.0+ 20.0*log10(1e-3)),
                      freqFactor = "20 dB",
                      distFactor = "22.0 dB",
                      minDist = self.minDistance,
                      maxDist = self.dBPe)

        	self.addRange(offset = "%.5f dB" % (7.8-18.0*log10(self.hBS-1)-18.0*log10(self.hUT-1)+ 2.0*log10(1e-3) ),
                      freqFactor = "2 db",
                      distFactor = "40.0 dB",
                      minDist = self.dBPe,
                      maxDist = self.maxDistance)

class LOSSubUrbanMacro(MultiSlope):
    #""" IMT-Advanced Pathloss Model, suburban macro cell LOS, all Values according to Rep. ITU-R M.2135"""
        c0 = 299792458 # Speed of light in [m/s]
        hBS = None # m
        hUT = None # m
        f = None # MHz
        h = None #m, avg. building height
        W = None #m, street width
        dBP = None  # breakpoint between slopes
	dBPe = None   # breakpoint distance with effective antenna hights
        minDistance = None # m
        maxDistance = None # m
	def __init__(self, minDistance, maxDistance, hBS=35, hUT=1.5, f=2000):
        	self.hBS = hBS
        	self.hUT = hUT
        	self.f = f
        	self.h = 10.0
        	self.W = 20.0
        	self.dBP = 2*math.pi*(self.hBS)*(self.hUT)*f*1e6/self.c0   # breakpoint between slopes
        	self.dBPe = 4*(self.hBS-1)*(self.hUT-1)*f*1e6/self.c0   # breakpoint distance with effective antenna hights
        	self.minDistance = minDistance
        	self.maxDistance = maxDistance
	
        	super(LOSSubUrbanMacro, self).__init__(validFrequencies = Interval(800, 6000),
                                             validDistances = Interval(minDistance, maxDistance), # [m]
                                             ranges = [], # ranges defined below
                                             distanceUnit = "m", # nur fuer die Formel, nicht fuer validDistances
					                         minPathloss = "%.5f dB" % (20.0*log10(40*math.pi*self.minDistance*self.f/3)+ min(0.03*math.pow(self.h,1.72),10)*log10(self.minDistance)- min(0.044*math.pow(self.h,1.72),14.77)+ 0.002*log10(self.h*self.minDistance)+ 1.0+ 20.0*log10(1e-3)),
                                             #minPathloss = "58.73 dB", # pathloss at 8m distance
                                             outOfMinRange = FreeSpace("44.35 dB"), # Pathloss
                                             outOfMaxRange = Deny())

        	self.addRange(offset = "%.5f dB" % (20.0*log10(40*math.pi/3)- min(0.044*math.pow(self.h,1.72),14.77)+ 0.002*log10(self.h)+ 1.0+ 20.0*log10(1e-3)),
                      freqFactor = "20 dB",
                      distFactor = "%.5f dB" % (20.0+ min(0.03*math.pow(self.h,1.72),10)+ 0.002),
                      minDist = self.minDistance,
                      maxDist = self.dBP)

        	self.addRange(offset = "%.5f dB" % (20.0*log10(40*math.pi/3)- min(0.044*math.pow(self.h,1.72),14.77)+ 0.002*log10(self.h)+ 1.0-40.0*log10(self.dBP)+ 20.0*log10(1e-3)),
                      freqFactor = "20 db",
                      distFactor = "%.5f dB" % (20.0+ min(0.03*math.pow(self.h,1.72),10)+ 0.002+ 40),
                      minDist = self.dBP,
                      maxDist = self.maxDistance)

class LOSRuralMacro(MultiSlope):
    #""" IMT-Advanced Pathloss Model, rural macro cell LOS, all Values according to Rep. ITU-R M.2135"""
        c0 = 299792458 # Speed of light in [m/s]
        hBS = None # m
        hUT = None # m
        f = None # MHz
        h = None #m, avg. building height
        W = None #m, street width
        dBP = None  # breakpoint between slopes
        dBPe = None   # breakpoint distance with effective antenna hights
        minDistance = None # m
        maxDistance = None # m
	def __init__(self, minDistance, maxDistance, hBS=35, hUT=1.5, f=800):
        	self.hBS = hBS
        	self.hUT = hUT
        	self.f = f
        	self.h = 5.0
        	self.W = 20.0
        	self.dBP = 2*math.pi*(self.hBS)*(self.hUT)*f*1e6/self.c0   # breakpoint between slopes
        	self.dBPe = 4*(self.hBS-1)*(self.hUT-1)*f*1e6/self.c0   # breakpoint distance with effective antenna hights
        	self.minDistance = minDistance
        	self.maxDistance = maxDistance
	
	
        	super(LOSRuralMacro, self).__init__(validFrequencies = Interval(450, 6000),
                                             validDistances = Interval(minDistance, maxDistance), # [m]
                                             ranges = [], # ranges defined below
                                             distanceUnit = "m", # nur fuer die Formel, nicht fuer validDistances
					     minPathloss = "%.5f dB" % (20.0*log10(40*math.pi*self.minDistance*self.f/3)+ min(0.03*math.pow(self.h,1.72),10)*log10(self.minDistance)- min(0.044*math.pow(self.h,1.72),14.77)+ 0.002*log10(self.h*self.minDistance)+ 1.0+ 20.0*log10(1e-3)),
                                             #minPathloss = "51.28 dB", # pathloss at 8m distance
                                             outOfMinRange = FreeSpace("44.35 dB"), # Pathloss
                                             outOfMaxRange = Deny())

        	self.addRange(offset = "%.5f dB" % (20.0*log10(40*math.pi/3)- min(0.044*math.pow(self.h,1.72),14.77)+ 0.002*log10(self.h)+ 1.0+ 20.0*log10(1e-3)),
                      freqFactor = "20 dB",
                      distFactor = "%.5f dB" % (20.0+ min(0.03*math.pow(self.h,1.72),10)+ 0.002),
                      minDist = self.minDistance,
                      maxDist = self.dBP)

       	 	self.addRange(offset = "%.5f dB" % (20.0*log10(40*math.pi/3)- min(0.044*math.pow(self.h,1.72),14.77)+ 0.002*log10(self.h)+ 1.0-40.0*log10(self.dBP)+ 20.0*log10(1e-3)),
                      freqFactor = "20 db",
                      distFactor = "%.5f dB" % (20.0+ min(0.03*math.pow(self.h,1.72),10)+ 0.002+ 40),
                      minDist = self.dBP,
                      maxDist = self.maxDistance)

class NLOSUrbanMicroHexagonal(SingleSlope):
   # """ IMT-Advanced Pathloss Model, urban micro cell NLOS, all Values according to Rep. ITU-R M.2135"""
        c0 = 299792458 # Speed of light in [m/s]
        hBS = None # m
        hUT = None # m
        f = None # MHz
        dBP = None  # breakpoint between slopes
	dBPe = None   # breakpoint distance with effective antenna hights
        minDistance = None # m
        maxDistance = None # m
	def __init__(self, minDistance, maxDistance, hBS=10, hUT=1.5, f=2500):
        	self.hBS = hBS
        	self.hUT = hUT
        	self.f = f
        	self.dBP = 2*math.pi*(self.hBS)*(self.hUT)*f*1e6/self.c0   # breakpoint between slopes
        	self.dBPe = 4*(self.hBS-1)*(self.hUT-1)*f*1e6/self.c0   # breakpoint distance with effective antenna hights
        	self.minDistance = minDistance
        	self.maxDistance = maxDistance
	
	
        	super(NLOSUrbanMicroHexagonal, self).__init__(validFrequencies = Interval(800, 6000),
                                             validDistances = Interval(minDistance, maxDistance), # [m]
                                             offset = "%.5f dB" % (22.7+ 26.0*log10(1e-3)),
                                             freqFactor = "26 dB",
                                             distFactor = "36.7 dB",
                                             distanceUnit = "m", # nur fuer die Formel, nicht fuer validDistances
					                         minPathloss = "%.5f dB" % (36.7*log10(self.minDistance)+ 22.7+ 26.0*log10(self.f*(1e-3))),
                                             #minPathloss = "69.74 dB", # pathloss at 8m distance
                                             outOfMinRange = FreeSpace("44.35 dB"), # Pathloss
                                             outOfMaxRange = Deny())


class NLOSUrbanMacro(SingleSlope):
    #""" IMT-Advanced Pathloss Model, urban macro cell NLOS, all Values according to Rep. ITU-R M.2135"""
        c0 = 299792458 # Speed of light in [m/s]
        hBS = None # m
        hUT = None # m
        f = None # MHz
        h = None #m, avg. building height
        W = None #m, street width
        dBP = None  # breakpoint between slopes
        dBPe = None   # breakpoint distance with effective antenna hights
        minDistance = None # m
        maxDistance = None # m
	def __init__(self, minDistance, maxDistance, hBS=25, hUT=1.5, f=2000):
        	self.hBS = hBS
        	self.hUT = hUT
        	self.f = f
        	self.h = 20.0
        	self.W = 20.0
        	self.dBP = 2*math.pi*(self.hBS)*(self.hUT)*f*1e6/self.c0   # breakpoint between slopes
        	self.dBPe = 4*(self.hBS-1)*(self.hUT-1)*f*1e6/self.c0   # breakpoint distance with effective antenna hights
        	self.minDistance = minDistance
        	self.maxDistance = maxDistance
	
	
        	super(NLOSUrbanMacro, self).__init__(validFrequencies = Interval(800, 6000),
                                             validDistances = Interval(minDistance, maxDistance), # [m]
                                             offset = "%.5f dB" % (161.04- 7.1*log10(self.W)+ 7.5*log10(self.h)- (24.37- 3.7*pow(self.h/self.hBS,2))*log10(self.hBS)- 3*(43.42- 3.1*log10(self.hBS))- (3.2*pow(log10(11.75*self.hUT),2)-4.97)+ 20.0*log10(1e-3)),
                                             freqFactor = "20 dB",
                                             distFactor = "%.5f dB" % (43.42- 3.1*log10(self.hBS)),
                                             distanceUnit = "m", # nur fuer die Formel, nicht fuer validDistances
					                         minPathloss = "%.5f dB" % (161.04- 7.1*log10(self.W)+ 7.5*log10(self.h)- (24.37- 3.7*pow(self.h/self.hBS,2))*log10(self.hBS)+ (43.42- 3.1*log10(self.hBS))*(log10(self.minDistance)-3)- (3.2*pow(log10(11.75*self.hUT),2)-4.97)+ 20.0*log10(self.f*(1e-3))),
                                             #minPathloss = "58.65 dB", # pathloss at 8m distance
                                             outOfMinRange = FreeSpace("44.35 dB"), # Pathloss
                                             outOfMaxRange = Deny())

class NLOSSubUrbanMacro(SingleSlope):
    #""" IMT-Advanced Pathloss Model, suburban macro cell NLOS, all Values according to Rep. ITU-R M.2135"""
        c0 = 299792458 # Speed of light in [m/s]
        hBS = None # m
        hUT = None # m
        f = None # MHz
        h = None #m, avg. building height
        W = None #m, street width
        dBP = None  # breakpoint between slopes
	dBPe = None   # breakpoint distance with effective antenna hights
        minDistance = None # m
        maxDistance = None # m
	def __init__(self, minDistance, maxDistance, hBS=35, hUT=1.5, f=2000):
        	self.hBS = hBS
        	self.hUT = hUT
        	self.f = f
        	self.h = 10.0
        	self.W = 20.0
        	self.dBP = 2*math.pi*(self.hBS)*(self.hUT)*f*1e6/self.c0   # breakpoint between slopes
        	self.dBPe = 4*(self.hBS-1)*(self.hUT-1)*f*1e6/self.c0   # breakpoint distance with effective antenna hights
        	self.minDistance = minDistance
        	self.maxDistance = maxDistance
		
        	super(NLOSSubUrbanMacro, self).__init__(validFrequencies = Interval(800, 6000),
                                             validDistances = Interval(minDistance, maxDistance), # [m]    
                                     	     offset = "%.5f dB" % (161.04- 7.1*log10(self.W)+ 7.5*log10(self.h)- (24.37- 3.7*pow(self.h/self.hBS,2))*log10(self.hBS)- 3*(43.42- 3.1*log10(self.hBS))- (3.2*pow(log10(11.75*self.hUT),2)-4.97)+ 20.0*log10(1e-3)),
                                     	     freqFactor = "20 dB",
                                     	     distFactor = "%.5f dB" % (43.42- 3.1*log10(self.hBS)),
                                             distanceUnit = "m", # nur fuer die Formel, nicht fuer validDistances
					                         minPathloss = "%.5f dB" % (161.04- 7.1*log10(self.W)+ 7.5*log10(self.h)- (24.37- 3.7*pow(self.h/self.hBS,2))*log10(self.hBS)+ (43.42- 3.1*log10(self.hBS))*(log10(self.minDistance)-3)- (3.2*pow(log10(11.75*self.hUT),2)-4.97)+ 20.0*log10(self.f*(1e-3))),
                                             #minPathloss = "50.89 dB", # pathloss at 8m distance
                                             outOfMinRange = FreeSpace("44.35 dB"), # Pathloss
                                             outOfMaxRange = Deny())

class NLOSRuralMacro(SingleSlope):
    #""" IMT-Advanced Pathloss Model, rural macro cell NLOS, all Values according to Rep. ITU-R M.2135"""
        c0 = 299792458 # Speed of light in [m/s]
        hBS = None # m
        hUT = None # m
        f = None # MHz
        h = None #m, avg. building height
        W = None #m, street width
        dBP = None  # breakpoint between slopes
	dBPe = None   # breakpoint distance with effective antenna hights
        minDistance = None # m
        maxDistance = None # m
	def __init__(self, minDistance, maxDistance, hBS=35, hUT=1.5, f=800):
        	self.hBS = hBS
        	self.hUT = hUT
        	self.f = f
        	self.h = 5.0
        	self.W = 20.0
        	self.dBP = 2*math.pi*(self.hBS)*(self.hUT)*f*1e6/self.c0   # breakpoint between slopes
        	self.dBPe = 4*(self.hBS-1)*(self.hUT-1)*f*1e6/self.c0   # breakpoint distance with effective antenna hights
        	self.minDistance = minDistance
        	self.maxDistance = maxDistance
	
	
        	super(NLOSRuralMacro, self).__init__(validFrequencies = Interval(450, 6000),
                                             validDistances = Interval(minDistance, maxDistance), # [m]
                                             offset = "%.5f dB" % (161.04- 7.1*log10(self.W)+ 7.5*log10(self.h)- (24.37- 3.7*pow(self.h/self.hBS,2))*log10(self.hBS)- 3*(43.42- 3.1*log10(self.hBS))- (3.2*pow(log10(11.75*self.hUT),2)-4.97)+ 20.0*log10(1e-3)),
					                         freqFactor = "20 dB",
					                         distFactor = "%.5f dB" % (43.42- 3.1*log10(self.hBS)),
                                             distanceUnit = "m", # nur fuer die Formel, nicht fuer validDistances
					                         minPathloss = "%.5f dB" % (161.04- 7.1*log10(self.W)+ 7.5*log10(self.h)- (24.37- 3.7*pow(self.h/self.hBS,2))*log10(self.hBS)+ (43.42- 3.1*log10(self.hBS))*(log10(self.minDistance)-3)- (3.2*pow(log10(11.75*self.hUT),2)-4.97)+ 20.0*log10(self.f*(1e-3))),
                                             #minPathloss = "40.32 dB", # pathloss at 8m distance
                                             outOfMinRange = FreeSpace("44.35 dB"), # Pathloss
                                             outOfMaxRange = Deny())


class ITUInH(Pathloss):
    """
    Indoor Hotspot scenario according to ITU-R M.2135 pp. 29-31
    """
    minPathloss = None

    __plugin__ = 'ITUInH'

    def __init__(self, minPathloss = "0.0 dB"):
        self.minPathloss = minPathloss

class ITUUMa(Pathloss):
    """
    Urban Macro scenario according to ITU-R M.2135 pp. 29-31
    """
    minPathloss = None

    streetWidth = None

    buildingHeight = None

    __plugin__ = 'ITUUMa'

    def __init__(self, minPathloss = "0.0 dB"):
        self.minPathloss = minPathloss
        self.streetWidth = 20.0
        self.buildingHeight = 20.0

class ITUSMa(Pathloss):
    """
    Suburban Urban Macro scenario according to ITU-R M.2135 pp. 29-31
    """
    minPathloss = None

    streetWidth = None

    buildingHeight = None

    __plugin__ = 'ITUSMa'

    def __init__(self, minPathloss = "0.0 dB"):
        self.minPathloss = minPathloss
        self.streetWidth = 10.0
        self.buildingHeight = 5.0

class ITURMa(Pathloss):
    """
    Rural Macro scenario according to ITU-R M.2135 pp. 29-31
    """
    minPathloss = None

    streetWidth = None

    buildingHeight = None

    __plugin__ = 'ITURMa'

    def __init__(self, minPathloss = "0.0 dB"):
        self.minPathloss = minPathloss
        self.streetWidth = 20.0
        self.buildingHeight = 5.0

class ITUUMi(Pathloss):
    """
    Urban Micro scenario according to ITU-R M.2135 pp. 29-31
    """
    minPathloss = None
    streetWidth = None
    __plugin__ = 'ITUUMi'

    def __init__(self, minPathloss = "0.0 dB"):
        self.minPathloss = minPathloss 
        self.streetWidth = 20.0
