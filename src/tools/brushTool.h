#pragma once

#include <vector>

#include "tool.h"
#include "ofVec2f.h"
#include "ofColor.h"
#include "../document/brushPoint.h"
#include "brushMode.h"

class BrushTool : public Tool {
public:
	explicit BrushTool(Editor * editor);

	void MousePressed(int x, int y) override;
	void MouseDragged(int x, int y) override;
	void MouseReleased(int x, int y) override;

	void SetMode(BrushMode mode);
	void ToggleMode();

private:
	bool isDrawing = false;

	BrushMode mode = BrushMode::Paint;

	float radius = 15.0f;
	ofColor color = ofColor::black;

	ofVec2f lastPoint;
};
