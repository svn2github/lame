#!/usr/bin/env python

"""Setup script for the pyhip module distribution."""

import os, re, sys, string
from distutils.core import setup
from distutils.extension import Extension

VERSION_MAJOR = 0
VERSION_MINOR = 2
pyhip_version = str(VERSION_MAJOR) + '.' + str(VERSION_MINOR)

def get_setup():
    data = {}
    r = re.compile(r'(\S+)\s*?=\s*?(.+)')
    
    if not os.path.isfile('Setup'):
        print "No 'Setup' file. Perhaps you need to run the configure script."
        sys.exit(1)

    f = open('Setup', 'r')
    
    for line in f.readlines():
        m = r.search(line)
        if not m:
            print "Error in setup file:", line
            sys.exit(1)
        key = m.group(1)
        val = m.group(2)
        data[key] = val
        
    return data

data = get_setup()

hip_include_dir = data['mp3hip_include_dir']
hip_lib_dir = data['mp3hip_lib_dir']
hip_libs = string.split(data['mp3hip_libs'])

hipmodule = Extension(name='hip',
                         sources=['src/hipmodule.c',
                                  'src/pyhip.c',
                                  'src/pympeginfo.c'],
                         define_macros = [('VERSION', '"%s"' % pyhip_version)],
                         include_dirs=[hip_include_dir],
                         library_dirs=[hip_lib_dir],
                         libraries=hip_libs)

setup ( name = "pyhip",
        version = pyhip_version,
        description = "A wrapper for libmp3hip, a LGPLed mp3 decoder.",
        author = "Myers Carpenter",
        author_email = "myers_carpenter@users.sf.net",
        url = "http://maski.org/pyhip",
        ext_modules = [hipmodule])




