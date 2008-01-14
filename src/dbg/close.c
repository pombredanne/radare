/*
 * Copyright (C) 2007, 2008
 *       pancake <youterm.com>
 *
 * radare is part of the radare project
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

#include "libps2fd.h"
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include "debug.h"
#include "mem.h"

int debug_close(int fd)
{
	char buf[16];

	if (ps.opened)
	if (fd == ps.fd) {
		if (getv())
			printf("Do you want to kill the process? (Y/n/c) ");

		terminal_set_raw(1);
		while(read(0,buf,1)>0) {
			buf[1] = '\n';
			write(1, buf, 2);
	
			/* free mapped memory */
			dealloc_all();

			switch(buf[0]) {
			case 'c': case 'C':
				eprintf("Cancelled\n");
				terminal_set_raw(0);
				return -2;
			case 'y': case 'Y': case '\n': case '\r':
				/* TODO: w32 stuff here */
#if __WIN32__ || __CYGWIN__
#else
				ptrace(PTRACE_KILL, ps.pid, 0, 0);
				ptrace(PTRACE_DETACH, ps.pid, 0, 0);
#endif
				kill(ps.pid, SIGKILL);
			case 'n': case 'N':
				/* TODO: w32 stuff here */
#if __WIND32__ || __CYGWIN__
#else
				ptrace(PTRACE_CONT, ps.pid, 0, 0);
				ptrace(PTRACE_DETACH, ps.pid, 0, 0);
#endif
				free(ps.filename);
				ps.opened = 0;
				terminal_set_raw(0);
				return 0;
			}
		}
	}

	return 1;
}
