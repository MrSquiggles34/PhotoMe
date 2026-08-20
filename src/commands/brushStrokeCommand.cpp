#include "brushStrokeCommand.h"
#include "../document/selection.h"

BrushStrokeCommand::BrushStrokeCommand(Layer * layer, Compositor * compositor, const std::vector<BrushPoint> & points, float radius, const ofColor & color, BrushMode mode, const Selection * selection)
	: layer(layer), compositor(compositor), points(points), radius(radius) , color(color) , mode(mode) {
	if (selection && selection->IsActive()) {

		selectionMask.allocate(selection->GetMask().getWidth(), selection->GetMask().getHeight(), GL_R8);

		selectionMask.begin();

		ofClear(0, 0, 0, 0);

		selection->GetMask().getTexture().draw(0, 0, selectionMask.getWidth(), selectionMask.getHeight());

		selectionMask.end();
	}
}


void BrushStrokeCommand::Execute() {
	if (!layer || !compositor)
		return;

	const ofFbo * mask = nullptr;

	if (selectionMask.isAllocated())
		mask = &selectionMask;

	for (const BrushPoint & point : points) {
		if (mode == BrushMode::Paint) {
			compositor->Paint(*layer, point.position.x, point.position.y, radius, color, mask);
		} else {
			compositor->Erase(*layer, point.position.x, point.position.y, radius, mask);
		}
	}
}

void BrushStrokeCommand::Undo() {
	if (!layer)
		return;

	layer->GetFbo().begin();
	ofClear(0, 0, 0, 0);
	before.draw(0, 0);
	layer->GetFbo().end();
}

void BrushStrokeCommand::Begin() {
	if (!layer)
		return;

	before.allocate(layer->GetFbo().getWidth(), layer->GetFbo().getHeight(), GL_RGBA);

	before.begin();

	ofClear(0, 0, 0, 0);

	layer->GetFbo().draw(0, 0);

	before.end();
}

void BrushStrokeCommand::AddPoint(const BrushPoint & point) {
	points.push_back(point);
}

void BrushStrokeCommand::Finish() {
	// Nothing needed yet.
}
