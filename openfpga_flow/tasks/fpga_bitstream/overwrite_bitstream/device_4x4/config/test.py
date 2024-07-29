import xml.etree.ElementTree as ET
import sys
import shutil
import os
import random

random.seed()
assert len(sys.argv) >= 2
assert sys.argv[1] in ["run_golden", "generate_testcase", "validate"]
TEST_BIT_COUNT = 200

def read_fabric_bitstream_xml(file) :

  bit_count = 0
  tree = ET.parse(file)
  root = tree.getroot()
  assert root.tag == "fabric_bitstream", "Root tag is not 'fabric_bitstream', but '%s'" % root.tag
  for region in root :
    assert region.tag == "region", "fabric_bitstream child node tag is not 'region', but '%s'" % region.tag
    for bit in region :
      assert bit.tag == "bit", "region child node tag is not 'bit', but '%s'" % bit.tag
      assert "path" in bit.attrib, "Attribute 'path' does not exist in bit node"
      assert "value" in bit.attrib, "Attribute 'value' does not exist in bit node"
      assert bit.attrib["value"] in ["0", "1"]
      bit_count += 1
  return [tree, bit_count]
  
def read_bitstream_annotation_xml(file) :
  
  xml = {}
  tree = ET.parse(file)
  root = tree.getroot()
  assert root.tag == "openfpga_bitstream_setting", "Root tag is not 'openfpga_bitstream_setting', but '%s'" % root.tag
  for overwrite_bitstream in root :
    assert overwrite_bitstream.tag == "overwrite_bitstream", "openfpga_bitstream_setting child node tag is not 'overwrite_bitstream', but '%s'" % overwrite_bitstream.tag
    for bit in overwrite_bitstream :
      assert bit.tag == "bit", "overwrite_bitstream child node tag is not 'bit', but '%s'" % bit.tag
      assert "path" in bit.attrib, "Attribute 'path' does not exist in bit node"
      assert "value" in bit.attrib, "Attribute 'value' does not exist in bit node"
      path = bit.attrib["path"]
      assert path not in xml
      index = path.rfind("[")
      assert index != -1
      path = "%s.mem_out%s" % (path[:index], path[index:])
      assert path not in xml
      assert bit.attrib["value"] in ["0", "1"]
      xml[path] = bit.attrib["value"]
  return xml

if sys.argv[1] == "run_golden" :

  assert len(sys.argv) >= 3
  openfpga_exe = os.path.abspath("%s/build/openfpga/openfpga" % sys.argv[2])
  assert os.path.exists(openfpga_exe)
  shutil.rmtree("golden", ignore_errors=True)
  os.mkdir("golden")
  original_openfpga = open("and2_run.openfpga")
  golden_openfpga = open("golden/and2_run.openfpga", "w")
  for line in original_openfpga :
    if line.find("ext_exec") == 0 :
      pass
    else :
      golden_openfpga.write(line)
  golden_openfpga.close()
  original_openfpga.close()
  bitstream_annotation = open("golden/bitstream_annotation.xml", "w")
  bitstream_annotation.write("<openfpga_bitstream_setting/>\n")
  bitstream_annotation.close()
  shutil.copyfile("and2.blif", "golden/and2.blif")
  shutil.copyfile("and2_ace_out.act", "golden/and2_ace_out.act")
  cmd = "cd golden && %s -batch -f and2_run.openfpga > golden.log" % (openfpga_exe)
  assert os.system(cmd) == 0

elif sys.argv[1] == "generate_testcase" :
  
  (tree, bit_count) = read_fabric_bitstream_xml("golden/fabric_bitstream.xml")
  random_bits = []
  while len(random_bits) != TEST_BIT_COUNT :
    bit = random.randint(0, bit_count - 1)
    if bit not in random_bits :
      random_bits.append(bit)
  bitstream_annotation = open("bitstream_annotation.xml", "w")
  bitstream_annotation.write("<openfpga_bitstream_setting>\n")
  bitstream_annotation.write("  <overwrite_bitstream>\n")
  index = 0
  for region in tree.getroot() :
    for bit in region :
      if index in random_bits :
        path = bit.attrib["path"]
        value = bit.attrib["value"]
        assert value in ["0", "1"]
        path = path.replace(".mem_out[", "[")
        bitstream_annotation.write("    <bit value=\"%s\" path=\"%s\"/>\n" % ("1" if value == "0" else "0", path))
      index += 1
  bitstream_annotation.write("  </overwrite_bitstream>\n")
  bitstream_annotation.write("</openfpga_bitstream_setting>\n")
  bitstream_annotation.close()

else :

  gtree = ET.parse("golden/fabric_bitstream.xml")
  tree = ET.parse("fabric_bitstream.xml")
  bitstream_annotation = read_bitstream_annotation_xml("bitstream_annotation.xml")
  checked_count = 0
  for gregion, region in zip(gtree.getroot(), tree.getroot()) :
    for gbit, bit in zip(gregion, region) :
      assert bit.attrib["path"] == gbit.attrib["path"]
      path = bit.attrib["path"]
      if path in bitstream_annotation :
        # This is something we want to overwrite, hence the value should 
        #   Same in the annotation file
        #   Not same in golden fabric
        assert bit.attrib["value"] != gbit.attrib["value"]
        assert bit.attrib["value"] == bitstream_annotation[path]
      else :
        # This is not what we want to overwrite
        # Hence the value should same in golden fabric
        assert bit.attrib["value"] == gbit.attrib["value"]

exit(0)
