#include "brushTool.h"

#include "../core/editor.h"
#include "../rendering/camera2D.h"

BrushTool::BrushTool(Editor * editor) : Tool(editor) { }

void BrushTool::MousePressed(int x, int y) {
	isDrawing = true;

	glm::vec2 world = editor->GetCamera()->ScreenToWorld(glm::vec2(x, y));

	lastPoint = ofVec2f(world.x, world.y);

	editor->BeginBrushStroke(lastPoint, radius, color, mode);
}

void BrushTool::MouseDragged(int x, int y) {
	if (!isDrawing)
		return;

	glm::vec2 world = editor->GetCamera()->ScreenToWorld(glm::vec2(x, y));

	ofVec2f current(world.x, world.y);

	float distance = lastPoint.distance(current);

	// Distance between brush dabs.
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
