#include "moveLayerDownCommand.h"

#include "../document/document.h"

MoveLayerDownCommand::MoveLayerDownCommand(Document* document, LayerID id) : document(document), layerID(id) { }

void MoveLayerDownCommand::Execute() {

	document->MoveLayerDown(layerID);
}

void MoveLayerDownCommand::Undo() {

	document->MoveLayerUp(layerID);
}
