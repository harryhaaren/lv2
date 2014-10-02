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


// please see this link for details about the GUI aspect of LV2 programming
// http://harryhaaren.blogspot.ie/2012/07/writing-lv2-guis-making-it-look-snazzy.html

#include <string>
#include <iostream>

// X window ID

// include the URI and global data of this plugin
#include "sinsynth.h"

// core spec include

// GUI
#include "lv2/lv2plug.in/ns/lv2core/lv2.h"
#include "lv2/lv2plug.in/ns/extensions/ui/ui.h"

#include <FL/x.H>

// this is our custom widget include
#include "widget.h"

using namespace std;

typedef struct {
    Widget* widget;
} SinSynthGUI;


static LV2UI_Handle instantiate(const struct _LV2UI_Descriptor * descriptor,
                const char * plugin_uri,
                const char * bundle_path,
                LV2UI_Write_Function write_function,
                LV2UI_Controller controller,
                LV2UI_Widget * widget,
                const LV2_Feature * const * features)
{
    if (strcmp(plugin_uri, SINSYNTH_URI) != 0) {
        fprintf(stderr, "SINSYNTH_URI error: this GUI does not support plugin with URI %s\n", plugin_uri);
        return NULL;
    }
    
    void* parentXwindow = 0;
    
    LV2UI_Resize* resize = NULL;
    
    for (int i = 0; features[i]; ++i) {
      //cout << "feature " << features[i]->URI << endl;
      if (!strcmp(features[i]->URI, LV2_UI__parent)) {
        parentXwindow = features[i]->data;
        cout << "got parent UI feature: X11 id = " << (Window)parentXwindow << endl;
      } else if (!strcmp(features[i]->URI, LV2_UI__resize)) {
        resize = (LV2UI_Resize*)features[i]->data;
      }
    }
    
    
    SinSynthGUI* self = (SinSynthGUI*)malloc(sizeof(SinSynthGUI));
    
    cout << "Allocated GUI!" << endl;
    
    if (self == NULL) return NULL;
    
    cout << "Creating UI!" << endl;
    self->widget = new Widget( parentXwindow );
    
    cout << "Writing controller f(x)!" << endl;
    
    self->widget->controller = controller;
    self->widget->write_function = write_function;
    
    if (resize) {
      resize->ui_resize(resize->handle, self->widget->getWidth(),self->widget->getHeight());
    }
    
    //cout << "returning..." << int(self->widget->getXID()) << endl;
    
    return (LV2UI_Handle)self;
}

static void cleanup(LV2UI_Handle ui) {
    printf("cleanup()\n");
    SinSynthGUI *pluginGui = (SinSynthGUI *) ui;
    delete pluginGui->widget;
    free( pluginGui);
}

static void port_event(LV2UI_Handle ui,
               uint32_t port_index,
               uint32_t buffer_size,
               uint32_t format,
               const void * buffer)
{
    SinSynthGUI *self = (SinSynthGUI *) ui;
    
    cout << "Port event on index " << port_index << "  Format is " << format << endl;
    
      
    /*
    Fl::lock();
    ui->filterLowpass->value( argv[0]->f );
    Fl::unlock();
    Fl::awake();
    */
    
    /*
    if ( format == 0 )
    {
      float value =  *(float *)buffer;
      switch ( port_index )
      {
        case REFRACTOR_CONTROL_RETRIGGER:
            cout << "Refractor: Retrigger control event, value = " << value << endl;
            //self->guiState->retrigger = value;
            //self->widget->redraw();
            break;
        case REFRACTOR_MASTER_VOLUME:
            cout << "Refractor: Master volume event, value = " << value << endl;
            self->guiState->masterVol = value;
            self->widget->redraw();
      }
    }
    else
    {
      LV2_ATOM_SEQUENCE_FOREACH( (LV2_Atom_Sequence*)buffer, ev)
      {
        //self->frame_offset = ev->time.frames;
        
        if (ev->body.type == self->guiState->uris.midiEvent)
        {
          cout << "Refractor GUI got MIDI event!" << endl;
          //uint8_t* const data = (uint8_t* const)(ev + 1);
        }
      }
    }
    */
    
    return;
}

static int ui_show(LV2UI_Handle handle)
{
  SinSynthGUI* self = (SinSynthGUI*)handle;
  self->widget->show();
  return 0;
}

static int ui_hide(LV2UI_Handle handle)
{
  SinSynthGUI* self = (SinSynthGUI*)handle;
  return self->widget->hide();
}


static int idle(LV2UI_Handle handle)
{
  SinSynthGUI* self = (SinSynthGUI*)handle;
  self->widget->idle();
  return 0;
}


static const void*
extension_data(const char* uri)
{
  static const LV2UI_Show_Interface show_iface = { ui_show, ui_hide };
  static const LV2UI_Idle_Interface idle_iface = { idle };
  
  if (!strcmp(uri, LV2_UI__showInterface))
  {
    return (void*)&show_iface;
  }
  else if (!strcmp(uri, LV2_UI__idleInterface))
  {
    return &idle_iface;
  }
  
  return NULL;
}

static LV2UI_Descriptor descriptors[] = {
    {SINSYNTH_UI_URI, instantiate, cleanup, port_event, extension_data}
};

const LV2UI_Descriptor * lv2ui_descriptor(uint32_t index) {
    printf("lv2ui_descriptor(%u) called\n", (unsigned int)index); 
    if (index >= sizeof(descriptors) / sizeof(descriptors[0])) {
        return NULL;
    }
    return descriptors + index;
}
