#include "removeLayerCommand.h"

#include "../document/document.h"

RemoveLayerCommand::RemoveLayerCommand(Document * document, LayerID id) : document(document), layerID(id) { }

void RemoveLayerCommand::Execute() {
	if (!document)
		return;

	oldIndex = document->GetLayerIndexByID(layerID);

	if (oldIndex == document->GetLayerCount())
		return;

	previousActiveLayer = document->GetActiveLayerID();

	removedLayer.emplace(document->TakeLayer(layerID));
}

void RemoveLayerCommand::Undo() {
	if (!document || !removedLayer.has_value())
		return;

	document->InsertLayer(oldIndex, std::move(*removedLayer));
	document->SetActiveLayer(previousActiveLayer);

	removedLayer.reset();
}
