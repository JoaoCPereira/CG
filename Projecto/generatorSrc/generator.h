#ifndef __GENERATOR__
#define __GENERATOR__

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define _USE_MATH_DEFINES
#include <math.h>
#include <vector>

using namespace std;

typedef struct point{
    float x;
    float y;
    float z;
}*Point;


/**
@file generator.hpp

*/

/**
\brief Write the dots to draw a sphere
*@param radius Size of radius
*@param slices Number of slices
*@param stacks Number of stacks
*@param file_name File name where to write the dots
*@return A file with the dots to draw a sphere with the given parameters
*/
void print_sphere(float radius,float slices,float stacks,char* file_name);

/**
*@brief Write the dots to draw a plane
*@param x The size
*@param file_name File name where to write the dots
*@return A file with the dots to draw a plane with the given parameters
*/
void print_plane(float x,char* file_name);

/**
*@brief Write the dots to draw a box
 * @param x The X dimension
 * @param x The Y dimension
 * @param x The Z dimension
 * @param divisions Number of divisions
 * @param file_name File name where to write the dots
*@return A file with the dots to draw a box with the given parameters
*/
void print_box(float x,float y,float z,float divisions,char* file_name);

/**
*@brief Write the dots to draw a cone
* @param radius The radius of the base
 * @param height The cone height
 * @param slices The number of slices
 * @param stacks The number of stacks
 * @param file_name File name where to write the dots
*@return A file with the dots to draw a cone with the given parameters
*/
void print_cone(float radius,float height,float slices,float stacks,char* file_name);


void process_patch(char *filename, int tesselation);

#endif