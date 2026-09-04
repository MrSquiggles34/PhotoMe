#pragma once

#include "layerEffect.h"
#include "ofMain.h"

class ShaderEffect : public LayerEffect {

public:
	ShaderEffect();
	virtual ~ShaderEffect() = default;

	void ReloadShader() override;

	bool Load(
		const std::string & vertexPath,
		const std::string & fragmentPath);

	bool Reload();

	bool IsLoaded() const;

	ofShader & GetShader();

protected:
	std::string vertexPath;
	std::string fragmentPath;

	ofShader shader;

	bool loaded = false;
};
