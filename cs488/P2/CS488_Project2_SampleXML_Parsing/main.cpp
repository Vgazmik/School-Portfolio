/*
 * File:   SampleMain.cpp
 * Author: Chihua Ma
 *
 * Date: Oct 5th, 2013
 *
 * Operating System: Windows 7 64-bit
 * Compiler: Visual Studio 2010
 */


#ifdef WIN32
 #define WINDOWS
#elif WIN64
 #define WINDOWS
#endif
#ifdef WINDOWS
  #include <windows.h> 
#endif


#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <iostream>
#include "reader.h"


const char * filename = "project2.xml";  // Opens the XML file based on the entire pathname "filename"

int main(int argc, char** argv) {
    
    printf("file name %s \n", filename);
    openFile(filename);
    parseDatabase();
    printf("Printing out database \n");
    printDatabase();
    
    return(EXIT_SUCCESS);
    
}

