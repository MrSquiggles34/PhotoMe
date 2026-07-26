#pragma once

#include "../document/layerID.h"
#include "command.h"

class Document;

class MoveLayerUpCommand : public Command {
public:
	MoveLayerUpCommand(Document * document, LayerID id);

	void Execute() override;
	void Undo() override;

private:
	Document * document = nullptr;

	LayerID layerID;
};
