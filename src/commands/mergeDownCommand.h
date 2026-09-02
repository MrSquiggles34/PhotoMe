#pragma once

#include <memory>

#include "ofMain.h"
#include "command.h"
#include "../document/layerID.h"

class Renderer;
class Document;
class Layer;

class MergeDownCommand : public Command {
public:
	MergeDownCommand(
		Document & document,
		Renderer & renderer,
		LayerID layerID);

	void Execute() override;
	void Undo() override;

private:
	Document & document;
	Renderer & renderer;

	LayerID layerID;

	std::unique_ptr<Layer> originalLowerLayer;
	std::unique_ptr<Layer> originalUpperLayer;

	size_t originalUpperIndex = 0;
	LayerID originalActiveLayerID = InvalidLayerID;

	ofFbo mergeFbo;

	bool hasBackup = false;
};
