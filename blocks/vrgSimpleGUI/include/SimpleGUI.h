/*
 *  SimpleGUI.h
 *
 *  Created by Marcin Ignac on 6/2/10.
 *  Copyright 2010 marcinignac.com.
 *
 */

#pragma once

#include <vector>
#include "cinder/app/App.h"
#include "cinder/Text.h"
#include "cinder/gl/Texture.h"
#include "EventListener.h"

using namespace ci;
using namespace ci::app;

namespace vrg { namespace sgui {
	
//-----------------------------------------------------------------------------
	
class Control;
class FloatVarControl;
class IntVarControl;
class BoolVarControl;
class LabelControl;
class SeparatorControl;
class ColumnControl;
class PanelControl;
class TextureVarControl;
class ColorVarControl;
	
//-----------------------------------------------------------------------------

class SimpleGUI {
private:
	Vec2f	mousePos;
	std::vector<Control*> controls;
	Control* selectedControl;
	
	CallbackId	cbMouseDown;
	CallbackId	cbMouseUp;
	CallbackId  cbMouseDrag;	

	void	init(App* app);	
public:
	static ColorA darkColor;
	static ColorA lightColor;
	static ColorA bgColor;
	static ColorA textColor;
	static float spacing;
	static Vec2f padding;
	static Vec2f sliderSize;
	static Vec2f labelSize;
	static Vec2f separatorSize;
	static Font textFont;
	
	enum {
		RGB,
		HSV
	};
public:
	SimpleGUI(App* app);
	bool	isSelected() { return selectedControl != NULL; }
	std::vector<Control*>& getControls() { return controls; }	
	
	bool	onMouseDown(MouseEvent event);
	bool	onMouseUp(MouseEvent event);
	bool	onMouseDrag(MouseEvent event);
	
	void	draw();
	void	dump();
	void	save(std::string fileName = "");
	void	load(std::string fileName = "");
	
	FloatVarControl* 	addParam(const std::string& paramName, float* var, float min=0, float max=1, float defaultValue = 0);
	IntVarControl*		addParam(const std::string& paramName, int* var, int min=0, int max=1, int defaultValue = 0);
	BoolVarControl*		addParam(const std::string& paramName, bool* var, bool defaultValue = false, int groupId = -1);
	ColorVarControl*	addParam(const std::string& paramName, ColorA* var, ColorA const defaultValue = ColorA(0, 1, 1, 1), int colorModel = RGB);
	TextureVarControl*	addParam(const std::string& paramName, gl::Texture* var, int scale = 1, bool flipVert = false);
	
	LabelControl*		addLabel(const std::string& labelName);	
	SeparatorControl*	addSeparator();	
	ColumnControl*		addColumn(int x = 0, int y = 0);	
	PanelControl*		addPanel();
	
	Control*			getControlByName(const std::string& name);
	
	static Vec2f		getStringSize(const std::string& str);		
	static Rectf		getScaledWidthRectf(Rectf rect, float scale);
};
	
//-----------------------------------------------------------------------------
	
	
class Control {
private:
	std::vector<EventListener*> updateListeners;	
	
public:
	enum Type {
		FLOAT_VAR,
		INT_VAR,
		BOOL_VAR,
		COLOR_VAR,
		TEXTURE_VAR,
		LABEL,
		SEPARATOR,
		COLUMN,
		PANEL
	};
	
	Vec2f	position;
	Rectf	activeArea;
	ColorA	bgColor;
	Type	type;
	std::string name;
	SimpleGUI* parentGui;
	
	Control();	
	virtual ~Control() {};
	void setBackgroundColor(ColorA color);	
	Control* addUpdateListener(EventListener* listener);	
	void notifyUpdateListeners();
	virtual Vec2f draw(Vec2f pos) = 0;
	virtual std::string toString() { return ""; };
	virtual void fromString(std::string& strValue) {};
	virtual void onMouseDown(MouseEvent event) {};
	virtual void onMouseUp(MouseEvent event) {};
	virtual void onMouseDrag(MouseEvent event) {};
};
	
//-----------------------------------------------------------------------------

class FloatVarControl : public Control {
public:	
	float* var;
	float min;
	float max;
public:
	FloatVarControl(const std::string& name, float* var, float min=0, float max=1, float defaultValue = 0);
	float getNormalizedValue();
	void setNormalizedValue(float value);
	Vec2f draw(Vec2f pos);
	std::string toString();
	void fromString(std::string& strValue);
	void onMouseDown(MouseEvent event);	
	void onMouseDrag(MouseEvent event);
};
	
//-----------------------------------------------------------------------------
	
class IntVarControl : public Control {
public:
	int* var;
	int min;
	int max;
public:
	IntVarControl(const std::string& name, int* var, int min=0, int max=1, int defaultValue = 0);
	float getNormalizedValue();
	void setNormalizedValue(float value);
	Vec2f draw(Vec2f pos);
	std::string toString();	
	void fromString(std::string& strValue);
	void onMouseDown(MouseEvent event);	
	void onMouseDrag(MouseEvent event);	
};
	
//-----------------------------------------------------------------------------

class BoolVarControl : public Control {
public:
	bool* var;
	int groupId;
public:
	BoolVarControl(const std::string& name, bool* var, bool defaultValue, int groupId);
	Vec2f draw(Vec2f pos);	
	std::string toString();	
	void fromString(std::string& strValue);
	void onMouseDown(MouseEvent event);
};
	
//-----------------------------------------------------------------------------

class ColorVarControl : public Control {
public:
	ColorA* var;
	Rectf	activeArea1;
	Rectf	activeArea2;
	Rectf	activeArea3;
	Rectf	activeArea4;	
	int		activeTrack;
	int		colorModel;
public:
	ColorVarControl(const std::string& name, ColorA* var, ColorA defaultValue, int colorModel);
	Vec2f draw(Vec2f pos);
	std::string toString();	//saved as "r g b a"
	void fromString(std::string& strValue); //expecting "r g b a"
	void onMouseDown(MouseEvent event);	
	void onMouseDrag(MouseEvent event);
};
	
//-----------------------------------------------------------------------------
	
class LabelControl : public Control {
public:
	LabelControl(const std::string& name);
	void setText(const std::string& text);
	Vec2f draw(Vec2f pos);	
};
	
//-----------------------------------------------------------------------------
	
class SeparatorControl : public Control {
public:
	SeparatorControl();
	Vec2f draw(Vec2f pos);	
};

//-----------------------------------------------------------------------------

class ColumnControl : public Control {
public:
	int x;
	int y;
	ColumnControl(int x = 0, int y = 0);
	Vec2f draw(Vec2f pos);	
}; 
	
//-----------------------------------------------------------------------------
 
class PanelControl : public Control {
public:
	bool enabled;
	PanelControl();
	Vec2f draw(Vec2f pos);
};
	

//-----------------------------------------------------------------------------

class TextureVarControl : public Control {
public:
	gl::Texture* var;
	float scale;
	bool flipVert;	
	TextureVarControl(const std::string& name, gl::Texture* var, int scale, bool flipVert = false);
	Vec2f draw(Vec2f pos);	
};
		
//-----------------------------------------------------------------------------

} //namespace sgui
} //namespace vrg