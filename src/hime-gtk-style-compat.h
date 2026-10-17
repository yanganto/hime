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
 * GTK4 Style Compatibility Header
 *
 * GTK4 removed gtk_widget_modify_* and gtk_widget_override_* functions.
 * This header provides compatibility functions for font and color styling.
 */

#ifndef HIME_GTK_STYLE_COMPAT_H
#define HIME_GTK_STYLE_COMPAT_H

#include <gdk/gdk.h>
#include <gtk/gtk.h>
#include <pango/pango.h>

#if GTK_CHECK_VERSION(4, 0, 0)

/* ============================================================================
 * GTK4 CSS-based styling functions
 * ============================================================================ */

/* Set font for a widget using CSS */
static inline void hime_widget_set_font_from_desc(GtkWidget *widget,
                                                   PangoFontDescription *font_desc) {
    if (!widget || !font_desc) {
        return;
    }

    gchar *font_str = pango_font_description_to_string(font_desc);
    if (!font_str) {
        return;
    }

    /* Build CSS for font */
    gchar *css = g_strdup_printf("* { font-family: %s; font-size: %dpt; }",
                                 pango_font_description_get_family(font_desc),
                                 pango_font_description_get_size(font_desc) / PANGO_SCALE);

    GtkCssProvider *provider = gtk_css_provider_new();
    gtk_css_provider_load_from_data(provider, css, -1);

    GtkStyleContext *context = gtk_widget_get_style_context(widget);
    gtk_style_context_add_provider(context,
                                   GTK_STYLE_PROVIDER(provider),
                                   GTK_STYLE_PROVIDER_PRIORITY_APPLICATION);

    g_object_unref(provider);
    g_free(css);
    g_free(font_str);
}

/* Set background color for a widget using CSS */
static inline void hime_widget_set_bg_color(GtkWidget *widget, GdkRGBA *color) {
    if (!widget || !color) {
        return;
    }

    gchar *css = g_strdup_printf("* { background-color: rgba(%d,%d,%d,%f); }",
                                 (int)(color->red * 255),
                                 (int)(color->green * 255),
                                 (int)(color->blue * 255),
                                 color->alpha);

    GtkCssProvider *provider = gtk_css_provider_new();
    gtk_css_provider_load_from_data(provider, css, -1);

    GtkStyleContext *context = gtk_widget_get_style_context(widget);
    gtk_style_context_add_provider(context,
                                   GTK_STYLE_PROVIDER(provider),
                                   GTK_STYLE_PROVIDER_PRIORITY_APPLICATION);

    g_object_unref(provider);
    g_free(css);
}

/* Set foreground/text color for a widget using CSS */
static inline void hime_widget_set_fg_color(GtkWidget *widget, GdkRGBA *color) {
    if (!widget || !color) {
        return;
    }

    gchar *css = g_strdup_printf("* { color: rgba(%d,%d,%d,%f); }",
                                 (int)(color->red * 255),
                                 (int)(color->green * 255),
                                 (int)(color->blue * 255),
                                 color->alpha);

    GtkCssProvider *provider = gtk_css_provider_new();
    gtk_css_provider_load_from_data(provider, css, -1);

    GtkStyleContext *context = gtk_widget_get_style_context(widget);
    gtk_style_context_add_provider(context,
                                   GTK_STYLE_PROVIDER(provider),
                                   GTK_STYLE_PROVIDER_PRIORITY_APPLICATION);

    g_object_unref(provider);
    g_free(css);
}

#else

/* GTK2/GTK3: use native functions */

static inline void hime_widget_set_font_from_desc(GtkWidget *widget,
                                                   PangoFontDescription *font_desc) {
    gtk_widget_override_font(widget, font_desc);
}

static inline void hime_widget_set_bg_color(GtkWidget *widget, GdkRGBA *color) {
    gtk_widget_override_background_color(widget, GTK_STATE_FLAG_NORMAL, color);
}

static inline void hime_widget_set_fg_color(GtkWidget *widget, GdkRGBA *color) {
    gtk_widget_override_color(widget, GTK_STATE_FLAG_NORMAL, color);
}

#endif /* GTK_CHECK_VERSION(4, 0, 0) */

#endif /* HIME_GTK_STYLE_COMPAT_H */
