#include "shaderEffect.h"

ShaderEffect::ShaderEffect() {
}

bool ShaderEffect::Load(
	const std::string & vertexPath,
	const std::string & fragmentPath) {

	ofLogNotice()
		<< "ShaderEffect this: "
		<< static_cast<const void *>(this);

	ofLogNotice()
		<< "LOAD 1";

	this->vertexPath = vertexPath;
	this->fragmentPath = fragmentPath;

	ofLogNotice()
		<< "Vertex path: "
		<< this->vertexPath;

	ofLogNotice()
		<< "Fragment path: "
		<< this->fragmentPath;

	ofLogNotice() << "LOAD 2";

	bool result = shader.load(
		this->vertexPath,
		this->fragmentPath);

	ofLogNotice() << "LOAD 3";

	if (!result) {

		ofLogError()
			<< "Shader load failed.";

		loaded = false;

		return false;
	}

	loaded = true;

	ofLogNotice()
		<< "Shader loaded successfully.";

	return true;
}

bool ShaderEffect::Reload() {

	if (vertexPath.empty() || fragmentPath.empty()) {
		return false;
	}

	ofShader newShader;

	if (!newShader.load(vertexPath, fragmentPath)) {

		ofLogError()
			<< "Shader reload failed: "
			<< fragmentPath;

		// Keep the old shader alive.
		return false;
	}

	shader = std::move(newShader);

	loaded = true;

	ofLogNotice()
		<< "Reloaded shader: "
		<< fragmentPath;

	return true;
}

void ShaderEffect::ReloadShader() {
	Reload();
}

bool ShaderEffect::IsLoaded() const {
	return loaded;
}

ofShader & ShaderEffect::GetShader() {
	return shader;
}
