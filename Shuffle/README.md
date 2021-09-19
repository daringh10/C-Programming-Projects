#Purpose

Builds a doubley linked list ADT in c++ with a cursor placed
in between linked list items rather than on them. The shuffle algorithm is run as many times as needed to return a list of integers to their original ordering.


#Build

Important Commands:

$make Shuffle:
	Executes neccesary makefile commands to create the Shuffle binary file.


#Run

-Run the executable "Lex" with two program arguments:
	./Shuffle <integer>


#Files

Shuffle.cpp
List.cpp
List.h
ListTest.cpp
MakeFile
README.md


#Errors

As of final build, the only error is in regards to concat. The concat function doesnt return a list with the new position of the cursor at index 0. No fixes as of yet.
 		

