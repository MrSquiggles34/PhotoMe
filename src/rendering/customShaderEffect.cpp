#include "customShaderEffect.h"

CustomShaderEffect::CustomShaderEffect() {
}

bool CustomShaderEffect::Setup(
	const std::string & fragmentPath) {

	ofLogNotice() << "CUSTOM SETUP 1";

	if (fragmentPath.empty()) {

		ofLogError()
			<< "Fragment path is empty.";

		return false;
	}

	ofLogNotice() << "CUSTOM SETUP 2";

	const std::string vertexPath = "shaders/customTest.vert";

	ofLogNotice() << "CUSTOM SETUP 3";

	ofLogNotice()
		<< "Fragment path length: "
		<< fragmentPath.length();

	ofLogNotice()
		<< "Vertex path: "
		<< vertexPath;

	ofLogNotice() << "CUSTOM SETUP 4";

	ofLogNotice() << "BEFORE SHADER LOAD";

	if (!Load(
			vertexPath,
			fragmentPath)) {

		ofLogError() << "SHADER LOAD FAILED";
		return false;
	}

	ofLogNotice() << "AFTER SHADER LOAD";

	ofLogNotice() << "BEFORE LOAD PARAMETERS";

	std::string jsonPath = fragmentPath;

	size_t extensionPosition = jsonPath.find_last_of('.');

	if (extensionPosition != std::string::npos) {

		jsonPath = jsonPath.substr(
			0,
			extensionPosition);
	}

	jsonPath += ".json";

	if (!LoadParameters(jsonPath)) {

		ofLogError()
			<< "Failed to load shader parameters: "
			<< jsonPath;

		return false;
	}

	return true;
}

bool CustomShaderEffect::LoadParameters(
	const std::string & jsonPath) {

	ofLogNotice()
		<< "Loading shader JSON: "
		<< jsonPath;

	ofLogNotice()
		<< "Before ofLoadJson";

	ofJson json = ofLoadJson(jsonPath);

	ofLogNotice()
		<< "After ofLoadJson";

	parameters.clear();

	if (!json.contains("parameters")) {

		ofLogWarning()
			<< "Shader JSON contains no parameters: "
			<< jsonPath;

		return true;
	}

	for (const auto & jsonParameter :
		json["parameters"]) {

		ShaderParameter parameter;

		// --------------------------------------------------
		// Basic information
		// --------------------------------------------------

		parameter.name = jsonParameter.value(
			"name",
			"");

		parameter.label = jsonParameter.value(
			"label",
			parameter.name);

		std::string type = jsonParameter.value(
			"type",
			"float");

		// --------------------------------------------------
		// Parameter type
		// --------------------------------------------------

		if (type == "float") {

			parameter.type = ShaderParameterType::Float;

		} else if (type == "int") {

			parameter.type = ShaderParameterType::Int;

		} else if (type == "bool") {

			parameter.type = ShaderParameterType::Bool;

		} else if (type == "color") {

			parameter.type = ShaderParameterType::Color;

		} else if (type == "floatArray") {

			parameter.type = ShaderParameterType::FloatArray;

		} else {

			ofLogError()
				<< "Unknown shader parameter type: "
				<< type;

			return false;
		}

		// --------------------------------------------------
		// Numeric ranges
		// --------------------------------------------------

		parameter.min = jsonParameter.value(
			"min",
			0.0f);

		parameter.max = jsonParameter.value(
			"max",
			1.0f);

		// --------------------------------------------------
		// Default value
		// --------------------------------------------------

		if (parameter.type == ShaderParameterType::Float) {

			parameter.floatValue = jsonParameter.value(
				"default",
				1.0f);
		} else if (
			parameter.type == ShaderParameterType::Int) {

			parameter.intValue = jsonParameter.value(
				"default",
				0);
		} else if (
			parameter.type == ShaderParameterType::Bool) {

			parameter.boolValue = jsonParameter.value(
				"default",
				false);
		} else if (
			parameter.type == ShaderParameterType::Color) {

			if (jsonParameter.contains("default") && jsonParameter["default"].is_array()) {

				const auto & color = jsonParameter["default"];

				if (color.size() >= 4) {

					parameter.colorValue = glm::vec4(
						color[0].get<float>(),
						color[1].get<float>(),
						color[2].get<float>(),
						color[3].get<float>());
				}
			}
		} else if (
			parameter.type == ShaderParameterType::FloatArray) {

			if (jsonParameter.contains("default") && jsonParameter["default"].is_array()) {

				for (const auto & value :
					jsonParameter["default"]) {

					parameter.floatArrayValue.push_back(
						value.get<float>());
				}
			}
		}

		parameters.push_back(
			std::move(parameter));
	}

	// --------------------------------------------------
	// Debug output
	// --------------------------------------------------

	ofLogNotice()
		<< "Loaded "
		<< parameters.size()
		<< " shader parameters.";

	for (const ShaderParameter & parameter :
		parameters) {

		ofLogNotice()
			<< "  Parameter: "
			<< parameter.name
			<< " ("
			<< parameter.label
			<< ")";
	}

	return true;
}

void CustomShaderEffect::Apply(
	ofFbo & source,
	ofFbo & destination) {

	if (!IsLoaded())
		return;

	ofPushStyle();

	destination.begin();

	ofClear(
		0,
		0,
		0,
		0);

	ofSetColor(255);

	shader.begin();

	shader.setUniform2f(
		"uResolution",
		destination.getWidth(),
		destination.getHeight());

	shader.setUniformTexture(
		"uTexture",
		source.getTexture(),
		0);

	// Add custom shader variables
	for (const ShaderParameter & parameter : parameters) {
		switch (parameter.type) {
		case ShaderParameterType::Float:

			shader.setUniform1f(
				parameter.name,
				parameter.floatValue);

			break;

		case ShaderParameterType::Int:

			shader.setUniform1i(
				parameter.name,
				parameter.intValue);

			break;

		case ShaderParameterType::Bool:

			shader.setUniform1i(
				parameter.name,
				parameter.boolValue ? 1 : 0);

			break;

		case ShaderParameterType::Color:

			shader.setUniform4f(
				parameter.name,
				parameter.colorValue.r,
				parameter.colorValue.g,
				parameter.colorValue.b,
				parameter.colorValue.a);

			break;

		case ShaderParameterType::FloatArray:

			shader.setUniform1fv(
				parameter.name,
				parameter.floatArrayValue.data(),
				parameter.floatArrayValue.size());

			break;
		}
	}

	ofDrawRectangle(
		0,
		0,
		destination.getWidth(),
		destination.getHeight());

	shader.end();

	ofSetColor(255);

	destination.end();

	ofPopStyle();
}

void CustomShaderEffect::ReloadShader() {
	Reload();
}

const std::vector<ShaderParameter> &
CustomShaderEffect::GetParameters() const {

	return parameters;
}

bool CustomShaderEffect::SetFloatParameter(
	const std::string & name,
	float value) {
	for (ShaderParameter & parameter : parameters) {
		if (parameter.name != name)
			continue;

		if (parameter.type != ShaderParameterType::Float)
			return false;

		parameter.floatValue = value;
		return true;
	}

	return false;
}

bool CustomShaderEffect::SetIntParameter(
	const std::string & name,
	int value) {
	for (ShaderParameter & parameter : parameters) {
		if (parameter.name != name)
			continue;

		if (parameter.type != ShaderParameterType::Int)
			return false;

		parameter.intValue = value;
		return true;
	}

	return false;
}

bool CustomShaderEffect::SetBoolParameter(
	const std::string & name,
	bool value) {
	for (ShaderParameter & parameter : parameters) {
		if (parameter.name != name)
			continue;

		if (parameter.type != ShaderParameterType::Bool)
			return false;

		parameter.boolValue = value;
		return true;
	}

	return false;
}

bool CustomShaderEffect::SetColorParameter(
	const std::string & name,
	const glm::vec4 & value) {
	for (ShaderParameter & parameter : parameters) {
		if (parameter.name != name)
			continue;

		if (parameter.type != ShaderParameterType::Color)
			return false;

		parameter.colorValue = value;
		return true;
	}

	return false;
}

bool CustomShaderEffect::SetFloatArrayParameter(
	const std::string & name,
	const std::vector<float> & value) {
	for (ShaderParameter & parameter : parameters) {
		if (parameter.name != name)
			continue;

		if (parameter.type != ShaderParameterType::FloatArray)
			return false;

		parameter.floatArrayValue = value;

		return true;
	}

	return false;
}
