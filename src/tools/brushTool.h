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


	// Brush settings
	void SetMode(BrushMode mode);
	void ToggleMode();

	void SetSize(float size);
	void SetHardness(float hardness);
	void SetColor(const ofColor & color);

	float GetSize() const;
	float GetHardness() const;
	const ofColor & GetColor() const;

private:
	bool isDrawing = false;

	BrushMode mode = BrushMode::Paint;

	float size = 15.0f;
	float hardness = 1.0f;
	ofColor color = ofColor::black;

	ofVec2f lastPoint;
};
