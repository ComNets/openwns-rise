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
import openwns.logger
import rise.CoderSpecification

# Info on MI: http://en.wikipedia.org/wiki/Mutual_information

class PLMapping:
    mapperName = None
    mi2per = None

# Generic MI2PER Mapping
class GENERIC(PLMapping):
    mapperName = "rise.plmapping.GENERIC" # __plugin__ for C++ STATIC_FACTORY
    logger = None
    def __init__(self, parent = None):
        self.logger = openwns.logger.Logger("RISE", "MI2PER", True, parent)
        self.mi2per = rise.CoderSpecification.default

# MI2PER Mapping according to the Winner L2S interface
class WINNER(PLMapping):
    mapperName = "rise.plmapping.WINNER" # __plugin__ for C++ STATIC_FACTORY
    logger = None
    def __init__(self, parent = None):
        self.logger = openwns.logger.Logger("RISE", "MI2PER", True, parent)
        self.mi2per = rise.CoderSpecification.default

# MI2PER mapping to a fixed PER
class Fixed(PLMapping):
    mapperName = "rise.plmapping.Fixed"
    PER  = 0.05
    def __init__(self, **kw):
        self.mi2per = rise.CoderSpecification.default
        attrsetter(self, kw)

# MI2PER mapping using a formula
class Formula(PLMapping):
    mapperName = "rise.plmapping.Formula"
    logger = None
    def __init__(self, **kw):
        self.logger = openwns.logger.Logger("RISE", "MI2PER", True, parent)
        self.mi2per = rise.CoderSpecification.CoderMapping(mapping = "Formula")
        attrsetter(self, kw)

