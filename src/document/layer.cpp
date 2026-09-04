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

	fbo.end();

	ofLogNotice()
		<< "SET IMAGE LAYER "
		<< id
		<< " FBO TEX ID = "
		<< fbo.getTexture()
			   .getTextureData()
			   .textureID;

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


void Layer::SetEffect(std::unique_ptr<LayerEffect> newEffect) {
	effect = std::move(newEffect);
}

void Layer::SetPosition(const glm::vec2 & position) {
	this->position = position;
}

void Layer::SetPosition(float x, float y) {
	position = glm::vec2(x, y);
}

void Layer::Resize(int width, int height) {
	if (width <= 0 || height <= 0)
		return;

	if (fbo.getWidth() == width && fbo.getHeight() == height) {
		return;
	}

	ofFbo newFbo;

	ofFbo::Settings settings;

	settings.width = width;
	settings.height = height;
	settings.internalformat = GL_RGBA8;
	settings.useDepth = false;
	settings.useStencil = false;
	settings.textureTarget = GL_TEXTURE_2D;
	settings.numSamples = 0;

	newFbo.allocate(settings);

	newFbo.begin();

	ofClear(0, 0, 0, 0);

	ofSetColor(255);

	fbo.draw(
		0,
		0,
		fbo.getWidth(),
		fbo.getHeight());

	newFbo.end();

	fbo = std::move(newFbo);
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

void Layer::RemoveEffect() {
	effect.reset();
}

LayerEffect * Layer::GetEffect() {
	return effect.get();
}

const LayerEffect * Layer::GetEffect() const {
	return effect.get();
}

const glm::vec2 & Layer::GetPosition() const {
	return position;
}
