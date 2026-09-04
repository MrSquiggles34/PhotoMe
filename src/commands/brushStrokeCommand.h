#pragma once

#include "command.h"

#include "../document/brushPoint.h"
#include "../document/layer.h"
#include "../tools/brushMode.h"
#include "../rendering/compositor.h"

#include "ofColor.h"
#include "ofFbo.h"


#include <vector>

class Selection;

class BrushStrokeCommand : public Command {
public:
	BrushStrokeCommand(
		Layer * layer,
		Compositor * compositor,
		const std::vector<BrushPoint> & points,
		float radius,
		float hardness,
		const ofColor & color,
		BrushMode mode,
		const Selection * selection);

	void Execute() override;
	void Undo() override;

	void Begin();
	void AddPoint(const BrushPoint & point);
	void Finish();

private:
	Layer * layer;
	Compositor * compositor;

	std::vector<BrushPoint> points;

	float radius;
	ofColor color;
	float hardness;

	BrushMode mode;

	ofFbo before;
	ofFbo selectionMask;


};
