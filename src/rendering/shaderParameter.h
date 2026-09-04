#pragma once

#include <glm/vec4.hpp>
#include <string>

enum class ShaderParameterType {
	Float,
	Int,
	Bool,
	Color,
	FloatArray
};

struct ShaderParameter {
	std::string name;
	std::string label;

	ShaderParameterType type;

	float floatValue = 0.0f;
	int intValue = 0;
	bool boolValue = false;
	glm::vec4 colorValue = glm::vec4(1.0f);

	std::vector<float> floatArrayValue;

	float min = 0.0f;
	float max = 1.0f;
};
