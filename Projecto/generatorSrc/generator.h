#ifndef __GENERATOR__
#define __GENERATOR__
#include <stdlib.h>
#include <string.h>
#define _USE_MATH_DEFINES
#include <math.h>
#include <vector>

using namespace std;

struct point{
	float x;
	float y;
	float z;
};

typedef struct point Point;


/**
@file generator.h

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

void crossProduct(Point *A, Point *B, Point *R);
void normalize(float *a);
float length(Point *a);

void evalBezierCurve(const Point *P, const float &t, Point *res);
void evalBezierPatch(const Point *controlPoints, const float &u, const float &v , Point *res);
void dUBezier(const Point *controlPoints, const float &u, const float &v, Point *du);
void dVBezier(const Point *controlPoints, const float &u, const float &v, Point *dv);
void calculate_surface(int tesselation);
/**
*@brief Process Bezier patches from a file
* @param filename File name where to read the patches
* @param tesselation Tesselation level
*@return A file with the dots to draw the teapot surfaces
*/
void process_patch(char *filename, int tesselation);

#endif