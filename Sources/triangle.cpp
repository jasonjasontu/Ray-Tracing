#include "triangle.h"
//Jason edited:
#include <algorithm>

//constructor given  center, radius, and material
triangle::triangle(glm::vec3 p0, glm::vec3 p1, glm::vec3 p2, float tx0, float tx1, float tx2, float ty0, float ty1, float ty2, int m, scene* s) : rtObject(s)
{
	point0 = p0;
	point1 = p1;
	point2 = p2;
	
	texX0 = tx0;
	texX1 = tx1;
	texX2 = tx2;
	texY0 = ty0;
	texY1 = ty1;
	texY2 = ty2;
	matIndex = m;
	myScene = s;
}

float triangle::testIntersection(glm::vec3 eye, glm::vec3 dir){
	const float EPSILON = 0.0001;
	float a, f, u, v;
	glm::vec3 edge1, edge2, h, s, q;

	//get the edges
	edge1 = point1 - point0;
	edge2 = point2 - point0;

	//two vector for directions
	h = glm::cross(dir, edge2);
	a = glm::dot(edge1, h);

	//check for epsilon
	if (a > -EPSILON && a < EPSILON)
		return 9999999;
	//more vector for calculation
	f = 1 / a;
	s = eye - point0;
	u = f * glm::dot(s, h);

	//out of boundary
	if (u < 0.0 || u > 1.0)
		return 9999999;

	//for direction
	q = glm::cross(s, edge1);
	v = f * glm::dot(dir, q);

	//beyond the reach
	if (v < 0.0 || u + v > 1.0)
		return 9999999;

	// compute t to find intersection point
	float t = f * glm::dot(edge2, q);
	if (t > EPSILON) // ray intersection
		return t;
	else //  a line intersection but not a ray intersection.
		return 9999999;
}

glm::vec3 triangle::getNormal(glm::vec3 eye, glm::vec3 dir)
{
	//construct the barycentric coordinates for the plane
	glm::vec3 bary1 = point1 - point0;
	glm::vec3 bary2 = point2 - point0;

	//cross them to get the normal to the plane
	//note that the normal points in the direction given by right-hand rule
	//(this can be important for refraction to know whether you are entering or leaving a material)
	glm::vec3 normal = glm::normalize(glm::cross(bary1, bary2));
	return normal;
}

glm::vec2 triangle::getTextureCoords(glm::vec3 eye, glm::vec3 dir)
{
	//get all the basic vector		
	float d = testIntersection(eye, dir);
	glm::vec3 n = getNormal(eye, dir);
	glm::vec3 p = eye + d*dir;
		
	//get the area size
	float ABC = glm::length(glm::cross(point1 - point0, point2 - point0));
	float PBA = glm::length(glm::cross(point1 - p, point0 - p));
	float PCA = glm::length(glm::cross(point2 - p, point0 - p));

	//find alpha and beta
	float a = PBA / ABC, b = PCA / ABC;

	//find cooresponding texture
	float texX = texX0 + (texX1 - texX0 + texY1 - texY0) *a;
	float texY = texY0 + (texX2 - texX0 + texY2 - texY0) *b;
	
	//get the format
	glm::vec2 coords = glm::vec2(texX, texY);
	return coords;
}