
#include "disassembler.h"
#include "utils.h"
#include <gobject/gvaluecollector.h>




enum  {
	RADARE_DISASSEMBLER_DUMMY_PROPERTY
};
static gpointer radare_disassembler_parent_class = NULL;
static void radare_disassembler_finalize (RadareDisassembler* obj);



#line 9 "disassembler.vala"
char* radare_disassembler_arch (void) {
#line 11 "disassembler.vala"
	return radare_utils_get ("ARCH");
}


#line 3 "disassembler.vala"
RadareDisassembler* radare_disassembler_construct (GType object_type) {
	RadareDisassembler* self;
	self = ((RadareDisassembler*) (g_type_create_instance (object_type)));
	return self;
}


#line 3 "disassembler.vala"
RadareDisassembler* radare_disassembler_new (void) {
#line 3 "disassembler.vala"
	return radare_disassembler_construct (RADARE_TYPE_DISASSEMBLER);
}


static void radare_value_disassembler_init (GValue* value) {
	value->data[0].v_pointer = NULL;
}


static void radare_value_disassembler_free_value (GValue* value) {
	if (value->data[0].v_pointer) {
		radare_disassembler_unref (value->data[0].v_pointer);
	}
}


static void radare_value_disassembler_copy_value (const GValue* src_value, GValue* dest_value) {
	if (src_value->data[0].v_pointer) {
		dest_value->data[0].v_pointer = radare_disassembler_ref (src_value->data[0].v_pointer);
	} else {
		dest_value->data[0].v_pointer = NULL;
	}
}


static gpointer radare_value_disassembler_peek_pointer (const GValue* value) {
	return value->data[0].v_pointer;
}


static gchar* radare_value_disassembler_collect_value (GValue* value, guint n_collect_values, GTypeCValue* collect_values, guint collect_flags) {
	if (collect_values[0].v_pointer) {
		RadareDisassembler* object;
		object = value->data[0].v_pointer;
		if (object->parent_instance.g_class == NULL) {
			return g_strconcat ("invalid unclassed object pointer for value type `", G_VALUE_TYPE_NAME (value), "'", NULL);
		} else if (!g_value_type_compatible (G_OBJECT_TYPE (object), G_VALUE_TYPE (value))) {
			return g_strconcat ("invalid object type `", G_OBJECT_TYPE (object), "' for value type `", G_VALUE_TYPE_NAME (value), "'", NULL);
		}
	} else {
		value->data[0].v_pointer = NULL;
	}
	return NULL;
}


static gchar* radare_value_disassembler_lcopy_value (const GValue* value, guint n_collect_values, GTypeCValue* collect_values, guint collect_flags) {
	RadareDisassembler** object_p;
	object_p = collect_values[0].v_pointer;
	if (!object_p) {
	}
	if (!value->data[0].v_pointer) {
		*object_p = NULL;
	} else if (collect_flags && G_VALUE_NOCOPY_CONTENTS) {
		*object_p = value->data[0].v_pointer;
	} else {
		*object_p = radare_disassembler_ref (value->data[0].v_pointer);
	}
	return NULL;
}


GParamSpec* radare_param_spec_disassembler (const gchar* name, const gchar* nick, const gchar* blurb, GType object_type, GParamFlags flags) {
	RadareParamSpecDisassembler* spec;
	g_return_val_if_fail (g_type_is_a (object_type, RADARE_TYPE_DISASSEMBLER), NULL);
	spec = g_param_spec_internal (G_TYPE_PARAM_OBJECT, name, nick, blurb, flags);
	G_PARAM_SPEC (spec)->value_type = object_type;
	return G_PARAM_SPEC (spec);
}


gpointer radare_value_get_disassembler (const GValue* value) {
	g_return_val_if_fail (G_TYPE_CHECK_VALUE_TYPE (value, RADARE_TYPE_DISASSEMBLER), NULL);
	return value->data[0].v_pointer;
}


void radare_value_set_disassembler (GValue* value, gpointer v_object) {
	RadareDisassembler* old;
	g_return_if_fail (G_TYPE_CHECK_VALUE_TYPE (value, RADARE_TYPE_DISASSEMBLER));
	old = value->data[0].v_pointer;
	if (v_object) {
		g_return_if_fail (G_TYPE_CHECK_INSTANCE_TYPE (v_object, RADARE_TYPE_DISASSEMBLER));
		g_return_if_fail (g_value_type_compatible (G_TYPE_FROM_INSTANCE (v_object), G_VALUE_TYPE (value)));
		value->data[0].v_pointer = v_object;
		radare_disassembler_ref (value->data[0].v_pointer);
	} else {
		value->data[0].v_pointer = NULL;
	}
	if (old) {
		radare_disassembler_unref (old);
	}
}


static void radare_disassembler_class_init (RadareDisassemblerClass * klass) {
	radare_disassembler_parent_class = g_type_class_peek_parent (klass);
	RADARE_DISASSEMBLER_CLASS (klass)->finalize = radare_disassembler_finalize;
}


static void radare_disassembler_instance_init (RadareDisassembler * self) {
	self->INTEL = g_strdup ("intel");
	self->ARM = g_strdup ("arm");
	self->JAVA = g_strdup ("java");
	self->ref_count = 1;
}


static void radare_disassembler_finalize (RadareDisassembler* obj) {
	RadareDisassembler * self;
	self = RADARE_DISASSEMBLER (obj);
	self->INTEL = (g_free (self->INTEL), NULL);
	self->ARM = (g_free (self->ARM), NULL);
	self->JAVA = (g_free (self->JAVA), NULL);
}


GType radare_disassembler_get_type (void) {
	static GType radare_disassembler_type_id = 0;
	if (radare_disassembler_type_id == 0) {
		static const GTypeValueTable g_define_type_value_table = { radare_value_disassembler_init, radare_value_disassembler_free_value, radare_value_disassembler_copy_value, radare_value_disassembler_peek_pointer, "p", radare_value_disassembler_collect_value, "p", radare_value_disassembler_lcopy_value };
		static const GTypeInfo g_define_type_info = { sizeof (RadareDisassemblerClass), (GBaseInitFunc) NULL, (GBaseFinalizeFunc) NULL, (GClassInitFunc) radare_disassembler_class_init, (GClassFinalizeFunc) NULL, NULL, sizeof (RadareDisassembler), 0, (GInstanceInitFunc) radare_disassembler_instance_init, &g_define_type_value_table };
		static const GTypeFundamentalInfo g_define_type_fundamental_info = { (G_TYPE_FLAG_CLASSED | G_TYPE_FLAG_INSTANTIATABLE | G_TYPE_FLAG_DERIVABLE | G_TYPE_FLAG_DEEP_DERIVABLE) };
		radare_disassembler_type_id = g_type_register_fundamental (g_type_fundamental_next (), "RadareDisassembler", &g_define_type_info, &g_define_type_fundamental_info, 0);
	}
	return radare_disassembler_type_id;
}


gpointer radare_disassembler_ref (gpointer instance) {
	RadareDisassembler* self;
	self = instance;
	g_atomic_int_inc (&self->ref_count);
	return instance;
}


void radare_disassembler_unref (gpointer instance) {
	RadareDisassembler* self;
	self = instance;
	if (g_atomic_int_dec_and_test (&self->ref_count)) {
		RADARE_DISASSEMBLER_GET_CLASS (self)->finalize (self);
		g_type_free_instance (((GTypeInstance *) (self)));
	}
}




