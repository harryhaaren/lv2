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

#include <math.h>
#include <stdlib.h>
#include <string.h>

// open this file, and read it! It contains essential data for this part
// of the plugin (the audio bit), but the GUI part also needs the same
// data, that is why its in a seperate header.
#include "sinsynth.h"

typedef enum {
  AMP_FREQ   = 0,
  AMP_OUTPUT,
  AMP_MIDI,
} PortIndex;

typedef struct {
  // nessisary to generate a sin wave of the correct frequency
  double sample_rate;

  LV2_Event_Buffer *MidiIn;
  LV2_Event_Iterator in_iterator;

  LV2_Event_Feature* event_ref;
  int midi_event_id;
  float* channel_p;

  // used internally to keep track of where we are in the wave
  float phase;

  float* freq;
  float* input;
  float* output;
} SinSynth;

static LV2_Handle
instantiate(const LV2_Descriptor*     descriptor,
            double                    rate,
            const char*               bundle_path,
            const LV2_Feature* const* features)
{
  SinSynth* self = (SinSynth*)malloc(sizeof(SinSynth));

  // store the sample rate in "self" so we can retrieve it in run()
  self->sample_rate = rate;

  // initialize the phase so we start at the beginning of the wave
  self->phase = 0.f;

  return (LV2_Handle)self;
}

void
connect_port(LV2_Handle instance,
             uint32_t   port,
             void*      data)
{
  SinSynth* self=(SinSynth*)instance;

  switch (port) {
  case AMP_FREQ:
    self->freq = (float*)data;
    break;
  case AMP_MIDI:
    self->MidiIn = data;
    break;
  case AMP_OUTPUT:
    self->output = (float*)data;
    break;
  }
}

static void
activate(LV2_Handle instance)
{
}

static void
run(LV2_Handle instance, uint32_t n_samples)
{
  SinSynth* self = (SinSynth*)instance;

  lv2_event_begin(&self->in_iterator,self->MidiIn);

  const float        PI     = 3.1415;
  const float        volume = 0.3;
  const float        freq   = *(self->freq);
  float* const       output = self->output;

  float samples_per_cycle = self->sample_rate / freq;

  // calculate the phase offset per sample: phase ranges from 0->1,
  // so phase_increment is a floating point number so that we get
  // "freq" number of cycles in "sample_rate" amount of samples.
  float phase_increment = (1.f / samples_per_cycle);

  // synthesize the sinwave here
  for (uint32_t pos = 0; pos < n_samples; pos++) {


    while(lv2_event_is_valid(&self->in_iterator)) {
      uint8_t* data;
      LV2_Event* event= lv2_event_get(&self->in_iterator,&data);
      if (event->type == 0) {
        printf("Note!");
        self->event_ref->lv2_event_unref(self->event_ref->callback_data, event);
      } else if(event->type==self->midi_event_id) {
        printf("Note!");
        if(event->frames > pos) {
          break;
        } else {
          printf("Note!");
          const uint8_t* evt=(uint8_t*)data;
          if((evt[0]&MIDI_CHANNELMASK)==(int) (*self->channel_p)) {
            if((evt[0]&MIDI_COMMANDMASK)==MIDI_NOTEON) 	{
              printf("Note!");
              unsigned int note = evt[1];
            }
            else if((evt[0]&MIDI_COMMANDMASK)==MIDI_NOTEOFF )	{
              printf("Note!");
              unsigned int note = evt[1];
            }
          }
        }
      }
      lv2_event_increment(&self->in_iterator);
    }

    // calculate the next sample: phase ranges from 0 -> 1, but sin()
    // expects its input in radians. Just multiply by 2 PI to convert it!
    // We also multiply by volume so its not extremely loud.
    output[pos] = sin( self->phase * 2 * PI ) * volume;

    // increment the phase so we generate the next sample
    self->phase += phase_increment;
    if ( self->phase > 1.0f )
      self->phase = 0.f;
  }
}

static void
deactivate(LV2_Handle instance)
{
}

static void
cleanup(LV2_Handle instance)
{
  free(instance);
}

const void*
extension_data(const char* uri)
{
  return NULL;
}

static const LV2_Descriptor descriptor = {
  SINSYNTH_URI,
  instantiate,
  connect_port,
  activate,
  run,
  deactivate,
  cleanup,
  extension_data
};

LV2_SYMBOL_EXPORT
const LV2_Descriptor*
lv2_descriptor(uint32_t index)
{
  switch (index) {
  case 0:
    return &descriptor;
  default:
    return NULL;
  }
}
