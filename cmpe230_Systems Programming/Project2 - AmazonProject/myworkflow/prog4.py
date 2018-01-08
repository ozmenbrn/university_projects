#!/usr/bin/python

# -*- coding: utf-8 -*-
"""
Created on Sun Nov 27 00:46:20 2016

@author: okan
"""

import sys

fp = open('out.txt', 'a+')
# sum given two numbers and print out the result
fp.write(sys.argv[1]+"\n")
fp.write(sys.argv[2]+"\n")
fp.write(sys.argv[3]+"\n")
fp.close()