#pragma once

#include <vector>

#include "layer.h"

class Layer;

class Document {
public:
	Document();

	bool LoadImage(const std::string & path);

	void AddLayer();
	void RemoveLayer(size_t index);

	void MoveLayer(size_t from, size_t to);

	void MoveLayerUp(size_t index);
	void MoveLayerDown(size_t index);

	Layer* FindLayerByID(LayerID id);

	bool HasLayers() const;

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
