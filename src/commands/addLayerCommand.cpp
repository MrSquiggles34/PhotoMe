#include "addLayerCommand.h"

#include "../document/document.h"

AddLayerCommand::AddLayerCommand(Document* document) : document(document) { }

void AddLayerCommand::Execute() {
	layerID = document->AddLayer();
}

void AddLayerCommand::Undo() {
	document->RemoveLayer(layerID);
}
