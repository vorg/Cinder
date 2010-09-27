#include "cinder/app/AppBasic.h"
#include "SimpleGUI.h"
#include <list>
using namespace ci;
using namespace ci::app;
using namespace std;
using namespace vrg::sgui;

class BasicApp : public AppBasic {
private:
	SimpleGUI gui;
	float rotation;
	int size;
	ColorA color;
	bool autoRotation;
	
	Timer timer;
	float prevTime;
	
public:
	void mouseDown( MouseEvent event );
	void mouseDrag( MouseEvent event );
	void mouseUp( MouseEvent event );
	void draw();	
	void setup();
};

void BasicApp::setup() {
	gui.lightColor = ColorA(1, 1, 0, 1);	
	gui.addParam("Rotation", &rotation, 0, 360, 0);
	gui.addParam("Size", &size, 100, 300, 150); 	
	gui.addParam("Color", &color, ColorA(0,0.5,1,1)); //the color is in HSV color space
	gui.addSeparator();
	gui.addParam("Auto Rotation", &autoRotation, false); 
	
	timer.start();
	prevTime = timer.getSeconds();
}

void BasicApp::mouseDown( MouseEvent event ) {
	gui.mouseDown(event);
}

void BasicApp::mouseDrag( MouseEvent event ) {
	gui.mouseDrag(event);	
}

void BasicApp::mouseUp( MouseEvent event ) {
	gui.mouseUp(event);	
}

void BasicApp::draw() {
	float currTime = timer.getSeconds();
	float deltaTime = currTime - prevTime;
	prevTime = currTime;

	if (autoRotation) {
		rotation += deltaTime * 60;
	}
	
	gl::clear(ColorA(0,0,0,0));
	gl::translate(Vec2f(getWindowWidth()/2, getWindowHeight()/2));
	gl::rotate(rotation);
	gl::color(color);
	gl::drawSolidRect(Rectf(-size/2, -size/2, size/2, size/2));
	
	gui.draw();
}


CINDER_APP_BASIC( BasicApp, RendererGl )
