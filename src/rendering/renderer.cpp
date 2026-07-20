#include "renderer.h"
#include "../document/document.h"
#include "../document/layer.h"
#include "camera2D.h"

void Renderer::Draw(const Document& document, const Camera2D& camera) {

	CompositeLayers(document);

	ofPushMatrix();

	ApplyCamera(camera);

	compositeFbo.draw(0, 0);

	ofPopMatrix();
}

void Renderer::CompositeLayers(const Document & document) {
	if (!document.HasCanvas())
		return;

	const auto& layers = document.GetLayers();
	int width = document.GetWidth();
	int height = document.GetHeight();

	if (compositeFbo.getWidth() != width || compositeFbo.getHeight() != height) {
		compositeFbo.allocate(width, height, GL_RGBA8);
	}

	compositeFbo.begin();
	ofClear(0, 0, 0, 0);
	for (const Layer& layer : layers) {

		if (!layer.IsVisible())
			continue;

		ofSetColor(255, 255 * layer.GetOpacity());
		layer.GetFbo().draw(0, 0);
	}

	ofSetColor(255);
	compositeFbo.end();
}

void Renderer::ApplyCamera(const Camera2D & camera) {
	ofTranslate(camera.position);
	ofScale(camera.zoom, camera.zoom);
}
