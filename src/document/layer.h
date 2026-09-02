#pragma once

#include "../rendering/layerEffect.h"
#include "layerID.h"
#include "ofMain.h"

class ColorMixerEffect;

class Layer {
public:
	Layer(int width, int height, LayerID id);

	// Make Layer explicitly movable
	Layer(const Layer &) = delete;
	Layer & operator=(const Layer &) = delete;

	Layer(Layer &&) noexcept = default;
	Layer & operator=(Layer &&) noexcept = default;

	bool IsVisible() const;

	// --------------------------------------------------
	// Effect
	// --------------------------------------------------

	void SetEffect(std::unique_ptr<LayerEffect> effect);
	void RemoveEffect();

	LayerEffect * GetEffect();
	const LayerEffect * GetEffect() const;

	template <typename T>
	T * GetEffect() {
		if (!effect)
			return nullptr;

		return dynamic_cast<T *>(effect.get());
	}

	template <typename T>
	const T * GetEffect() const {
		if (!effect)
			return nullptr;

		return dynamic_cast<const T *>(effect.get());
	}

	// --------------------------------------------------
	// Image
	// --------------------------------------------------

	void SetImage(const ofImage & img);
	void SetVisible(bool visible);
	void SetOpacity(float opacity);
	void SetName(const std::string & name);

	// --------------------------------------------------
	// Gets
	// --------------------------------------------------

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

	// Exactly ONE effect per layer.
	std::unique_ptr<LayerEffect> effect;
};
