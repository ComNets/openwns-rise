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

from openwns.logger import Logger
from rise.scenario.Propagation import Propagation, DropInPropagation

class Transmitter(object):
    propagation = None
    propagationCharacteristicName = None
    propagationCharacteristicId = None
    logger = None

    def __init__(self, propagation, propagationCharacteristicName, parentLogger = None):
        self.propagation = propagation
	self.propagationCharacteristicName = propagationCharacteristicName
	self.propagationCharacteristicId = propagation.getId(propagationCharacteristicName)
	self.logger = Logger("RISE", "PHY.Transmitter", True, parentLogger)

class TransmitterDropIn(Transmitter):
    def __init__(self, propagation = DropInPropagation.getInstance(), propagationCharacteristicName = "DropIn", parentLogger = None):
	super(TransmitterDropIn, self).__init__(propagation, propagationCharacteristicName)
        self.logger = Logger("RISE", "PHY.TransmitterDropIn", True, parentLogger)

class SCTransmitterDropIn(Transmitter):
    def __init__(self, propagation = DropInPropagation.getInstance(), propagationCharacteristicName = "DropIn", parentLogger = None):
	super(SCTransmitterDropIn, self).__init__(propagation, propagationCharacteristicName)
