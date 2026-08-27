#include "layer.h"
#include "../rendering/colorMixerEffect.h"

Layer::Layer(int width, int height, LayerID id)
	: id(id) {
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

bool Layer::IsVisible() const {
	return visible;
}

// Sets
void Layer::SetImage(const ofImage & image) {
	fbo.begin();

	ofClear(0, 0, 0, 0);

	ofSetColor(255);

	image.draw(
		0,
		0,
		image.getWidth(),
		image.getHeight());

	ofSetColor(255);

	fbo.end();

	hasImage = true;
}

void Layer::SetVisible(bool visible) {
	this->visible = visible;
}

void Layer::SetOpacity(float opacity) {
	this->opacity = ofClamp(opacity, 0.0f, 1.0f);
}

void Layer::SetName(const string & name) {
	this->name = name;
}

// Gets
ofFbo& Layer::GetFbo() {
	return fbo;
}

const ofFbo & Layer::GetFbo() const {
	return fbo;
}

ofTexture& Layer::GetTexture() {
	return texture;
}

const ofTexture & Layer::GetTexture() const {
	return texture;
}

float Layer::GetOpacity() const {
	return opacity;
}

const string& Layer::GetName() const {
	return name;
}

LayerID Layer::GetID() const {
	return id;
}

std::vector<std::unique_ptr<LayerEffect>> & Layer::GetEffects() {
	return effects;
}

const std::vector<std::unique_ptr<LayerEffect>> & Layer::GetEffects() const {
	return effects;
}

void Layer::ApplyEffects() {

	if (effects.empty())
		return;

	int width = fbo.getWidth();
	int height = fbo.getHeight();

	if (effectBufferA.getWidth() != width || effectBufferA.getHeight() != height) {

		effectBufferA.allocate(width, height, GL_RGBA8);
		effectBufferB.allocate(width, height, GL_RGBA8);
	}

	// Start with the original layer.
	effectBufferA.begin();
	ofClear(0, 0, 0, 0);
	fbo.draw(0, 0);
	effectBufferA.end();

	ofFbo * source = &effectBufferA;
	ofFbo * destination = &effectBufferB;

	for (auto & effect : effects) {

		if (!effect)
			continue;

		effect->Apply(*source, *destination);

		std::swap(source, destination);
	}

	// Copy final result back into the layer FBO.
	fbo.begin();
	ofClear(0, 0, 0, 0);
	source->draw(0, 0);
	fbo.end();
}

void Layer::AddEffect(std::unique_ptr<LayerEffect> effect) {

	if (!effect)
		return;

	effects.push_back(std::move(effect));
}

void Layer::RemoveEffect(int index) {

	if (index < 0 || index >= static_cast<int>(effects.size()))
		return;

	effects.erase(effects.begin() + index);
}
