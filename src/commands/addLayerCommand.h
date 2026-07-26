#pragma once

#include "command.h"
#include "../document/layerID.h"

class Document;

class AddLayerCommand : public Command {
public:
	explicit AddLayerCommand(Document * document);

	void Execute() override;
	void Undo() override;

private:
	Document * document = nullptr;
	LayerID layerID = InvalidLayerID;
};
