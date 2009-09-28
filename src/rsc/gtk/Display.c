/* Display.c generated by valac, the Vala compiler
 * generated from Display.vala, do not modify */


#include <glib.h>
#include <glib-object.h>
#include <gtk/gtk.h>
#include <stdlib.h>
#include <string.h>
#include <glib/gstdio.h>
#include <unistd.h>
#include <float.h>
#include <math.h>


#define TYPE_DISPLAY (display_get_type ())
#define DISPLAY(obj) (G_TYPE_CHECK_INSTANCE_CAST ((obj), TYPE_DISPLAY, Display))
#define DISPLAY_CLASS(klass) (G_TYPE_CHECK_CLASS_CAST ((klass), TYPE_DISPLAY, DisplayClass))
#define IS_DISPLAY(obj) (G_TYPE_CHECK_INSTANCE_TYPE ((obj), TYPE_DISPLAY))
#define IS_DISPLAY_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE ((klass), TYPE_DISPLAY))
#define DISPLAY_GET_CLASS(obj) (G_TYPE_INSTANCE_GET_CLASS ((obj), TYPE_DISPLAY, DisplayClass))

typedef struct _Display Display;
typedef struct _DisplayClass DisplayClass;
typedef struct _DisplayPrivate DisplayPrivate;
#define _g_object_unref0(var) ((var == NULL) ? NULL : (var = (g_object_unref (var), NULL)))
#define _g_free0(var) (var = (g_free (var), NULL))
#define _g_error_free0(var) ((var == NULL) ? NULL : (var = (g_error_free (var), NULL)))

struct _Display {
	GtkDialog parent_instance;
	DisplayPrivate * priv;
};

struct _DisplayClass {
	GtkDialogClass parent_class;
};

struct _DisplayPrivate {
	GtkComboBox* cb;
	GtkSpinButton* width;
	GtkSpinButton* height;
	GtkSpinButton* depth;
	char* selected;
	char* filename;
	gint tmp_file;
};


static gpointer display_parent_class = NULL;

GType display_get_type (void);
#define DISPLAY_GET_PRIVATE(o) (G_TYPE_INSTANCE_GET_PRIVATE ((o), TYPE_DISPLAY, DisplayPrivate))
enum  {
	DISPLAY_DUMMY_PROPERTY
};
static void display_fill_combobox (Display* self);
Display* display_new (void);
Display* display_construct (GType object_type);
static void display_on_combobox_changed (Display* self);
static void display_ok_clicked (Display* self);
static void display_on_response (Display* self, GtkDialog* source, gint response_id);
static gint display_main (char** args, int args_length1);
static void _display_on_response_gtk_dialog_response (Display* _sender, gint response_id, gpointer self);
static void _gtk_main_quit_gtk_object_destroy (Display* _sender, gpointer self);
static void _lambda0_ (GtkComboBox* target, Display* self);
static void __lambda0__gtk_combo_box_changed (GtkComboBox* _sender, gpointer self);
static GObject * display_constructor (GType type, guint n_construct_properties, GObjectConstructParam * construct_properties);
static void display_finalize (GObject* obj);
static int _vala_strcmp0 (const char * str1, const char * str2);



Display* display_construct (GType object_type) {
	Display * self;
	gboolean _tmp0_ = FALSE;
	self = g_object_newv (object_type, 0, NULL);
	gtk_dialog_add_button ((GtkDialog*) self, GTK_STOCK_CANCEL, (gint) GTK_RESPONSE_CANCEL);
	gtk_dialog_add_button ((GtkDialog*) self, GTK_STOCK_OK, (gint) GTK_RESPONSE_OK);
	display_fill_combobox (self);
	gtk_combo_box_set_active (self->priv->cb, 0);
	if (g_getenv ("FILE") == NULL) {
		_tmp0_ = TRUE;
	} else {
		_tmp0_ = g_getenv ("OFFSET") == NULL;
	}
	if (_tmp0_) {
		g_error ("Display.vala:36: radare did not export FILE and OFFSET environment variables!\n");
	}
	return self;
}


Display* display_new (void) {
	return display_construct (TYPE_DISPLAY);
}


static void display_fill_combobox (Display* self) {
	g_return_if_fail (self != NULL);
	gtk_combo_box_append_text (self->priv->cb, "raw");
	gtk_combo_box_append_text (self->priv->cb, "jpeg");
	gtk_combo_box_append_text (self->priv->cb, "png");
	gtk_combo_box_append_text (self->priv->cb, "bmp");
	gtk_combo_box_append_text (self->priv->cb, "gif");
	gtk_combo_box_append_text (self->priv->cb, "tga");
	gtk_combo_box_append_text (self->priv->cb, "tif");
}


static void display_on_combobox_changed (Display* self) {
	char* _tmp0_;
	g_return_if_fail (self != NULL);
	self->priv->selected = (_tmp0_ = g_strdup (gtk_combo_box_get_active_text (self->priv->cb)), _g_free0 (self->priv->selected), _tmp0_);
	if (_vala_strcmp0 (self->priv->selected, "raw") == 0) {
		gtk_widget_set_sensitive ((GtkWidget*) self->priv->width, TRUE);
		gtk_widget_set_sensitive ((GtkWidget*) self->priv->height, TRUE);
		gtk_widget_set_sensitive ((GtkWidget*) self->priv->depth, TRUE);
	} else {
		gtk_widget_set_sensitive ((GtkWidget*) self->priv->width, FALSE);
		gtk_widget_set_sensitive ((GtkWidget*) self->priv->height, FALSE);
		gtk_widget_set_sensitive ((GtkWidget*) self->priv->depth, FALSE);
	}
}


static void display_on_response (Display* self, GtkDialog* source, gint response_id) {
	g_return_if_fail (self != NULL);
	g_return_if_fail (source != NULL);
	switch (response_id) {
		case GTK_RESPONSE_CANCEL:
		{
			gtk_object_destroy ((GtkObject*) self);
			break;
		}
		case GTK_RESPONSE_OK:
		{
			display_ok_clicked (self);
			gtk_object_destroy ((GtkObject*) self);
			break;
		}
	}
}


static void display_ok_clicked (Display* self) {
	GError * _inner_error_;
	char* tmpname;
	char* bytes;
	char* cmd;
	char* _tmp15_;
	char* _tmp14_;
	char* _tmp13_;
	char* _tmp12_;
	char* _tmp11_;
	char* _tmp10_;
	char* _tmp9_;
	char* _tmp8_;
	g_return_if_fail (self != NULL);
	_inner_error_ = NULL;
	tmpname = g_strdup ("radare_XXXXXX.");
	bytes = NULL;
	cmd = NULL;
	if (_vala_strcmp0 (self->priv->selected, "raw") == 0) {
		char* _tmp0_;
		tmpname = (_tmp0_ = g_strconcat (tmpname, "gray", NULL), _g_free0 (tmpname), _tmp0_);
	} else {
		char* _tmp1_;
		tmpname = (_tmp1_ = g_strconcat (tmpname, self->priv->selected, NULL), _g_free0 (tmpname), _tmp1_);
	}
	{
		char* _tmp4_;
		gint _tmp3_;
		char* _tmp2_ = NULL;
		gint _tmp5_;
		_tmp5_ = (_tmp3_ = g_file_open_tmp (tmpname, &_tmp2_, &_inner_error_), self->priv->filename = (_tmp4_ = _tmp2_, _g_free0 (self->priv->filename), _tmp4_), _tmp3_);
		if (_inner_error_ != NULL) {
			if (_inner_error_->domain == G_FILE_ERROR) {
				goto __catch0_g_file_error;
			}
			goto __finally0;
		}
		self->priv->tmp_file = _tmp5_;
	}
	goto __finally0;
	__catch0_g_file_error:
	{
		GError * e;
		e = _inner_error_;
		_inner_error_ = NULL;
		{
			g_error ("Display.vala:88: %s\n", e->message);
			_g_error_free0 (e);
		}
	}
	__finally0:
	if (_inner_error_ != NULL) {
		_g_free0 (tmpname);
		_g_free0 (bytes);
		_g_free0 (cmd);
		g_critical ("file %s: line %d: uncaught error: %s", __FILE__, __LINE__, _inner_error_->message);
		g_clear_error (&_inner_error_);
		return;
	}
	if (_vala_strcmp0 (self->priv->selected, "raw") == 0) {
		char* _tmp6_;
		bytes = (_tmp6_ = g_strdup_printf ("%d", ((gint) gtk_spin_button_get_value (self->priv->width)) * ((gint) gtk_spin_button_get_value (self->priv->height))), _g_free0 (bytes), _tmp6_);
	} else {
		char* _tmp7_;
		bytes = (_tmp7_ = g_strdup (g_getenv ("BSIZE")), _g_free0 (bytes), _tmp7_);
	}
	if (bytes == NULL) {
		g_error ("Display.vala:97: bytes is null, if you did not select raw, BSIZE needs to be exported!\n");
	}
	cmd = (_tmp15_ = g_strconcat (_tmp14_ = g_strconcat (_tmp13_ = g_strconcat (_tmp12_ = g_strconcat (_tmp11_ = g_strconcat (_tmp10_ = g_strconcat (_tmp9_ = g_strconcat (_tmp8_ = g_strconcat ("dd if=\"", g_getenv ("FILE"), NULL), "\" of=\"", NULL), self->priv->filename, NULL), "\" count=", NULL), bytes, NULL), " bs=1 skip=", NULL), g_getenv ("OFFSET"), NULL), " & ", NULL), _g_free0 (cmd), _tmp15_);
	_g_free0 (_tmp14_);
	_g_free0 (_tmp13_);
	_g_free0 (_tmp12_);
	_g_free0 (_tmp11_);
	_g_free0 (_tmp10_);
	_g_free0 (_tmp9_);
	_g_free0 (_tmp8_);
	system (cmd);
	if (_vala_strcmp0 (self->priv->selected, "raw") == 0) {
		char* _tmp25_;
		char* _tmp24_;
		char* _tmp23_;
		char* _tmp22_;
		char* _tmp21_;
		char* _tmp20_;
		char* _tmp19_;
		char* _tmp18_;
		char* _tmp17_;
		char* _tmp16_;
		system (_tmp25_ = g_strconcat (_tmp24_ = g_strconcat (_tmp23_ = g_strconcat (_tmp21_ = g_strconcat (_tmp20_ = g_strconcat (_tmp18_ = g_strconcat (_tmp17_ = g_strconcat ("display -size ", _tmp16_ = g_strdup_printf ("%d", (gint) gtk_spin_button_get_value (self->priv->width)), NULL), "x", NULL), _tmp19_ = g_strdup_printf ("%d", (gint) gtk_spin_button_get_value (self->priv->height)), NULL), " -depth ", NULL), _tmp22_ = g_strdup_printf ("%d", (gint) gtk_spin_button_get_value (self->priv->depth)), NULL), " ", NULL), self->priv->filename, NULL));
		_g_free0 (_tmp25_);
		_g_free0 (_tmp24_);
		_g_free0 (_tmp23_);
		_g_free0 (_tmp22_);
		_g_free0 (_tmp21_);
		_g_free0 (_tmp20_);
		_g_free0 (_tmp19_);
		_g_free0 (_tmp18_);
		_g_free0 (_tmp17_);
		_g_free0 (_tmp16_);
	} else {
		char* _tmp26_;
		system (_tmp26_ = g_strconcat ("display ", self->priv->filename, NULL));
		_g_free0 (_tmp26_);
	}
	unlink (self->priv->filename);
	_g_free0 (tmpname);
	_g_free0 (bytes);
	_g_free0 (cmd);
}


static gint display_main (char** args, int args_length1) {
	gint result;
	Display* dialog;
	gtk_init (&args_length1, &args);
	dialog = g_object_ref_sink (display_new ());
	gtk_widget_show_all ((GtkWidget*) dialog);
	gtk_main ();
	result = 0;
	_g_object_unref0 (dialog);
	return result;
}


int main (int argc, char ** argv) {
	g_type_init ();
	return display_main (argv, argc);
}


static void _display_on_response_gtk_dialog_response (Display* _sender, gint response_id, gpointer self) {
	display_on_response (self, _sender, response_id);
}


static void _gtk_main_quit_gtk_object_destroy (Display* _sender, gpointer self) {
	gtk_main_quit ();
}


static void _lambda0_ (GtkComboBox* target, Display* self) {
	g_return_if_fail (target != NULL);
	display_on_combobox_changed (self);
}


static void __lambda0__gtk_combo_box_changed (GtkComboBox* _sender, gpointer self) {
	_lambda0_ (_sender, self);
}


static GObject * display_constructor (GType type, guint n_construct_properties, GObjectConstructParam * construct_properties) {
	GObject * obj;
	GObjectClass * parent_class;
	Display * self;
	parent_class = G_OBJECT_CLASS (display_parent_class);
	obj = parent_class->constructor (type, n_construct_properties, construct_properties);
	self = DISPLAY (obj);
	{
		GtkLabel* _label0;
		GtkHBox* _hbox0;
		GtkLabel* _label1;
		GtkLabel* _label2;
		GtkLabel* _label3;
		GtkLabel* _tmp0_;
		GtkComboBox* _tmp1_;
		GtkHBox* _tmp2_;
		GtkLabel* _tmp3_;
		GtkSpinButton* _tmp4_;
		GtkLabel* _tmp5_;
		GtkSpinButton* _tmp6_;
		GtkLabel* _tmp7_;
		GtkSpinButton* _tmp8_;
		GtkVBox* _tmp9_;
		GtkVBox* _tmp10_;
		GtkVBox* _tmp11_;
		GtkVBox* _tmp12_;
		_label0 = NULL;
		_hbox0 = NULL;
		_label1 = NULL;
		_label2 = NULL;
		_label3 = NULL;
		_label0 = (_tmp0_ = g_object_ref_sink ((GtkLabel*) gtk_label_new_with_mnemonic ("Display data blocks as images using imagemagick")), _g_object_unref0 (_label0), _tmp0_);
		self->priv->cb = (_tmp1_ = g_object_ref_sink ((GtkComboBox*) gtk_combo_box_new_text ()), _g_object_unref0 (self->priv->cb), _tmp1_);
		_hbox0 = (_tmp2_ = g_object_ref_sink ((GtkHBox*) gtk_hbox_new (FALSE, 10)), _g_object_unref0 (_hbox0), _tmp2_);
		_label1 = (_tmp3_ = g_object_ref_sink ((GtkLabel*) gtk_label_new_with_mnemonic ("Width:")), _g_object_unref0 (_label1), _tmp3_);
		self->priv->width = (_tmp4_ = g_object_ref_sink ((GtkSpinButton*) gtk_spin_button_new_with_range ((double) 50, (double) 5000, (double) 2)), _g_object_unref0 (self->priv->width), _tmp4_);
		_label2 = (_tmp5_ = g_object_ref_sink ((GtkLabel*) gtk_label_new_with_mnemonic ("Height:")), _g_object_unref0 (_label2), _tmp5_);
		self->priv->height = (_tmp6_ = g_object_ref_sink ((GtkSpinButton*) gtk_spin_button_new_with_range ((double) 50, (double) 5000, (double) 2)), _g_object_unref0 (self->priv->height), _tmp6_);
		_label3 = (_tmp7_ = g_object_ref_sink ((GtkLabel*) gtk_label_new_with_mnemonic ("Depth:")), _g_object_unref0 (_label3), _tmp7_);
		self->priv->depth = (_tmp8_ = g_object_ref_sink ((GtkSpinButton*) gtk_spin_button_new_with_range ((double) 1, (double) 32, (double) 1)), _g_object_unref0 (self->priv->depth), _tmp8_);
		gtk_window_set_title ((GtkWindow*) self, "Display data blocks");
		gtk_dialog_set_has_separator ((GtkDialog*) self, FALSE);
		gtk_container_set_border_width ((GtkContainer*) self, (guint) 5);
		g_object_set ((GtkWindow*) self, "default-width", 300, NULL);
		g_object_set ((GtkWindow*) self, "default-height", 80, NULL);
		g_signal_connect_object ((GtkDialog*) self, "response", (GCallback) _display_on_response_gtk_dialog_response, self, 0);
		g_signal_connect ((GtkObject*) self, "destroy", (GCallback) _gtk_main_quit_gtk_object_destroy, NULL);
		gtk_label_set_mnemonic_widget (_label0, (GtkWidget*) self->priv->cb);
		gtk_box_pack_start ((GtkBox*) (_tmp9_ = ((GtkDialog*) self)->vbox, GTK_IS_VBOX (_tmp9_) ? ((GtkVBox*) _tmp9_) : NULL), (GtkWidget*) _label0, FALSE, TRUE, (guint) 0);
		g_signal_connect_object (self->priv->cb, "changed", (GCallback) __lambda0__gtk_combo_box_changed, self, 0);
		gtk_box_pack_start ((GtkBox*) (_tmp10_ = ((GtkDialog*) self)->vbox, GTK_IS_VBOX (_tmp10_) ? ((GtkVBox*) _tmp10_) : NULL), (GtkWidget*) self->priv->cb, FALSE, TRUE, (guint) 0);
		gtk_label_set_mnemonic_widget (_label1, (GtkWidget*) self->priv->width);
		gtk_box_pack_start ((GtkBox*) _hbox0, (GtkWidget*) _label1, FALSE, TRUE, (guint) 0);
		gtk_spin_button_set_value (self->priv->width, (double) 320);
		gtk_container_add ((GtkContainer*) _hbox0, (GtkWidget*) self->priv->width);
		gtk_label_set_mnemonic_widget (_label2, (GtkWidget*) self->priv->height);
		gtk_box_pack_start ((GtkBox*) _hbox0, (GtkWidget*) _label2, FALSE, TRUE, (guint) 0);
		gtk_spin_button_set_value (self->priv->height, (double) 240);
		gtk_container_add ((GtkContainer*) _hbox0, (GtkWidget*) self->priv->height);
		gtk_label_set_mnemonic_widget (_label3, (GtkWidget*) self->priv->depth);
		gtk_box_pack_start ((GtkBox*) _hbox0, (GtkWidget*) _label3, FALSE, TRUE, (guint) 0);
		gtk_spin_button_set_value (self->priv->depth, (double) 8);
		gtk_container_add ((GtkContainer*) _hbox0, (GtkWidget*) self->priv->depth);
		gtk_container_add ((GtkContainer*) (_tmp11_ = ((GtkDialog*) self)->vbox, GTK_IS_VBOX (_tmp11_) ? ((GtkVBox*) _tmp11_) : NULL), (GtkWidget*) _hbox0);
		gtk_box_set_spacing ((GtkBox*) (_tmp12_ = ((GtkDialog*) self)->vbox, GTK_IS_VBOX (_tmp12_) ? ((GtkVBox*) _tmp12_) : NULL), 10);
		_g_object_unref0 (_label0);
		_g_object_unref0 (_hbox0);
		_g_object_unref0 (_label1);
		_g_object_unref0 (_label2);
		_g_object_unref0 (_label3);
	}
	return obj;
}


static void display_class_init (DisplayClass * klass) {
	display_parent_class = g_type_class_peek_parent (klass);
	g_type_class_add_private (klass, sizeof (DisplayPrivate));
	G_OBJECT_CLASS (klass)->constructor = display_constructor;
	G_OBJECT_CLASS (klass)->finalize = display_finalize;
}


static void display_instance_init (Display * self) {
	self->priv = DISPLAY_GET_PRIVATE (self);
}


static void display_finalize (GObject* obj) {
	Display * self;
	self = DISPLAY (obj);
	_g_object_unref0 (self->priv->cb);
	_g_object_unref0 (self->priv->width);
	_g_object_unref0 (self->priv->height);
	_g_object_unref0 (self->priv->depth);
	_g_free0 (self->priv->selected);
	_g_free0 (self->priv->filename);
	G_OBJECT_CLASS (display_parent_class)->finalize (obj);
}


GType display_get_type (void) {
	static GType display_type_id = 0;
	if (display_type_id == 0) {
		static const GTypeInfo g_define_type_info = { sizeof (DisplayClass), (GBaseInitFunc) NULL, (GBaseFinalizeFunc) NULL, (GClassInitFunc) display_class_init, (GClassFinalizeFunc) NULL, NULL, sizeof (Display), 0, (GInstanceInitFunc) display_instance_init, NULL };
		display_type_id = g_type_register_static (GTK_TYPE_DIALOG, "Display", &g_define_type_info, 0);
	}
	return display_type_id;
}


static int _vala_strcmp0 (const char * str1, const char * str2) {
	if (str1 == NULL) {
		return -(str1 != str2);
	}
	if (str2 == NULL) {
		return str1 != str2;
	}
	return strcmp (str1, str2);
}




