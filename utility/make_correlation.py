#!/usr/bin/env python

import sys
import IMP.npctransport


config= IMP.npctransport.Configuration()
IMP.npctransport.set_default_configuration(config)
IMP.npctransport.create_range(config.interaction_k, .2, 20, 5)
IMP.npctransport.create_range(config.backbone_k, .2, 20, 5)
IMP.npctransport.create_range(config.nonspecific_range, 0, 5, 3, base=1)
IMP.npctransport.create_range(config.nonspecific_k, .2, 20, 3)
#IMP.npctransport.create_range(config.angular_D_factor, 10, 50, 3)
config.simulation_time_ns=100
config.box_side.lower=400
config.dump_interval_ns=2
config.number_of_trials=5


fg= IMP.npctransport.add_fg_type(config,
                                 number_of_beads=10,
                                 number=1,
                                 radius=10,
                                 interactions=1)
IMP.npctransport.create_range(fg.rest_length_factor, .5, .8, 3)
IMP.npctransport.create_range(fg.number_of_beads, 1, 10, 5)
IMP.npctransport.create_range(fg.number, 1, 5, 5)

kap= IMP.npctransport.add_float_type(config,
                                     number=1,
                                     radius=10)
IMP.npctransport.create_range(kap.number,0, 10,3)
IMP.npctransport.create_range(kap.interactions, 1, 10, 3)

crap= IMP.npctransport.add_float_type(config,
                                     number=1,
                                     radius=10,
                                      interactions=1)
IMP.npctransport.create_range(crap.number,0, 10,3)


interaction= IMP.npctransport.add_interaction(config, "fg0", "fg0")
IMP.npctransport.create_range(interaction.is_on,0,1,2)

interaction= IMP.npctransport.add_interaction(config, "fg0", "kap")
interaction= IMP.npctransport.add_interaction(config, "fg0", "crap0")


IMP.npctransport.write(config)
