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
	Font	textFont;
	std::vector<Control*> controls;
	Control* selectedControl;

	void	init();
	Vec2f	getStringSize(const std::string& str);
	Rectf	getScaledWidthRectf(Rectf rect, float scale);
	
public:
	static ColorA darkColor;
	static ColorA lightColor;
	static ColorA bgColor;
	static ColorA textColor;
	
public:
	SimpleGUI()	{ init(); }
	bool	isSelected() { return selectedControl != NULL; }
	
	bool	mouseDown(MouseEvent event);
	bool	mouseUp(MouseEvent event);
	bool	mouseDrag(MouseEvent event);
	void	draw();
	void	dump();
	
	FloatVarControl* 	addParam(const std::string& paramName, float* var, float min=0, float max=1, float defaultValue = 0);
	IntVarControl*		addParam(const std::string& paramName, int* var, int min=0, int max=1, int defaultValue = 0);
	BoolVarControl*		addParam(const std::string& paramName, bool* var, bool defaultValue = false, int groupId = -1);
	ColorVarControl*	addParam(const std::string& paramName, ColorA* var, ColorA const defaultValue = ColorA(0, 1, 1, 1));	//CM_HSV
	TextureVarControl*	addParam(const std::string& paramName, gl::Texture* var, int scale = 1, bool flipVert = false);
	
	LabelControl*		addLabel(const std::string& labelName);	
	SeparatorControl*	addSeparator();	
	ColumnControl*		addColumn(int x = 0, int y = 0);	
	PanelControl*		addPanel();	
	
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
	
	Control() {
		bgColor = ColorA(0,0,0,0.5);
	}
	
	void setBackgroundColor(ColorA color) {
		bgColor = color;
	}
	
	Control* addUpdateListener(EventListener* listener) {
		updateListeners.push_back(listener);
		return this;
	}
	
	void notifyUpdateListeners() {
		for(int i = 0; i<updateListeners.size(); i++) {
			updateListeners[i]->onUpdate();
		}
	}
};
	
//-----------------------------------------------------------------------------

class FloatVarControl : public Control {
public:	
	float* var;
	float min;
	float max;
public:
	FloatVarControl(const std::string& name, float* var, float min=0, float max=1, float defaultValue = 0) {
		this->type = Control::FLOAT_VAR;
		this->name = name;
		this->var = var;
		this->min = min;
		this->max = max;
		if (defaultValue < min) {
			*var = min;
		}
		else if (defaultValue > max) {
			*var = max;
		}
		else {
			*var = defaultValue;
		}
	}
	float getNormalizedValue() {
		return (*var - min)/(max - min);
	}
	void setNormalizedValue(float value) {
		float newValue = min + value*(max - min);
		if (newValue != *var) {
			*var = newValue;
			notifyUpdateListeners();
		}
	}
};
	
//-----------------------------------------------------------------------------
	
class IntVarControl : public Control {
public:
	int* var;
	int min;
	int max;
public:
	IntVarControl(const std::string& name, int* var, int min=0, int max=1, int defaultValue = 0) {
		this->type = Control::INT_VAR;
		this->name = name;
		this->var = var;
		this->min = min;
		this->max = max;
		if (defaultValue < min) {
			*var = min;
		}
		else if (defaultValue > max) {
			*var = max;
		}
		else {
			*var = defaultValue;
		}
	}
	float getNormalizedValue() {
		return (*var - min)/(float)(max - min);
	}
	void setNormalizedValue(float value) {
		int newValue = min + value*(max - min);
		if (newValue != *var) {
			*var = newValue;
			notifyUpdateListeners();
		}
	}
};
	
//-----------------------------------------------------------------------------

class BoolVarControl : public Control {
public:
	bool* var;
	int groupId;
public:
	BoolVarControl(const std::string& name, bool* var, bool defaultValue, int groupId) {
		this->type = Control::BOOL_VAR;
		this->name = name;
		this->var = var;
		this->groupId = groupId;
		*var = defaultValue;
	}
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
public:
	ColorVarControl(const std::string& name, ColorA* var, ColorA defaultValue) {
		this->type = Control::COLOR_VAR;
		this->name = name;
		this->var = var;
		*var = defaultValue;
		activeTrack = 0;
	}
};
	
//-----------------------------------------------------------------------------
	
class LabelControl : public Control {
public:
	std::string name;	
public:
	LabelControl(const std::string& name) {
		this->type = Control::LABEL;
		this->name = name;		
	}
	void setText(const std::string &text) {
		name = text;
	}
};
	
//-----------------------------------------------------------------------------
	
class SeparatorControl : public Control {
public:
	SeparatorControl() {
		this->type = Control::SEPARATOR;
	}	
};

//-----------------------------------------------------------------------------

class ColumnControl : public Control {
public:
	int x;
	int y;
	ColumnControl(int x = 0, int y = 0) {
		this->x = x;
		this->y = y;
		this->type = Control::COLUMN;
	}	
}; 
	
//-----------------------------------------------------------------------------
 
class PanelControl : public Control {
public:
	bool enabled;
	PanelControl() {
		this->enabled = true;
		this->type = Control::PANEL;
	}
};
	

//-----------------------------------------------------------------------------

class TextureVarControl : public Control {
public:
	gl::Texture* var;
	float scale;
	bool flipVert;
	
	TextureVarControl(const std::string& name, gl::Texture* var, int scale, bool flipVert = false) {
		this->type = Control::TEXTURE_VAR;
		this->name = name;
		this->var = var;
		this->scale = scale;
		this->flipVert = flipVert;
	}
};
		
//-----------------------------------------------------------------------------

} //namespace sgui
} //namespace vrg