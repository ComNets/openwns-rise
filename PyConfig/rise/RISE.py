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

from openwns.module import Module
from rise.Debug import Debug

class PropagationCache:
    cache = None
    def __init__(self):
        # vector | idVector | H2 | hash | no
        self.cache = "idVector"

class RISE(Module):
    numProbes = None
    debug     = None
    propagationCache = None
    probesConfig = None
    eventDrivenSim = None
    ownMobilityRNG = None
    mobilitySeed   = 42
    probes = None

    def __init__(self):
        super(RISE, self).__init__("rise", "rise")
	self.numProbes = 20
        self.debug = Debug() # all False
        self.propagationCache = PropagationCache()
        self.eventDrivenSim = True
