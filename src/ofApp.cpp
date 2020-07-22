#include <sstream>
#include "ofApp.h"
#include <string>


void allocateShader(ofxShaderFilter* shader, string shader_name) {
  shader->allocate(ofGetWidth(), ofGetHeight());
  shader->load(shader_name);
}

void allocateFbo(ofFbo* fbo, ofxShaderFilter* shader, string shader_name) {
  fbo->allocate(ofGetWidth(), ofGetHeight());
  fbo->begin();
  ofClear(255);
  allocateShader(shader, shader_name);
  fbo->end();
}

//--------------------------------------------------------------

void ofApp::setup(){
  midi.listInPorts();
  midi.openPort(1);
  midi.addListener(this);

  video.load("bugs.mkv");
  video.setVolume(0.);
  font.load("arcade.ttf", 70);
  ofDisableArbTex();
  ofSetWindowTitle("VJshader");
  ofSetFrameRate(120);
  ofSetVerticalSync(true);
  allocateFbo(&videoFbo, &videoShader, "video.frag");
  allocateShader(&sdfShader, "shader.frag");
  video.play();

}

//--------------------------------------------------------------
void ofApp::update(){

  this->printMidi();

  video.update();

  videoFbo.begin();

  sdfShader.setUniform1f("u_frame", frame);
  videoShader.setUniform1f("u_frame", frame);

  for(int i = 0; i < 16; i++) {
    string name = "u_midi" + std::to_string(i);
    //cout << name << " " << midi_args[i] << endl;
    videoShader.setUniform1f(name, midi_args[i]);
    sdfShader.setUniform1f(name, midi_args[i]);
  }

  sdfShader.render();

  videoShader.setUniformTexture("videoTexture", video.getTexture());
  videoShader.setUniformTexture("sdfTexture", sdfShader.getTexture());

  videoShader.draw(0, 0, ofGetWidth(), ofGetHeight());
  videoShader.render();

  std::ostringstream ss;
  ss << frame;
  std::string s(ss.str());
  font.drawString(string(s), 100,100);
  videoFbo.end();

  frame += 1.;
}

//--------------------------------------------------------------
void ofApp::draw(){
  videoFbo.getTextureReference().bind();
  videoFbo.getTextureReference().getTextureData().bFlipTexture = true;
  videoFbo.draw(0, 0);
  videoFbo.unbind();

}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
  if (key == 'f'){
    ofToggleFullscreen();
  }
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){
  videoShader.allocate(w, h);
  videoFbo.allocate(w, h);
}
//--------------------------------------------------------------
void ofApp::newMidiMessage(ofxMidiMessage &message) {
  mu.lock();
  messages.push_back(message);
  mu.unlock();

}

//--------------------------------------------------------------

void ofApp::printMidi() {

  mu.lock();
  while (int(messages.size()) > 33) {
    messages.erase(messages.begin());
  }
  for(int i = 0; i < int(messages.size()); i++ ) {
    ofxMidiMessage &message = messages[i];
    midi_args[message.control] = float(message.value) / float(127);
    //cout << message.control << message.value << endl;
    messages.erase(messages.begin());
  }
  mu.unlock();
}


//--------------------------------------------------------------

void ofApp::exit() {
  midi.closePort();
  midi.removeListener(this);
  video.stop();
}

