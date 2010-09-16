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

from rise.scenario.IdRegistry import IdRegistry
import rise.scenario.Pathloss as Pathloss
import rise.scenario.Shadowing as Shadowing
import rise.scenario.FastFading as FastFading

class Configuration:
    """Holds a propagation configuration item.

    pathloss, shadowing and fastFading are intended to hold objects of the classes in
    rise.scenario.Pathloss, rise.scenario.Shadowing and rise.scenario.FastFading respectivly.
    """
    pathloss = None
    shadowing = None
    fastFading = None

    def __init__(self, pathloss = None, shadowing = None, fastFading = None):
        self.pathloss = pathloss
        self.shadowing = shadowing
        self.fastFading = fastFading


class NotConfigured(Exception):
    """Exception to be thrown if an unconfigured pair is requested in Propagation.getPair().
    """
    def __init__(self, pair, msgOutput = True):
        if (msgOutput):
            print
            print "Propagation not properly configured: Requested non-existent pair " + pair

class ChannelModelConfiguration():
    """
    Holds the configuration for a specific transceiver pair
    """

    def __init__(self, transceiverPair, channelmodel):
        self.transceiverPair = transceiverPair
        self.channelmodel = channelmodel

class Propagation:
    """Class to hold the propagation configuration.

    This class manages the propagation configuration for transmitter-receiver pairs. This
    encompasses the name to id mapping and the mapping of id pairs to Configuration objects.
    """
    __pairs = None
    __ids = None
    testRun = False

    def __init__(self):
        self.__pairs = {}
        self.__ids = IdRegistry()

    def knowsPairByName(self, transmitterName, receiverName):
        """ Check if a name pair has been configured.
        """
        if ((not self.knowsName(transmitterName)) or (not self.knowsName(receiverName))):
            return False
        return self.knowsPairById(self.__ids.getId(transmitterName),
                                  self.__ids.getId(receiverName))

    def knowsPairById(self, transmitterId, receiverId):
        """ Check if an id pair has been configured.
        """
        if ((not self.knowsId(transmitterId)) or (not self.knowsId(receiverId))):
            return False
        return (transmitterId, receiverId) in self.__pairs

    def setPair(self, transmitterName, receiverName):
        """ Configure a propagation pair.

        setPair() returns a Configuration instance. To configure pathloss, shadowing
        and fast fading, call it consecutivly:

        import rise.scenario.Pathloss as Pathloss
        import rise.scenario.Shadowing as Shadowing
        import rise.scenario.FastFading as FastFading
        propagationInstance.setPair('Foo', 'Bar').pathloss = Pathloss.FreeSpace()
        propagationInstance.setPair('Foo', 'Bar').shadowing = Shadowing.No()
        propagationInstance.setPair('Foo', 'Bar').fastFading = FastFading.No()
        """
        transmitterId = self.__ids.getId(transmitterName)
        receiverId = self.__ids.getId(receiverName)
        if (not self.knowsPairById(transmitterId, receiverId)):
            self.__pairs[transmitterId, receiverId] = Configuration()
        return self.__pairs[transmitterId, receiverId]

    def configure(self, configurations):
        """
        Configure pairs given by a list of ChannelModelConfigurations
        """
        for c in configurations:
            self.configurePair(c.transceiverPair[0], c.transceiverPair[1], c.channelmodel)

    def configurePair(self, transmitterName, receiverName, configuration):
        """ Configure a propagation pair.

        configurePair() takes a Configuration instance. To configure pathloss, shadowing
        and fast fading, you can prepare this configuration Object in advance.
        """
        transmitterId = self.__ids.getId(transmitterName)
        receiverId = self.__ids.getId(receiverName)
        self.__pairs[transmitterId, receiverId] = configuration

    def getPair(self, transmitterId, receiverId):
        """ Get the configuration for a pair.

        Returns Configuration for (transmitterId, receiverId) or raises
        NotConfigured if none was set.
        """
        try:
            return self.__pairs[transmitterId, receiverId]
        except KeyError:
            raise NotConfigured, (self.findName(transmitterId) + "->" + self.findName(receiverId), not self.testRun)

    def knowsName(self, name):
        return self.__ids.knowsName(name)

    def knowsId(self, id):
        return self.__ids.knowsId(id)

    def maxId(self):
        return self.__ids.len()

    def getId(self, name):
        return self.__ids.getId(name)

    def findId(self, name):
        return self.__ids.findId(name)

    def findName(self, id):
        return self.__ids.findName(id)

    def clear(self):
        self.__pairs = {}
        self.__ids = IdRegistry()

class DropInPropagation:
    __instance = None

    @staticmethod
    def getInstance():
        if (DropInPropagation.__instance == None):
            DropInPropagation.__instance = Propagation()
            DropInPropagation.__instance.setPair("DropIn", "DropIn").pathloss = Pathloss.FreeSpace()
            DropInPropagation.__instance.setPair("DropIn", "DropIn").shadowing = Shadowing.No()
            DropInPropagation.__instance.setPair("DropIn", "DropIn").fastFading = FastFading.No()
        return DropInPropagation.__instance
