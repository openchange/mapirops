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
from pyparsing import *
import pprint

__docformat__ = 'restructuredText'

bnf = None
def MAPIROPS_MR_BNF():
    global bnf

    if not bnf:
        
        # punctuation
        lbrace   = Literal("{").suppress()
        rbrace   = Literal("}").suppress()
        lbrack   = Literal("[").suppress()
        rbrack   = Literal("]").suppress()
        lparen   = Literal("(").suppress()
        rparen   = Literal(")").suppress()
        equals   = Literal("=").suppress()
        comma    = Literal(",").suppress()
        semicol  = Literal(";").suppress()
        star     = Literal("*").suppress()
        hashsign = Literal("#").suppress()
        dquote   = Literal("\"").suppress()

        # keywords
        enum_      = Keyword("enum")
        union_     = Keyword("union")
        struct_    = Keyword("struct")
        typedef_   = Keyword("typedef")
        spec_      = Keyword("specification").suppress()

        # attributes
        switch_is_ = Keyword("switch_is")
        length_    = Keyword("length")
        value_     = Keyword("value")
        arraysize_ = Keyword("arraysize")

        # specification attributes
        revision_    = Keyword("revision")
        version_     = Keyword("version")
        release_     = Keyword("release")
        description_ = Keyword("description")

        # enum attributes
        enumtype_     = Keyword("enumtype")
        enumsize_     = Keyword("enumsize")
        # enumtype value
        enumflags_    = Keyword("flags")

        # union attributes
        switch_size_    = Keyword("switch_size")
        case_         = Keyword("case")
        default_      = Keyword("default")

        # primitive types
        bool_          = Keyword("bool")
        uint8_         = Keyword("uint8")
        uint16_        = Keyword("uint16")
        uint32_        = Keyword("uint32")
        uint64_        = Keyword("uint64")
        double_        = Keyword("double")
        ascii_string_  = Keyword("ascii_string")
        utf16_string_  = Keyword("utf16_string")
        GUID_          = Keyword("GUID")

        # General defs
        hexInteger = Combine(Optional("0x") + Word(hexnums))
        identifier = Word( alphanums + '_').setName("identifier")
        sizeInt = (Literal("8") ^ Literal("16") ^ Literal("32") ^ Literal("64"))
        typeInt = (uint8_ ^ uint16_ ^ uint32_ ^ uint64_)
        typeName = (bool_ ^ uint8_ ^ uint16_ ^ uint32_ ^ uint64_ 
                    ^ double_ ^ ascii_string_ ^ utf16_string_ ^ GUID_)
        typeUserDef = Combine((struct_ ^ union_ ^ enum_) + OneOrMore(' ') + identifier)
        typeUserDef2 = Group((struct_ ^ enum_) + identifier)

        # Attributes prefixing structure items
        attribute = Group((switch_is_ ^ value_ ^ length_ ^ arraysize_) + equals + \
                         (hexInteger ^ identifier))
        attributeList = Group(lbrack + delimitedList(attribute) + rbrack)

        # Structures
        structItem = Group(Optional(attributeList.setResultsName("attributes", True)) + (typeName ^ typeUserDef).setResultsName("structItemType", True) + identifier.setResultsName("structItemValue") + semicol).setResultsName("structItem", listAllMatches=True)
        structItemList = Group(ZeroOrMore(structItem)).setResultsName("structItems", listAllMatches=True)
        structDef = Group(struct_.setResultsName("struct") + identifier.setResultsName("structName", True) + lbrace + structItemList + rbrace + semicol).setResultsName("struct", True)

        # Enum
        enumAttribute = Group((enumsize_ + equals + sizeInt) ^ (enumtype_ + equals + enumflags_))
        enumAttributeList = Group(lbrack + delimitedList(enumAttribute) + rbrack)
        enumItemList = Group(identifier + equals + (hexInteger ^ Word(nums)))
        enumDef = Group(Optional(enumAttributeList.setResultsName("attributes", True)) + enum_.setResultsName("enum") + identifier.setResultsName("enumName", True) + lbrace + delimitedList(enumItemList).setResultsName("enumItem", True) + rbrace + semicol).setResultsName("enum", True)

        # Union
        unionDefAttribute = Group((switch_size_ + equals + sizeInt))
        unionDefAttributeList = Group(lbrack + delimitedList(unionDefAttribute) + rbrack)
        unionCaseValue = identifier ^ hexInteger ^ Combine(lparen + dblQuotedString.setParseAction(removeQuotes) + rparen)
        unionItem = Group(lbrack + ((case_ + equals + delimitedList(unionCaseValue, '|').setResultsName("unionval", True)) ^ default_.setResultsName("default", True)) + Optional(',' + arraysize_ + equals + hexInteger.setResultsName("arraysize")) + rbrack + (typeName ^ typeUserDef2).setResultsName("unionItemType", True) + identifier.setResultsName("unionItemValue", True) + semicol).setResultsName("unionItem", True)
        unionItemList = Group(ZeroOrMore(unionItem)).setResultsName("unionItems", True)
        unionDef = Group(Optional(unionDefAttributeList.setResultsName("attributes", True)) + union_.setResultsName("union") + identifier.setResultsName("unionName", True) + lbrace + unionItemList + rbrace + semicol).setResultsName("union", True)


        specItem = ZeroOrMore( structDef | enumDef | unionDef).setResultsName("specItem")

        # Specification attributes
        specAttr = Group((revision_ ^ version_ ^ release_ ^ description_) + equals + dblQuotedString.setParseAction(removeQuotes))
        specAttrList = Group(lbrack + delimitedList(specAttr) + rbrack)

        specDef = Group(Optional(specAttrList.setResultsName("attributeList", True)) + spec_ + identifier.setResultsName("name") + lbrace + specItem  + rbrace + semicol).setResultsName("specification", listAllMatches=True)

        bnf = ( ZeroOrMore(specDef) )

        return bnf

class MAPIRopsParser(object):

    def __init__(self):
        return

    def mrparser(self, mrfile=None):
        """Parse MR file.

        :param mrfile: MR file to parse
        """
        try:
            bnf = MAPIROPS_MR_BNF()
            parsingObject = bnf.parseFile(mrfile, parseAll=True)
        except ParseException, err:
            print err.line
            print " "*(err.column-1) + "^"
            print err

        return parsingObject
            
