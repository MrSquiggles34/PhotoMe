#pragma once

#include "ofFbo.h"
#include "ofMain.h"

class Selection {
public:
	Selection() = default;

	void Allocate(int width, int height);

	void Clear();

	void SetRectangle(int x, int y, int width, int height);

	bool HasSelection() const;

	ofFbo & GetMask();
	const ofFbo & GetMask() const;

	bool IsActive() const;

	int GetLeft() const;
	int GetTop() const;
	int GetRight() const;
	int GetBottom() const;

private:
	ofFbo mask;

	bool active = false;

	int left = 0;
	int top = 0;
	int right = 0;
	int bottom = 0;
};
