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

class Debug:
    main                 = None
    systemManager        = None
    scenario             = None
    medium               = None
    physicalResource   	 = None
    transmitter        	 = None
    receiver          	 = None
    stations             = None
    antennas             = None
    antennaPatterns      = None
    antPatternFileName   = None
    propCache        	 = None
    umtsTransmitter 	 = None
    umtsReceiver     	 = None
    umtsChannelInterface = None
    umtsManager      	 = None
    H2ChannelInterface 	 = None
    IEEE80211ChannelInterface = None
    simControl           = None
    environment          = None

    def __init__(self):
        self.main                 = False
        self.systemManager        = False
        self.scenario             = False
        self.medium               = False
        self.physicalResource     = False
        self.transmitter          = False
        self.receiver          	  = False
        self.antennas             = False
        self.antennaPatterns      = False
        self.antPatternFileName   = "Antenna.dat"
        self.propCache        	  = False
        self.umtsTransmitter 	  = False
        self.umtsReceiver     	  = False
        self.umtsChannelInterface = False
        self.umtsManager      	  = False
        self.H2ChannelInterface   = False
        self.IEEE80211ChannelInterface = False
        self.simControl           = False
        self.environment          = False

    def allOn(self):
        self.main                 = True
        self.systemManager        = True
        self.scenario             = True
        self.medium               = True
        self.physicalResource     = True
        self.transmitter          = True
        self.receiver          	  = True
        self.antennas             = True
        self.antennaPatterns      = True
        self.propCache        	  = True
        self.umtsTransmitter 	  = True
        self.umtsReceiver     	  = True
        self.umtsChannelInterface = True
        self.umtsManager      	  = True
        self.H2ChannelInterface   = True
        self.IEEE80211ChannelInterface = True
        self.simControl           = True
        self.environment          = True
