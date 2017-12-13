#include "sphere.h"

//constructor given  center, radius, and material
sphere::sphere(glm::vec3 c, float r, int m, scene* s) : rtObject(s)
{
	center = c;
	radius = r;
	matIndex = m;
	myScene = s;
}

float sphere::testIntersection(glm::vec3 eye, glm::vec3 dir)
{
	//see the book for a description of how to use the quadratic rule to solve
	//for the intersection(s) of a line and a sphere, implement it here and
	//return the minimum positive distance or 9999999 if none
	//get the location
	glm::vec3 L = eye - center;

	//solve for the equation
	float tca = glm::dot(L, dir);
	float d2 = glm::dot(L, L) - tca * tca;
	
	//check if it has root
	if (d2 > radius || d2 < 0.00001f) return 9999999;
	float thc = sqrt(radius - d2);

	//find two roots
	float t0 = tca - thc;
	float t1 = tca + thc;

	//get the base vectors
	float a = glm::dot(dir, dir);
	float b = 2 * glm::dot(dir, L);
	float c = glm::dot(L, L) - radius;
	float delta = b * b - 4 * a * c;
	float x0 = (-b + sqrt(delta)) / 2 / a;
	float x1 = (-b - sqrt(delta)) / 2 / a;

	//if it is too small
	if (abs(x0) < 0.001f && x1 < 0)
		return testIntersection(eye + glm::normalize(L)*0.002f, dir);

	//if it is not on the right side
	if (x0 > 0 && abs(x1) < 0.001f) return 9999999;

	//if it does not intersect
	if (x0 < 0 && x1 < 0) return 9999999;
	return fminf(x0, x1);
}

glm::vec3 sphere::getNormal(glm::vec3 eye, glm::vec3 dir)
{
	//once you can test for intersection,
	//simply add (distance * view direction) to the eye location to get surface location,
	//then subtract the center location of the sphere to get the normal out from the sphere
	glm::vec3 normal;

	float distance = testIntersection(eye, dir);

	normal = eye + distance*dir - center;

	//dont forget to normalize
	normal = glm::normalize(normal);
	
	return normal;
}

glm::vec2 sphere::getTextureCoords(glm::vec3 eye, glm::vec3 dir)
{
	//find the normal as in getNormal

	//use these to find spherical coordinates
	glm::vec3 x(1, 0, 0);
	glm::vec3 y(0, 1, 0);
	glm::vec3 z(0, 0, 1);

	const float PI = 3.1415926;
	
	//find phi using normal and z
	glm::vec3 n = getNormal(eye, dir);
	float phi = acos(glm::dot(z, n));
	
	//find theta using the n and x
	float theta = acos(glm::dot(x, n));

	//if x-y projection is in quadrant 3 or 4, then theta=2*PI-theta
	if (phi > PI && phi < 2*PI)
		phi = 2 * PI - phi;

	if (glm::dot(y, n) < 0) theta = 2*PI - theta;

	//return coordinates scaled to be between 0 and 1
	glm::vec2 coords(theta / PI*0.49, phi / PI);

	return coords;
}