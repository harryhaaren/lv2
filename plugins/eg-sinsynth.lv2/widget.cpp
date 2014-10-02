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

// this file is the UI, and creates widgets that NTK will show

#include "widget.h"

#include <FL/Fl.H>
#include <FL/Fl_Slider.H>
#include <FL/Fl_Double_Window.H>

#include <FL/x.H>
#include "lv2/lv2plug.in/ns/extensions/ui/ui.h"

#include <iostream>
using namespace std;

static void callback(Fl_Widget *w, void* userdata) {
  
  Widget* self = (Widget*)userdata;
  Fl_Slider* sliderWidget = (Fl_Slider*)w;
  
  // get value from the widget, and write a new frequency
  float newFrequency = 220 + 220 * (1-sliderWidget->value());
  
  // which port to write to, check your .ttl file for the index of
  // each port
  int portNumber = 0;
  
  // here we use the LV2UI_Controller and LV2UI_write_function "things"
  // to write some data to a port
  self->write_function( self->controller, portNumber, sizeof(float), 0, (const void*)&newFrequency);
}

Widget::Widget(void* parentXwindow)
{
  // In case FLTK hasn't set up yet
  fl_open_display();
  
  win = new Fl_Double_Window(100, 180);
  
  Fl_Slider* button = new Fl_Slider( 33,5,33,155,"Frequency");
  
  
  button->callback( callback, this );
  
  win->end();
  
  win->resizable( 0 );
  
  win->border(0);
  
  fl_embed( win, (Window)parentXwindow );
}

int Widget::show()
{
  win->show();
  return 0;
}

int Widget::hide()
{
  win->hide();
  return 0;
}

int Widget::idle()
{
  // takes care of handling events
  Fl::check();
  Fl::flush();
  
  // if window is closed during handling of events, notify the host that we've
  // closed the window.
  if ( !win->shown() )
    return 1;
  
  return 0;
};





















