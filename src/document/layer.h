#pragma once

#include "ofMain.h"
#include "layerID.h"

class Layer {
public:
	Layer(int width, int height, LayerID id);


	bool IsVisible() const;
	

	// Sets
	void SetImage(const ofImage& img);
	void SetVisible(bool visible);
	void SetOpacity(float opacity);
	void SetName(const std::string & name);


	// Gets
	ofFbo & GetFbo();
	const ofFbo & GetFbo() const;

	ofTexture & GetTexture();
	const ofTexture & GetTexture() const;

	float GetOpacity() const;
	const std::string & GetName() const;
	LayerID GetID() const;
	

private:

	std::string name = "Layer";

	bool visible = true;
	float opacity = 1.0f;

	ofFbo fbo;
	ofTexture texture;

	bool hasImage = false;

	LayerID id;
};
