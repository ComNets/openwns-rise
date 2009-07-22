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
from openwns.geometry.position import Position
from Antenna import AntennaDropIn
from Transmitter import TransmitterDropIn
from Receiver import ReceiverDropIn

class Station(object):
    antennas = None
    logger = None
    debug = None
    # deprecated receiver and transmitter: should be removed
    receiver = None
    transmitter = None
    pathlossMap = None
    pathlossInterpolation = None
    shadowingMap = None
    shadowingInterpolation = None

    def __init__(self, _antennas, _receiver, _transmitter, parentLogger = None):
        self.antennas = _antennas
        self.receiver = _receiver
        self.transmitter = _transmitter
        self.debug = False
        self.logger = Logger("RISE", "PHY.Station", True, parentLogger)

# not used anywhere:
class BaseStation(Station):
    def __init__(self, _antennas, _receiver, _transmitter, parentLogger = None):
        super(BaseStation, self).__init__(_antennas, _receiver, _transmitter, parentLogger)

# used in OFDMAPhy, but can be replaced by simple Station
class MobileStation(Station):
    def __init__(self, _antennas, _receiver, _transmitter, parentLogger = None):
        super(MobileStation, self).__init__(_antennas, _receiver, _transmitter, parentLogger)

# not used anywhere:
class RelayStation(Station):
    def __init__(self, _antennas, _receiver, _transmitter, parentLogger = None):
        super(RelayStation, self).__init__(_antennas, _receiver, _transmitter, parentLogger)

class StationDropIn(Station):
    def __init__(self, parentLogger = None):
        super(StationDropIn, self).__init__(_antennas = [AntennaDropIn()],
                                            _receiver = [ReceiverDropIn()],
                                            _transmitter = [TransmitterDropIn()],
                                            parentLogger = parentLogger)
        #self.logger = Logger("RISE", "StationDropIn", False, parentLogger)
        self.logger = Logger("RISE", "StationDropIn", True, parentLogger)
