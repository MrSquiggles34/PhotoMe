#pragma once

#include "ofMain.h"
#include "layerID.h"
#include "../rendering/layerEffect.h"

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

		void AddEffect(std::unique_ptr<LayerEffect> effect);
		void RemoveEffect(int index);
		void ApplyEffects();

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

		const std::vector<std::unique_ptr<LayerEffect>> & GetEffects() const;
		std::vector<std::unique_ptr<LayerEffect>> & GetEffects();

		template <typename T>
		T * GetEffect() {
			for (auto & effect : effects) {
				if (T * result = dynamic_cast<T *>(effect.get()))
					return result;
			}

			return nullptr;
		}

		template <typename T>
		const T * GetEffect() const {
			for (const auto & effect : effects) {
				if (const T * result = dynamic_cast<const T *>(effect.get()))
					return result;
			}

			return nullptr;
		}

	private:

		std::string name = "Layer";

		bool visible = true;
		float opacity = 1.0f;

		ofFbo fbo;
		ofTexture texture;

		bool hasImage = false;

		LayerID id;

		std::vector<std::unique_ptr<LayerEffect>> effects;
		ofFbo effectBufferA;
		ofFbo effectBufferB;

};

