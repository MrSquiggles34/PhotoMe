#pragma once

#include "shaderEffect.h"
#include "shaderParameter.h"

#include <vector>

class CustomShaderEffect : public ShaderEffect {

	public:
		CustomShaderEffect();

		bool Setup(const std::string & fragmentPath);

		void Apply(
			ofFbo & source,
			ofFbo & destination) override;

		void ReloadShader() override;

		const std::vector<ShaderParameter> & GetParameters() const;

		bool SetFloatParameter(
			const std::string & name,
			float value);

		bool SetIntParameter(
			const std::string & name,
			int value);

		bool SetBoolParameter(
			const std::string & name,
			bool value);

		bool SetColorParameter(
			const std::string & name,
			const glm::vec4 & value);

		bool SetFloatArrayParameter(
			const std::string & name,
			const std::vector<float> & value);

	private:
		bool LoadParameters(const std::string & jsonPath);

		std::vector<ShaderParameter> parameters;
};
