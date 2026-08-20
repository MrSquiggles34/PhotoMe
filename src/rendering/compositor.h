#pragma once

#include "ofMain.h"

class Layer;

class Compositor {
public:
	Compositor();

	bool Setup(int width, int height);

	void Paint(Layer & layer, float x, float y, float radius, const ofColor & color, const ofFbo * selectionMask = nullptr);

	void Erase(Layer & layer, float x, float y, float radius, const ofFbo * selectionMask = nullptr);

private:

	void DrawBrush(float x, float y, float radius, const ofColor & color);
	void Composite(Layer & layer, const ofFbo * selectionMask, ofShader & shader);

private:
	ofShader paintShader;
	ofShader eraseShader;

	ofFbo brushFbo;
	ofFbo compositeFbo;

	int width = 0;
	int height = 0;
};
