#pragma once

#include "ofMain.h"
#include "camera2D.h"
#include "../document/layerID.h"

class Document;
class Layer;

class Renderer {
	public:

		void Draw(Document& document, const Camera2D& camera);
		void RenderLayer(Layer & layer, ofFbo & destination);

	private:
		void CompositeLayers(Document & document);
		void ApplyCamera(const Camera2D & camera);

		void EnsureEffectFbos(int width, int height);
		void EnsureMergeFbo(int width, int height);
		void AllocateFbo(ofFbo & fbo, int width, int height);


		ofFbo compositeFbo;
		ofFbo effectFbo;
		ofFbo mergeFbo;
};
