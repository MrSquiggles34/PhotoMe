#pragma once
//information for the UI. The panel does not need the actual Layer object

#include "../document/layerID.h"

#include <string>
struct LayerInfo {

	LayerID id;
	std::string name;

	bool visible = true;
	bool selected = false;
};
