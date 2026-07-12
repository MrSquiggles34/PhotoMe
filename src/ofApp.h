#pragma once

#include <memory>

#include "ofMain.h"

class Editor;

class ofApp : public ofBaseApp {
	public:
		ofApp();
		~ofApp();

		void setup();
		void update();
		void draw();

		void keyPressed(int key);

	private:
		std::unique_ptr<Editor> editor;
};
