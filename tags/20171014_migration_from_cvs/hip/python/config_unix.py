#!/usr/bin/env python

import string
import os
import sys

def msg_checking(msg):
    print "Checking", msg, "...",

def execute(cmd, display = 0):
    if display:
        print cmd
    return os.system(cmd)

def run_test(input, flags = ''):
    try:
        f = open('_temp.c', 'w')
        f.write(input)
        f.close()
        compile_cmd = '%s -o _temp _temp.c %s' % (os.environ.get('CC', 'cc'),
                                                  flags)
        if not execute(compile_cmd):
            execute('./_temp')

    finally:
        execute('rm -f _temp.c _temp')
    
ogg_test_program = '''
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ogg/ogg.h>

int main ()
{
  system("touch conf.oggtest");
  return 0;
}
'''

def find_ogg(ogg_prefix = '/usr/local', enable_oggtest = 1):
    """A rough translation of ogg.m4"""

    ogg_include_dir = ogg_prefix + '/include'
    ogg_lib_dir = ogg_prefix + '/lib'
    ogg_libs = 'ogg'

    msg_checking('for Ogg')

    if enable_oggtest:
        execute('rm -f conf.oggtest', 0)

        try:
            run_test(ogg_test_program)
            if not os.path.isfile('conf.oggtest'):
                raise RuntimeError, "Did not produce output"
            execute('rm conf.oggtest', 0)
            
        except:
            print "test program failed"
            return None

    print "success"

    return {'ogg_libs' : ogg_libs,
            'ogg_lib_dir' : ogg_lib_dir,
            'ogg_include_dir' : ogg_include_dir}


vorbis_test_program = '''
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <vorbis/codec.h>

int main ()
{
  system("touch conf.vorbistest");
  return 0;
}
'''

def find_vorbis(vorbis_prefix = '/usr/local', enable_vorbistest = 1):
    """A rough translation of vorbis.m4"""

    vorbis_include_dir = vorbis_prefix + '/include'
    vorbis_lib_dir = vorbis_prefix + '/lib'
    vorbis_libs = 'vorbis vorbisfile vorbisenc'

    msg_checking('for Vorbis')

    if enable_vorbistest:
        execute('rm -f conf.vorbistest', 0)

        try:
            run_test(vorbis_test_program)
            if not os.path.isfile('conf.vorbistest'):
                raise RuntimeError, "Did not produce output"
            execute('rm conf.vorbistest', 0)
            
        except:
            print "test program failed"
            return None

    print "success"

    return {'vorbis_libs' : vorbis_libs,
            'vorbis_lib_dir' : vorbis_lib_dir,
            'vorbis_include_dir' : vorbis_include_dir}

def write_data(data):
    f = open('Setup', 'w')
    for item in data.items():
        f.write('%s = %s\n' % item)
    f.close()
    print "Wrote Setup file"
            
def print_help():
    print '''%s
    --prefix      Give the prefix in which vorbis was installed.''' % sys.argv[0]
    sys.exit(0)

def parse_args():
    data = {}
    argv = sys.argv
    for pos in range(len(argv)):
        if argv[pos] == '--help':
            print_help()
        if argv[pos] == '--prefix':
            pos = pos + 1
            if len(argv) == pos:
                print "Prefix needs an argument"
                sys.exit(1)
            data['prefix'] = argv[pos]

    return data
    
def main():
    args = parse_args()
    prefix = args.get('prefix', '/usr/local')

    data = find_vorbis(vorbis_prefix = prefix)
    if not data:
        print "Config failure"
        sys.exit(1)

    ogg_data = find_ogg(ogg_prefix = prefix)
    if not ogg_data:
        print "Config failure"
        sys.exit(1)
    data.update(ogg_data)
    
    write_data(data)

if __name__ == '__main__':
    main()




