/*
 * Copyright (C) 2008, 2009, 2010
 *       pancake <youterm.com>
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

static struct list_head sections;

void section_set(ut64 from, ut64 to, ut64 vaddr, ut64 paddr, int rwx, const char *comment)
{
	struct list_head *pos;
	list_for_each(pos, &sections) {
		struct section_t *s = (struct section_t *)list_entry(pos, struct section_t, list);
		if (s->from == from) {
			if (to != -1)
				s->to = to;
			if (vaddr != -1)
				s->vaddr = vaddr;
			if (paddr != -1)
				s->paddr = paddr;
			if (rwx != -1)
				s->rwx = rwx;
			if (comment)
				strncpy(s->comment, comment, 254);
		}
	}
}

void section_add(ut64 from, ut64 to, ut64 vaddr, ut64 paddr, int rwx, const char *comment)
{
	struct section_t *s = (struct section_t *)malloc(sizeof(struct section_t));
	s->from = from+vaddr;
	s->to = to+vaddr;
	s->vaddr = vaddr;
	s->paddr = paddr;
	s->rwx = rwx; //SECTION_R | SECTION_W | SECTION_X;
	if (comment)
		strncpy(s->comment, comment, 254);
	else s->comment[0]='\0';
	list_add(&(s->list), &sections);
}

struct section_t *section_get_i(int idx)
{
	int i = 0;
	struct list_head *pos;
	list_for_each_prev(pos, &sections) {
		struct section_t *s = (struct section_t *)list_entry(pos, struct section_t, list);
		if (i == idx)
			return s;
		i++;
	}
	return NULL;
}

int section_rm(int idx)
{
	struct section_t *s = section_get_i(idx);
	if (s != NULL) {
		list_del((&s->list));
		free(s);
		return 1;
	}
	return 0;
}

void section_list(ut64 addr, int rad)
{
	int i = 0;
	char buf[128];
	struct list_head *pos;
	list_for_each_prev(pos, &sections) {
		struct section_t *s = (struct section_t *)list_entry(pos, struct section_t, list);
		switch (rad) {
		case 2:
			if (addr >=s->from && addr <=s->to)
			cons_printf("%02d %c 0x%08llx - 0x%08llx bs=0x%08llx sz=0x%08llx phy=0x%08llx %s\n",
				i, (addr>=s->from && addr <=s->to)?'*':'.',
				s->from, s->to, s->vaddr, (ut64)((s->to)-(s->from)), s->paddr,
				s->comment[0]?s->comment:"");
			break;
		case 1:
			cons_printf("S 0x%08llx 0x%08llx %s @ 0x%08llx\n",
				s->to-s->from, s->vaddr, s->comment[0]?s->comment:"", s->from);
			cons_printf("Sd 0x%08llx @ 0x%08llx\n", s->paddr, s->from);
			break;
		case 0:
			cons_printf("%02d %c 0x%08llx - 0x%08llx bs=0x%08llx sz=0x%08llx phy=0x%08llx %s",
				i, (addr>=s->from && addr <=s->to)?'*':'.',
				s->from, s->to, s->vaddr, (ut64)((s->to)-(s->from)), s->paddr,
				s->comment[0]?s->comment:"");
			
			if (string_flag_offset(NULL, buf, s->from, 0))
				cons_printf(" ; %s", buf);
#if 0
			ol = section_overlaps(s);
			if (ol != -1)
				cons_printf(" ; Overlaps with %d", ol);
#endif
			cons_printf("\n");
			break;
		}
		i++;
	}
}

void section_list_visual(ut64 seek, ut64 len)
{
	ut64 min = -1;
	ut64 max = -1;
	ut64 mul;
	int j, i;
	struct list_head *pos;
	int width = config.width-50;

	list_for_each(pos, &sections) {
		struct section_t *s = (struct section_t *)list_entry(pos, struct section_t, list);
		if (min == -1 || s->from < min)
			min = s->from;
		if (max == -1 || s->to > max)
			max = s->to;
	}

	mul = (max-min) / width;
	if (min != -1 && mul != 0) {
		i = 0;
		list_for_each_prev(pos, &sections) {
			struct section_t *s = (struct section_t *)list_entry(pos, struct section_t, list);
			cons_printf("%02d  0x%08llx |", i, s->from);
			for(j=0;j<width;j++) {
				if ((j*mul)+min >= s->from && (j*mul)+min <=s->to)
					cons_printf("#");
				else
					cons_printf("-");
			}
			cons_printf("| 0x%08llx %s\n", s->to, s->comment[0]?s->comment:"");
			i++;
		}
		/* current seek */
		if (i>0 && len != 0) {
			cons_printf("=>  0x%08llx |", seek+config.vaddr);
			for(j=0;j<width;j++) {
				if ((j*mul)+min >= seek+config.vaddr && (j*mul)+min <= seek+config.vaddr+len)
					cons_printf("#");
				else
					cons_printf("-");
			}
			cons_printf("| 0x%08llx\n", seek+config.vaddr+len);
		}
	}
}

struct section_t *section_get(ut64 addr)
{
	struct list_head *pos;
	list_for_each(pos, &sections) {
		struct section_t *s = (struct section_t *)list_entry(pos, struct section_t, list);
		if (addr >= s->from && addr <= s->to)
			return s;
	}
	return NULL;
}

ut64 section_get_paddr(ut64 addr)
{
	struct section_t *s = section_get(addr);
	if (s != NULL)
		return s->paddr;
	return -1;
}

ut64 section_get_vaddr(ut64 addr)
{
	struct section_t *s = section_get(addr);
	if (s != NULL)
		return s->vaddr;
	return -1;
}

int section_overlaps(struct section_t *s)
{
	int i = 0;
	struct list_head *pos;
	list_for_each_prev(pos, &sections) {
		struct section_t *s2 = (struct section_t *)list_entry(pos, struct section_t, list);
		if (s != s2) {
			if (s->from >= s2->from) {
				if (s2->to < s->from)
					return i;
			} else {
				if (s->to < s2->from)
					return i;
			}
		}
		i++;
	}
	return -1;
}

// seek 
ut64 last_align = 0;
ut64 section_align(ut64 addr, ut64 vaddr, ut64 paddr)
{
	struct list_head *pos;
	if (addr == last_align)
		return last_align;

	list_for_each_prev(pos, &sections) {
		struct section_t *s = (struct section_t *)list_entry(pos, struct section_t, list);
		if (addr >= s->from && addr <= s->to)
			return ( s->paddr + addr - s->from ); 
	}
	last_align = addr-vaddr+paddr;
	//printf("? 0x%llx-0x%llx+0x%llx\n", addr, vaddr, paddr);
	return last_align;
}

void section_init(int foo)
{
	INIT_LIST_HEAD(&(sections));
}
