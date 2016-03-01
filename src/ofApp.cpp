#include "ofApp.h"

const int N = 256;  // number of bands in spectrum
float spectrum[N];  // smoothed spectrum values
float rad = 800;    // cloud radius parameter
float vel = 0.1;    // cloud points velocity parameter
int bandRad = 2;    // band index in spectrum, affecting rad value
int bandVel = 50;   // band index in spectrum, affecting vel value
const int n = 300;  // number of cloud points
float tx[n], ty[n]; // offsets for perlin noise calculation for points
ofPoint p[n];       // cloud's points position
float time0 = 0;    // time value, used for dt computing

void ofApp::setup() {
    // setup sound
    sound.load("surface.wav");
    sound.setLoop(true);
    sound.play();

    // set spectrum values to 0
    for (int i = 0; i < N; i++) {
        spectrum[i] = 0.0f;
    }

    // initialize point offsets by random numbers
    for (int j = 0; j < n; j++) {
        tx[j] = ofRandom(0, 1000);
        ty[j] = ofRandom(0, 1000);
    }
}

void ofApp::update() {
    // update sound engine
    // ofSoundUpdate();

    // Get current spectrum with N bands
    float *val = ofSoundGetSpectrum(N);
    // We should not release memory of val,
    // because it is managed by the sound engine

    // update smoothed spectrum, by slowly decreasing
    // its values and getting maximum with val
    // so we will have slowly falling peaks in spectrum
    for (int i = 0; i < N; i++) {
        spectrum[i] *= 0.97;
        spectrum[i] = max(spectrum[i], val[i]);
    }

    // update particles using spectrum values

    // compute dt as a time between the last
    // and the current call to update()
    float time = ofGetElapsedTimef();
    float dt = time - time0;
    dt = ofClamp(dt, 0.0, 0.1);
    time0 = time; // store current time

    // update rad and val from spectrum
    // note, the parameters in ofMaps were tuned for best results
    // just for the current music track
    rad = ofMap(spectrum[bandRad], 1, 3, 400, 800, true);
    vel = ofMap(spectrum[bandVel], 0, 0.1, 0.05, 0.5);

    // update particle positions
    for (int j = 0; j < n; j++) {
        tx[j] += vel * dt; // move offset
        ty[j] += vel * dt; // move offset

        // calculate perlin's noise in [-1, 1] and multiply on rad
        p[j].x = ofSignedNoise(tx[j]) * rad;
        p[j].y = ofSignedNoise(ty[j]) * rad;
    }
}

void ofApp::draw() {
    // ofBackground(255, 255, 255);

    // spectrum background
    // ofSetColor(230, 230, 230);
    // ofFill();

    // // ofRect(10, 700, N * 6, -100); // deprecated
    // ofDrawRectangle(10, 700, N * 6, -100);

    // // draw spectrum
    // ofSetColor(0, 0, 0);
    // for (int i = 0; i < N; i++) {
    //     // draw bandRad and bandVel in black and grey
    //     if (i == bandRad || i == bandVel) {
    //         ofSetColor(0, 0, 0);
    //     } else {
    //         ofSetColor(128, 128, 128);
    //     }
    //     ofDrawRectangle(10 + i * 5, 700, 3, -spectrum[i] * 100);
    // }

    ofBackground(0, 0, 0);
    // draw cloud
    ofPushMatrix();

    ofTranslate(ofGetWidth() / 2, ofGetHeight() / 2);
    // draw cloud points
    ofSetColor(255, 255, 255);
    ofFill();
    for (int i = 0; i < n; i++) {
        ofDrawCircle(p[i], 2);
    }
    // draw lines
    float dist = 20; // threshold parameter of distance
    for (int j = 0; j < n; j++) {
        for (int k = j + 1; k < n; k++) {
            if (ofDist(p[j].x, p[j].y, p[k].x, p[k].y) < dist) {
                ofDrawLine(p[j], p[k]);
            }
        }
    }

    ofPopMatrix();

}

// void ofApp::keyPressed(int key) { }
// void ofApp::keyReleased(int key) { }
// void ofApp::mouseMoved(int x, int y ) { }
// void ofApp::mouseDragged(int x, int y, int button) { }
// void ofApp::mousePressed(int x, int y, int button) { }
// void ofApp::mouseReleased(int x, int y, int button) { }
// void ofApp::mouseEntered(int x, int y) { }
// void ofApp::mouseExited(int x, int y) { }
// void ofApp::windowResized(int w, int h) { }
// void ofApp::gotMessage(ofMessage msg) { }
// void ofApp::dragEvent(ofDragInfo dragInfo) { }
