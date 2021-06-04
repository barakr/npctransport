from __future__ import print_function
from IMP.npctransport import *
import IMP
import IMP.test
import sys
import math
#import read_nups
import test_util

class Tests(IMP.test.TestCase):

    def _create_cfg_file_with_fgs(self, cfg_file):
        '''
        Create a configuration file 'cfg_file' with fg chains
        '''
        config = Configuration()
        IMP.npctransport.set_default_configuration(config)
        config.box_is_on.lower=1
        config.box_side.lower=200
        fgs= IMP.npctransport.add_fg_type(config,
                                          type_name="my_fg1",
                                          number_of_beads=3,
                                          number=2,
                                          radius=25,
                                          interactions=7,
                                          rest_length_factor = 1.5)
        fgs= IMP.npctransport.add_fg_type(config,
                                          type_name="my_fg2",
                                          number_of_beads=6,
                                          number=3,
                                          radius=25,
                                          interactions=7,
                                          rest_length_factor = 1.5)
        # diffuser_R=25
        # kaps= IMP.npctransport.add_float_type(config,
        #                                       number=1,
        #                                       radius=diffuser_R,
        #                                       interactions=12,
        #                                       type_name="my_kap",
        #                                       d_factor=3
        #                                       )

        # nonspecifics= IMP.npctransport.add_float_type(config,
        #                                               number=1,
        #                                               radius=diffuser_R, #-1,
        #                                               interactions=0)
        # dump to file
        f=open(cfg_file, "wb")
        f.write(config.SerializeToString())
#        print config
        f.close()


    def test_get_fg_chain_roots(self):
        '''
        Test that SimulationData.get_fg_chain_roots() works correctly
        '''

        IMP.set_log_level(IMP.SILENT)
        test_util.test_protobuf_installed(self)
        cfg_file = self.get_tmp_file_name("barak_config.pb")
        assign_file = self.get_tmp_file_name("barak_assign.pb")
        coords = self._create_cfg_file_with_fgs(cfg_file)
        print("assigning parameter ranges from config")
        num=assign_ranges( cfg_file, assign_file, 0, False, 10 )
        print ("Loading SimData")
        sd= IMP.npctransport.SimulationData(assign_file, False)
        sd.set_rmf_file( self.get_tmp_file_name("out.rmf") )
        fgs = sd.get_fg_chain_roots( ) # atom.Hierarchies
        print (" fgs ", fgs)
        ones = 0
        twos = 0
        for fg in fgs:
            chain = get_fg_chain(fg)
            type_name = IMP.core.Typed(fg.get_child(0)).get_type().get_string()
            if(type_name == "my_fg1"):
                self.assertEqual(chain.get_number_of_beads(), 3)
                ones = ones + 1
            if(type_name == "my_fg2"):
                self.assertEqual(chain.get_number_of_beads(), 6)
                twos = twos + 1
        self.assertTrue(ones == 2 and twos == 3)


if __name__ == '__main__':
    IMP.test.main()
