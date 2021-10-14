#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup() {

	ofSetFrameRate(60);
	ofSetWindowTitle("openframeworks");

	ofBackground(239);
	ofSetLineWidth(2);
	ofEnableDepthTest();
	ofSetRectMode(ofRectMode::OF_RECTMODE_CENTER);

	this->font_size = 195;
	ofTrueTypeFontSettings font_settings("fonts/msgothic.ttc", this->font_size);
	font_settings.antialiased = true;
	font_settings.addRanges(ofAlphabet::Japanese);
	this->font.load(font_settings);

	ofFbo fbo;
	ofPixels pixcels;
	fbo.allocate(320, 320);

	fbo.begin();

	ofClear(0);
	ofBackground(0);
	ofTranslate(fbo.getWidth() * 0.5, fbo.getHeight() * 0.5);

	ofSetColor(255);
	this->font.drawString(u8"壱", -130, 90);

	ofNoFill();
	ofDrawRectangle(glm::vec2(0, 0), 300, 300);

	fbo.end();
	
	fbo.readToPixels(pixcels);
	this->pixcels_list.push_back(pixcels);

	fbo.begin();

	ofClear(0);
	ofBackground(0);
	ofTranslate(fbo.getWidth() * 0.5, fbo.getHeight() * 0.5);

	ofSetColor(255);
	this->font.drawString(u8"弐", -130, 90);

	ofNoFill();
	ofDrawRectangle(glm::vec2(0, 0), 300, 300);

	fbo.end();

	fbo.readToPixels(pixcels);
	this->pixcels_list.push_back(pixcels);
}

//--------------------------------------------------------------
void ofApp::update() {

	this->face_list.clear();

	int x_span = 20;
	int y_span = 5;

	for (int i = 0; i < 2; i++) {

		vector<ofMesh> mesh_list;
		for (int x = 0; x < 320; x += x_span) {

			ofMesh mesh;
			for (int y = 0; y < 320; y += y_span) {

				int index = mesh.getNumVertices();

				float z = -160;

				vector<glm::vec3> vertices;
				vertices.push_back(glm::vec3(x, y, z) - glm::vec3(160, 160, 0));
				vertices.push_back(glm::vec3(x + x_span, y, z) - glm::vec3(160, 160, 0));
				vertices.push_back(glm::vec3(x + x_span, y + y_span, z) - glm::vec3(160, 160, 0));
				vertices.push_back(glm::vec3(x, y + y_span, z) - glm::vec3(160, 160, 0));

				auto deg = (x / 3 + ofGetFrameNum()) % 360;
				if (deg < 90) { deg = 0; }
				else if (deg < 180) { deg = ofMap(deg, 90, 180, 0, 180); }
				else if (deg < 270) { deg = 180; }
				else if (deg < 360) { deg = ofMap(deg, 270, 360, 180, 360); }

				deg += i * 180;

				auto rotation_x = glm::rotate(glm::mat4(), deg * (float)DEG_TO_RAD, glm::vec3(1, 0, 0));
				for (auto& vertex : vertices) {

					vertex = glm::vec4(vertex, 0) * rotation_x;
				}

				mesh.addVertices(vertices);

				mesh.addTexCoord(glm::vec3(x, y, 0));
				mesh.addTexCoord(glm::vec3(x + x_span, y, 0));
				mesh.addTexCoord(glm::vec3(x + x_span, y + y_span, 0));
				mesh.addTexCoord(glm::vec3(x, y + y_span, 0));

				mesh.addIndex(index + 0); mesh.addIndex(index + 1); mesh.addIndex(index + 2);
				mesh.addIndex(index + 0); mesh.addIndex(index + 2); mesh.addIndex(index + 3);
			}

			mesh_list.push_back(mesh);
		}

		this->face_list.push_back(mesh_list);
	}
}

//--------------------------------------------------------------
void ofApp::draw() {

	this->cam.begin();
	ofRotateX(180);

	int i = 0;
	for (auto& mesh_list : this->face_list) {

		ofImage image;
		image.setFromPixels(this->pixcels_list[i++]);

		for (auto& mesh : mesh_list) {

			image.bind();
			mesh.draw();
			image.unbind();
		}
	}

	this->cam.end();
}

//--------------------------------------------------------------
int main() {

	ofSetupOpenGL(720, 720, OF_WINDOW);
	ofRunApp(new ofApp());
}