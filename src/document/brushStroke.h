#pragma once
#include "brushPoint.h"
#include "ofColor.h"
#include <vector>

class BrushStroke {
public:
	std::vector<BrushPoint> points;
	float radius;
	ofColor color;
};
