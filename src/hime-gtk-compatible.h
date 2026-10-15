/*
 * Copyright (C) 2020 The HIME team, Taiwan
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation version 2.1
 * of the License.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
 */

/* ============================================================================
 * GTK Compatibility Layer - Version Guard Strategy
 * ============================================================================
 *
 * Supported GTK versions and requirements:
 * - GTK 2: Requires 2.24.31 or later
 * - GTK 3: Requires 3.22.30 or later
 * - GTK 4: Latest stable version recommended
 *
 * Version Guard Patterns:
 *   GTK2 only:           #if !GTK_CHECK_VERSION(3, 0, 0)
 *   GTK3+ (3.22.30+):    #if GTK_CHECK_VERSION(3, 0, 0)
 *   GTK4+:               #if GTK_CHECK_VERSION(4, 0, 0)
 *   GTK2/3 (not 4):      #elif GTK_CHECK_VERSION(3, 0, 0) [used after GTK4 check]
 *
 * Note: No micro version checks for GTK3 (e.g., 3.9.10, 3.13.4) needed
 *       since minimum is 3.22.30. Use GTK_CHECK_VERSION(3, 0, 0) instead.
 */

/* Minimum version validation */
#if GTK_CHECK_VERSION(3, 0, 0) && !GTK_CHECK_VERSION(3, 22, 30)
#error "GTK+ 3.22.30 or later is required"
#endif

#if !GTK_CHECK_VERSION(2, 24, 31) && !GTK_CHECK_VERSION(3, 0, 0)
#error "GTK+ 2.24.31 or later is required"
#endif

#ifndef HIME_GTK_COMPATIBLE_H
#define HIME_GTK_COMPATIBLE_H

#include <gdk/gdk.h>
#include <gtk/gtk.h>
#include <gdk-pixbuf/gdk-pixbuf.h>

#if GTK_CHECK_VERSION(4, 0, 0)
/* Helper function for gtk_container_add compatibility in GTK4 */
static inline void hime_container_add(GtkWidget *container, GtkWidget *widget) {
    if (GTK_IS_WINDOW(container)) {
        gtk_window_set_child(GTK_WINDOW(container), widget);
    } else if (GTK_IS_BOX(container)) {
        gtk_box_append(GTK_BOX(container), widget);
    } else if (GTK_IS_FRAME(container)) {
        gtk_frame_set_child(GTK_FRAME(container), widget);
    } else {
        gtk_box_append(GTK_BOX(container), widget);
    }
}

/* GTK4 keyboard event signal compatibility
 * GTK4 uses "key-pressed" signal instead of "key-press-event"
 * with a different callback signature:
 *   gboolean (*key_pressed) (GtkEventControllerKey *controller, guint keyval, guint keycode,
 *                            GdkModifierType state, gpointer user_data)
 *
 * For GTK2/3: Use legacy "key-press-event" signal on the widget directly
 * For GTK4: Use "key-pressed" signal on an event controller
 */

/* Helper to connect keyboard events in version-independent way
 * Note: Caller must use GTK4/GTK2-3 specific callback signatures
 */
static inline void hime_widget_connect_key_press(GtkWidget *widget,
                                                  GCallback callback,
                                                  gpointer user_data) {
    GtkEventController *controller = gtk_event_controller_key_new();
    g_signal_connect(controller, "key-pressed", callback, user_data);
    gtk_widget_add_controller(widget, controller);
}
#endif  /* GTK_CHECK_VERSION(4, 0, 0) - keyboard event handling */

/* compat macro for Gtk+2/Gtk+3/Gtk+4 */
#undef GDK_DISPLAY
#if GTK_CHECK_VERSION(4, 0, 0)
#define GDK_DISPLAY() gdk_display_get_default ()
#else
#define GDK_DISPLAY() GDK_DISPLAY_XDISPLAY (gdk_display_get_default ())
#endif  /* GTK_CHECK_VERSION(4, 0, 0) - display handling */

// ============================================================================
// GTK+4 compatibility section
// ============================================================================
#if GTK_CHECK_VERSION(4, 0, 0)

/* Keyboard event handling - GTK4 uses GtkEventControllerKey instead of key-press-event
 * GTK2/3: Connect to widget's "key-press-event" signal with callback:
 *         gboolean (*callback)(GtkWidget *widget, GdkEventKey *event, gpointer user_data)
 * GTK4:   Create GtkEventControllerKey, connect to "key-pressed" signal with callback:
 *         gboolean (*callback)(GtkEventControllerKey *controller, guint keyval,
 *                              guint keycode, GdkModifierType state, gpointer user_data)
 */
#include <gtk/gtkgestureclick.h>

#define gtk_hbox_new(x, y) gtk_box_new (GTK_ORIENTATION_HORIZONTAL, 0)
#define gtk_vbox_new(x, y) gtk_box_new (GTK_ORIENTATION_VERTICAL, 0)
#define gtk_hseparator_new() gtk_separator_new (GTK_ORIENTATION_HORIZONTAL)
#define gtk_vseparator_new() gtk_separator_new (GTK_ORIENTATION_VERTICAL)

#define gtk_box_pack_start(box, child, expand, fill, padding) \
    do { \
        gtk_box_append (GTK_BOX (box), child); \
        if (expand) gtk_widget_set_hexpand (child, TRUE); \
    } while (0)

#define gtk_box_pack_end(box, child, expand, fill, padding) \
    do { \
        gtk_box_prepend (GTK_BOX (box), child); \
        if (expand) gtk_widget_set_hexpand (child, TRUE); \
    } while (0)

#define gtk_container_add(container, widget) hime_container_add(container, widget)

#define GTK_TABLE GTK_GRID
#define gtk_table_attach_defaults(u, v, w, x, y, z) gtk_grid_attach (GTK_GRID (u), v, w, y, 1, 1)
#define gtk_table_attach(u, v, w, x, y, z, a, b, c, d) gtk_grid_attach (GTK_GRID (u), v, w, y, 1, 1)
#define gtk_table_new(x, y, z) gtk_grid_new ()

#define gtk_window_new(type) gtk_window_new ()

#define gtk_widget_modify_font(w, f) gtk_widget_override_font (w, f)
#define gtk_widget_override_font(w, f) ;

#define gtk_widget_modify_bg(w, s, c) ;
#define gtk_widget_modify_fg(w, s, c) ;

#define gtk_window_set_has_resize_grip(x, y) ;

#define GTK_OBJECT

/* GTK4 removes GtkEventBox - modules should use GtkBox instead
 * This is handled with #if guards in module code, not a macro replacement */

#define gtk_button_new_from_stock(x) gtk_button_new_from_icon_name (x, GTK_ICON_SIZE_BUTTON)
#define GTK_STOCK_CANCEL "gtk-cancel"
#define GTK_STOCK_OK "gtk-ok"
#define GTK_STOCK_QUIT "gtk-quit"
#define GTK_STOCK_SAVE "gtk-save"
#define GTK_STOCK_OPEN "gtk-open"
#define GTK_STOCK_CLOSE "gtk-close"
#define GTK_STOCK_DELETE "gtk-delete"
#define GTK_STOCK_FIND "gtk-find"
#define GTK_STOCK_ABOUT "gtk-about"
#define GTK_STOCK_PREFERENCES "gtk-preferences"
#define GTK_STOCK_INDEX "gtk-index"

#define GDK_WINDOW_XWINDOW GDK_WINDOW_XID

#define GtkStatusIcon GObject
#define gtk_status_icon_position_menu NULL

#define GTK_COLOR_SELECTION
#define GTK_COLOR_SELECTION_DIALOG

#else  /* GTK 3 or later */

#if GTK_CHECK_VERSION(3, 0, 0)
/* GTK3 specific compatibility (requires 3.22.30 or later) */

#define GTK_OBJECT

#define GtkStatusIcon GObject
#define gtk_status_icon_position_menu NULL

#define GTK_COLOR_SELECTION
#define GTK_COLOR_SELECTION_DIALOG

#define GDK_WINDOW_XWINDOW GDK_WINDOW_XID

#define gtk_separator_new(orientation) gtk_separator_new (orientation)
#define gtk_box_new(orientation, y) gtk_box_new (orientation, y)

#else  /* GTK 2 */

/* GTK2 specific compatibility (requires 2.24.31 or later) */
#define gtk_separator_new(orientation) (orientation == GTK_ORIENTATION_HORIZONTAL ? gtk_hseparator_new () : gtk_vseparator_new ())

#define gtk_box_new(orientation, y) (orientation == GTK_ORIENTATION_HORIZONTAL ? gtk_hbox_new (FALSE, y) : gtk_vbox_new (FALSE, y))

#define gtk_widget_get_preferred_size(x, y, z) gtk_widget_size_request (x, z)
#define gtk_widget_set_halign(x, y) ;

#define gtk_window_set_has_resize_grip(x, y) ;
#define gtk_widget_set_hexpand(x, y) ;
#define gtk_widget_set_vexpand(x, y) ;

#define gtk_grid_set_column_homogeneous(x, y) ;
#define gtk_grid_set_row_homogeneous(x, y) ;

#define gtk_orientable_set_orientation(x, y) ;

#define GdkRGBA GdkColor
#define gdk_rgba_parse(rgba, spec) gdk_color_parse (spec, rgba)
#define gdk_rgba_to_string(rgba) gdk_color_to_string (rgba)
#define gtk_color_selection_get_current_rgba(colorsel, rgba) gtk_color_selection_get_current_color (colorsel, rgba)
#define gtk_color_selection_set_current_rgba(colorsel, rgba) gtk_color_selection_set_current_color (colorsel, rgba)
#define gtk_widget_override_color(widget, state, color) gtk_widget_modify_fg (widget, state, color)
#define gtk_widget_override_background_color(widget, state, color) gtk_widget_modify_bg (widget, state, color)
#define GTK_STATE_FLAG_NORMAL GTK_STATE_NORMAL
#define GTK_STATE_FLAG_ACTIVE GTK_STATE_ACTIVE
#define GTK_STATE_FLAG_PRELIGHT GTK_STATE_PRELIGHT
#define GTK_STATE_FLAG_SELECTED GTK_STATE_SELECTED

#endif  /* GTK2 */
#endif  /* GTK_CHECK_VERSION(3, 0, 0) */

/* Deprecated API compatibility (works for all GTK versions)
 * XXX(xatier): both gtk_widget_modify_font and gtk_widget_override_font are deprecated
 */
#ifndef gtk_widget_override_font
#define gtk_widget_override_font gtk_widget_modify_font
#endif

/* Note: Micro version checks for GTK3 are removed since minimum GTK3 version is 3.22.30
 * All GTK3.22.30+ features are automatically available in GTK3 section
 * Previously checked versions: 3.3.2 (GTK_GRID), 3.9.10 (stock icons), 3.13.4 (resize grip)
 */

#ifndef PANGO_VERSION_CHECK
#define PANGO_VERSION_CHECK(x, y, z) FALSE
#endif

#endif /* HIME_GTK_COMPATIBLE_H */
