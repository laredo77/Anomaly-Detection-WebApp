/*
 * minCircle.cpp
 *
 * Author: 311547087, Itamar Laredo
 */
#include "minCircle.h"
#include <math.h>
#include <vector>
using namespace std;

/*
 * A function that returns the distance between two given points
 */
float dist(Point a, Point b){
	float x2=(a.x-b.x)*(a.x-b.x);
	float y2=(a.y-b.y)*(a.y-b.y);
	return sqrt(x2+y2);
}
/*
 * Calculating the middle point between those two points
 * and make it the center point. In addition the radius
 * will be the half distance between the points.
 */
Circle from2points(Point a,Point b){
	float x=(a.x+b.x)/2;
	float y=(a.y+b.y)/2;
	float r=dist(a,b)/2;
	return Circle(Point(x,y),r);
}
/*
 * Base on the equation x^2 + y^2 + 2*g*x + 2*f*y + c = 0
 * we can calculate circle from 3 given points.
 * initialize the equation with 3 points will give 3 equations to solve.
 * (1): x1^2 + y1^2 +2gx1 + 2fy1 + c = 0
 * (2): x2^2 + y2^2 +2gx2 + 2fy2 + c = 0
 * (3): x3^2 + y3^2 +2gx3 + 2fy3 + c = 0
 */
Circle from3Points(Point a, Point b, Point c){
	// find the circumcenter of the triangle a,b,c
	Point mAB((a.x+b.x)/2 , (a.y+b.y)/2); // mid point of line AB
	float slopAB = (b.y - a.y) / (b.x - a.x); // the slop of AB
	float pSlopAB = - 1/slopAB; // the perpendicular slop of AB
	// pSlop equation is:
	// y - mAB.y = pSlopAB * (x - mAB.x) ==> y = pSlopAB * (x - mAB.x) + mAB.y

	Point mBC((b.x+c.x)/2 , (b.y+c.y)/2); // mid point of line BC
	float slopBC = (c.y - b.y) / (c.x - b.x); // the slop of BC
	float pSlopBC = - 1/slopBC; // the perpendicular slop of BC
	// pSlop equation is:
	// y - mBC.y = pSlopBC * (x - mBC.x) ==> y = pSlopBC * (x - mBC.x) + mBC.y

	/*
	pSlopAB * (x - mAB.x) + mAB.y = pSlopBC * (x - mBC.x) + mBC.y
	pSlopAB*x - pSlopAB*mAB.x + mAB.y = pSlopBC*x - pSlopBC*mBC.x + mBC.y

	x*(pSlopAB - pSlopBC) = - pSlopBC*mBC.x + mBC.y + pSlopAB*mAB.x - mAB.y
	x = (- pSlopBC*mBC.x + mBC.y + pSlopAB*mAB.x - mAB.y) / (pSlopAB - pSlopBC);

	*/

	float x = (- pSlopBC*mBC.x + mBC.y + pSlopAB*mAB.x - mAB.y) / (pSlopAB - pSlopBC);
	float y = pSlopAB * (x - mAB.x) + mAB.y;
	Point center(x,y);
	float R=dist(center,a);

	return Circle(center,R);
}

/*
 * return circle center from two points
 */
//Point circumcenter(Point b, Point c) {
//    float B = b.x * b.x + b.y * b.y;
//    float C = c.x * c.x + c.y * c.y;
//    float D = b.x * c.y - b.y * c.x;
//
//    return { (c.y * B - b.y * C) / (2 * D), (b.x * C - c.x * B) / (2 * D) };
//}

Circle trivial(vector<Point>& P){
	if(P.size()==0)
		return Circle(Point(0,0),0);
	else if(P.size()==1)
		return Circle(P[0],0);
	else if (P.size()==2)
		return from2points(P[0],P[1]);

	// maybe 2 of the points define a small circle that contains the 3ed point
	Circle c=from2points(P[0],P[1]);
	if(dist(P[2],c.center)<=c.radius)
		return c;
	c=from2points(P[0],P[2]);
	if(dist(P[1],c.center)<=c.radius)
		return c;
	c=from2points(P[1],P[2]);
	if(dist(P[0],c.center)<=c.radius)
		return c;
	// else find the unique circle from 3 points
	return from3Points(P[0],P[1],P[2]);
}
/* This algorithm works recursively when its stop condition is if there are no more
 * given points or if the size of the vector of boundary points 3.
 * If we enter the stop condition we call function get_circle otherwise return the call
 * to this function with smaller input (without the last point in the array).
 */
Circle welzl(Point** P,vector<Point> R, size_t n){
	if(n==0 || R.size()==3){
		return trivial(R);
	}

	// remove random point p
	// swap is more efficient than remove
	//srand (time(NULL));
	int i=rand()%n;
	Point p(P[i]->x,P[i]->y);
	swap(P[i],P[n-1]);

	Circle c=welzl(P,R,n-1);

	if(dist(p,c.center)<=c.radius)
		return c;

	R.push_back(p);

	return welzl(P,R,n-1);
}
/*
 * Using Welzl algorithm recursively to find the smallest circle
 * that contains all the points. Any point that is outside a given circle
 * will be on the boundary of the new circle that includes it.
 * This process continue recursively for all given points.
 */
Circle findMinCircle(Point** points,size_t size){
	return welzl(points,{},size);
}
