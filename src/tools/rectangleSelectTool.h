#pragma once

#include "tool.h"
#include "ofVec2f.h"

class RectangleSelectTool : public Tool {
public:
	explicit RectangleSelectTool(Editor * editor);

	void MousePressed(int x, int y) override;
	void MouseDragged(int x, int y) override;
	void MouseReleased(int x, int y) override;

private:
	bool isSelecting = false;

	ofVec2f startPoint;
	ofVec2f currentPoint;
};
