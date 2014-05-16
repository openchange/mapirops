#! /usr/bin/env python
# encoding: utf-8

top = '.'
out = 'build'

APPNAME = 'mapirops'
VERSION = 0.1

def options(ctx):
    ctx.load('compiler_c')

def set_options(ctx):
    ctx.recurse('lib')

def configure(ctx):
    ctx.recurse('lib')
    ctx.write_config_header('config.h')

def build(ctx):
    if ctx.cmd == 'doc':
        ctx.recurse('lib')
    else:
        ctx.recurse('lib')
    
from waflib.Build import BuildContext
class doc_class(BuildContext):
    cmd = 'doc'
