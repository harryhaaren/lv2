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

// its good practice to put the "global" things like this plugins URI in
// a header file. That way the GUI and DSP file can include it, and you
// know they'll always be up to date with eachother.

#ifndef SIN_SYNTH
#define SIN_SYNTH

#include "lv2/lv2plug.in/ns/lv2core/lv2.h"

#define SINSYNTH_URI "http://lv2plug.in/plugins/eg-sinsynth"
#define SINSYNTH_UI_URI "http://lv2plug.in/plugins/eg-sinsynth/gui"

#endif
