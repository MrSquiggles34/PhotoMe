#pragma once

#include "../document/layerID.h"
#include "command.h"

class Document;

class MoveLayerDownCommand : public Command {
public:
	MoveLayerDownCommand(Document * document, LayerID id);

	void Execute() override;
	void Undo() override;

private:
	Document * document = nullptr;

	LayerID layerID;
};
