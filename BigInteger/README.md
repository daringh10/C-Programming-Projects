/*********************************************************************************
* Darin Gharib, dgharib
* 2021 Spring CSE101 PA#5
* README.md
* README
*********************************************************************************/

#Purpose

Since c++ doesn't  have a big number libary, we are restricted to our CPU's computing capabilities.
This program bridges that gap and enables any large number to be computed by 
way of storing groups of digits into items in a linked list, and printing them
as a string. The BigInteger ADT takes a string argument and stores it appropriatley 
in its List ADT named digits that can perfrom all basic mathematical calculations except
division.


#Build

Important Commands:

$make Arithmetic.cpp:
	Executes neccesary makefile commands to create the Arithmetic binary file.


#Run

-Run the executable "Arithmetic" with two program arguments:
	./Arithmetic <infile> <outfile>

-Make sure to format Arithmetic so that line 1 and 3 contain a number,
and line 2 does not.


#Files

Arithmetic.cpp 
BigInteger.cpp 
BigInteger.h 
BigIntegerTest.cpp
List.cpp 
List.h 
ListTest.cpp 
Makefile 
README


#Errors

As of final build, the only error is in regards to concat. The concat function doesnt return a list with the new position of the cursor at index 0. No fixes as of yet.
 		

