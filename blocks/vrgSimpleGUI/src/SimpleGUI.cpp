/*
 *  SimpleGUI.cpp
 *
 *  Created by Marcin Ignac on 6/2/10.
 *  Copyright 2010 marcinignac.com.
 *
 *	Temptesta Seven font by Yusuke Kamiyamane http://p.yusukekamiyamane.com/fonts/
 *
 */

#include "SimpleGUI.h"
#include "cinder/Utilities.h"
#include "cinder/Font.h"
#include "cinder/CinderMath.h"

namespace vrg { namespace sgui {
	
//-----------------------------------------------------------------------------
	
ColorA SimpleGUI::darkColor = ColorA(0.3, 0.3, 0.3, 1);
ColorA SimpleGUI::lightColor = ColorA(1, 1, 1, 1);
ColorA SimpleGUI::bgColor = ColorA(0, 0, 0, 0.5);
ColorA SimpleGUI::textColor = ColorA(1,1,1,1);	
	
void SimpleGUI::init() {	
	textFont = Font(loadResource("pf_tempesta_seven.ttf"), 8);
	//textFont = Font("Arial", 12);
	selectedControl = NULL;
}

FloatVarControl* SimpleGUI::addParam(const std::string& paramName, float* var, float min, float max, float defaultValue) {
	FloatVarControl* control = new FloatVarControl(paramName, var, min, max, defaultValue);
	controls.push_back(control);
	return control;
}

IntVarControl* SimpleGUI::addParam(const std::string& paramName, int* var, int min, int max, int defaultValue) {
	IntVarControl* control = new IntVarControl(paramName, var, min, max, defaultValue);
	controls.push_back(control);
	return control;
}

BoolVarControl* SimpleGUI::addParam(const std::string& paramName, bool* var, bool defaultValue, int groupId) {
	BoolVarControl* control = new BoolVarControl(paramName, var, defaultValue, groupId);
	controls.push_back(control);
	return control;
}
	
ColorVarControl* SimpleGUI::addParam(const std::string& paramName, ColorA* var, ColorA defaultValue) {
	ColorVarControl* control = new ColorVarControl(paramName, var, defaultValue);
	controls.push_back(control);
	return control;
}

TextureVarControl* SimpleGUI::addParam(const std::string& paramName, gl::Texture* var, int scale, bool flipVert) {
	TextureVarControl* control = new TextureVarControl(paramName, var, scale, flipVert);
	controls.push_back(control);
	return control;
}
	
LabelControl* SimpleGUI::addLabel(const std::string& labelName) {
	LabelControl* control = new LabelControl(labelName);
	controls.push_back(control);
	return control;
}

SeparatorControl* SimpleGUI::addSeparator() {
	SeparatorControl* control = new SeparatorControl();
	controls.push_back(control);
	return control;
}
	
ColumnControl* SimpleGUI::addColumn(int x, int y) {
	ColumnControl* control = new ColumnControl(x, y);
	controls.push_back(control);
	return control;
}

PanelControl* SimpleGUI::addPanel() {
	PanelControl* control = new PanelControl();
	controls.push_back(control);
	return control;
}
	
void SimpleGUI::draw() {
	
	gl::setMatricesWindow(getWindowSize());
	gl::disableDepthRead();	
	gl::disableDepthWrite();		
	gl::enableAlphaBlending();

	//Color8u darkColor = Color8u(150, 150, 0);
	//Color8u lightColor = Color8u(255, 255, 50);
	float spacing = 10;
	Vec2f startPos = Vec2f(spacing, spacing);;
	Vec2f pos = Vec2f(spacing, spacing);
	Vec2f padding = Vec2f(3, 3);
	Vec2f sliderSize = Vec2f(125, 10);
	Vec2f labelSize = Vec2f(125, 10);
	Vec2f separatorSize = Vec2f(125, 1);

	int numColumns = 0;
	FloatVarControl* floatVar;
	IntVarControl* intVar;
	BoolVarControl* boolVar;
	LabelControl* label;	
	ColumnControl* column;
	PanelControl* panel = NULL;
	TextureVarControl* textureVar;
	ColorVarControl* colorVar;
	Vec2f textureSize;
	Vec3f hsv;
	Rectf rect1;
	Rectf rect2;
	Rectf rect3;
	Rectf rect4;	
	
	std::vector<Control*>::iterator it = controls.begin();
	while(it != controls.end()) {
		Control* control = *it++;
		control->position = pos;
		
		if (control->type != Control::PANEL && control->type != Control::COLUMN) {
			if (panel && panel->enabled == false) {
				continue;
			}
		}
		
		switch(control->type) {
			case Control::FLOAT_VAR:
				floatVar = (FloatVarControl*)control;
				control->activeArea = Rectf(pos.x, pos.y + labelSize.y + padding.y, pos.x + sliderSize.x, pos.y + labelSize.y + padding.y + sliderSize.y);				
				gl::color(bgColor);
				gl::drawSolidRect(Rectf((pos - padding).x, (pos - padding).y, (pos + sliderSize + padding).x, (pos + labelSize + sliderSize + padding*2).y));
				gl::drawString(floatVar->name, pos, textColor, textFont);							
				gl::color(darkColor);
				gl::drawSolidRect(control->activeArea);
				gl::color(lightColor);
				gl::drawSolidRect(getScaledWidthRectf(control->activeArea, floatVar->getNormalizedValue()));					
				pos.y += labelSize.y + sliderSize.y;
				break;
			case Control::INT_VAR:
				intVar = (IntVarControl*)control;
				control->activeArea = Rectf(pos.x, pos.y + labelSize.y + padding.y, pos.x + sliderSize.x, pos.y + labelSize.y + padding.y + sliderSize.y);								
				gl::color(bgColor);
				gl::drawSolidRect(Rectf((pos - padding).x, (pos - padding).y, (pos + sliderSize + padding).x, (pos + labelSize + sliderSize + padding*2).y));
				gl::drawString(intVar->name, pos, textColor, textFont);	
				gl::color(darkColor);
				gl::drawSolidRect(control->activeArea);
				gl::color(lightColor);
				gl::drawSolidRect(getScaledWidthRectf(control->activeArea, intVar->getNormalizedValue()));				
				pos.y += labelSize.y + sliderSize.y;
				break;
			case Control::BOOL_VAR:
				boolVar = (BoolVarControl*)control;
				control->activeArea = Rectf(pos.x, pos.y, pos.x + sliderSize.y, pos.y + sliderSize.y);	
				gl::color(bgColor);
				gl::drawSolidRect(Rectf((pos - padding).x, (pos - padding).y, (pos + sliderSize + padding).x, (pos + sliderSize + padding).y));
				gl::drawString(boolVar->name, Vec2f(pos.x + sliderSize.y + padding.x*2, pos.y), textColor, textFont);					
				gl::color((*boolVar->var) ? lightColor : darkColor);
				gl::drawSolidRect(control->activeArea);
				pos.y += sliderSize.y;				
				break;
			case Control::COLOR_VAR:
				colorVar = (ColorVarControl*)control;
				colorVar->activeArea1 = Rectf(pos.x, pos.y + labelSize.y + padding.y, pos.x + sliderSize.x, pos.y + sliderSize.y + labelSize.y + padding.y);	
				colorVar->activeArea2 = Rectf(pos.x, pos.y + labelSize.y + sliderSize.y + padding.y*2, pos.x + sliderSize.x, pos.y + sliderSize.y*2 + padding.y*2 + labelSize.y);	
				colorVar->activeArea3 = Rectf(pos.x, pos.y + labelSize.y + sliderSize.y*2 + padding.y*3, pos.x + sliderSize.x, pos.y + sliderSize.y*3 + padding.y*3 + labelSize.y);	
				colorVar->activeArea4 = Rectf(pos.x, pos.y + labelSize.y + sliderSize.y*3 + padding.y*4, pos.x + sliderSize.x, pos.y + sliderSize.y*4 + padding.y*4 + labelSize.y);	
				colorVar->activeArea = Rectf(pos.x, pos.y + labelSize.y, pos.x + sliderSize.x, pos.y + sliderSize.y*4 + padding.y*4 + labelSize.y);	
				hsv = rgbToHSV(*colorVar->var);
				///hsv = Vec3f(colorVar->var->r, colorVar->var->g, colorVar->var->b);
				gl::color(bgColor);
				gl::drawSolidRect(Rectf((pos - padding).x, (pos - padding).y, (pos + sliderSize + padding).x, (pos + labelSize + sliderSize*4 + padding*5).y));
				gl::drawString(colorVar->name, pos, textColor, textFont);	
				gl::color(darkColor);
				gl::drawSolidRect(colorVar->activeArea1);
				gl::drawSolidRect(colorVar->activeArea2);
				gl::drawSolidRect(colorVar->activeArea3);
				gl::drawSolidRect(colorVar->activeArea4);
				gl::color(lightColor);
				rect1 = getScaledWidthRectf(colorVar->activeArea1, hsv.x);
				rect2 = getScaledWidthRectf(colorVar->activeArea2, hsv.y);
				rect3 = getScaledWidthRectf(colorVar->activeArea3, hsv.z);
				rect4 = getScaledWidthRectf(colorVar->activeArea4, colorVar->var->a);
				gl::drawLine(Vec2f(rect1.x2, rect1.y1), Vec2f(rect1.x2, rect1.y2));				
				gl::drawLine(Vec2f(rect2.x2, rect2.y1), Vec2f(rect2.x2, rect2.y2));				
				gl::drawLine(Vec2f(rect3.x2, rect3.y1), Vec2f(rect3.x2, rect3.y2));				
				gl::drawLine(Vec2f(rect4.x2, rect4.y1), Vec2f(rect4.x2, rect4.y2));				
				pos.y += labelSize.y + sliderSize.y * 4 + padding.y * 3;		
				break;
			case Control::TEXTURE_VAR:
				textureVar = (TextureVarControl*)control;
				if (!*textureVar->var) {
					std::cout << "Missing texture" << std::endl;
					break;				
				}

				textureSize.x = math<float>::floor(sliderSize.x * textureVar->scale + spacing * (textureVar->scale - 1));
				textureSize.y = math<float>::floor(textureSize.x / textureVar->var->getAspectRatio());
				//textureVar->activeArea = Rectf(pos.x - padding.x, pos.y - padding.y, pos.x + textureSize.x + padding.x, pos.y + textureSize.y + padding.y);	
				textureVar->activeArea = Rectf(pos.x, pos.y, pos.x + textureSize.x, pos.y + textureSize.y);	
				gl::color(bgColor);
				gl::drawSolidRect(Rectf(textureVar->activeArea.x1 - padding.x, textureVar->activeArea.y1 - padding.y, textureVar->activeArea.x2 + padding.x, textureVar->activeArea.y2 + padding.y));
				gl::color(ColorA(1,1,1,1));
				textureVar->var->enableAndBind();
				glBegin(GL_QUADS);
				glTexCoord2f(0, textureVar->flipVert ? 1 : 0);
				glVertex2f(textureVar->activeArea.x1, textureVar->activeArea.y1);
				glTexCoord2f(0, textureVar->flipVert ? 0 : 1);
				glVertex2f(textureVar->activeArea.x1, textureVar->activeArea.y2);				
				glTexCoord2f(1, textureVar->flipVert ? 0 : 1);
				glVertex2f(textureVar->activeArea.x2, textureVar->activeArea.y2);				
				glTexCoord2f(1, textureVar->flipVert ? 1 : 0);
				glVertex2f(textureVar->activeArea.x2, textureVar->activeArea.y1);						
				glEnd();				
				textureVar->var->unbind();
				textureVar->var->disable();
				pos.y += control->activeArea.getHeight() - padding.y * 2;
				break;
			case Control::LABEL:
				label = (LabelControl*)control;				
				if (label->bgColor) {
					gl::color(label->bgColor);
				}
				else {
					gl::color(bgColor);
				}
				gl::drawSolidRect(Rectf((pos - padding).x, (pos - padding).y, (pos + sliderSize + padding).x, (pos + labelSize + padding).y));				
				gl::drawString(label->name, pos, textColor, textFont);					
				pos.y += labelSize.y;				
				break;
			case Control::SEPARATOR:				
				control->activeArea = Rectf(pos - padding, pos + separatorSize + padding);
				gl::color(bgColor);
				gl::drawSolidRect(control->activeArea);				
				pos.y += separatorSize.y;
				break;
			case Control::COLUMN:
				column = (ColumnControl*)control;
				if (column->x > 0 || column->y > 0) {
					pos.x = column->x;
					pos.y = column->y;
					startPos = Vec2f(pos.x, pos.y);
				}
				else {
					if (numColumns > 0) {
						pos.x += (sliderSize.x + spacing);
						pos.y = startPos.y;
					}
					else {
						pos.x = spacing;
						pos.y = spacing;
					}
				}
				numColumns++;
				panel = NULL;
				break;
			case Control::PANEL:
				panel = (PanelControl*)control;
				break;
				
		}
		if (control->type != Control::COLUMN && control->type != Control::PANEL) {
			pos.y += spacing;
		}
	}

	gl::disableAlphaBlending();
	gl::enableDepthRead();
	gl::enableDepthWrite();	
	
}
	
void SimpleGUI::dump() {
	FloatVarControl* floatVar;
	IntVarControl* intVar;
	BoolVarControl* boolVar;
	LabelControl* label;	
	ColumnControl* column;
	PanelControl* panel = NULL;
	TextureVarControl* textureVar;
	ColorVarControl* colorVar;
	
	std::vector<Control*>::iterator it = controls.begin();
	while(it != controls.end()) {
		Control* control = *it++;		
		switch(control->type) {
			case Control::FLOAT_VAR:
				floatVar = (FloatVarControl*)control;
				std::cout << floatVar->name << ": " << *floatVar->var << std::endl;
				break;
			case Control::INT_VAR:
				intVar = (IntVarControl*)control;
				std::cout << intVar->name << ": " << *intVar->var << std::endl;				
				break;
			case Control::BOOL_VAR:
				boolVar = (BoolVarControl*)control;
				std::cout << boolVar->name << ": " << *boolVar->var << std::endl;				
				break;
			case Control::COLOR_VAR:
				colorVar = (ColorVarControl*)control;
				std::cout << colorVar->name << ": " << colorVar->var->r << "," << colorVar->var->g << "," << colorVar->var->b << "," << colorVar->var->a << std::endl;				
				break;				
			case Control::TEXTURE_VAR:
				textureVar = (TextureVarControl*)control;
				break;
			case Control::LABEL:
				label = (LabelControl*)control;				
				break;
			case Control::SEPARATOR:				
				break;
			case Control::COLUMN:
				column = (ColumnControl*)control;			
				break;
			case Control::PANEL:
				panel = (PanelControl*)control;
				break;
				
		}		
	}
}

bool SimpleGUI::mouseDown(MouseEvent event) {
	std::vector<Control*>::iterator it = controls.begin();	
	while(it != controls.end()) {
		Control* control = *it++;
		if (control->activeArea.contains(event.getPos())) {
			selectedControl = control;
			if (selectedControl->type == Control::COLOR_VAR) {
				ColorVarControl* colorVar = (ColorVarControl*)selectedControl;
				if (colorVar->activeArea1.contains(event.getPos())) {
					colorVar->activeTrack = 1;
				}
				else if (colorVar->activeArea2.contains(event.getPos())) {
					colorVar->activeTrack = 2;
				}
				else if (colorVar->activeArea3.contains(event.getPos())) {
					colorVar->activeTrack = 3;
				}
				else if (colorVar->activeArea4.contains(event.getPos())) {
					colorVar->activeTrack = 4;
				}
			}
			mouseDrag(event);	
			if (selectedControl->type == Control::BOOL_VAR) {
				BoolVarControl* boolVarControl = (BoolVarControl*)selectedControl;				
				if (boolVarControl->groupId > -1) {
					for(std::vector<Control*>::iterator it2 = controls.begin(); it2 != controls.end(); it2++) {
						if (((*it2)->type == Control::BOOL_VAR) && (((BoolVarControl*)(*it2))->groupId == boolVarControl->groupId)) {
							*((BoolVarControl*)(*it2))->var = (*it2 == boolVarControl);
						}						
					}
				}
				else {
					*boolVarControl->var = ! *boolVarControl->var;
				}
			}			
			break;
			return true;
		}
	}	
	return false;
}

bool SimpleGUI::mouseUp(MouseEvent event) {
	selectedControl = NULL;
	return false;
}
	
bool SimpleGUI::mouseDrag(MouseEvent event) {
	mousePos = event.getPos();
	
	float a;
	Vec3f hsv;
	FloatVarControl* floatVar;
	IntVarControl* intVar;
	ColorVarControl* colorVar;
	if (selectedControl) {
		float value = (event.getPos().x - selectedControl->activeArea.x1)/(selectedControl->activeArea.x2 - selectedControl->activeArea.x1);
		value = math<float>::max(0.0, math<float>::min(value, 1.0));
		switch(selectedControl->type) {
			case Control::FLOAT_VAR:
				floatVar = (FloatVarControl*)selectedControl;
				floatVar->setNormalizedValue(value);				
				break;
			case Control::INT_VAR:
				intVar = (IntVarControl*)selectedControl;
				intVar->setNormalizedValue(value);				
				break;
			case Control::COLOR_VAR:
				colorVar = (ColorVarControl*)selectedControl;
				hsv = rgbToHSV(*colorVar->var);
				a = colorVar->var->a;
				//hsv = Vec3f(colorVar->var->r, colorVar->var->g, colorVar->var->b);
				if (colorVar->activeTrack == 1) {
					//colorVar->var->r = value;
					hsv.x = value;
				}
				if (colorVar->activeTrack == 2) {
					//colorVar->var->g = value;
					hsv.y = value;
				}
				if (colorVar->activeTrack == 3) {
					//colorVar->var->b = value;
					hsv.z = value;
				}
				if (colorVar->activeTrack == 4) {
					//colorVar->var->b = value;
					a = value;
				}
				*colorVar->var = ColorA(CM_HSV, hsv.x, hsv.y, hsv.z, a);
				colorVar->notifyUpdateListeners();
				break;
				
		}
		return true;
	}
	return false;
}
	
Vec2f SimpleGUI::getStringSize(const std::string& str) {
	TextLayout text;
	text.addLine(str);
	Surface s = text.render(true);
	return s.getSize();
}
	
Rectf SimpleGUI::getScaledWidthRectf(Rectf rect, float scale) {
	return Rectf(
		rect.getX1(),
		rect.getY1(),
		rect.getX1() + rect.getWidth() * scale,
		rect.getY1() + rect.getHeight()
	);
}
	
//-----------------------------------------------------------------------------

} //namespace sgui
} //namespace vrg