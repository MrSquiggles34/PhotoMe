#include "renderer.h"
#include "../document/document.h"
#include "../document/layer.h"
#include "camera2D.h"

void Renderer::Draw(const Document& document, const Camera2D& camera) {

	ofPushMatrix();

	ofTranslate(camera.position.x, camera.position.y);
	ofScale(camera.zoom, camera.zoom);

	const auto & layers = document.GetLayers();

	for (const Layer& layer : layers) {

		if (!layer.IsVisible())
			continue;

		ofSetColor(255, 255 * layer.GetOpacity());
		layer.GetFbo().draw(0, 0);
	}

	ofSetColor(255);

	ofPopMatrix();
}
