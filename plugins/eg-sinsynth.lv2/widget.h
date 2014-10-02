/*
  LV2 Sinewave synth plugin : based on eg-amp
  
  Copyright 2014 Harry van Haaren <harryhaaren@gmail.com>
  
  Permission to use, copy, modify, and/or distribute this software for any
  purpose with or without fee is hereby granted, provided that the above
  copyright notice and this permission notice appear in all copies.

  THIS SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
  WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
  MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
  ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
  WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
  ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
  OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
*/

#ifndef WIDGET
#define WIDGET

#include <FL/Fl.H>
#include <FL/Fl_Single_Window.H>

// LV2UI stuff
extern "C" {
#include "lv2/lv2plug.in/ns/extensions/ui/ui.h"
}

class Widget
{
  public:
    
    // public Lv2 communication stuff, gets set in instantiate
    LV2UI_Controller controller;
    LV2UI_Write_Function write_function;
    
    // xWindowID to embed into
    Widget(void* xWindowID);
    
    // gets called repeatedly to update the UI
    int idle();
    int show();
    int hide();
    
    int getWidth() {return win->w();}
    int getHeight(){return win->h();}
  
  protected:
    Fl_Window* win;
    
    // stores frequency we're currently at.
    float frequency;
};

#endif
