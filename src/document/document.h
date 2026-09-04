#pragma once

#include <vector>
#include <algorithm>

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
	void MoveLayerImage(LayerID id, const glm::vec2 & position);


	Layer* FindLayerByID(LayerID id);
	size_t GetLayerIndexByID(LayerID id) const;
	void InsertLayer(size_t index, Layer layer);

	Layer TakeLayer(LayerID id);


	bool HasLayers() const;
	bool HasCanvas() const;

	// Sets
	void SetActiveLayer(LayerID id);
	void SetSize(int width, int height);
	void SetNextLayerID(LayerID id);
	void Resize(int width, int height);


	// Gets
	const std::vector<Layer> & GetLayers() const;
	std::vector<Layer> & GetLayers();

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
