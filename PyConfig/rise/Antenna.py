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

class Antenna(object):
    __plugin__ = None
    coordOffset = None
    radius = 10
    drawAntennaPattern = False
    patternOutputFile = "Antenna.dat"

    def __init__(self, plugin, _coordOffset):
	self.__plugin__ = plugin
	self.coordOffset = _coordOffset


class Internal(Antenna):
    elevation = None
    azimuth = None
    pattern = None
    def __init__(self, _pattern, _coordOffset, _azimuth, _elevation):
        super(Internal, self).__init__("Internal", _coordOffset)
        self.pattern = _pattern
        self.azimuth = _azimuth
        self.elevation = _elevation


class Isotropic(Internal):
    def __init__(self, _coordOffset):
        super(Isotropic, self).__init__("Isotropic", _coordOffset, 0, 0)


class OmniDirectional(Internal):
    def __init__(self, _coordOffset):
        super(OmniDirectional, self).__init__("Omnidirectional", _coordOffset, 0, 0)

class OmniDirectional7(Internal):
    def __init__(self, _coordOffset):
        super(OmniDirectional7, self).__init__("Omnidirectional7", _coordOffset, 0, 0)

class OmniDirectional9(Internal):
    def __init__(self, _coordOffset):
        super(OmniDirectional9, self).__init__("Omnidirectional9", _coordOffset, 0, 0)


class LambdaHalf(Internal):
    def __init__(self, _coordOffset):
        super(LambdaHalf, self).__init__("Lambda_Half", _coordOffset, 0, 0)


class Directed(Internal):
    def __init__(self, degrees, _coordOffset, _azimuth, _elevation):
        assert degrees in [13, 17, 30, 66, 90, 120]
        super(Directed, self).__init__("Directed_"+str(degrees),
                                       _coordOffset,
                                       _azimuth,
                                       _elevation)

class WINNER70(Internal):
    def __init__(self, _coordOffset, _azimuth, _elevation):
        super(WINNER70, self).__init__("WINNER_70",
                                       _coordOffset,
                                       _azimuth,
                                       _elevation)

class Antenna3D(Antenna):
    pattern = None
    elevation = None
    azimuth = None
    def __init__(self, fileName, _coordOffset, azimuth, _elevation):
        super(Antenna3D, self).__init__("Antenna3D", _coordOffset, _azimuth, _elevation)
        self.pattern = fileName


class Planet(Antenna):
    pattern = None
    elevation = None
    azimuth = None
    def __init__(self, fileName, _coordOffset, azimuth, _elevation):
        super(Planet, self).__init__("Planet", _coordOffset, _azimuth, _elevation)
        self.pattern = fileName


class BFAntenna(Antenna):
    arrayLayout = None
    noOfElements = None
    positionErrorVariance = None
    def __init__(self, _noOfElements, _coordsOffset, _arrayLayout="linear", _positionErrorVariance = 0.0):
        super(BFAntenna, self).__init__("BFAntenna", _coordsOffset)
        self.noOfElements = _noOfElements
        self.arrayLayout = _arrayLayout
        self.positionErrorVariance = _positionErrorVariance

class AntennaDropIn(Isotropic):
    def __init__(self):
        super(AntennaDropIn, self).__init__([0, 0, 0])


class AntennaPosDropIn(Isotropic):
    def __init__(self, position = [0,0,0] ):
        super(AntennaPosDropIn, self).__init__(position)
   
