#!/usr/bin/env python

'''short.py - a really brief demonstration of using the ao and hip modules'''

import hip
import ao
import sys

filename = sys.argv[1]
device = 'esd'
SIZE = 4806

hf = hip.hip(filename)
id = ao.driver_id(device)
ao = ao.AudioDevice(id)

while 1:
    (buff, bytes, bit) = hf.read(SIZE)
    if bytes == 0: break
    ao.play(buff, bytes)
