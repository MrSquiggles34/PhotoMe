#pragma once

#include "ofMain.h"

struct Camera2D {
	glm::vec2 position { 0.0f, 0.0f };
	float zoom = 1.0f;

	glm::vec2 ScreenToWorld(const glm::vec2& screen) const {
		return (screen - position) / zoom;
	}

	glm::vec2 WorldToScreen(const glm::vec2& world) const {
		return world * zoom + position;
	}
};
