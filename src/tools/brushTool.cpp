#include "brushTool.h"

#include "../core/editor.h"
#include "../rendering/camera2D.h"

BrushTool::BrushTool(Editor * editor) : Tool(editor) { }

void BrushTool::MousePressed(int x, int y) {
	isDrawing = true;

	glm::vec2 world = editor->GetCamera()->ScreenToWorld(glm::vec2(x, y));

	lastPoint = ofVec2f(world.x, world.y);

	float radius = size * 0.5f;
	editor->BeginBrushStroke(lastPoint, radius, color, mode);
}

void BrushTool::MouseDragged(int x, int y) {
	if (!isDrawing)
		return;

	glm::vec2 world = editor->GetCamera()->ScreenToWorld(glm::vec2(x, y));

	ofVec2f current(world.x, world.y);

	float distance = lastPoint.distance(current);

	// Distance between brush dabs.
	float radius = size * 0.5f;
	float spacing = radius * 0.25f;

	if (distance > 0.0f) {
		int steps = std::ceil(distance / spacing);

		for (int i = 1; i <= steps; ++i) {
			float t = static_cast<float>(i) / steps;

			ofVec2f point = lastPoint.getInterpolated(current, t);

			editor->ContinueBrushStroke(point, radius, color, mode);
		}
	}

	lastPoint = current;
}

void BrushTool::MouseReleased(int x, int y) {
	if (!isDrawing)
		return;

	glm::vec2 world = editor->GetCamera()->ScreenToWorld(
	glm::vec2(x, y));
	ofVec2f current(world.x, world.y);

	float distance = lastPoint.distance(current);
	float radius = size * 0.5f;
	float spacing = radius * 0.25f;

	if (distance > 0.0f) {
		int steps = std::ceil(distance / spacing);

		for (int i = 1; i <= steps; ++i) {
			float t = static_cast<float>(i) / steps;

			ofVec2f point = lastPoint.getInterpolated(current, t);

			editor->ContinueBrushStroke(point, radius, color, mode);
		}
	}

	isDrawing = false;

	editor->FinishBrushStroke();
}

void BrushTool::SetMode(BrushMode newMode) {
	mode = newMode;
}

void BrushTool::ToggleMode() {
	if (mode == BrushMode::Paint)
		mode = BrushMode::Erase;
	else
		mode = BrushMode::Paint;
}

void BrushTool::SetSize(float size) {
	this->size = std::max(1.0f, size);
}

void BrushTool::SetHardness(float hardness) {
	this->hardness = ofClamp(
		hardness,
		0.0f,
		1.0f);
}

void BrushTool::SetColor(const ofColor & color) {
	this->color = color;
}

float BrushTool::GetSize() const {
	return size;
}

float BrushTool::GetHardness() const {
	return hardness;
}

const ofColor & BrushTool::GetColor() const {
	return color;
}
