#!/usr/bin/python

# -*- coding: utf-8 -*-
"""
Created on Sun Nov 27 00:46:20 2016

@author: okan
"""

import sys

fp = open('out.txt', 'a+')
# sum given two numbers and print out the result
fp.write(str(float(sys.argv[1])+float(sys.argv[2]))+"\n")
fp.close()