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
	if (key == 'l') {
		editor->LoadImageLayer("test.png");
	}
}
