/* Copyright (C) 2011 Edward Der-Hua Liu, Hsin-Chu, Taiwan
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

#include "hime.h"
#include "hime-gtk-event-compat.h"

#include "gtab.h"

static GtkWidget *menu;
gboolean init_in_method (int in_no);

void cb_update_menu_select (GtkWidget *item, gpointer data) {
    if (!current_CS)
        return;

    int idx = GPOINTER_TO_INT (data);

    if (!current_CS->b_im_enabled)
        current_CS->b_im_enabled = TRUE;

    init_in_method (idx);
}

void get_icon_path (char *iconame, char fname[]);

void create_inmd_switch () {
    menu = gtk_menu_new ();

    int i;
    for (i = 0; i < inmdN; i++) {
        if (!inmd[i].cname || !inmd[i].cname[0])
            continue;

        char tt[64];
        snprintf (tt, sizeof (tt), "%s ctrl-alt-%c", inmd[i].cname, inmd[i].key_ch);

#if GTK_CHECK_VERSION(3, 0, 0)
        GtkWidget *item = gtk_menu_item_new_with_label (tt);
#else
        GtkWidget *item = gtk_image_menu_item_new_with_label (tt);
        if (inmd[i].icon) {
            char fname[512];
            get_icon_path (inmd[i].icon, fname);
            GtkWidget *img = gtk_image_new_from_file (fname);
            if (img) {
                gtk_image_menu_item_set_image (GTK_IMAGE_MENU_ITEM (item), img);
            }
        }
#endif

        g_signal_connect (G_OBJECT (item), "activate",
                          G_CALLBACK (cb_update_menu_select), GINT_TO_POINTER (i));

        gtk_widget_show (item);

        gtk_menu_shell_append (GTK_MENU_SHELL (menu), item);
    }
}

gint inmd_switch_popup_handler (GtkWidget *widget, GdkEvent *event) {
    if (!menu)
        create_inmd_switch ();

    if (event && gdk_event_get_event_type(event) == GDK_BUTTON_PRESS) {
        guint button = hime_gdk_event_get_button(event);
        guint32 event_time = gdk_event_get_time(event);
        gtk_menu_popup (GTK_MENU (menu), NULL, NULL, NULL, NULL,
                        button, event_time);
        return TRUE;
    }

    return FALSE;
}

void show_inmd_menu () {
    guint32 event_time = gtk_get_current_event_time ();
    gtk_menu_popup (GTK_MENU (menu), NULL, NULL, NULL, NULL, 1, event_time);
}

void destroy_inmd_menu () {
    if (!menu)
        return;
    gtk_widget_destroy (menu);
    menu = NULL;
}
