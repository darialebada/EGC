#include <vector>
#include <string>
#include <iostream>

#include "lab_m1/Tema2/Tank.h"

using namespace std;
using namespace m1;

Tank::Tank()
{
}

Tank::~Tank()
{
}

// R_tank = 0.6f
void Tank::Init()
{
	x = 0;
	y = -0.05f;
	z = 0;
	radians_rotation = 0;
	radians_turret = 0;
	num_hits = 0;
	start_firing = time(NULL);
	color_body = glm::vec3(0, 0.1f, 0);
	color_turret = glm::vec3(0, 0.3f, 0);
	color_cannon = glm::vec3(0.6f, 0.6f, 0.6f);
	color_tracks = glm::vec3(0.5f, 0.5f, 0.5f);
}


