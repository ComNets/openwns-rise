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
from openwns.logger import Logger
from rise.scenario.Propagation import Propagation, DropInPropagation

from rise.scenario.FTFading import *

class Receiver(object):
    propagation = None
    propagationCharacteristicName = None
    propagationCharacteristicId = None
    receiverNoiseFigure = None
    logger = None
    FTFadingStrategy = None # None will switch fading off; FTFadingOff() at least gives measurements

    def __init__(self, propagation, propagationCharacteristicName, parentLogger = None, **kwds):
        self.propagation = propagation
	self.propagationCharacteristicName = propagationCharacteristicName
	self.propagationCharacteristicId = propagation.getId(propagationCharacteristicName)
        self.FTFadingStrategy = FTFadingOff()
	self.logger = Logger("RISE", "PHY.Receiver", True, parentLogger)
        attrsetter(self, kwds)


class ReceiverDropIn(Receiver):
    def __init__(self, propagation = DropInPropagation.getInstance(), propagationCharacteristicName = "DropIn", parentLogger = None):
        super(ReceiverDropIn, self).__init__(propagation , propagationCharacteristicName, parentLogger)
        self.logger = Logger("RISE", "PHY.ReceiverDropIn", True, parentLogger)
        self.FTFadingStrategy = FTFadingOff()
        self.receiverNoiseFigure = "0 dB"

class SCReceiverDropIn(ReceiverDropIn):
    def __init__(self, propagation = DropInPropagation.getInstance(), propagationCharacteristicName = "DropIn", parentLogger = None):
        super(ReceiverDropIn, self).__init__(propagation, propagationCharacteristicName)

