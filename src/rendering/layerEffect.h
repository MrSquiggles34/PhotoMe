#pragma once

#include "ofMain.h"

class LayerEffect {
public:
	virtual ~LayerEffect() = default;

	virtual void Apply(ofFbo & source, ofFbo & destination) = 0;

	virtual void ReloadShader() = 0;
};
