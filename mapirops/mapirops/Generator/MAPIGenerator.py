#!/usr/bin/python

# OpenChange MAPI Rops Compiler 
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

import os,sys
import string

__docformat__ = 'restructuredText'

def MAPICommonPushItemHub(fd, indent, item, itemType, itemValue, itemAttr={}, arrayVal=""):
    """ Hub for pushing items    
    """
    if itemType.startswith("struct"):
        return MAPIGeneratorStruct(fd).pushItem(indent, item, itemType, itemValue, itemAttr, arrayVal)
    if itemType.startswith("union"):
        return MAPIGeneratorUnion(fd).pushItem(indent, item, itemType, itemValue, itemAttr, arrayVal)
    if itemType.startswith("enum"):
        return MAPIGeneratorEnum(fd).pushItem(indent, item, itemType, itemValue, itemAttr, arrayVal)
    if itemType.startswith("ascii_string") or itemType.startswith("utf16_string"):
        return MAPIGeneratorString(fd).pushItem(indent, item, itemType, itemValue, itemAttr, arrayVal)

    return MAPIGeneratorDefault(fd).pushItem(indent, item, itemType, 
                                             itemValue, itemAttr, arrayVal)


def MAPICommonPullItemHub(fd, indent, item, itemType, itemValue, itemAttr={}, arrayVal=""):
    """Hub for pulling items
    """
    if itemType.startswith("struct"):
        return MAPIGeneratorStruct(fd).pullItem(indent, item, itemType, itemValue, itemAttr, arrayVal)
    if itemType.startswith("union"):
        return MAPIGeneratorUnion(fd).pullItem(indent, item, itemType, itemValue, itemAttr, arrayVal)
    if itemType.startswith("enum"):
        return MAPIGeneratorEnum(fd).pullItem(indent, item, itemType, itemValue, itemAttr, arrayVal)
    if itemType.startswith("ascii_string") or itemType.startswith("utf16_string"):
        return MAPIGeneratorString(fd).pullItem(indent, item, itemType, itemValue, itemAttr, arrayVal)

    return MAPIGeneratorDefault(fd).pullItem(indent, item, itemType,
                                             itemValue, itemAttr, arrayVal)
    

class MAPIGeneratorDefault(object):
    def __init__(self, fd):
        self.fd = fd
        return

    def pushItem(self, indent, item, itemType, itemValue, itemAttr, arrayVal=""):
        if itemType.startswith('GUID'):
            self.fd.write("%sMAPIROPS_CHECK(mapirops_push_%s(mr, &r->%s%s));\n" % 
                      ('\t' * indent, itemType, itemValue, arrayVal))
        else:
            self.fd.write("%sMAPIROPS_CHECK(mapirops_push_%s(mr, r->%s%s));\n" % 
                          ('\t' * indent, itemType, itemValue, arrayVal))
        return

    def pullItem(self, indent, item, itemType, itemValue, itemAttr, arrayVal=""):
        self.fd.write("%sMAPIROPS_CHECK(mapirops_pull_%s(mr, &r->%s%s));\n" 
                      % ('\t' * indent, itemType, itemValue, arrayVal))
        return

class MAPIGeneratorString(object):
    def __init__(self, fd):
        self.fd = fd
        return

    def pushItem(self, indent, item, itemType, itemValue, itemAttr=[], arrayVal=""):
        length = [value for (attr, value) in itemAttr if 'length' in attr]
        if len(length):
            length = length[0]
            try:
                lengthSize = int(length)
            except ValueError:
                lengthSize = "r->%s" % length

        self.fd.write("%sMAPIROPS_CHECK(mapirops_push_%s(mr, 0, r->%s%s));\n" % ('\t' * indent, itemType, itemValue, arrayVal))

        return

    def pullItem(self, indent, item, itemType, itemValue, itemAttr={}, arrayVal=""):
        length = [value for (attr, value) in itemAttr if 'length' in attr]
        lengthSize = None
        if len(length):
            length = length[0]
            try:
                lengthSize = int(length)
            except ValueError:
                lengthSize = "r->%s" % length

        if lengthSize is None:
            self.fd.write("%sMAPIROPS_CHECK(mapirops_pull_%s(mr, mr->mem_ctx, MAPIROPS_STR_NOSIZE, &r->%s%s, %s));\n" % ('\t' * indent, itemType, itemValue, arrayVal, 0))
        else:
            self.fd.write("%sMAPIROPS_CHECK(mapirops_pull_%s(mr, mr->mem_ctx, 0, &r->%s%s, %s));\n" % ('\t' * indent, itemType, itemValue, arrayVal, lengthSize))
        return



class MAPIGeneratorStruct(object):
    """ Generate MAPI code for push, pull and print functions
    """

    def __init__(self, fd, struct={}):
        self.fd = fd
        self.struct = struct
        self.indent = 0
        if "structName" in struct:
            self.name = self.struct["structName"][0]
        if "structItems" in struct:
            self.structItems = struct["structItems"][0]
        else:
            self.structItems = []
        return

    def pushItem(self, indent, item, itemType, itemValue, itemAttr, arrayVal=""):
        """ Write the push call for a structure
        """
        self.fd.write('%sMAPIROPS_CHECK(mapirops_push_%s(mr, &r->%s%s));\n' %
                      ('\t' * indent, itemType, itemValue, arrayVal))
        return

    def pullItem(self, indent, item, itemType, itemValue, itemAttr, arrayVal=""):
        """ Write the pull call for a structure
        """
        self.fd.write('%sMAPIROPS_CHECK(mapirops_pull_%s(mr, &r->%s%s));\n' %
                      ('\t' * indent, itemType, itemValue, arrayVal))
        return

    def _direction(self, direction):
        self.fd.write("\n")
        if direction == "push":
            fmt_string = "enum mapirops_err_code "\
                "mapirops_push_struct_%s(" \
                "struct mapirops_push *mr, const struct %s *r"\
                ")\n"
        elif direction == "pull":
            fmt_string = "enum mapirops_err_code "\
                "mapirops_pull_struct_%s("\
                "struct mapirops_pull *mr, struct %s *r)\n"
        self.fd.write(fmt_string % (self.name, self.name))
        self.fd.write("{\n")
        self.indent += 1

        # Deal with empty structures
        if len(self.structItems) == 0:
            self.fd.write("%sreturn MAPIROPS_ERR_SUCCESS;\n" % ('\t' * self.indent))
            self.fd.write("}\n")
            return

        for i in range(len(self.structItems)):
            item = self.structItems[i]
            itemType = item["structItemType"][0].replace(' ', '_')
            itemValue = item["structItemValue"]
            if "attributes" in item:
                itemAttr = item["attributes"][0].asList()
            else:
                itemAttr = []

            # Handle array of items
            arraysize = [value for (attr,value) in itemAttr if 'arraysize' in attr]
            if len(arraysize):
                arraysize = arraysize[0]

                # Check if we are dealing with static (int) or dynamic array
                try:
                    arrayVal = int(arraysize)
                except ValueError:
                    arrayVal = "r->%s" % arraysize

                self.fd.write('%s{\n' % ('\t' * self.indent))
                self.indent += 1
                cntr = 'cntr_%s' % itemValue
                self.fd.write('%suint32_t %s;\n\n' % ('\t' * self.indent, cntr))
                self.fd.write('%sfor (%s = 0; %s < %s; %s++) {\n' % ('\t' * self.indent, cntr, cntr, arrayVal, cntr))
                self.indent += 1
                if direction == "push":
                    MAPICommonPushItemHub(self.fd, self.indent, item, itemType, itemValue, itemAttr, "[%s]" % cntr)
                elif direction == "pull":
                    MAPICommonPullItemHub(self.fd, self.indent, item, itemType, itemValue, itemAttr, "[%s]" % cntr)
                self.indent -= 1
                self.fd.write("%s}\n" % ('\t' * self.indent))
                self.indent -= 1
                self.fd.write('%s}\n' % ('\t' * self.indent))
            else:
                if direction == "push":
                    MAPICommonPushItemHub(self.fd, self.indent, item, itemType, itemValue, itemAttr)
                elif direction == "pull":
                    MAPICommonPullItemHub(self.fd, self.indent, item, itemType, itemValue, itemAttr)

        self.fd.write('\n%sreturn MAPIROPS_ERR_SUCCESS;\n' % ('\t' * self.indent))
        self.indent -= 1
        self.fd.write("}\n")
        return


    def push(self):
        """Generate push function for structure items.
        """
        self._direction("push")
        return

    def pull(self):
        """ Generate pull function for structure items.
        """
        self._direction("pull")
        return



class MAPIGeneratorUnion(object):
    """
    """

    def __init__(self, fd, union={}):
        self.fd = fd
        self.union = union
        self.indent = 0
        self.name = ''
        self.unionItems = []
        self.attributes = []
        if 'unionName' in self.union:
            self.name = self.union['unionName'][0]
        if 'unionItems' in self.union:
            self.unionItems = self.union['unionItems'][0]
        if 'attributes' in self.union:
            self.attributes = self.union['attributes'][0]

        return

    def pushItem(self, indent, item, itemType, itemValue, itemAttr=[], arrayVal=''):
        switchType = [value for (attr, value) in itemAttr if 'switch_is' in attr]
        if not len(switchType): raise
        switchType = switchType[0]
        
        self.fd.write('%sMAPIROPS_CHECK(mapirops_push_%s(mr, r->%s, &r->%s%s));\n' %
                      ('\t' * indent, itemType, switchType, itemValue, arrayVal))
        return

    def pullItem(self, indent, item, itemType, itemValue, itemAttr=[], arrayVal=''):
        switchType = [value for (attr, value) in itemAttr if 'switch_is' in attr]
        if not len(switchType): raise
        switchType = switchType[0]

        self.fd.write('%sMAPIROPS_CHECK(mapirops_pull_%s(mr, r->%s, &r->%s%s));\n' % ('\t' * indent, itemType, switchType, itemValue, arrayVal))

        return

    def _direction(self, direction):
        switchSize = [value for (attr,value) in self.attributes if 'switch_size' in attr]
        if len(switchSize):
            switchSize = switchSize[0]
        else:
            switchSize = "32"

        self.fd.write("\n")
        if direction == "push":
            self.fd.write("enum mapirops_err_code mapirops_push_union_%s("\
                          "struct mapirops_push *mr, uint%s_t lvl, "\
                          "const union %s *r)\n" % 
                          (self.name, switchSize, self.name))
        elif direction == "pull":
            self.fd.write("enum mapirops_err_code mapirops_pull_union_%s("\
                          "struct mapirops_pull *mr, uint%s_t lvl, "\
                          "union %s *r)\n" %
                          (self.name, switchSize, self.name))
        self.fd.write("{\n")
        self.indent += 1
        self.fd.write("%sswitch(lvl) {\n" % ('\t' * self.indent))
        self.indent += 1
        default_found = False
        for i in range(len(self.unionItems)):
            if 'default' in self.unionItems[i]:
                self.fd.write('%sdefault:\n' % ('\t' * self.indent))
                default_found = True
                self.indent += 1
            elif 'unionval' in self.unionItems[i]:
                caseList = self.unionItems[i]['unionval'][0].asList()
                for case in caseList:
                    self.fd.write('%scase (%s):\n' % 
                                  (('\t' * self.indent), case))
                self.indent += 1
            else:
                raise 'Unsupported unionval'

            unionItemType = '_'.join(self.unionItems[i]['unionItemType'][0].asList())
            unionItemValue = self.unionItems[i]['unionItemValue'][0]
            if direction == "push":
                MAPICommonPushItemHub(self.fd, self.indent, 
                                      self.unionItems[i], 
                                      unionItemType, unionItemValue)
            elif direction == "pull":
                MAPICommonPullItemHub(self.fd, self.indent,
                                      self.unionItems[i],
                                      unionItemType, unionItemValue)

            self.fd.write('%sbreak;\n' % ('\t' * self.indent))
            self.indent -= 1

        # Add a default if not explicitely defined
        if default_found is False:
            self.fd.write('%sdefault:\n' % ('\t' * self.indent))
            self.indent += 1
            self.fd.write('%sbreak;\n' % ('\t' * self.indent))
            self.indent -= 1

        self.indent -= 1
        self.fd.write("%s}\n\n" % ('\t' * self.indent))
        self.fd.write("%sreturn MAPIROPS_ERR_SUCCESS;\n" % ('\t' * self.indent))
        self.indent -= 1
        self.fd.write("}\n")
        return

        

    def push(self):
        """ Generate push function call for union items.
        """
        return self._direction("push")

    def pull(self):
        return self._direction("pull")

class MAPIGeneratorEnum(object):
    """ Generator enum MAPI code for push/pull/print functions
    """
    def __init__(self, fd, enum={}):
        self.fd = fd
        self.enum = enum
        self.indent = 0
        self.attributes = []
        self.enumItems = []
        if 'enumName' in self.enum: 
            self.name = self.enum['enumName'][0]
        if 'attributes' in self.enum: 
            self.attributes = self.enum['attributes'][0]
        if 'enumItem' in self.enum: 
            self.enumItems = self.enum['enumItem'][0]
        return

    def pushItem(self, indent, item, itemType, itemValue, itemAttr, arrayVal=""):
        """ Write the push call for an enum item
        """
        self.fd.write("%sMAPIROPS_CHECK(mapirops_push_%s(mr, r->%s%s));\n" %
                      ('\t' * indent, itemType, itemValue, arrayVal))
        return

    def pullItem(self, indent, item, itemType, itemValue, itemAttr, arrayVal=""):
        """ Write the pull call for an enum item
        """
        self.fd.write("%sMAPIROPS_CHECK(mapirops_pull_%s(mr, &r->%s%s));\n"
                      % ('\t' * indent, itemType, itemValue, arrayVal))
        return

    def push(self):
        """Generate push function for enum items.
        """
        self.fd.write("\n")
        
        # flags structure support
        enumType = [value for (attr,value) in self.attributes.asList() 
                    if 'enumtype' in attr]
        enumSize = [value for (attr, value) in self.attributes.asList()
                    if 'enumsize' in attr]
        if len(enumType): enumType = enumType[0]
        if len(enumSize): enumSize = enumSize[0]
        if enumType == "flags":
            self.fd.write("enum mapirops_err_code mapirops_push_enum_%s("\
                          "struct mapirops_push *mr, uint%s_t %s)\n" %
                          (self.name, enumSize, self.name))
            self.fd.write("{\n")
            self.indent += 1
            self.fd.write("%suint%s_t val = %s;\n\n" %
                          ('\t' * self.indent, enumSize, self.name))
            for (item,value) in self.enumItems:
                self.fd.write("%sif (val & %s) val &= ~%s;\n" % ('\t' * self.indent, item, item))
            self.fd.write("%sif (val) return MAPIROPS_ERR_INVALID_FLAGS;\n\n" % ('\t' * self.indent))

        else:
            self.fd.write("enum mapirops_err_code mapirops_push_enum_%s("\
                          "struct mapirops_push *mr, enum %s %s)\n" % 
                          (self.name, self.name, self.name))
            self.fd.write("{\n")
            self.indent += 1

            # Ensure incoming value is between min,max enum values
            (minval, _) = min(self.enumItems, key=lambda enumItem: enumItem[1]);
            (maxval, _) = max(self.enumItems, key=lambda enumItem: enumItem[1]);
            self.fd.write("\n/* Sanity Checks */\n")
            self.fd.write("%sif (%s < %s) return MAPIROPS_ERR_INVALID_VAL;\n" % ('\t' * self.indent, self.name, minval))
            self.fd.write("%sif (%s > %s) return MAPIROPS_ERR_INVALID_VAL;\n\n" % ('\t' * self.indent, self.name, maxval))


        self.fd.write("%sMAPIROPS_CHECK(mapirops_push_uint%s(mr, %s));\n\n" % ('\t' * self.indent, enumSize, self.name))
        self.fd.write("%sreturn MAPIROPS_ERR_SUCCESS;\n" % 
                      ('\t' * self.indent))
        self.indent -= 1
        self.fd.write("}\n")
        return

    def pull(self):
        """ Generate pull function for enum items
        """
        enumType = [value for (attr, value) in self.attributes.asList()
                    if 'enumtype' in attr]
        enumSize = [value for (attr, value) in self.attributes.asList()
                    if 'enumsize' in attr]
        if len(enumType): enumType = enumType[0]
        if len(enumSize): enumSize = enumSize[0]

        self.fd.write("\n")
        self.fd.write("enum mapirops_err_code mapirops_pull_enum_%s("\
                      "struct mapirops_pull *mr, enum %s *r)\n" %
                      (self.name, self.name))
        self.fd.write("{\n")
        self.indent += 1
        self.fd.write('%suint%s_t v;\n' % ('\t' * self.indent, enumSize))
        self.fd.write('\n')
        self.fd.write('%sMAPIROPS_CHECK(mapirops_pull_uint%s(mr, &v));\n' %
                      ('\t' * self.indent, enumSize))

	self.fd.write('%s*r = (enum %s) v;\n\n' % ('\t' * self.indent, self.name))
        self.fd.write('%sreturn MAPIROPS_ERR_SUCCESS;\n' % 
                      ('\t' * self.indent))
        self.indent -= 1
        self.fd.write('}\n')
        return
        

class MAPIGenerator(object):

    def __init__(self, mrdict, outputdir, assets):
        self.mrdict = mrdict
        self.outputdir = outputdir
        self.spec_index = 0
        self.assets = assets

        # precedence dict
        self.decls = []
        return

    def getSpecificationCount(self):
        """Return the number of available specifications.
        """
        return len(self.mrdict["specification"])

    def getNextSpecification(self):
        """Return the next available specification.
        """
        spec_count = len(self.mrdict["specification"])
        if self.spec_index < spec_count:
            spec = self.mrdict["specification"][self.spec_index]
            self.spec_index = self.spec_index + 1
            return spec
        return None

    def writeLicense(self, fd):
        """Write the license to the specified file descriptor.
        """
        try:
            lp = open(os.path.join(self.assets, "LICENSE"))
        except IOError as e:
            raise
        else:
            with lp:
                text = lp.read()
        lp.close()

        fd.write(text)
        return

    def writeDoxygenFileDef(self, fd, filename, spec):
        revision = None
        version = None
        date = None
        desc = None
        for pair in spec["attributeList"][0].asList():
            if pair[0] == "version": version = pair[1]
            if pair[0] == "revision": revision = pair[1]
            if pair[0] == "release": date = pair[1]
            if pair[0] == "description": desc = pair[1]
        

        fd.write("\n/**\n")
        fd.write("   \\file %s\n" % filename)
        if version is not None: fd.write("   \\version %s\n" % version)
        if revision is not None: fd.write("   \\revision %s\n" % revision)
        if date is not None: fd.write("   \\date %s\n" % date)
        if desc is not None: fd.write("   \\brief %s\n" % desc)
        fd.write("*/\n")
        
        return

    def writeSpecDefineDef(self, fd, specname, spec):
        revision = ""
        version = ""
        date = ""
        desc = ""
        for pair in spec["attributeList"][0].asList():
            if pair[0] == "version": version = pair[1]
            if pair[0] == "revision": revision = pair[1]
            if pair[0] == "release": date = pair[1]
            if pair[0] == "description": desc = pair[1]

        fd.write("\n")
        fd.write("#define %s_VERSION            \"%s\"\n" % (specname, version))
        fd.write("#define %s_REVISION           \"%s\"\n" % (specname, revision))
        fd.write("#define %s_DATE               \"%s\"\n" % (specname, date))
        fd.write("#define %s_DESCRIPTION        \"%s\"\n" % (specname, desc))

        return

    def writeDblInclusionStart(self, fd, headerName):
        headerName = headerName.upper().replace('.', '_')
        fd.write("\n")
        fd.write("#ifndef __%s__\n" % headerName)
        fd.write("#define __%s__\n" % headerName)
        return

    def writeDblInclusionEnd(self, fd, headerName):
        headerName = headerName.upper().replace('.', '_')
        fd.write("\n")
        fd.write("#endif /*! __%s__ */\n" % headerName)
        return

    def writeHeaderEnum(self, fd, enum):
        """ Write enum definition in header files.
        """

        # Retrieve data for prototype gen
        enumsize = 32
        enumtype = ''
        if "attributes" in enum:
            attrs = enum["attributes"][0].asList()
            for (attr,value) in attrs:
                if attr == 'enumsize':
                    enumsize = value
                if attr == 'enumtype':
                    enumtype = value
        self.decls.append(("enum", enum["enumName"][0], enumsize, enumtype))

        # Write the enum
        fd.write("\nenum " + enum['enumName'][0] + ' {\n')
        enumItems = enum['enumItem'][0].asList()
        maxlen = max(len(enumItem) for (enumItem, value) in enumItems)
        fmt_string = "\t%%-%ss = (int)(%%s)" % maxlen
        fd.write(',\n'.join((fmt_string % (enumItem, value)) 
                            for (enumItem, value) in enumItems))
        fd.write('\n};\n')
        return

    def writeHeaderUnion(self, fd, union):
        """ Write union definition within header files.
        """

        # Retrieve data for prototype gen
        unionswitch = 32
        if "attributes" in union:
            attrs = union["attributes"][0].asList()
            for (attr, value) in attrs:
                if attr == 'switch_size':
                    unionswitch = value
        self.decls.append(("union", union["unionName"][0], unionswitch))

        fd.write("\nunion %s {\n" % union["unionName"][0])
        # Deal with empty unions
        if not "unionItems" in union:
            fd.write("};\n")
            return

        unionItems = union["unionItems"][0]

        # find the longest structure item type
        ilen = 0
        maxvlen = 0
        for i in range(len(unionItems)):
            unionItem = unionItems[i]
            tmp = len(' '.join(unionItem["unionItemType"][0].asList())) + 2
            if tmp > ilen: ilen = tmp
            tmp = len(unionItem["unionItemValue"][0])
            if tmp > maxvlen: maxvlen = tmp

        # write the union
        for i in range(len(unionItems)):
            fmt_string = '\t%%-%ds %%s;' % (ilen)
            unionItem = unionItems[i]
            unionType = ' '.join(unionItem["unionItemType"][0].asList())
            if unionType.startswith("uint"):
                unionType = unionType + '_t'
            unionValue =  unionItem["unionItemValue"][0]
            if "unionval" in unionItem:
                cases = unionItem["unionval"].asList()
                comment = ', '.join(str(v) for v in cases[0])
            elif "default" in unionItem:
                comment = 'default'

            # Handle arraysize attributes
            if "arraysize" in unionItem:
                arraysize = unionItem["arraysize"][0]
                if all(c in string.hexdigits for c in arraysize):
                    unionValue = '%s[%s]' % (unionValue, arraysize)

            cfmt = '%s /* case: %%s */' % (' ' * (maxvlen - len(unionValue)))
            fmt_string = fmt_string + cfmt + '\n'
            fd.write(fmt_string % (unionType, unionValue, comment))

        fd.write("};\n")
        return

    def writeHeaderStruct(self, fd, struct):
        """ Write struct definition in header files.
        """
        fd.write("\nstruct %s {\n" % struct["structName"][0])
        self.decls.append(("struct", struct["structName"][0]))

        # Deal with empty structures
        if not "structItems" in struct:
            fd.write("};\n")
            return
            
        structItems = struct["structItems"]
        for i in range(len(structItems)):
            structItemList = structItems[i]
            if "structItem" in structItemList:
                # find the longest structure item type
                ilen = 0
                for j in range(len(structItemList["structItem"])):
                    itemtype = structItemList["structItem"][j]["structItemType"][0]
                    tmp = len(itemtype) + 2
                    if tmp > ilen: ilen = tmp

                # Write structure items
                fmt_string = '\t%%-%ds %%s;\t%%s\n' % ilen
                for j in range(len(structItemList["structItem"])):
                    comments = ''
                    itemtype = structItemList["structItem"][j]["structItemType"][0]
                    if itemtype.startswith("uint"):
                        itemtype = itemtype + '_t'
                    itemval = structItemList["structItem"][j]["structItemValue"]
                    # Rename strings type to char *
                    if (itemtype == 'ascii_string' or 
                        itemtype == 'utf16_string'):
                        itemtype = 'char'
                        itemval = '*' + itemval

                    # Handle attributes
                    if "attributes" in structItemList["structItem"][j]:
                        attributes = structItemList["structItem"][j]["attributes"]
                        for attr in attributes.asList():
                            # Handle array
                            if 'arraysize' in attr[0]:
                                arraysize = attr[0][1]
                                if all(c in string.hexdigits for c in arraysize):
                                    itemval = '%s[%s]' % (itemval, arraysize) # static
                                else:
                                    itemval = '*' + itemval # dynamic
                                    comments = '/* [array_size]: %s */' % arraysize

                            # Handle switch type
                            if 'switch_is' in attr[0]:
                                comments = '/* [switch_is]: %s */' % attr[0][1]

                    fd.write(fmt_string % (itemtype, itemval, comments))
        fd.write("};\n")
        return

    def writeHeaderTypes(self, fd, spec):
        if not "specItem" in spec: return
        count = len(spec["specItem"])
    
        for i in range(count):
            element = spec["specItem"][i]
            if 'enum' in element:
                self.writeHeaderEnum(fd, element)
            if 'struct' in element:
                self.writeHeaderStruct(fd, element)
            if 'union' in element:
                self.writeHeaderUnion(fd, element)

    def writeBeginDecls(self, fd):
        beginDecls = """
#ifndef __BEGIN_DECLS
#ifdef  __cplusplus
#define __BEGIN_DECLS   extern "C" {
#define __END_DECLS     }
#else
#define __BEGIN_DECLS
#define __END_DECLS
#endif
#endif

__BEGIN_DECLS

"""
        fd.write(beginDecls)
        return

    def writeEndDecls(self, fd):
        fd.write("\n__END_DECLS\n")
        return

    def writeDecls(self, fd):
        for decl in self.decls:
            if decl[0] == 'struct':
                fd.write("enum mapirops_err_code mapirops_push_struct_%s(struct mapirops_push *, const struct %s *);\n" % (decl[1], decl[1]))
                fd.write("enum mapirops_err_code mapirops_pull_struct_%s(struct mapirops_pull *, struct %s *);\n" % (decl[1], decl[1]))
            elif decl[0] == 'union':
                fd.write("enum mapirops_err_code mapirops_push_union_%s(struct mapirops_push *, uint%s_t, const union %s *);\n" % (decl[1], decl[2], decl[1]))
                fd.write("enum mapirops_err_code mapirops_pull_union_%s(struct mapirops_pull *, uint%s_t, union %s *);\n" % (decl[1], decl[2], decl[1]))
            elif decl[0] == 'enum':
                if decl[3] == 'flags':
                    fd.write("enum mapirops_err_code mapirops_push_enum_%s(struct mapirops_push *, uint%s_t);\n" % (decl[1], decl[2]))
                    fd.write("enum mapirops_err_code mapirops_pull_enum_%s(struct mapirops_pull *, enum %s *);\n" % (decl[1], decl[1]))
                else:
                    fd.write("enum mapirops_err_code mapirops_push_enum_%s(struct mapirops_push *, enum %s);\n" % (decl[1], decl[1]))
                    fd.write("enum mapirops_err_code mapirops_pull_enum_%s(struct mapirops_pull *, enum %s *);\n" % (decl[1], decl[1]))
                    
        return

    def writeSpecificationHeader(self, spec):
        """ Write specification header file
        """

        name = spec["name"].lower() + '.h'
        headerFile = os.path.join(self.outputdir, name)
        print 'Generating ' + headerFile

        try:
            sh = open(headerFile, "w")
        except IOError as e:
            print 'cannot open file for writing: ', e
        else:
            self.writeLicense(sh)
            self.writeDoxygenFileDef(sh, name, spec)
            self.writeDblInclusionStart(sh, name)
            self.writeSpecDefineDef(sh, spec["name"], spec)
            self.writeHeaderTypes(sh, spec)
            self.writeBeginDecls(sh)
            self.writeDecls(sh)
            self.writeEndDecls(sh)
            self.writeDblInclusionEnd(sh, name)
        sh.close()
        return

    def writeCodeIncludes(self, fd):
        fd.write("\n")
        fd.write("#include <libmapirops.h>\n")
        fd.write("\n")
        return

    def writeCodeTypes(self, fd, spec):
        """ Write mapirops code.
        """
        if not "specItem" in spec: return
        count = len(spec["specItem"])

        for i in range(count):
            element = spec["specItem"][i]
            if 'enum' in element:
                enum = MAPIGeneratorEnum(fd, element)
                enum.push()
                enum.pull()
            if 'struct' in element:
                struct = MAPIGeneratorStruct(fd, element)
                struct.push()
                struct.pull()
            if 'union' in element:
                union = MAPIGeneratorUnion(fd, element)
                union.push()
                union.pull()

        return

    def writeSpecificationCode(self, spec):
        """ Write specification source code file
        """

        name = spec["name"].lower() + '.c'
        codeFile = os.path.join(self.outputdir, name)
        print 'Generating ' + codeFile

        try:
            sh = open(codeFile, "w")
        except IOError as e:
            print 'cannot open file for writing: ', e
        else:
            self.writeLicense(sh)
            self.writeDoxygenFileDef(sh, name, spec)
            self.writeCodeIncludes(sh)
            self.writeCodeTypes(sh, spec)
        sh.close()
        return

    def getSpecificationInfo(self, spec):
        """Return version, release, description and specification
        name.
        """
        info = {}
        info["name"] = spec["name"].lower()
        print spec["name"].lower()


