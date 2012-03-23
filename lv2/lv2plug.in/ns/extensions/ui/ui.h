/*
  LV2 UI Extension
  Copyright 2009-2011 David Robillard <d@drobilla.net>
  Copyright 2006-2008 Lars Luthman <lars.luthman@gmail.com>

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

/**
 * @file
 * C header for the LV2 UI extension <http://lv2plug.in/ns/extensions/ui>.
 */

#ifndef LV2_UI_H
#define LV2_UI_H

#include "lv2/lv2plug.in/ns/lv2core/lv2.h"

#define LV2_UI_URI    "http://lv2plug.in/ns/extensions/ui"
#define LV2_UI_PREFIX LV2_UI_URI "#"

#define LV2_UI__UI               LV2_UI_PREFIX "UI"
#define LV2_UI__GtkUI            LV2_UI_PREFIX "GtkUI"
#define LV2_UI__Qt4UI            LV2_UI_PREFIX "Qt4UI"
#define LV2_UI__X11UI            LV2_UI_PREFIX "X11UI"
#define LV2_UI__noUserResize     LV2_UI_PREFIX "noUserResize"
#define LV2_UI__fixedSize        LV2_UI_PREFIX "fixedSize"
#define LV2_UI__parent           LV2_UI_PREFIX "parent"
#define LV2_UI__PortNotification LV2_UI_PREFIX "PortNotification"
#define LV2_UI__portNotification LV2_UI_PREFIX "portNotification"
#define LV2_UI__plugin           LV2_UI_PREFIX "plugin"
#define LV2_UI__portIndex        LV2_UI_PREFIX "portIndex"
#define LV2_UI__notifyType       LV2_UI_PREFIX "notifyType"
#define LV2_UI__resize           LV2_UI_PREFIX "resize"

#ifdef __cplusplus
extern "C" {
#endif

/**
   A pointer to some widget or other type of UI handle.

   The actual type is defined by the type URI of the UI.  All the functionality
   provided by this extension is toolkit independent, the host only needs to
   pass the necessary callbacks and display the widget, if possible.  Plugins
   may have several UIs, in various toolkits.
*/
typedef void* LV2UI_Widget;

/**
   A pointer to a particular instance of a UI.

   It is valid to compare this to NULL (0 for C++) but otherwise the host MUST
   not attempt to interpret it.  The UI plugin may use it to reference internal
   instance data.
*/
typedef void* LV2UI_Handle;

/**
   A pointer to a particular plugin controller, provided by the host.

   It is valid to compare this to NULL (0 for C++) but otherwise the UI plugin
   MUST NOT attempt to interpret it.  The host may use it to reference internal
   instance data.
*/
typedef void* LV2UI_Controller;

/**
   The type of the host-provided function that the UI can use to
   send data to a plugin's input ports.

   The @c buffer parameter must point to a block of data, @c buffer_size bytes
   large.  The contents of this buffer and what the host should do with it
   depends on the value of the @c format parameter.

   The @c format parameter should either be 0 or a numeric ID for a "Transfer
   mechanism".  Transfer mechanisms are Features and may be defined in
   meta-extensions.  They specify how to translate the data buffers passed to
   this function to input data for the plugin ports.  If a UI wishes to write
   data to an input port, it must list a transfer mechanism Feature for that
   port's class as an optional or required feature (depending on whether the UI
   will work without being able to write to that port or not).  The only
   exception is when the UI wants to write single float values to input ports
   of the class lv2:ControlPort, in which case @c buffer_size should always be
   4, the buffer should always contain a single IEEE-754 float, and @c format
   should be 0.

   The numeric IDs for the transfer mechanisms are provided by a URI-to-integer
   mapping function provided by the host, using the URI Map feature
   <http://lv2plug.in/ns/ext/uri-map> with the map URI
   "http://lv2plug.in/ns/extensions/ui".  Thus a UI that requires transfer
   mechanism features also requires the URI Map feature, but this is implicit -
   the UI does not have to list the URI map feature as a required or optional
   feature in it's RDF data.

   An UI MUST NOT pass a @c format parameter value (except 0) that has not been
   returned by the host-provided URI mapping function for a host-supported
   transfer mechanism feature URI.

   The UI MUST NOT try to write to a port for which there is no specified
   transfer mechanism, or to an output port.  The UI is responsible for
   allocating the buffer and deallocating it after the call.
*/
typedef void (*LV2UI_Write_Function)(LV2UI_Controller controller,
                                     uint32_t         port_index,
                                     uint32_t         buffer_size,
                                     uint32_t         format,
                                     const void*      buffer);

/**
   The implementation of a UI.

   A pointer to an object of this type is returned by the lv2ui_descriptor()
   function.
*/
typedef struct _LV2UI_Descriptor {
  /**
     The URI for this UI (not for the plugin it controls).
  */
  const char* URI;

  /**
     Create a new UI object and return a handle to it.  This function works
     similarly to the instantiate() member in LV2_Descriptor.

     @param descriptor The descriptor for the UI that you want to instantiate.

     @param plugin_uri The URI of the plugin that this UI will control.

     @param bundle_path The path to the bundle containing the RDF data file
     that references this shared object file, including the trailing '/'.

     @param write_function A function provided by the host that the UI can use
     to send data to the plugin's input ports.

     @param controller A handle for the plugin instance that should be passed
     as the first parameter of @c write_function.

     @param widget A pointer to an LV2UI_Widget.  The UI will write a widget
     pointer to this location (what type of widget depends on the RDF class of
     the UI) that will be the main UI widget.

     @param features An array of LV2_Feature pointers.  The host must pass all
     feature URIs that it and the UI supports and any additional data, just
     like in the LV2 plugin instantiate() function.  Note that UI features and
     plugin features are NOT necessarily the same, they just share the same
     data structure - this will probably not be the same array as the one the
     plugin host passes to a plugin.

  */
  LV2UI_Handle (*instantiate)(const struct _LV2UI_Descriptor* descriptor,
                              const char*                     plugin_uri,
                              const char*                     bundle_path,
                              LV2UI_Write_Function            write_function,
                              LV2UI_Controller                controller,
                              LV2UI_Widget*                   widget,
                              const LV2_Feature* const*       features);


  /**
     Destroy the UI object and the associated widget. The host must not try
     to access the widget after calling this function.
   */
  void (*cleanup)(LV2UI_Handle ui);

  /**
     Tell the UI that something interesting has happened at a plugin port.

     What is interesting and how it is written to the buffer passed to this
     function is defined by the @c format parameter, which has the same meaning
     as in LV2UI_Write_Function.  The only exception is ports of the class
     lv2:ControlPort, for which this function should be called when the port
     value changes (it does not have to be called for every single change if
     the host's UI thread has problems keeping up with the thread the plugin is
     running in), @c buffer_size should be 4, the buffer should contain a
     single IEEE-754 float, and @c format should be 0.

     By default, the host should only call this function for input ports of the
     lv2:ControlPort class.  However, the default setting can be modified by
     using the following URIs in the UI's RDF data:
     <pre>
     uiext:portNotification
     uiext:noPortNotification
     uiext:plugin
     uiext:portIndex
     </pre>
     For example, if you want the UI with uri
     <code><http://my.pluginui></code> for the plugin with URI
     <code><http://my.plugin></code> to get notified when the value of the
     output control port with index 4 changes, you would use the following
     in the RDF for your UI:
     <pre>
     <http://my.pluginui> uiext:portNotification [ uiext:plugin <http://my.plugin> ;
     uiext:portIndex 4 ] .
     </pre>
     and similarly with <code>uiext:noPortNotification</code> if you wanted
     to prevent notifications for a port for which it would be on by default
     otherwise. The UI is not allowed to request notifications for ports of
     types for which no transfer mechanism is specified, if it does it should
     be considered broken and the host should not load it.

     The @c buffer is only valid during the time of this function call, so if
     the UI wants to keep it for later use it has to copy the contents to an
     internal buffer.

     This member may be set to NULL if the UI is not interested in any
     port events.
  */
  void (*port_event)(LV2UI_Handle ui,
                     uint32_t     port_index,
                     uint32_t     buffer_size,
                     uint32_t     format,
                     const void*  buffer);

  /**
     Return a data structure associated with an extension URI, for example
     a struct containing additional function pointers.

     Avoid returning function pointers directly since standard C/C++ has no
     valid way of casting a void* to a function pointer. This member may be set
     to NULL if the UI is not interested in supporting any extensions. This is
     similar to the extension_data() member in LV2_Descriptor.
  */
  const void* (*extension_data)(const char* uri);

} LV2UI_Descriptor;

typedef void* LV2_UI_Resize_Handle;

/**
   UI Resize Feature (LV2_UI__resize)

   This structure may be used in two ways: as a feature passed by the host
   (e.g. via the features parameter of LV2UI_Descriptor::instantiate()) or
   as a feature exposed by a UI (e.g. via LV2UI_Descriptor::extension_data()).
*/
typedef struct {

	LV2_UI_Resize_Handle handle;

	/**
	   Request or notify a size change.

	   When this struct is provided by the host, the UI may call this
	   function to notify the host that a size change is desired, or notify
	   the host of the initial size of the UI.

	   When this struct is provided by the plugin, the host may call this
	   function in the UI thread to notify the UI that it should change its
	   size to the given dimensions.

	   @return 0 on success.
	*/
	int (*ui_resize)(LV2_UI_Resize_Handle handle,
	                 int                  width,
	                 int                  height);

} LV2_UI_Resize;

/**
   A plugin UI programmer must include a function called "lv2ui_descriptor"
   with the following function prototype within the shared object file.  This
   function will have C-style linkage (if you are using C++ this is taken care
   of by the 'extern "C"' clause at the top of the file).  This function will
   be accessed by the UI host using the @c dlsym() function and called to get a
   LV2UI_UIDescriptor for the wanted plugin.

   Just like lv2_descriptor(), this function takes an index parameter.  The
   index should only be used for enumeration and not as any sort of ID number -
   the host should just iterate from 0 and upwards until the function returns
   NULL or a descriptor with an URI matching the one the host is looking for.
*/
const LV2UI_Descriptor* lv2ui_descriptor(uint32_t index);

/**
   The type of the lv2ui_descriptor() function.
*/
typedef const LV2UI_Descriptor* (*LV2UI_DescriptorFunction)(uint32_t index);

#ifdef __cplusplus
}
#endif

#endif /* LV2_UI_H */
