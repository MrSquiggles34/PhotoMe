#include "editor.h"
#include "../commands/commandManager.h"
#include "../document/document.h"
#include "../rendering/renderer.h"

Editor::Editor() = default;
Editor::~Editor() = default;

void Editor::Setup() {
	document = std::make_unique<Document>();
	renderer = std::make_unique<Renderer>();
	commands = std::make_unique<CommandManager>();
}

void Editor::Update() {

	SetCameraPosition();
	SetCameraZoom();

}

void Editor::Draw() {
	renderer->Draw(*document, camera);	
}

Document* Editor::GetDocument() {
	return document.get();
}
Renderer* Editor::GetRenderer() {
	return renderer.get();
}
CommandManager* Editor::GetCommandManager() {
	return commands.get();
}

void Editor::SetCameraPosition() {
	// Pan
	float speed = 5.0f / camera.zoom;

	if (ofGetKeyPressed(OF_KEY_LEFT)) camera.position.x += speed;
	if (ofGetKeyPressed(OF_KEY_RIGHT)) camera.position.x -= speed;
	if (ofGetKeyPressed(OF_KEY_UP)) camera.position.y += speed;
	if (ofGetKeyPressed(OF_KEY_DOWN)) camera.position.y -= speed;
}

void Editor::SetCameraZoom() {

	glm::vec2 mouse(ofGetMouseX(), ofGetMouseY());

	// world point under mouse before zoom
	glm::vec2 beforeZoom = camera.ScreenToWorld(mouse);

	float zoomFactor = 1.0f;

	if (ofGetKeyPressed('w')) {
		zoomFactor = 1.05f;
	}
	if (ofGetKeyPressed('s')) {
		zoomFactor = 0.95f;
	}

	if (zoomFactor != 1.0f) {

		camera.zoom *= zoomFactor;
		camera.zoom = std::clamp(camera.zoom, 0.1f, 10.0f);

		// world point under mouse a zoom must match
		glm::vec2 afterZoom = camera.ScreenToWorld(mouse);

		// adjust position so point stays stable
		camera.position += (afterZoom - beforeZoom) * camera.zoom;
	}
}
