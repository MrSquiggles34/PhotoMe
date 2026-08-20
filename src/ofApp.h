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
		void mousePressed(int x, int y, int button);
		void mouseDragged(int x, int y, int button);
		void mouseReleased(int x, int y, int button);

	private:
		std::unique_ptr<Editor> editor;
};
