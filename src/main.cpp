#include "ofMain.h"
#include "ofApp.h"

//========================================================================
int main( ){
  ofGLFWWindowSettings settings;
  
  settings.setSize(1280, 720);
  settings.setGLVersion(3, 2);
  settings.resizable = true;
  ofCreateWindow(settings);
	//ofSetupOpenGL(1024,768,OF_WINDOW);			// <-------- setup the GL context

	// this kicks off the running of my app
	// can be OF_WINDOW or OF_FULLSCREEN
	// pass in width and height too:
	ofRunApp(new ofApp());

}
