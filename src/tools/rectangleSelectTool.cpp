#include "rectangleSelectTool.h"

#include "../core/editor.h"
#include "../document/selection.h"
#include "../rendering/camera2D.h"

RectangleSelectTool::RectangleSelectTool(Editor * editor) : Tool(editor) { }

void RectangleSelectTool::MousePressed(int x, int y) {
	isSelecting = true;

	startPoint = editor->GetCamera()->ScreenToWorld(glm::vec2(x, y));

	currentPoint = startPoint;
}

void RectangleSelectTool::MouseDragged(int x, int y) {
	if (!isSelecting)
		return;

	currentPoint = editor->GetCamera()->ScreenToWorld(glm::vec2(x, y));

	float left = std::min(startPoint.x, currentPoint.x);
	float top = std::min(startPoint.y, currentPoint.y);

	float width = std::abs(currentPoint.x - startPoint.x);
	float height = std::abs(currentPoint.y - startPoint.y);

	Selection * selection = editor->GetSelection();

	if (selection) {
		selection->SetRectangle(
			static_cast<int>(left),
			static_cast<int>(top),
			static_cast<int>(width),
			static_cast<int>(height));
	}
}

void RectangleSelectTool::MouseReleased(int x, int y) {
	if (!isSelecting)
		return;

	currentPoint = editor->GetCamera()->ScreenToWorld(glm::vec2(x, y));

	float left = std::min(startPoint.x, currentPoint.x);
	float top = std::min(startPoint.y, currentPoint.y);

	float width = std::abs(currentPoint.x - startPoint.x);
	float height = std::abs(currentPoint.y - startPoint.y);

	Selection * selection = editor->GetSelection();

	if (selection) {
		selection->SetRectangle(
			static_cast<int>(left),
			static_cast<int>(top),
			static_cast<int>(width),
			static_cast<int>(height));
	}

	isSelecting = false;
}

