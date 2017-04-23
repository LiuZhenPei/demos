#!/usr/bin/env python
# -*- coding: utf-8 -*-

#---Generator function---
def fib(Max):
    n, a, b = 0, 0, 1
    while n < Max:
        yield b
        n += 1
        a, b = b, a + b

#---Generator---
g = fib(100)

#---Open a file with w+---
fp = open("fib.txt","w+")

#---Write fib-number to fib.txt---
print('-'*25)
print("Writing data to file...")
print('-'*25)
for i in g:
    strfib = str(i)
    fp.write("%s\n" % strfib)
print("Competely to writed\n")

#---Locate to begin of fib.txt---
print('-'*25)
print("Seeking to the begin...")
print('-'*25)
fp.seek(0,0)
pos = fp.tell()
print("Position: %d\n" % pos)

#---Read the file of fib.txt and calculate sum---
print('-'*25)
print("Reading data from file and cal...")
print('-'*25)
sum = 0
for i in range(100):
    str2int = int(fp.readline())
    sum += str2int
print("Sum: %d\n" % sum)

#---Close file---
fp.close()

#--- fib.txt info---
print('-'*25)
print("File's info")
print('-'*25)
print("File name: %s" % fp.name)
print("Open mode: %s" % fp.mode)
print("If closed file: %s" % fp.closed)

