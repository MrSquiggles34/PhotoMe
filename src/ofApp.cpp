#include "ofApp.h"
#include "core/editor.h"
#include "document/document.h"
#include "commands/commandManager.h"

ofApp::ofApp() = default;
ofApp::~ofApp() = default;

void ofApp::setup() {
	editor = std::make_unique<Editor>();
	editor->Setup();
}

void ofApp::update() {
	editor->Update();
}

void ofApp::draw() {
	editor->Draw();
}

void ofApp::keyPressed(int key) {
	if (key == 'v') {
		editor->GetDocument()->GetActiveLayer()->SetVisible(!editor->GetDocument()->GetActiveLayer()->IsVisible());
	}

	if (key == 'z') {
		editor->GetCommandManager()->Undo();
		editor->RefreshLayerPanel();
	}

	if (key == 'y') {
		editor->GetCommandManager()->Redo();
		editor->RefreshLayerPanel();
	}

	if (key == 't') {
		editor->MoveActiveLayer(
			0.0f,
			-10.0f);
	}

	if (key == 'g') {
		editor->MoveActiveLayer(
			0.0f,
			10.0f);
	}

	if (key == 'f') {
		editor->MoveActiveLayer(
			-10.0f,
			0.0f);
	}

	if (key == 'h') {
		editor->MoveActiveLayer(
			10.0f,
			0.0f);
	}
}

void ofApp::mousePressed(int x, int y, int button) {
	editor->MousePressed(x, y);
}

void ofApp::mouseDragged(int x, int y, int button) {
	editor->MouseDragged(x, y);
}

void ofApp::mouseReleased(int x, int y, int button) {
	editor->MouseReleased(x, y);
}
