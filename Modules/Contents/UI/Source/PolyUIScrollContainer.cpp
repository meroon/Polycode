/*
 Copyright (C) 2012 by Ivan Safrin
 
 Permission is hereby granted, free of charge, to any person obtaining a copy
 of this software and associated documentation files (the "Software"), to deal
 in the Software without restriction, including without limitation the rights
 to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 copies of the Software, and to permit persons to whom the Software is
 furnished to do so, subject to the following conditions:
 
 The above copyright notice and this permission notice shall be included in
 all copies or substantial portions of the Software.
 
 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 THE SOFTWARE.
 */

#include "PolyUIScrollContainer.h"
#include "PolyConfig.h"
#include "PolyInputEvent.h"
#include "PolyLabel.h"
#include "PolyCoreServices.h"
#include <math.h>

using namespace Polycode;

UIScrollContainer::UIScrollContainer(ScreenEntity *scrolledEntity, bool hScroll, bool vScroll, Number width, Number height) : ScreenEntity() {
	
	scrolledEntity->setPositionMode(ScreenEntity::POSITION_TOPLEFT);
	
	scrolledEntity->rebuildTransformMatrix();
	Config *conf = CoreServices::getInstance()->getConfig();
	
	hasHScroll = hScroll;
	hasVScroll = vScroll;	
	
	
	this->width = width;
	this->height = height;
	this->hitwidth = width;
	this->hitheight = height;
	
	Number uiScrollPanePadding = conf->getNumericValue("Polycode", "uiScrollPanePadding");			
	
	defaultScrollSize = conf->getNumericValue("Polycode", "uiScrollDefaultSize");			
	
	maskShape = new ScreenShape(ScreenShape::SHAPE_RECT, width, height);
	maskShape->setPositionMode(ScreenEntity::POSITION_TOPLEFT);
	addChild(maskShape);
	maskShape->setColor(1.0f,0,0,0.8f);
	
	scrollChild = scrolledEntity;
	addChild(scrollChild);
	
	scrollChild->setMask(maskShape);
	
	
	vScrollBar = new UIVScrollBar(defaultScrollSize, height, height / scrolledEntity->getHeight());
	addChild(vScrollBar);
	vScrollBar->setPosition(width+uiScrollPanePadding,0);	
	vScrollBar->addEventListener(this, Event::CHANGE_EVENT);
	
	if(!vScroll)
		vScrollBar->enabled = false;
	
	hScrollBar = new UIHScrollBar(width, defaultScrollSize, width / scrolledEntity->getWidth());
	addChild(hScrollBar);
	hScrollBar->setPosition(0,height+uiScrollPanePadding);	
	hScrollBar->addEventListener(this, Event::CHANGE_EVENT);
	if(!hScroll)
		hScrollBar->enabled = false;
	
	
	
	setContentSize(scrollChild->getWidth()*scrollChild->getScale().x, scrollChild->getHeight()*scrollChild->getScale().y);
	Resize(width, height);	
}

void UIScrollContainer::Resize(int x, int y) {
	width = x;
	height = y;
	hitwidth = width;
	hitheight = height;
	
	maskShape->setShapeSize(x, y);
	vScrollBar->Resize(y);
	setContentSize(contentWidth, contentHeight);
	vScrollBar->setPositionY(0);
}

void UIScrollContainer::onMouseWheelUp(Number x, Number y) {
	if(vScrollBar->enabled)
		vScrollBar->scrollUpOneTick();
}

void UIScrollContainer::onMouseWheelDown(Number x, Number y) {
	if(vScrollBar->enabled)
		vScrollBar->scrollDownOneTick();
}


void UIScrollContainer::setContentSize(Number newContentWidth, Number newContentHeight) {
	
	contentHeight = newContentHeight;
	contentWidth = newContentWidth;
	
	vScrollBar->setHandleRatio(height / newContentHeight);	
	hScrollBar->setHandleRatio(width / newContentWidth);	
	
	if(hasVScroll) {
		if((height / newContentHeight) >= 1) {
			vScrollBar->enabled = false;
		} else {
			vScrollBar->enabled = true;		
		}
	}
	
	if(hasHScroll) {
		if((width / newContentWidth) >= 1) {
			hScrollBar->enabled = false;
		} else {
			hScrollBar->enabled = true;		
		}
	}	
}

void UIScrollContainer::handleEvent(Event *event) {
	if(event->getDispatcher() == vScrollBar) {
		if(event->getEventCode() == Event::CHANGE_EVENT) {
			scrollChild->setPositionY(floor(-((contentHeight-height) )*vScrollBar->getScrollValue()));
		}
	}
	
	if(event->getDispatcher() == hScrollBar) {
		if(event->getEventCode() == Event::CHANGE_EVENT) {
			scrollChild->setPositionX(floor(-((contentWidth-width) )*hScrollBar->getScrollValue()));
		}
	}
	
}

UIScrollContainer::~UIScrollContainer() {
	
}
