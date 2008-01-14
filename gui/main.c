/*
 * Copyright (C) 2007
 *       pancake <pancake@phreaker.net>
 *
 * radare is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * radare is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with radare; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
 *
 */

#include "main.h"
#include <stdio.h>
#include <string.h>
#include <getopt.h>

//#define FONT "-adobe-courier-bold-o-normal--18-180-75-75-m-110-iso8859-15"
#define FONT "Sans Bold 8"

GtkWidget *term = NULL;
char *filename = NULL;
char *command = NULL;
int is_debugger = 0;
char *font = FONT;

void show_help_message()
{
	printf("Usage: gradare [-h] [-e command] [-f font] file\n");
	exit(1);
}

GtkWidget *tool;

void init_home_directory()
{
	char buf[4096];
	strcpy(buf, g_get_home_dir());
	strcat(buf, "/.radare/");
	mkdir(buf);
	strcat(buf, "toolbar/");
	mkdir(buf);
}

GtkWidget *acti;
GtkWidget *menu;

GtkClipboard *clip = NULL;

void seek_to()
{
	gchar *str;

	clip = gtk_widget_get_clipboard(term, 1);
	vte_terminal_copy_clipboard(term);
	str = gtk_clipboard_wait_for_text(clip);
	vte_terminal_feed_child(VTE_TERMINAL(term), ":s ", 3);
	vte_terminal_feed_child(VTE_TERMINAL(term), str, strlen(str));
	vte_terminal_feed_child(VTE_TERMINAL(term), "\n\n", 2);
	if (gtk_combo_box_get_active(GTK_COMBO_BOX(combo)) == 1)
		gtk_combo_box_set_active(GTK_COMBO_BOX(combo), 2);

	gtk_widget_destroy(menu);
}

void breakpoint_to()
{
	gchar *str;

	clip = gtk_widget_get_clipboard(term, 1);
	vte_terminal_copy_clipboard(term);
	str = gtk_clipboard_wait_for_text(clip);
	vte_terminal_feed_child(VTE_TERMINAL(term), ":!bp ", 5);
	vte_terminal_feed_child(VTE_TERMINAL(term), str, strlen(str));
	vte_terminal_feed_child(VTE_TERMINAL(term), "\n\n", 2);
	if (gtk_combo_box_get_active(GTK_COMBO_BOX(combo)) == 1)
		gtk_combo_box_set_active(GTK_COMBO_BOX(combo), 2);

	gtk_widget_destroy(menu);
}

gboolean popup_context_menu(GtkWidget *tv, GdkEventButton *event, gpointer user_data)
{
        GtkWidget *menu_item;

	if (event->type == GDK_BUTTON_PRESS && event->button == 3) {
		menu = gtk_menu_new();

		gtk_container_set_border_width(menu, 2);

		menu_item = gtk_image_menu_item_new_from_stock("Seek to...", "gtk-find");
		g_signal_connect(menu_item, "button-release-event", G_CALLBACK(seek_to), NULL);
		gtk_menu_shell_append(GTK_MENU_SHELL(menu), menu_item);

		menu_item = gtk_image_menu_item_new_from_stock("Add breakpoint", "gtk-find");
		g_signal_connect(menu_item, "button-release-event", G_CALLBACK(breakpoint_to), NULL);
		gtk_menu_shell_append(GTK_MENU_SHELL(menu), menu_item);

		gtk_widget_show_all(menu);
		gtk_menu_popup(GTK_MENU(menu), NULL, NULL, NULL, NULL, 3, NULL);
		return 1;
	}

	return 0;
}

struct gradare_mon_t {
	int id;
	GtkWidget *entry;
	GtkWidget *but;
	GtkWidget *term;
};

gboolean monitor_button_clicked(GtkWidget *but, gpointer user_data)
{
	struct gradare_mon_t *mon = user_data;
	char miau[16];
	char buf[1024];
	char *cmd[4]={"/usr/bin/rsc","monitor", &miau, 0};
	sprintf(miau, "mon%d", mon->id);

	snprintf(buf, 1023, "/usr/bin/rsc monitor %s \"%s\"", miau, gtk_entry_get_text(mon->entry));
	system(buf);

	vte_terminal_fork_command(
		VTE_TERMINAL(mon->term),
		cmd[0], cmd, NULL, ".",
		FALSE, FALSE, FALSE);

	return 0;
}

int mon_id = 0;

void gradare_new_monitor()
{
	GtkWidget *w;
	GtkWidget *vbox;
	GtkWidget *hbox;
	GtkWidget *hpan;
	struct gradare_mon_t *mon;
	mon = (struct gradare_mon_t *)malloc(sizeof(struct gradare_mon_t));
	mon->id = mon_id++;

	w = gtk_window_new(GTK_WINDOW_TOPLEVEL);
	gtk_window_resize(GTK_WINDOW(w), 600,400);
	gtk_window_set_title(GTK_WINDOW(w), "gradare monitor");
	// XXX memleak !!! should be passed to a destroyer function for 'mon'
	g_signal_connect(w, "destroy", G_CALLBACK(gtk_widget_hide), mon);

	vbox = gtk_vbox_new(FALSE, 0);
	gtk_container_add(GTK_CONTAINER(w), vbox);

	hbox = gtk_hbox_new(FALSE, 3);
	mon->entry = gtk_entry_new();
	mon->but = gtk_button_new_with_label("Go");
	// mouse
        g_signal_connect(mon->but, "released", (gpointer)monitor_button_clicked, (gpointer)mon);
	// keyboard
        g_signal_connect(mon->but, "activate", (gpointer)monitor_button_clicked, (gpointer)mon);
        g_signal_connect(mon->entry, "activate", (gpointer)monitor_button_clicked, (gpointer)mon);
	gtk_container_add(GTK_CONTAINER(hbox), mon->entry);
	gtk_box_pack_start(GTK_CONTAINER(hbox), mon->but, FALSE, FALSE, 5);
	gtk_box_pack_start(GTK_CONTAINER(vbox), hbox, FALSE, FALSE, 5);

	mon->term = vte_terminal_new();
	//vte_terminal_set_background_transparent(term, TRUE);
	//vte_terminal_set_opacity(term, 10);
	//vte_terminal_set_color_background(term, (GdkColor));
	//vte_terminal_set_color_foreground(term, (GdkColor));
	vte_terminal_set_cursor_blinks((VteTerminal*)mon->term, TRUE);
	vte_terminal_set_mouse_autohide((VteTerminal*)mon->term, TRUE);
	vte_terminal_set_scrollback_lines((VteTerminal*)mon->term, 3000);
	vte_terminal_set_font_from_string_full((VteTerminal*)mon->term, font, VTE_ANTI_ALIAS_FORCE_DISABLE);
	//(VTE_TERMINAL_CLASS(term))->increase_font_size(term);
         g_signal_connect (mon->term, "button-press-event",
                G_CALLBACK (popup_context_menu), NULL);

/*
	vte_terminal_fork_command(
		VTE_TERMINAL(term),
		cmd[0], cmd, NULL, ".",
		FALSE, FALSE, FALSE);

*/
	gtk_container_add(GTK_CONTAINER(vbox), mon->term);
	gtk_widget_show_all(GTK_WIDGET(w));
}

int main(int argc, char **argv, char **envp)
{
	int c;
	GtkWidget *w;
	GtkWidget *chos;
	GtkWidget *vbox;
	GtkWidget *hpan;

	while ((c = getopt(argc, argv, "e:hf:")) != -1) {
		switch( c ) {
		case 'h':
			show_help_message();
			break;
		case 'e':
			command = optarg;
			break;
		case 'f':
			font = optarg;
			break;
		}
	}

	if (optind<argc)
		filename = argv[optind];

	gtk_init(&argc, &argv);
	init_home_directory();
	w = gtk_window_new(GTK_WINDOW_TOPLEVEL);
	gtk_window_resize(GTK_WINDOW(w), 800,600);
	gtk_window_set_title(GTK_WINDOW(w), "gradare");
	g_signal_connect(w, "destroy", G_CALLBACK(gtk_main_quit), 0);

	vbox = gtk_vbox_new(FALSE, 0);
	gtk_container_add(GTK_CONTAINER(w), vbox);
	gtk_box_pack_start(GTK_BOX(vbox),
		GTK_WIDGET(gradare_menubar_new(w)), FALSE, FALSE, 0);

	tool = gradare_toolbar_new(NULL);
	gtk_box_pack_start(GTK_BOX(vbox), tool, FALSE, FALSE, 0);
	chos = gradare_sidebar_new();
	gtk_box_pack_start(GTK_BOX(vbox), chos, FALSE, FALSE, 0);

	hpan = gtk_hpaned_new();
	gtk_container_add(GTK_CONTAINER(vbox), hpan);

	acti = gradare_actions_new();
	gtk_paned_pack1(GTK_PANED(hpan), acti, TRUE, TRUE);

	term = vte_terminal_new();
	//vte_terminal_set_background_transparent(term, TRUE);
	//vte_terminal_set_opacity(term, 10);
	//vte_terminal_set_color_background(term, (GdkColor));
	//vte_terminal_set_color_foreground(term, (GdkColor));
	vte_terminal_set_cursor_blinks((VteTerminal*)term, TRUE);
	vte_terminal_set_mouse_autohide((VteTerminal*)term, TRUE);
	vte_terminal_set_scrollback_lines((VteTerminal*)term, 3000);
	vte_terminal_set_font_from_string_full((VteTerminal*)term, font, VTE_ANTI_ALIAS_FORCE_DISABLE);
	//(VTE_TERMINAL_CLASS(term))->increase_font_size(term);
 g_signal_connect (term, "button-press-event",
                G_CALLBACK (popup_context_menu), NULL);


	gtk_paned_pack2(GTK_PANED(hpan), term, TRUE, TRUE);
	gtk_widget_show_all(GTK_WIDGET(w));

	setenv("BEP", "entry", 1); // force debugger to stop at entry point
	if (command) {
		char *arg[2] = { command, NULL};
		vte_terminal_fork_command(
			VTE_TERMINAL(term),
			command, arg, envp, ".",
			FALSE, FALSE, FALSE);
	} else
	if (filename) {
		char *arg[6] = { "/usr/bin/radare", "-c", "-b", "1024", filename, NULL};
		char str[1024];
printf("FILE(%s)\n", filename);

		vte_terminal_fork_command(
			VTE_TERMINAL(term),
			arg[0], arg, envp, ".",
			FALSE, FALSE, FALSE);
		vte_terminal_feed_child(VTE_TERMINAL(term), "V\n", 2);
		sprintf(str, "radare -b 1024 -c - %s", filename);
sleep (1);
		gtk_window_set_title(GTK_WINDOW(w), str);
	} else
		vte_terminal_fork_command(
			VTE_TERMINAL(term),
			GRSCDIR"/Shell", NULL, envp, ".",
			FALSE, FALSE, FALSE);

	if (filename && (strstr(filename, "dbg://") || strstr(filename, "pid://"))) {
		is_debugger = 1;
		gtk_combo_box_set_active(GTK_COMBO_BOX(combo), 1);
	}

	gtk_widget_show_all(GTK_WIDGET(w));
	gtk_widget_grab_focus(term);

#if 0
	printf("Terminal size: %dx%d\n",
		vte_terminal_get_char_width(term),
		vte_terminal_get_char_height(term));
#endif

	gtk_main();
}
