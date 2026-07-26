#pragma once

#include <memory>
#include <optional>

#include "../document/layerID.h"
#include "../document/layer.h"
#include "command.h"

class Document;
class Layer;

class RemoveLayerCommand : public Command {
public:
	RemoveLayerCommand(Document * document, LayerID id);

	void Execute() override;
	void Undo() override;

private:

	Document * document = nullptr;
	LayerID layerID;


	size_t oldIndex;
	std::optional<Layer> removedLayer;
	LayerID previousActiveLayer;
};
