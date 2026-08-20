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
	if (key == 'j') {
		editor->AddImageLayer("test.png");
	}

	if (key == 'k') {
		editor->AddImageLayer("test2.png");
	}

	if (key == 'l') {
		editor->AddImageLayer("test3.png");
	}

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
