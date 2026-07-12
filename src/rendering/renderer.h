#pragma once

#include "ofMain.h"
#include "camera2D.h"

class Document;

class Renderer {
	public:

		void Draw(const Document& document, const Camera2D& camera);
};
