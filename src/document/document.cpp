#include "document.h"
#include "layer.h"

Document::Document() {
}

void Document::AddLayer()
{
    if (width <= 0 || height <= 0)
    {
        ofLogWarning()
            << "Cannot create layer: no document size set.";

        return;
    }

    LayerID id = nextLayerID++;

    Layer layer(width, height, id);

    layer.SetName(
        "Layer " + std::to_string(id)
    );

    layers.push_back(std::move(layer));

    activeLayerID = id;
}

bool Document::LoadImage(const std::string & path) {
	std::string resolvedPath = ofToDataPath(path, true);

	ofImage image;

	if (!image.load(resolvedPath)) {
		ofLogError() << "Failed to load " << path;
		return false;
	}

	width = image.getWidth();
	height = image.getHeight();

	layers.clear();

	LayerID id = nextLayerID++;

	Layer layer(width, height, id);

	layer.SetName(
		"Layer " + std::to_string(id));

	layer.SetImage(image);

	layers.push_back(std::move(layer));

	activeLayerID = id;

	return true;
}

Layer* Document::FindLayerByID(LayerID id) {
	for (Layer & layer : layers) {
		if (layer.GetID() == id)
			return &layer;
	}

	return nullptr;
}

void Document::MoveLayer(size_t from, size_t to) {

	if (from >= layers.size() || to >= layers.size())
		return;

	if (from == to)
		return;

	auto layer = std::move(layers[from]);
	layers.erase(layers.begin() + from);
	layers.insert(layers.begin() + to, std::move(layer));
}

void Document::MoveLayerUp(size_t index) {
	if (index + 1 < layers.size())
		MoveLayer(index, index + 1);
}

void Document::MoveLayerDown(size_t index) {
	if (index > 0)
		MoveLayer(index, index - 1);
}

void Document::RemoveLayer(size_t index) {

	LayerID removedID = layers[index].GetID();

	layers.erase(layers.begin() + index);

	if (removedID == activeLayerID) {
		if (!layers.empty())
			activeLayerID = layers.back().GetID();
		else
			activeLayerID = 0;
	}
	
}

bool Document::HasLayers() const {
	return !layers.empty();
}

bool Document::HasCanvas() const {
	return width > 0 && height > 0;
}

// Sets
void Document::SetActiveLayer(LayerID id) {
	if (FindLayerByID(id))
		activeLayerID = id;
}


// Gets
Layer * Document::GetActiveLayer() {
	return FindLayerByID(activeLayerID);
}

const std::vector<Layer>& Document::GetLayers() const {
	return layers;
}

LayerID Document::GetActiveLayerID() const {
	return activeLayerID;
}

size_t Document::GetLayerCount() const {
	return layers.size();
}


