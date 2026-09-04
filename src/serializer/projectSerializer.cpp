#include "projectSerializer.h"

#include "../document/document.h"
#include "../document/layer.h"

#include "ofMain.h"

#include <filesystem>

bool ProjectSerializer::Save(
	const Document & document,
	const std::string & path) {
	namespace fs = std::filesystem;

	fs::path projectPath(path);

	fs::path projectDirectory = projectPath.parent_path();

	fs::path layersDirectory = projectDirectory / "layers";

	std::error_code error;

	fs::create_directories(
		layersDirectory,
		error);

	if (error) {
		ofLogError()
			<< "Failed to create layers directory: "
			<< layersDirectory.string();

		return false;
	}

	// Remove old layer images from the previous save.
	// Only PNG files inside this project's layers directory
	// are removed.
	for (const fs::directory_entry & entry :
		fs::directory_iterator(layersDirectory)) {
		if (!entry.is_regular_file())
			continue;

		if (entry.path().extension() != ".png")
			continue;

		fs::remove(
			entry.path(),
			error);

		if (error) {
			ofLogError()
				<< "Failed to remove old layer image: "
				<< entry.path().string();

			return false;
		}
	}

	ofJson project;

	project["version"] = 1;

	project["document"]["width"] = document.GetWidth();

	project["document"]["height"] = document.GetHeight();

	project["layers"] = ofJson::array();

	for (const Layer & layer :
		document.GetLayers()) {
		ofJson jsonLayer;

		jsonLayer["id"] = layer.GetID();

		jsonLayer["name"] = layer.GetName();

		jsonLayer["visible"] = layer.IsVisible();

		jsonLayer["opacity"] = layer.GetOpacity();

		jsonLayer["position"] = { layer.GetPosition().x, layer.GetPosition().y };

		std::string imageFileName = "layer_" + std::to_string(layer.GetID()) + ".png";

		fs::path imagePath = layersDirectory / imageFileName;

		ofPixels pixels;

		layer.GetFbo().readToPixels(
			pixels);

		if (!ofSaveImage(
				pixels,
				imagePath.string(),
				OF_IMAGE_QUALITY_BEST)) {
			ofLogError()
				<< "Failed to save layer image: "
				<< imagePath.string();

			return false;
		}

		jsonLayer["image"] = ("layers/" + imageFileName);

		project["layers"].push_back(
			jsonLayer);
	}

	if (!ofSaveJson(
			path,
			project)) {
		ofLogError()
			<< "Failed to save project: "
			<< path;

		return false;
	}

	ofLogNotice()
		<< "Project saved: "
		<< path;

	return true;
}

bool ProjectSerializer::Load(
	Document & document,
	const std::string & path) {
	namespace fs = std::filesystem;

	fs::path projectPath(path);

	if (!fs::exists(projectPath)) {
		ofLogError()
			<< "Project file does not exist: "
			<< path;

		return false;
	}

	// --------------------------------------------------
	// Load project JSON
	// --------------------------------------------------

	ofJson project = ofLoadJson(path);

	if (!project.contains("version")) {
		ofLogError()
			<< "Project has no version.";

		return false;
	}

	int version = project["version"].get<int>();

	if (version != 1) {
		ofLogError()
			<< "Unsupported project version: "
			<< version;

		return false;
	}

	if (!project.contains("document")) {
		ofLogError()
			<< "Project has no document section.";

		return false;
	}

	const ofJson & jsonDocument = project["document"];

	int width = jsonDocument["width"].get<int>();

	int height = jsonDocument["height"].get<int>();

	if (width <= 0 || height <= 0) {
		ofLogError()
			<< "Invalid document dimensions.";

		return false;
	}

	if (!project.contains("layers") || !project["layers"].is_array()) {
		ofLogError()
			<< "Project has no valid layers array.";

		return false;
	}

	// --------------------------------------------------
	// Create a temporary document
	// --------------------------------------------------

	Document loadedDocument;

	loadedDocument.SetSize(
		width,
		height);

	LayerID highestLayerID = 0;

	bool hasLoadedLayer = false;

	// --------------------------------------------------
	// Load each layer
	// --------------------------------------------------

	for (const ofJson & jsonLayer :
		project["layers"]) {
		LayerID id = jsonLayer["id"].get<LayerID>();

		std::string name = jsonLayer["name"].get<std::string>();

		bool visible = jsonLayer["visible"].get<bool>();

		float opacity = jsonLayer["opacity"].get<float>();

		std::string imageRelativePath = jsonLayer["image"].get<std::string>();

		fs::path imagePath = projectPath.parent_path() / imageRelativePath;

		ofImage image;

		if (!image.load(imagePath.string())) {
			ofLogError()
				<< "Failed to load layer image: "
				<< imagePath.string();

			return false;
		}

		// Create the layer using the ID stored
		// in the project file.
		Layer layer(
			width,
			height,
			id);

		layer.SetName(name);
		layer.SetVisible(visible);
		layer.SetOpacity(opacity);

		if (jsonLayer.contains("position") && jsonLayer["position"].is_array() && jsonLayer["position"].size() >= 2) {
			layer.SetPosition(
				jsonLayer["position"][0].get<float>(),
				jsonLayer["position"][1].get<float>());
		}

		layer.SetImage(image);

		loadedDocument.InsertLayer(
			loadedDocument.GetLayerCount(),
			std::move(layer));

		if (!hasLoadedLayer || id >= highestLayerID) {
			highestLayerID = id;
			hasLoadedLayer = true;
		}
	}

	// --------------------------------------------------
	// Restore the next available layer ID
	// --------------------------------------------------

	if (hasLoadedLayer) {
		loadedDocument.SetNextLayerID(
			highestLayerID + 1);
	}

	// --------------------------------------------------
	// Restore active layer
	// --------------------------------------------------

	if (loadedDocument.HasLayers()) {
		const Layer & activeLayer = loadedDocument.GetLayers().back();

		loadedDocument.SetActiveLayer(
			activeLayer.GetID());
	}

	// --------------------------------------------------
	// Replace the current document
	// --------------------------------------------------

	document = std::move(loadedDocument);

	ofLogNotice()
		<< "Project loaded: "
		<< path;

	return true;
}
