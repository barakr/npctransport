from __future__ import print_function
import IMP.npctransport
import IMP.test
import sys
import test_util

class Tests(IMP.test.TestCase):
  def test_Avro2PBReader(self):
    """ Testing whether an avro file is read properly by Avro2PBReader """
    test_util.test_protobuf_installed(self)
    in_avro= self.get_input_file_name( "avro.sample");
    print("parsing", in_avro)
    a = IMP.npctransport.Avro2PBReader([in_avro])
    o = IMP.npctransport.Output()
    s = a.read_next()
    while len(s) > 0: # while valid output
      o.ParseFromString(s)
      rg = o.statistics.fgs[0].radius_of_gyration
#     print rg
      s = a.read_next()
    self.assertAlmostEqual(rg, 40.2645193868, 7)

if __name__ == '__main__':
    IMP.test.main()
