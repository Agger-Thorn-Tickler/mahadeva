#pragma once

#include "ofMain.h"
#include "ofxMidi.h"
#include "ofxShader.h"
#include "ofxShaderFilter.h"
#include "ofFbo.h"
#include "ofTrueTypeFont.h"
#include "ofVideoPlayer.h"
#include "ofSoundStream.h"
#include <mutex>

class ofApp : public ofBaseApp, public ofxMidiListener{

	public:
		void setup();
		void update();
		void draw();
    void exit();

		void keyPressed(int key);
		void keyReleased(int key);
		void windowResized(int w, int h);


    void newMidiMessage(ofxMidiMessage &message);
    void printMidi();

  ofxShaderFilter sdfShader;
  ofxShaderFilter videoShader;
  ofFbo videoFbo;

  ofxMidiIn midi;
  vector <ofxMidiMessage> messages;

  float frame;
  ofTrueTypeFont font;
  ofVideoPlayer video;

  mutex mu;
  float midi_args[16] = {1., 1., 1., 1., 1., 1., 1., 1., 1., 1., 1., 1., 1., 1., 1., 1.};


};
