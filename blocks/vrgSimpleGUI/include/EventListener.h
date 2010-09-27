/*
 *  IUpdateable.h
 *  Identity06
 *
 *  Created by Marcin Ignac on 6/28/10.
 *  Copyright 2010 marcinignac.com. All rights reserved.
 *
 */

#pragma once

namespace vrg { namespace sgui {

class EventListener {
public:
	virtual void onUpdate() = 0;
};

}}