# Region Query
Computes overlapping area of polygons

## SYNOPSIS:

regionquery [-verbose] <input file> <query file> <output file>

This program supports five different commands: 

FIND_INSIDE - find the number of polygons inside the given window area.

FIND_INTERACT - find the number of polygons interacting with the given window area. (Even point touching counts.)

FIND_NO_OVERLAP - find the number of polygons interacting with a given window that do not overlap with any other polygons interacting with the window. That means you need to use FIND_INTERACT to select the polygons interacting with the window. Among these selected polygons, find the number of polygons that do not overlap other selected polygons.

FIND_AREA_LESS_THAN - find the number of polygons whose areas are less than the given number.

FIND_DENSITY_INSIDE - find the percentage of the given window area that is covered by polygons.

## DIRECTORY:

	bin/	executable binary
	doc/	report, and the webpage about the regionquery problem
	inputs/ input file and query file
	outputs/
	lib/	library
	src/ 	source C++ codes

## HOW TO COMPILE:

Then compile the demo, simply follow the following steps
	make

## HOW TO RUN:
	cd ../bin
	regionquery [-verbose] <input file> <query file> <output file>
	For example,
	regionquery ./../inputs/testin ./../inputs/testin_q ./../inputs/test.out

## SOURCE CODE:

	polygon.h:		Header file. 
	polygon.cpp: 	Bsic implementation of polygon.h and system functions: 
					ex. readPolygons(), respondQuery() etc
	polygon1.cpp:	Problem 1 implementation
	polygon2.cpp:	Problem 2 implementation
	polygon3.cpp:	Problem 3 implementation
	polygon4.cpp:	Problem 4 implementation
	polygon5.cpp:   Problem 5 implementation
	main.cpp:		main function

## OTHER NOTICE:
