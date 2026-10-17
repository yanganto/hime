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

/*
 * GTK4 Event Compatibility Header
 *
 * GTK4 changed the event system significantly:
 * - GdkEvent is now an opaque type with accessor functions
 * - Event callbacks use GdkEvent* instead of specific event types
 * - This header provides compatibility wrappers for the old API
 */

#ifndef HIME_GTK_EVENT_COMPAT_H
#define HIME_GTK_EVENT_COMPAT_H

#include <gdk/gdk.h>
#include <gtk/gtk.h>

#if GTK_CHECK_VERSION(4, 0, 0)

/* ============================================================================
 * GTK4 Event Compatibility Functions
 * ============================================================================ */

/* Scroll event helper functions */
static inline guint hime_gdk_event_get_scroll_direction(GdkEvent *event) {
    GdkScrollDirection direction;
    if (gdk_scroll_event_get_direction(event, &direction)) {
        return (guint)direction;
    }
    return GDK_SCROLL_SMOOTH;
}

static inline gboolean hime_gdk_event_get_scroll_deltas(GdkEvent *event,
                                                         double *delta_x,
                                                         double *delta_y) {
    return gdk_scroll_event_get_deltas(event, delta_x, delta_y);
}

/* Button event helper functions */
static inline guint hime_gdk_event_get_button(GdkEvent *event) {
    guint button;
    if (gdk_button_event_get_button(event, &button)) {
        return button;
    }
    return 0;
}

static inline gboolean hime_gdk_event_get_button_position(GdkEvent *event,
                                                           double *x,
                                                           double *y) {
    return gdk_event_get_position(event, x, y);
}

/* Key event helper functions */
static inline guint hime_gdk_event_get_keyval(GdkEvent *event) {
    guint keyval;
    if (gdk_key_event_get_keyval(event, &keyval)) {
        return keyval;
    }
    return 0;
}

static inline guint hime_gdk_event_get_keycode(GdkEvent *event) {
    guint keycode;
    if (gdk_key_event_get_keycode(event, &keycode)) {
        return keycode;
    }
    return 0;
}

static inline GdkModifierType hime_gdk_event_get_state(GdkEvent *event) {
    GdkModifierType state;
    if (gdk_event_get_modifier_state(event, &state)) {
        return state;
    }
    return 0;
}

/* Motion event helper functions */
static inline gboolean hime_gdk_event_get_motion_position(GdkEvent *event,
                                                           double *x,
                                                           double *y) {
    return gdk_event_get_position(event, x, y);
}

/* Focus event helper functions */
static inline gboolean hime_gdk_event_is_focus_in(GdkEvent *event) {
    gboolean focus_in;
    if (gdk_focus_event_get_in(event, &focus_in)) {
        return focus_in;
    }
    return FALSE;
}

/* Event type checking for GTK4 */
static inline guint hime_gdk_event_get_event_type(GdkEvent *event) {
    return gdk_event_get_event_type(event);
}

static inline guint32 hime_gdk_event_get_time(GdkEvent *event) {
    return gdk_event_get_time(event);
}

#else

/* GTK2/GTK3: use native structures directly */
/* These are just pass-through functions for compatibility */

static inline guint hime_gdk_event_get_scroll_direction(GdkEvent *event) {
    return ((GdkEventScroll *)event)->direction;
}

static inline gboolean hime_gdk_event_get_scroll_deltas(GdkEvent *event,
                                                         double *delta_x,
                                                         double *delta_y) {
    *delta_x = ((GdkEventScroll *)event)->delta_x;
    *delta_y = ((GdkEventScroll *)event)->delta_y;
    return TRUE;
}

static inline guint hime_gdk_event_get_button(GdkEvent *event) {
    return ((GdkEventButton *)event)->button;
}

static inline gboolean hime_gdk_event_get_button_position(GdkEvent *event,
                                                           double *x,
                                                           double *y) {
    *x = ((GdkEventButton *)event)->x;
    *y = ((GdkEventButton *)event)->y;
    return TRUE;
}

static inline guint hime_gdk_event_get_keyval(GdkEvent *event) {
    return ((GdkEventKey *)event)->keyval;
}

static inline guint hime_gdk_event_get_keycode(GdkEvent *event) {
    return ((GdkEventKey *)event)->hardware_keycode;
}

static inline GdkModifierType hime_gdk_event_get_state(GdkEvent *event) {
    GdkEventAny *any_event = (GdkEventAny *)event;
    switch (any_event->type) {
        case GDK_KEY_PRESS:
        case GDK_KEY_RELEASE:
            return ((GdkEventKey *)event)->state;
        case GDK_BUTTON_PRESS:
        case GDK_BUTTON_RELEASE:
        case GDK_2BUTTON_PRESS:
        case GDK_3BUTTON_PRESS:
            return ((GdkEventButton *)event)->state;
        case GDK_MOTION_NOTIFY:
            return ((GdkEventMotion *)event)->state;
        case GDK_SCROLL:
            return ((GdkEventScroll *)event)->state;
        default:
            return 0;
    }
}

static inline gboolean hime_gdk_event_get_motion_position(GdkEvent *event,
                                                           double *x,
                                                           double *y) {
    *x = ((GdkEventMotion *)event)->x;
    *y = ((GdkEventMotion *)event)->y;
    return TRUE;
}

static inline gboolean hime_gdk_event_is_focus_in(GdkEvent *event) {
    return ((GdkEventFocus *)event)->in;
}

/* Event type checking for GTK2/3 */
static inline guint hime_gdk_event_get_event_type(GdkEvent *event) {
    return ((GdkEventAny *)event)->type;
}

static inline guint32 hime_gdk_event_get_time(GdkEvent *event) {
    return ((GdkEventAny *)event)->time;
}

#endif /* GTK_CHECK_VERSION(4, 0, 0) */

#endif /* HIME_GTK_EVENT_COMPAT_H */
