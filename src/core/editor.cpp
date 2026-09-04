#include "editor.h"
#include "../commands/commandManager.h"
#include "../document/document.h"
#include "../rendering/renderer.h"
#include "../rendering/compositor.h"
#include "../ui/layerPanel.h"
#include "../rendering/camera2D.h"
#include "../tools/toolManager.h"
#include "../tools/brushTool.h"
#include "../tools/rectangleSelectTool.h"
#include "../tools/poissonStampTool.h"

#include "../commands/addLayerCommand.h"
#include "../commands/removeLayerCommand.h"
#include "../commands/moveLayerUpCommand.h"
#include "../commands/moveLayerDownCommand.h"
#include "../commands/brushStrokeCommand.h"
#include "../commands/mergeDownCommand.h"

#include "../rendering/colorMixerEffect.h"
#include "../ui/colorMixerPanel.h"
#include "../rendering/customShaderEffect.h"
#include "../ui/customShaderPanel.h"

Editor::Editor() = default;
Editor::~Editor() = default;

void Editor::Setup() {
	document = std::make_unique<Document>();
	renderer = std::make_unique<Renderer>();
	commandManager = std::make_unique<CommandManager>();
	camera = std::make_unique<Camera2D>();
	selection = std::make_unique<Selection>();

	toolManager = std::make_unique<ToolManager>();
	SetBrushTool();

	compositor = std::make_unique<Compositor>();

	// Layers panel
	layerPanel = std::make_unique<LayerPanel>(this);
	layerPanel->resize(300, 500);
	layerPanel->show();

	// Custom shader panel
	customShaderPanel = std::make_unique<CustomShaderPanel>(this);
	customShaderPanel->resize(300, 200);
}

void Editor::Update() {

	SetCameraPosition();
	SetCameraZoom();

	PoissonStampTool * stamp = dynamic_cast<PoissonStampTool *>(
		toolManager->GetTool());

	if (stamp && stamp->IsActive()) {
		stamp->Update();
	}
}

void Editor::Draw() {

	renderer->Draw(*document, *camera);

	// Draw Poisson stamp preview
	PoissonStampTool * stamp = dynamic_cast<PoissonStampTool *>(
		toolManager->GetTool());

	if (stamp && stamp->IsActive()) {

		ofPushMatrix();

		camera->Apply();

		stamp->Draw();

		ofPopMatrix();
	}

	// Draw selection rectangle
	Selection * selection = GetSelection();

	if (!selection || !selection->IsActive())
		return;

	glm::vec2 topLeft = camera->WorldToScreen(
		glm::vec2(
			selection->GetLeft(),
			selection->GetTop()));

	glm::vec2 bottomRight = camera->WorldToScreen(
		glm::vec2(
			selection->GetRight(),
			selection->GetBottom()));

	ofNoFill();
	ofSetColor(0, 120, 255);

	ofDrawRectangle(
		topLeft.x,
		topLeft.y,
		bottomRight.x - topLeft.x,
		bottomRight.y - topLeft.y);

	ofFill();
	ofSetColor(255);
}

// Accessors =================================================
Document* Editor::GetDocument() {
	return document.get();
}
Renderer* Editor::GetRenderer() {
	return renderer.get();
}
CommandManager* Editor::GetCommandManager() {
	return commandManager.get();
}

ToolManager* Editor::GetToolManager() {
	return toolManager.get();
}

Camera2D* Editor::GetCamera() {
	return camera.get();
}

Selection* Editor::GetSelection() {
	return selection.get();
}

Compositor* Editor::GetCompositor() {
	return compositor.get();
}

CustomShaderPanel* Editor::GetCustomShaderPanel() {
	return customShaderPanel.get();
}

// Sets =================================================

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

void Editor::SetBrushTool() {
	toolManager->SetTool(
		std::make_unique<BrushTool>(this));
}

void Editor::SetRectangleSelectTool() {
	toolManager->SetTool(
		std::make_unique<RectangleSelectTool>(this));
}

// Stamp Tool =================================================

void Editor::ToggleStampTool() {

	PoissonStampTool * stamp = dynamic_cast<PoissonStampTool *>(toolManager->GetTool());

	// If stamp tool is already active, cancel it and return to brush.
	if (stamp) {
		stamp->CancelStamp();
		SetBrushTool();
		return;
	}

	// Otherwise create the stamp tool.
	auto newStamp = std::make_unique<PoissonStampTool>(this);

	// Capture the current selection.
	if (!newStamp->StartStamp()) {
		ofLogNotice() << "Cannot start stamp: no active selection.";
		return;
	}

	// Transfer ownership to the ToolManager.
	toolManager->SetTool(std::move(newStamp));
}

// Layer Management =================================================

bool Editor::AddImageLayer(const std::string & path) {
	bool success = document->AddImageLayer(path);

	if (success) {
		selection->Allocate( document->GetWidth(), document->GetHeight());

		compositor->Setup(document->GetWidth(), document->GetHeight());

		layerPanel->Refresh();
	}

	return success;
}

void Editor::ExecuteCommand(std::unique_ptr<Command> command) {
	if (!command)
		return;

	commandManager->Execute(std::move(command));

	layerPanel->Refresh();
}

void Editor::RecordCommand(std::unique_ptr<Command> command) {
	commandManager->Record(std::move(command));
}

void Editor::SetActiveLayer(LayerID id) {
	document->SetActiveLayer(id);
}


// Layer Commands =================================================

void Editor::AddLayer() {
	ExecuteCommand(std::make_unique<AddLayerCommand>(document.get()));
}

void Editor::RemoveLayer(LayerID id) {
	ExecuteCommand(std::make_unique<RemoveLayerCommand>(document.get(), id));
}

void Editor::MoveLayerUp(LayerID id) {
	ExecuteCommand(std::make_unique<MoveLayerUpCommand>(document.get(), id));
}

void Editor::MoveLayerDown(LayerID id) {
	ExecuteCommand(std::make_unique<MoveLayerDownCommand>(document.get(), id));
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

// Mouse Events =================================================

void Editor::MousePressed(int x, int y) {
	toolManager->MousePressed(x, y);
}

void Editor::MouseDragged(int x, int y) {
	toolManager->MouseDragged(x, y);
}

void Editor::MouseReleased(int x, int y) {
	toolManager->MouseReleased(x, y);
}

// Painting =================================================
void Editor::PaintPoint(const ofVec2f & point, float radius, const ofColor & color, BrushMode mode) {

	Layer * layer = document->GetActiveLayer();

	if (!layer)
		return;

	Compositor * compositor = GetCompositor();

	if (!compositor)
		return;

	Selection * selection = GetSelection();
	const ofFbo * selectionMask = nullptr;

	if (selection && selection->IsActive()) {
		selectionMask = &selection->GetMask();
	}

	if (mode == BrushMode::Paint) {
		compositor->Paint(*layer, point.x, point.y, radius, color, selectionMask);
	} else {
		compositor->Erase(*layer, point.x, point.y, radius, selectionMask);
	}
}

void Editor::BeginBrushStroke(const ofVec2f & point, float radius, const ofColor & color, BrushMode mode) {
	Layer * layer = document->GetActiveLayer();

	if (!layer)
		return;

	Compositor * compositor = GetCompositor();

	if (!compositor)
		return;

	Selection * selection = GetSelection();

	const ofFbo * selectionMask = nullptr;

	if (selection && selection->IsActive()) {

		selectionMask = &selection->GetMask();
	}

	activeBrushStroke = std::make_unique<BrushStrokeCommand>(layer, compositor, std::vector<BrushPoint> {}, radius, color, mode, selection);

	activeBrushStroke->Begin();

	activeBrushStroke->AddPoint({ point });

	if (mode == BrushMode::Paint) {
		ofLogNotice() << "CALLING COMPOSITOR PAINT";
		compositor->Paint(*layer, point.x, point.y, radius, color, selectionMask);
	} else {
		compositor->Erase(*layer, point.x, point.y, radius, selectionMask);
	}
}

void Editor::ContinueBrushStroke(const ofVec2f & point, float radius, const ofColor & color, BrushMode mode) {
	if (!activeBrushStroke)
		return;

	Layer * layer = document->GetActiveLayer();

	if (!layer)
		return;

	Compositor * compositor = GetCompositor();

	if (!compositor)
		return;

	activeBrushStroke->AddPoint({ point });

	Selection * selection = GetSelection();

	const ofFbo * selectionMask = nullptr;

	if (selection && selection->IsActive()) {
		selectionMask = &selection->GetMask();
	}

	if (mode == BrushMode::Paint) {
		compositor->Paint(*layer, point.x, point.y, radius, color, selectionMask);
	} else {
		compositor->Erase(*layer, point.x, point.y, radius, selectionMask);
	}
}

void Editor::FinishBrushStroke() {
	if (!activeBrushStroke)
		return;

	activeBrushStroke->Finish();

	commandManager->Execute(std::move(activeBrushStroke));
}

void Editor::ToggleBrushMode() {
	auto * brush = dynamic_cast<BrushTool *>(toolManager->GetTool());

	if (!brush)
		return;

	brush->ToggleMode();
}

// Selection =================================================
void Editor::ClearSelection() {
	if (!selection)
		return;

	selection->Clear();
}

// Shaders ===================================================
void Editor::AddColorMixerEffect() {

	Layer * layer = document->GetActiveLayer();

	if (!layer)
		return;

	auto effect = std::make_unique<ColorMixerEffect>();

	if (!effect->Setup()) {

		ofLogError()
			<< "Failed to setup Color Mixer effect.";

		return;
	}

	layer->SetEffect(std::move(effect));
}

bool Editor::AddCustomShader(
	const std::string & fragmentPath) {

	ofLogNotice() << "A";

	Layer * layer = document->GetActiveLayer();

	ofLogNotice() << "B";

	if (!layer)
		return false;

	ofLogNotice() << "C";

	auto effect = std::make_unique<CustomShaderEffect>();

	ofLogNotice() << "D";

	if (!effect->Setup(fragmentPath)) {

		ofLogError()
			<< "Custom shader setup failed.";

		return false;
	}

	ofLogNotice() << "E";

	layer->SetEffect(std::move(effect));

	ofLogNotice() << "F";

	return true;
}

// Merge Down ========================================
void Editor::MergeDown(LayerID layerID) {
	auto command = std::make_unique<MergeDownCommand>(
		*document,
		*renderer,
		layerID);

	commandManager->Execute(
		std::move(command));

	layerPanel->Refresh();
}

void Editor::ToggleCustomShaderPanel() {
	if (!customShaderPanel)
		return;

	customShaderPanel->setVisible(
		!customShaderPanel->isVisible());
}
