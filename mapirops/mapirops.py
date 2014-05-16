#!/usr/bin/python

# OpenChange MAPI ROPs Compiler
#
# Copyright (C) Julien Kerihuel <j.kerihuel@openchange.org> 2012
#   
# This program is free software; you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation; either version 3 of the License, or
# (at your option) any later version.
#   
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#   
# You should have received a copy of the GNU General Public License
# along with this program.  If not, see <http://www.gnu.org/licenses/>.
#

import optparse
import os,sys
import pprint

# To allow running from the source directory
sys.path.append("mapirops/mapirops")
sys.path.append("mapirops")

from mapirops import MAPIRopsParser
from mapirops.Generator import MAPIGenerator

def mapirops_dump(mrdict, mr):
    print '-' * 80
    for (keyword, value) in mrdict["specification"][0]["attributeList"][0]:
        print '* %s = %s' % (keyword, value)
    print '-' * 80

    structureItem = mrdict["specification"][0]["struct"][0]["structItems"][0]["structItem"][0]
    print structureItem.keys()
    if not type(structureItem["structItemType"][0]) is list:
        print structureItem["structItemType"][0]
    else:
        print structureItem["structItemType"][0][0]
        print structureItem["structItemType"][0][1]
    print structureItem["structItemValue"]
    attributeList = structureItem["attributes"][0]
    for attr,value in attributeList:
        print '%s = %s' % (attr, value)

    enum = mrdict["specification"][0]["struct"][1]
    print enum
    print enum.keys()

    pprint.pprint(mr.asList())
    for spec in mr.asList():
        if spec[0] == 'specification':
            print 'Specification: ' + spec[1]
            

def mapirops_mapi_generator(mrdict, outputdir):
    assets = os.path.dirname(os.path.realpath(__file__))
    assets = os.path.join(assets, "assets")
    mgen = MAPIGenerator.MAPIGenerator(mrdict, outputdir, assets)
    spec = mgen.getNextSpecification()
    while spec:
        mgen.writeSpecificationHeader(spec)
        mgen.writeSpecificationCode(spec)
        spec = mgen.getNextSpecification()

if __name__ == '__main__':
    parser = optparse.OptionParser("%prog --file=INFILE [options]")
    parser.add_option("--file", type="string", metavar="MRFILE",
                      help="specify the MR file to process")
    parser.add_option("--outputdir", type="string", metavar="OUTPUTDIR",
                      help="set output folder for generated files")
    parser.add_option("--dump", action="store_true", 
                      help="Print debugging information")
    parser.add_option("--mapi-gen", action="store_true", 
                      help="Generate C files for the MAPI parser")

    opts,args = parser.parse_args()
    if len(args) != 0 or not opts.file:
        parser.print_usage()
        sys.exit(1)

    if not opts.outputdir:
        opts.outputdir = os.path.dirname(os.path.realpath(opts.file))
        print opts.outputdir

    mr = ""
    mrparser = MAPIRopsParser.MAPIRopsParser()

    try:
        mr = mrparser.mrparser(opts.file)
    except IOError:
        print "[ERROR]: Cannot find or read data from file '%s'" % opts.file
        exit(1)


    mrdict = mr.asDict()

    if (opts.mapi_gen is not None):
        mapirops_mapi_generator(mrdict, opts.outputdir)

    if (opts.dump is not None):
        mapirops_dump(mrdict, mr)



