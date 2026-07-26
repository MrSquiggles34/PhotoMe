#include "moveLayerUpCommand.h"

#include "../document/document.h"

MoveLayerUpCommand::MoveLayerUpCommand(Document* document, LayerID id) : document(document), layerID(id) { }

void MoveLayerUpCommand::Execute() {

	document->MoveLayerUp(layerID);
}

void MoveLayerUpCommand::Undo() {

	document->MoveLayerDown(layerID);
}
