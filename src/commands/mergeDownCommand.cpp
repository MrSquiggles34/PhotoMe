#include "mergeDownCommand.h"

#include "../rendering/renderer.h"
#include "../document/document.h"
#include "../document/layer.h"


MergeDownCommand::MergeDownCommand(
	Document & document,
	Renderer & renderer,
	LayerID layerID)
	: document(document)
	, renderer(renderer)
	, layerID(layerID) {
}

static void AllocateMergeFbo(
	ofFbo & fbo,
	int width,
	int height) {
	ofFbo::Settings settings;

	settings.width = width;
	settings.height = height;
	settings.internalformat = GL_RGBA8;

	settings.useDepth = false;
	settings.useStencil = false;

	settings.textureTarget = GL_TEXTURE_2D;
	settings.numSamples = 0;

	fbo.allocate(settings);

	fbo.begin();
	ofClear(0, 0, 0, 0);
	fbo.end();
}

void MergeDownCommand::Execute() {
	// --------------------------------------------------
	// Find the upper layer.
	// --------------------------------------------------

	size_t upperIndex = document.GetLayerIndexByID(layerID);

	if (upperIndex == document.GetLayerCount())
		return;

	// Cannot merge the bottom layer down.
	if (upperIndex == 0)
		return;

	Layer * upper = document.FindLayerByID(layerID);

	if (!upper)
		return;

	Layer * lower = &document.GetLayers()[upperIndex - 1];

	// --------------------------------------------------
	// Save information needed for Undo.
	// --------------------------------------------------

	if (!hasBackup) {
		originalUpperIndex = upperIndex;

		originalActiveLayerID = document.GetActiveLayerID();

		hasBackup = true;
	}

	// --------------------------------------------------
	// Prepare the merge FBO.
	// --------------------------------------------------

	const int width = document.GetWidth();

	const int height = document.GetHeight();

	if (mergeFbo.getWidth() != width || mergeFbo.getHeight() != height) {
		AllocateMergeFbo(
			mergeFbo,
			width,
			height);
	}

	// --------------------------------------------------
	// Render the lower and upper layers into the
	// merge FBO.
	// --------------------------------------------------

	mergeFbo.begin();

	ofClear(
		0,
		0,
		0,
		0);

	mergeFbo.end();

	renderer.RenderLayer(
		*lower,
		mergeFbo);

	renderer.RenderLayer(
		*upper,
		mergeFbo);

	// --------------------------------------------------
	// Take ownership of the original layers.
	//
	// Upper is taken first because removing it does
	// not change the index of lower.
	// --------------------------------------------------

	LayerID lowerID = lower->GetID();

	std::unique_ptr<Layer> upperBackup = std::make_unique<Layer>(
		document.TakeLayer(layerID));

	std::unique_ptr<Layer> lowerBackup = std::make_unique<Layer>(
		document.TakeLayer(lowerID));

	// --------------------------------------------------
	// Store the originals for Undo.
	// --------------------------------------------------

	originalUpperLayer = std::move(upperBackup);

	originalLowerLayer = std::move(lowerBackup);

	// --------------------------------------------------
	// Create the new baked layer.
	// --------------------------------------------------

	Layer mergedLayer(
		width,
		height,
		originalLowerLayer->GetID());

	mergedLayer.SetName(
		originalLowerLayer->GetName());

	mergedLayer.SetVisible(true);
	mergedLayer.SetOpacity(1.0f);

	// --------------------------------------------------
	// Copy the merged pixels into the new layer.
	// --------------------------------------------------

	mergedLayer.GetFbo().begin();

	ofClear(
		0,
		0,
		0,
		0);

	ofSetColor(255);

	mergeFbo.draw(
		0,
		0,
		width,
		height);

	mergedLayer.GetFbo().end();

	// --------------------------------------------------
	// Insert the merged layer where the lower layer
	// originally was.
	// --------------------------------------------------

	document.InsertLayer(
		originalUpperIndex - 1,
		std::move(mergedLayer));

	// --------------------------------------------------
	// The effects have been baked into the pixels,
	// so the new layer has no effect.
	// --------------------------------------------------

	document.SetActiveLayer(
		originalLowerLayer->GetID());
}

void MergeDownCommand::Undo() {
	if (!hasBackup)
		return;

	LayerID mergedID = originalLowerLayer->GetID();

	document.TakeLayer(mergedID);

	document.InsertLayer(
		originalUpperIndex - 1,
		std::move(*originalLowerLayer));

	document.InsertLayer(
		originalUpperIndex,
		std::move(*originalUpperLayer));

	document.SetActiveLayer(
		originalActiveLayerID);
}
