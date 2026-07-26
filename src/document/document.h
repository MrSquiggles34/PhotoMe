#pragma once

#include <vector>

#include "layer.h"

class Layer;

class Document {
public:
	Document();

	LayerID AddLayer();
	bool AddImageLayer(const std::string & path);

	void MoveLayer(size_t from, size_t to);
	void RemoveLayer(LayerID id);
	void MoveLayerUp(LayerID id);
	void MoveLayerDown(LayerID id);


	Layer* FindLayerByID(LayerID id);
	size_t GetLayerIndexByID(LayerID id) const;
	void InsertLayer(size_t index, Layer layer);

	Layer TakeLayer(LayerID id);


	bool HasLayers() const;
	bool HasCanvas() const;

	// Sets
	void SetActiveLayer(LayerID id);


	// Gets
	const std::vector<Layer> & GetLayers() const;

	Layer * GetActiveLayer();
	LayerID GetActiveLayerID() const;

	size_t GetLayerCount() const;

	int GetWidth() const;
	int GetHeight() const;

private:

	int width = 0;
	int height = 0;

	std::vector<Layer> layers;

	LayerID nextLayerID = 0;
	LayerID activeLayerID = 0;
};
