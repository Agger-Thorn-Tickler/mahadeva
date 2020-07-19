#include <sstream>
#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
  video.load("shiva.mp4");
  video.setVolume(0.);
  font.load("arcade.ttf", 50);
  ofDisableArbTex();
  ofSetWindowTitle("VJshader");
  ofSetFrameRate(60);
  ofSetVerticalSync(true);

  fbo.allocate(ofGetWidth(), ofGetHeight());
  fbo.begin();
  ofClear(255);
  shader.allocate(fbo.getWidth(), fbo.getHeight());
  shader.load("shader.frag");
  fbo.end();
  video.play();

}

//--------------------------------------------------------------
void ofApp::update(){
  fbo.begin();
  video.update();
  shader.setUniform1f("u_frame", frame);
  shader.setUniformTexture("fboTexture", fbo.getTextureReference(0));
  shader.setUniformTexture("videoTexture", video.getTexture());
  fbo.end();
  
  fbo.begin();
  shader.render();
  shader.draw(0, 0, ofGetWidth(), ofGetHeight());
  std::ostringstream ss;
  ss << frame;
  ss << "♃";
  std::string s(ss.str());
  font.drawString(string(s), 100,100);
  fbo.end();
  frame += 1.;
}

//--------------------------------------------------------------
void ofApp::draw(){
  fbo.getTextureReference().bind();
  fbo.getTextureReference().getTextureData().bFlipTexture = true;
  fbo.draw(0, 0);
  fbo.unbind();
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
void ofApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y){

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){
  shader.allocate(w, h);
  fbo.allocate(w, h);
}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}
