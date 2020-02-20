#ifndef __GENERATOR__
#define __GENERATOR__

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/**
@file generator.h

*/

/**
\brief Write the dots to draw a sphere
@param radius Size of radius
@param slices Number of slices
@param stacks Number of stacks
@param file_name File name where to write the dots
@return A file with the dots to draw a sphere with the given parameters
*/
void print_sphere(float radius,float slices,float stacks,char* file_name);

/**
*@brief 
*
*@return A plane
*/
void print_plane(float ,char*);

/**
*@brief 
*
*@return A box
*/
void print_box(float ,float ,float ,float ,char*);

/**
*@brief 
*
*@return A cone
*/
void print_cone(float ,float ,float ,float ,char*);








#endif