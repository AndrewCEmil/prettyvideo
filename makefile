#
#  EasyBMP Cross-Platform Windows Bitmap Library  
#                                                
#  Author: Paul Macklin                          
#   email: macklin01@users.sourceforge.net       
# support: http://easybmp.sourceforge.net        
#          file: makefile                
#    date added: 04-22-2006                      
# date modified: 12-01-2006                      
#       version: 1.06                            
#                                                
#   License: BSD (revised/modified)              
# Copyright: 2005-6 by the EasyBMP Project        
#                                                
# description: Sample makefile for compiling with
#              the EasyBMP library. This compiles
#              the EasyBMPsample.cpp program.
#

CC = g++

# this line gives compiler optimizations that are geared towards g++ and Pentium4 
# computers. Comment it out if you don't have a Pentium 4 (or Athlon XP) or up

# CFLAGS = -O3 -Wno-deprecated -mcpu=pentium4 -march=pentium4 \
# -mfpmath=sse -msse -mmmx -msse2 -pipe -fomit-frame-pointer -s 

# Uncomment these two lines to use with any Pentium with MMX or up.

# CFLAGS = -Wno-deprecated -mcpu=pentium -march=pentium -pipe \
# -fomit-frame-pointer -mmmx -funroll-all-loops -s

# Uncomment these lines for some "safe" optimization flags

CFLAGS = -pipe -fomit-frame-pointer -ggdb -std=c++11 -stdlib=libc++

images: EasyBMP.o ppix.o dagcontainer.o triops.o triopgenerator.o daggenerator.o
	clang++ $(CFLAGS) -g EasyBMP.o ppix.o dagcontainer.o triops.o triopgenerator.o daggenerator.o -o ppix

EasyBMP.o: EasyBMP.cpp EasyBMP*.h
	clang++ $(CFLAGS) -c EasyBMP.cpp

ppix.o: ppix.cpp
	clang++ $(CFLAGS) -c ppix.cpp

dagcontainer.o: ppix.cpp
	clang++ $(CFLAGS) -c dagcontainer.cpp

triops.o: ppix.cpp
	clang++ $(CFLAGS) -c triops.cpp

triopgenerator.o: ppix.cpp
	clang++ $(CFLAGS) -c triopgenerator.cpp

daggenerator.o: ppix.cpp
	clang++ $(CFLAGS) -c daggenerator.cpp

clean: 
	rm -f *.o
	rm ppix
	rm img*.bmp
