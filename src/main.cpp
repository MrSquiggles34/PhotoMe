#include "ofMain.h"
#include "ofApp.h"

#include <QApplication>

//========================================================================
int main(int argc, char * argv[]) {
	QApplication qtApp(argc, argv);

	ofGLWindowSettings settings;
	settings.setGLVersion(3, 3);

	auto window = ofCreateWindow(settings);

	ofRunApp(window, std::make_shared<ofApp>());
	ofRunMainLoop();
}
