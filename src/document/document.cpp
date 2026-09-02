#include "document.h"
#include "layer.h"

Document::Document() {
}

LayerID Document::AddLayer()
{	
    if (width <= 0 || height <= 0)
    {
        ofLogWarning()
            << "Cannot create layer: no document size set.";

        return InvalidLayerID;
    }

    LayerID id = nextLayerID++;

    Layer layer(width, height, id);

    layer.SetName(
        "Layer " + std::to_string(id)
    );

    layers.push_back(std::move(layer));

    activeLayerID = id;

	return id;
}

bool Document::AddImageLayer(const std::string & path) {

	std::string resolvedPath = ofToDataPath(path, true);

	ofImage image;

	if (!image.load(resolvedPath)) {

		ofLogError()
			<< "Failed to load "
			<< path;

		return false;
	}

	// First imported image defines canvas size
	if (!HasCanvas()) {

		width = image.getWidth();
		height = image.getHeight();
	}

	LayerID id = nextLayerID++;

	Layer layer(
		width,
		height,
		id);

	layer.SetName(
		"Layer " + std::to_string(id));

	layer.SetImage(image);

	ofLogNotice()
		<< "NEW LAYER CREATED: "
		<< id
		<< " FBO TEX ID = "
		<< layer.GetFbo().getTexture().getTextureData().textureID;

	layers.push_back(
		std::move(layer));

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

void Document::MoveLayerUp(LayerID id) {
	size_t index = GetLayerIndexByID(id);

	if (index == layers.size())
		return;

	if (index + 1 < layers.size())
		MoveLayer(index, index + 1);
}

void Document::MoveLayerDown(LayerID id) {
	size_t index = GetLayerIndexByID(id);

	if (index == layers.size())
		return; // Layer not found

	if (index > 0)
		MoveLayer(index, index - 1);
}

void Document::RemoveLayer(LayerID id) {
	size_t index = GetLayerIndexByID(id);

	if (index == layers.size())
		return; // Layer not found

	layers.erase(layers.begin() + index);

	if (id == activeLayerID) {
		if (!layers.empty()) {
			// Make the top-most remaining layer active
			activeLayerID = layers.back().GetID();
		} else {
			activeLayerID = 0;
		}
	}
}

size_t Document::GetLayerIndexByID(LayerID id) const {
	for (size_t i = 0; i < layers.size(); ++i) {
		if (layers[i].GetID() == id)
			return i;
	}
	return layers.size();
}

void Document::InsertLayer(size_t index, Layer layer) {
	if (index > layers.size())
		index = layers.size();
	layers.insert(layers.begin() + index, std::move(layer));
}

Layer Document::TakeLayer(LayerID id) {
	size_t index = GetLayerIndexByID(id);

	if (index == layers.size()) {
		throw std::runtime_error(
			"Cannot take layer: layer not found.");
	}

	Layer layer = std::move(layers[index]);

	layers.erase(
		layers.begin() + index);

	if (id == activeLayerID) {
		if (!layers.empty())
			activeLayerID = layers.back().GetID();
		else
			activeLayerID = InvalidLayerID;
	}

	return layer;
}

// Status checks
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

std::vector<Layer> & Document::GetLayers() {
	return layers;
}

const std::vector<Layer> & Document::GetLayers() const {
	return layers;
}

LayerID Document::GetActiveLayerID() const {
	return activeLayerID;
}

size_t Document::GetLayerCount() const {
	return layers.size();
}

int Document::GetWidth() const {
	return width;
}

int Document::GetHeight() const {
	return height;
}
