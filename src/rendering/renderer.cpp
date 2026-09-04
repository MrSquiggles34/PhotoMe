#include "renderer.h"
#include "../document/document.h"
#include "../document/layer.h"
#include "camera2D.h"
#include "layerEffect.h"

void Renderer::Draw(
	Document & document,
	const Camera2D & camera) {
	CompositeLayers(document);

	ofPushStyle();
	ofPushMatrix();

	ApplyCamera(camera);

	compositeFbo.draw(0, 0);

	ofPopMatrix();
	ofPopStyle();
}

void Renderer::CompositeLayers(Document & document) {
	if (!document.HasCanvas())
		return;

	auto & layers = document.GetLayers();

	const int width = document.GetWidth();
	const int height = document.GetHeight();

	// --------------------------------------------------
	// Allocate composite FBO
	// --------------------------------------------------

	if (compositeFbo.getWidth() != width || compositeFbo.getHeight() != height) {
		AllocateFbo(
			compositeFbo,
			width,
			height);
	}

	// --------------------------------------------------
	// Allocate effect FBO
	// --------------------------------------------------

	EnsureEffectFbos(
		width,
		height);

	// --------------------------------------------------
	// Clear composite once
	// --------------------------------------------------

	compositeFbo.begin();

	ofClear(
		0,
		0,
		0,
		0);

	compositeFbo.end();

	// --------------------------------------------------
	// Render each layer independently
	// --------------------------------------------------

	for (Layer & layer : layers) {
		RenderLayer(layer, compositeFbo);
	}
}

void Renderer::ApplyCamera(const Camera2D & camera) {
	ofTranslate(camera.position.x, camera.position.y);
	ofScale(camera.zoom, camera.zoom);
}

void Renderer::EnsureEffectFbos(int width, int height) {

	if (
		effectFbo.getWidth() == width && effectFbo.getHeight() == height) {

		return;
	}

	AllocateFbo(
		effectFbo,
		width,
		height);
}

void Renderer::EnsureMergeFbo(
	int width,
	int height) {
	if (mergeFbo.getWidth() == width && mergeFbo.getHeight() == height) {
		return;
	}

	AllocateFbo(
		mergeFbo,
		width,
		height);
}

void Renderer::AllocateFbo(
	ofFbo & fbo,
	int width,
	int height) {
	ofFbo::Settings settings;

	settings.width = width;
	settings.height = height;
	settings.internalformat = GL_RGBA8;

	settings.useDepth = false;
	settings.useStencil = false;

	settings.textureTarget = GL_TEXTURE_2D;
	settings.numSamples = 0;

	fbo.allocate(settings);

	fbo.begin();
	ofClear(0, 0, 0, 0);
	fbo.end();
}

void Renderer::RenderLayer(
	Layer & layer,
	ofFbo & destination) {
	if (!layer.IsVisible())
		return;

	const int width = destination.getWidth();
	const int height = destination.getHeight();
	const glm::vec2 & position = layer.GetPosition();

	LayerEffect * effect = layer.GetEffect();

	if (!effect) {
		destination.begin();

		ofPushStyle();

		ofSetColor(
			255,
			255,
			255,
			static_cast<unsigned char>(
				255.0f * layer.GetOpacity()));

		layer.GetFbo().draw(
			position.x,
			position.y,
			width,
			height);

		ofPopStyle();

		destination.end();

		return;
	}

	effect->Apply(
		layer.GetFbo(),
		effectFbo);

	destination.begin();

	ofPushStyle();

	ofSetColor(
		255,
		255,
		255,
		static_cast<unsigned char>(
			255.0f * layer.GetOpacity()));

	effectFbo.draw(
		position.x,
		position.y,
		width,
		height);

	ofPopStyle();

	destination.end();
}
