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

import math
import random
import copy

from rise.scenario.Pathloss import Pathloss
from openwns import dB, fromdB
from openwns.interval import Interval
from Shadowing import SpatialCorrelated


class PyPathlossABC(Pathloss):
    __plugin__ = 'PyFunction'

    def __init__(self, minPathloss):
        super(PyPathlossABC, self).__init__(minPathloss = minPathloss)

    def calculatePathloss(self, sourceX, sourceY, targetX, targetY, frequency, baseHeight):
        raise Exception, "Must be overwritten"

class PyDeny(PyPathlossABC):
    def calculatePathloss(self, sourceX, sourceY, targetX, targetY, frequency, baseHeight):
        raise Exception, "Pathloss calculation denied"

class PyConstant(PyPathlossABC):
    def __init__(self, c):
        super(PyConstant, self).__init__(minPathloss = c)
        self.c = c

    def calculatePathloss(self, sourceX, sourceY, targetX, targetY, frequency, baseHeight):
        return self.c

class EuclideanDistance:
    def __init__(self):
        pass

    def calc(self, sourceX, sourceY, targetX, targetY):
        return math.sqrt((sourceX-targetX)**2 + (sourceY-targetY)**2)

class WrapAroundDistance:
    def __init__(self, sizeX, sizeY):
        self.sizeX = sizeX
        self.sizeY = sizeY

    def calc(self, sourceX, sourceY, targetX, targetY):
        d_x = min(math.fabs(sourceX - targetX), self.sizeX - math.fabs(sourceX - targetX))
        d_y = min(math.fabs(sourceY - targetY), self.sizeY - math.fabs(sourceY - targetY))
        return math.sqrt(d_x**2 + d_y**2)

class SingleSlope(PyPathlossABC):
    def __init__(self,
                 validFrequencies,
                 validDistances,
                 offset,
                 freqFactor,
                 distFactor,
                 minPathloss,
                 outOfMinRange = PyDeny,
                 outOfMaxRange = PyDeny,
                 distanceClass = None):
        super(SingleSlope, self).__init__(minPathloss)

        self.validFrequencies = validFrequencies
        self.validDistances = validDistances
        self.offset = offset
        self.freqFactor = freqFactor
        self.distFactor = distFactor
        self.outOfMinRange = outOfMinRange
        self.outOfMaxRange = outOfMaxRange
        if(distanceClass is None):
            self.distance = EuclideanDistance()
        else:
            self.distance = distanceClass

    def calculatePathloss(self, sourceX, sourceY, targetX, targetY, frequency, baseHeight):
        if(not frequency in self.validFrequencies):
            if frequency < self.validFrequencies.lowerBound:
                return self.outOfMinRange.calculatePathloss(sourceX, sourceY, targetX, targetY, frequency, baseHeight)
            else:
                return self.outOfMaxRange.calculatePathloss(sourceX, sourceY, targetX, targetY, frequency, baseHeight)

        d = self.distance.calc(sourceX, sourceY, targetX, targetY)
        if(not d in self.validDistances):
            if d < self.validDistances.lowerBound:
                return self.outOfMinRange.calculatePathloss(sourceX, sourceY, targetX, targetY, frequency, baseHeight)
            else:
                return self.outOfMaxRange.calculatePathloss(sourceX, sourceY, targetX, targetY, frequency, baseHeight)

        return dB(fromdB(self.offset) + self.freqFactor * math.log(frequency, 10) + self.distFactor * math.log(d, 10))

class MultiSlope(PyPathlossABC):
    def __init__(self,
                 minPathloss,
                 outOfMinRange = PyDeny,
                 outOfMaxRange = PyDeny,
                 distanceClass = None):

        super(MultiSlope, self).__init__(minPathloss)

        self.outOfMinRange = outOfMinRange
        self.outOfMaxRange = outOfMaxRange
        if(distanceClass is None):
            self.distance = EuclideanDistance()
        else:
            self.distance = distanceClass

        self.min = 1000
        self.max = 0
        self.slopes = []

    def addSlope(self, s):
        self.min = min(self.min, s.validDistances.lowerBound)
        self.max = max(self.max, s.validDistances.upperBound)
        self.slopes.append(copy.deepcopy(s))

    def calculatePathloss(self, sourceX, sourceY, targetX, targetY, frequency, baseHeight):
        d = self.distance.calc(sourceX, sourceY, targetX, targetY)
        if d < self.min:
            return self.outOfMinRange.calculatePathloss(sourceX, sourceY, targetX, targetY, frequency, baseHeight)
        elif d > self.max:
            return self.outOfMaxRange.calculatePathloss(sourceX, sourceY, targetX, targetY, frequency, baseHeight)

        for s in self.slopes:
            if d in s.validDistances:
                return s.calculatePathloss(sourceX, sourceY, targetX, targetY, frequency, baseHeight)

        raise Exception, "No valid pathloss found for d = " + str(d)


class UMiLoS(MultiSlope):
    def __init__(self,
                 f_c,
                 seed,
                 distanceClass = None):
        super(UMiLoS, self).__init__(dB(42.0),
                                     outOfMinRange = PyConstant(dB(42.0)),
                                     outOfMaxRange = PyDeny,
                                     distanceClass = distanceClass)
        c = 3e8
        h_BS = 10.0
        h_UT = 1.5
        d_BP = 4.0 * (h_BS - 1.0) * (h_UT - 1.0) * f_c * 1e6 / c

        # slope < d_BP
        self.addSlope(SingleSlope(validFrequencies = Interval(f_c - 500, f_c + 500),
                                  validDistances = Interval(10, d_BP),
                                  offset = dB(28.0 + 20 * math.log10(1e-3)),
                                  freqFactor = 20.0,
                                  distFactor = 22.0,
                                  minPathloss = dB(42.0),
                                  outOfMinRange = PyConstant(dB(42.0)),
                                  outOfMaxRange = PyDeny,
                                  distanceClass = distanceClass))
        # slope > d_BP
        self.addSlope(SingleSlope(validFrequencies = Interval(f_c - 500, f_c + 500),
                                  validDistances = Interval(d_BP, 5000),
                                  offset = dB(7.8 - 18*math.log10(h_BS - 1) - 18*math.log10(h_UT - 1) + 2*math.log10(1e-3)),
                                  freqFactor = 2.0,
                                  distFactor = 40.0,
                                  minPathloss = dB(42.0),
                                  outOfMinRange = PyDeny,
                                  outOfMaxRange = PyDeny,
                                  distanceClass = distanceClass))

        self.shadowing = SpatialCorrelated(3.0, 10, seed = seed)

    def calculatePathloss(self, sourceX, sourceY, targetX, targetY, frequency, baseHeight, withShadowing=True):
        pl = super(UMiLoS, self).calculatePathloss(sourceX, sourceY, targetX, targetY, frequency, baseHeight)
        if(withShadowing):
            return dB(fromdB(pl) + fromdB(self.shadowing.getShadowing(sourceX, sourceY, targetX, targetY)))
        else:
            return pl

class UMiNLoS(SingleSlope):
    def __init__(self,
                 seed,
                 distanceClass = None):
        super(UMiNLoS, self).__init__(validFrequencies = Interval(2000, 6000),
                                      validDistances = Interval(10, 2000),
                                      offset = dB(22.7 + 26 * math.log10(1e-3)),
                                      freqFactor = 26.0,
                                      distFactor = 36.7,
                                      minPathloss = dB(42.0),
                                      outOfMinRange = PyConstant(dB(42.0)),
                                      outOfMaxRange = PyDeny,
                                      distanceClass = distanceClass)

        self.shadowing = SpatialCorrelated(4.0, 13, seed = seed)

    def calculatePathloss(self, sourceX, sourceY, targetX, targetY, frequency, baseHeight, withShadowing=True):
        pl = super(UMiNLoS, self).calculatePathloss(sourceX, sourceY, targetX, targetY, frequency, baseHeight)
        if(withShadowing):
            return dB(fromdB(pl) + fromdB(self.shadowing.getShadowing(sourceX, sourceY, targetX, targetY)))
        else:
            return pl

class randomNumberGenerator:

    def __init__(self, minX, minY, maxDistance):
        self.minX = minX
        self.minY = minY
        self.maxDistance = maxDistance
        self.lastHash = 0
        self.r = random.Random()

    def random(self, sourceX, sourceY, targetX, targetY):
        # Hash must be symmetric
        hash = ((sourceX+targetX-2*self.minX) + (sourceY+targetY-2*self.minY)*self.maxDistance)+1
        if(hash == self.lastHash):
            return self.r.random()
        else:
            self.lastHash = hash
            self.r.seed(hash)
            return self.r.random()

class NLoSDecider:
    def __init__(self,
                 P_LoS,
                 minX, maxX, minY, maxY,
                 distanceClass = None,
                 rng = None):
        self.P_LoS = P_LoS

        if(distanceClass is None):
            self.distance = EuclideanDistance()
        else:
            self.distance = distanceClass

        d = 1.0
        self.min = 1.0
        while self.P_LoS.calc(d) > 0.99:
            d += 1.0
        self.min = d - 1.0
        while self.P_LoS.calc(d) > 0.01:
            d += 1.0
        self.max = d

        # own instance of pseudo-random number generator to decide LoS/NLoS
        if(rng is None):
            self.r = randomNumberGenerator(minX, minY, math.sqrt((maxX-minX)**2 + (maxY-minY)**2))
        else:
            self.r = rng

    def isLoS(self, sourceX, sourceY, targetX, targetY):
        d = self.distance.calc(sourceX, sourceY, targetX, targetY)
        if(d < self.min):
            return True
        if(d > self.max):
            return False

        p_los = self.P_LoS.calc(d)
        return(p_los >= self.r.random(sourceX, sourceY, targetX, targetY))


class UMiP_LoS:
    def calc(self, d):
        if(d > 0):
            return(min(18.0/d, 1.0) * (1.0 - math.exp(-d/36)) + math.exp(-d/36))
        else:
            return 1.0

class UMiOutdoor(PyPathlossABC):
    def __init__(self,
                 f_c,
                 seed,
                 minX, maxX, minY, maxY,
                 distanceClass = None):
        super(UMiOutdoor, self).__init__(minPathloss = dB(42.0))

        if(distanceClass is None):
            self.distance = EuclideanDistance()
        else:
            self.distance = distanceClass

        self.LoS = UMiLoS(f_c,
                          seed,
                          distanceClass = distanceClass)

        self.NLoS = UMiNLoS(seed,
                            distanceClass)

        self.decider = NLoSDecider(UMiP_LoS(),
                                   minX, maxX, minY, maxY,
                                   distanceClass)

    def calculatePathloss(self, sourceX, sourceY, targetX, targetY, frequency, baseHeight):
        if(self.decider.isLoS(sourceX, sourceY, targetX, targetY)):
            return self.LoS.calculatePathloss(sourceX, sourceY, targetX, targetY, frequency, baseHeight)
        else:
            return self.NLoS.calculatePathloss(sourceX, sourceY, targetX, targetY, frequency, baseHeight)

    def calculateDistance(self, sourceX, sourceY, targetX, targetY):
        return self.distance.calc(sourceX, sourceY, targetX, targetY)

class UMi(UMiOutdoor):
    def __init__(self,
                 f_c,
                 seed,
                 minX, maxX, minY, maxY,
                 distanceClass = None):
        super(UMi, self).__init__(f_c, seed, minX, maxX, minY, maxY, distanceClass)

        self.r = randomNumberGenerator(minX, minY, math.sqrt((maxX-minX)**2 + (maxY-minY)**2))
        self.decider = NLoSDecider(UMiP_LoS(),
                                   minX, maxX, minY, maxY,
                                   distanceClass,
                                   self.r)
        self.shadowing = SpatialCorrelated(7.0, 13, seed = seed)

    def calculatePathloss(self, sourceX, sourceY, targetX, targetY, frequency, baseHeight):
        isLos = self.decider.isLoS(sourceX, sourceY, targetX, targetY)
        isOutdoor = (self.r.random(sourceX, sourceY, targetX, targetY) > 0.5)

        if(isOutdoor):
            if(isLos):
                return(self.LoS.calculatePathloss(sourceX, sourceY, targetX, targetY, frequency, baseHeight))
            else:
                return(self.NLoS.calculatePathloss(sourceX, sourceY, targetX, targetY, frequency, baseHeight))
        else:
            if(isLos):
                pl = self.LoS.calculatePathloss(sourceX, sourceY, targetX, targetY, frequency, baseHeight, withShadowing=False)
            else:
                pl = self.NLoS.calculatePathloss(sourceX, sourceY, targetX, targetY, frequency, baseHeight, withShadowing=False)

            # add indoor part
            pl = fromdB(pl) + self.r.random(sourceX, sourceY, targetX, targetY)*25*0.5 + 20

            # add indoor shadowing (sigma = 7)
            pl += fromdB(self.shadowing.getShadowing(sourceX, sourceY, targetX, targetY))

            return(dB(pl))





