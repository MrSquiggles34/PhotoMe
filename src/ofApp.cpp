#include "ofApp.h"
#include "core/editor.h"
#include "document/document.h"

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
}
