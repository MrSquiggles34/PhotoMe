#pragma once

#include <limits>

using LayerID = std::size_t;

constexpr LayerID InvalidLayerID = std::numeric_limits<LayerID>::max();
