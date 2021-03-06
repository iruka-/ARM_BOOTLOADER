/***************************************************************************
 *   Copyright (C) 2008 by Øyvind Harboe                                   *
 *   oyvind.harboe@zylin.com                                               *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/
#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "windows.h"

#include <jtag/interface.h>
#include "bitbang.h"
#include "hello.h"


HINSTANCE h_DLL = NULL;		// dll handle

void driver_load_dll(void);

extern	struct jtag_interface *jtag_interfaces[];
//struct jtag_command *jtag_command_queue;


typedef	void  *(__stdcall *_get_if_spec)(struct jtag_command **q);

//
//	get_if_spec is a function pointer.
//
_get_if_spec  get_if_spec;

/* The dummy driver is used to easily check the code path
 * where the target is unresponsive.
 */
struct jtag_interface dummy_interface =
{
	.name = "dummy",
	.execute_queue = NULL,
	.speed = NULL,
	.commands = NULL,
	.init = NULL,
	.quit = NULL,
	.khz = NULL,
	.speed_div = NULL,
	.power_dropout = NULL,
	.srst_asserted = NULL,
};

#define	DRIVER_DLL	"cdclink.dll"

void driver_load_dll(void)
{
	printf("driver_load_dll:\n");
	if(	h_DLL != NULL ) return;

	h_DLL = LoadLibrary(DRIVER_DLL);
	if (!h_DLL) {
		fprintf(stderr, "Error at Load '%s'\n",DRIVER_DLL);
//		MessageBox(NULL, "Error at Load " DRIVER_DLL, "ERR", MB_OK);
		return ;
	}	

	get_if_spec = (_get_if_spec )GetProcAddress(h_DLL, "get_if_spec");
	if(get_if_spec!=NULL) {
		void *p = get_if_spec(&jtag_command_queue);
		jtag_interfaces[0] = (struct jtag_interface *) p;
	}else{
		fprintf(stderr, "Error at find entry point of '%s::get_if_spec()'\n",DRIVER_DLL);
	}
}
