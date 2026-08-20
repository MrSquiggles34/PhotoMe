#include "selection.h"

void Selection::Allocate(int width, int height) {
	ofFbo::Settings settings;

	settings.width = width;
	settings.height = height;
	settings.internalformat = GL_RGBA8;

	settings.useDepth = false;
	settings.useStencil = false;

	settings.textureTarget = GL_TEXTURE_2D;

	mask.allocate(settings);

	Clear();
}

void Selection::Clear() {
	if (mask.isAllocated()) {
		mask.begin();
		ofClear(0, 0, 0, 0);
		mask.end();
	}

	active = false;

	left = 0;
	top = 0;
	right = 0;
	bottom = 0;
}

void Selection::SetRectangle(int x, int y, int width, int height) {
	if (!mask.isAllocated())
		return;

	left = x;
	top = y;
	right = x + width;
	bottom = y + height;

	mask.begin();

	ofClear(0, 0, 0, 0);

	ofSetColor(255, 255, 255, 255);
	ofDrawRectangle(x, y, width, height);

	ofSetColor(255);

	mask.end();

	active = true;
}

bool Selection::HasSelection() const {
	return active;
}

ofFbo & Selection::GetMask() {
	return mask;
}

const ofFbo & Selection::GetMask() const {
	return mask;
}


bool Selection::IsActive() const {
	return active;
}

int Selection::GetLeft() const {
	return left;
}

int Selection::GetTop() const {
	return top;
}

int Selection::GetRight() const {
	return right;
}

int Selection::GetBottom() const {
	return bottom;
}
