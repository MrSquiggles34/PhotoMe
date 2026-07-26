#include "editor.h"
#include "../commands/commandManager.h"
#include "../document/document.h"
#include "../rendering/renderer.h"
#include "../ui/layerPanel.h"
#include "../rendering/camera2D.h"

#include "../commands/addLayerCommand.h"
#include "../commands/removeLayerCommand.h"
#include "../commands/moveLayerUpCommand.h"
#include "../commands/moveLayerDownCommand.h"

Editor::Editor() = default;
Editor::~Editor() = default;

void Editor::Setup() {
	document = std::make_unique<Document>();
	renderer = std::make_unique<Renderer>();
	commandManager = std::make_unique<CommandManager>();
	camera = std::make_unique<Camera2D>();

	layerPanel = std::make_unique<LayerPanel>(this);
	layerPanel->resize(300, 500);
	layerPanel->show();
}

void Editor::Update() {

	SetCameraPosition();
	SetCameraZoom();

}

void Editor::Draw() {
	renderer->Draw(*document, *camera);	
}

Document* Editor::GetDocument() {
	return document.get();
}
Renderer* Editor::GetRenderer() {
	return renderer.get();
}
CommandManager* Editor::GetCommandManager() {
	return commandManager.get();
}

Camera2D* Editor::GetCamera() {
	return camera.get();
}

void Editor::SetCameraPosition() {
	// Pan
	float speed = 5.0f / camera->zoom;

	if (ofGetKeyPressed(OF_KEY_LEFT)) camera->position.x += speed;
	if (ofGetKeyPressed(OF_KEY_RIGHT)) camera->position.x -= speed;
	if (ofGetKeyPressed(OF_KEY_UP)) camera->position.y += speed;
	if (ofGetKeyPressed(OF_KEY_DOWN)) camera->position.y -= speed;
}

void Editor::SetCameraZoom() {

	glm::vec2 mouse(ofGetMouseX(), ofGetMouseY());

	// world point under mouse before zoom
	glm::vec2 beforeZoom = camera->ScreenToWorld(mouse);

	float zoomFactor = 1.0f;

	if (ofGetKeyPressed('w')) {
		zoomFactor = 1.05f;
	}
	if (ofGetKeyPressed('s')) {
		zoomFactor = 0.95f;
	}

	if (zoomFactor != 1.0f) {

		camera->zoom *= zoomFactor;
		camera->zoom = std::clamp(camera->zoom, 0.1f, 10.0f);

		// world point under mouse a zoom must match
		glm::vec2 afterZoom = camera->ScreenToWorld(mouse);

		// adjust position so point stays stable
		camera->position += (afterZoom - beforeZoom) * camera->zoom;
	}
}

bool Editor::AddImageLayer(const std::string& path) {
	bool success = document->AddImageLayer(path);

	if (success) {
		layerPanel->Refresh();
	}

	return success;
}

void Editor::AddLayer() {
	commandManager->Execute(std::make_unique<AddLayerCommand>(document.get()));
	layerPanel->Refresh();
}

void Editor::RemoveLayer(LayerID id) {
	commandManager->Execute(std::make_unique<RemoveLayerCommand>(document.get(), id));
	layerPanel->Refresh();
}

void Editor::MoveLayerUp(LayerID id) {
	commandManager->Execute(std::make_unique<MoveLayerUpCommand>(document.get(), id));
	layerPanel->Refresh();
}

void Editor::MoveLayerDown(LayerID id) {
	commandManager->Execute(std::make_unique<MoveLayerDownCommand>(document.get(), id));
	layerPanel->Refresh();
}

void Editor::SetActiveLayer(LayerID id) {
	document->SetActiveLayer(id);
}

void Editor::RefreshLayerPanel() {
	layerPanel->Refresh();
}

std::vector<LayerInfo> Editor::GetLayerInfo() const {

	std::vector<LayerInfo> result;

	const auto & layers = document->GetLayers();

	for (auto it = layers.rbegin(); it != layers.rend(); ++it) {

		LayerInfo info;

		info.id = it->GetID();
		info.name = it->GetName();
		info.visible = it->IsVisible();
		info.selected = (info.id == document->GetActiveLayerID());

		result.push_back(info);
	}

	return result;
}
