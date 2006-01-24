/* 
   EyeDB Object Database Management System
   Copyright (C) 1994-1999,2004,2005 SYSRA
   
   EyeDB is free software; you can redistribute it and/or
   modify it under the terms of the GNU Lesser General Public
   License as published by the Free Software Foundation; either
   version 2.1 of the License, or (at your option) any later version.
   
   EyeDB is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Lesser General Public License for more details.
   
   You should have received a copy of the GNU Lesser General Public
   License along with this library; if not, write to the Free Software
   Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA 
*/

/*
   Author: Eric Viara <viara@sysra.com>
*/

#include <eyedbconfig.h>

#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <eyedbsm_p.h>

/*#define	PRIV_DEBUG*/

namespace eyedbsm {

static gid_t egid;
static gid_t rgid;
static enum state { uninitialized, unprivileged, privileged } privileges_state = uninitialized;

static void
check(enum state wanted)
{
	if (privileges_state == wanted)
		return;
	if (privileges_state == uninitialized)
		fprintf(stderr, "Fatal error: Privileges uninitilized (call idbInit at the beginning of main)\n");
	else if (privileges_state == unprivileged)
		fprintf(stderr, "Fatal error: Privileges not acquired (acquire/accept imbalance)\n");
	else if (privileges_state == privileged)
		fprintf(stderr, "Fatal error: Privileges already acquired (acquire/accept imbalance)\n");
	exit(1);
}

Status
privilegeInit()
{
	if (privileges_state == uninitialized)
	{
		egid = getegid();
		rgid = getgid();
		if (egid != rgid && setegid(rgid) < 0)
			return statusMake(FATAL_ERROR, "");
		privileges_state = unprivileged;
	}
	return Success;
}

Status
privilegeAcquire()
{
	check(unprivileged);
	if (egid != rgid && setegid(egid) < 0)
		return statusMake(FATAL_ERROR, "");
#ifdef	PRIV_DEBUG
	fprintf(stderr, "privilegeAcquire: rgid = %d egid = %d\n", (int)getgid(), (int)getegid());
#endif
	privileges_state = privileged;
	return Success;
}

Status
privilegeRelease()
{
	check(privileged);
	if (egid != rgid && setegid(rgid) < 0)
		return statusMake(FATAL_ERROR, "");
	privileges_state = unprivileged;
#ifdef	PRIV_DEBUG
	fprintf(stderr, "privilegeRelease: rgid = %d egid = %d\n", (int)getgid(), (int)getegid());
#endif
	return Success;
}

Status
privilegeCheck()
{
	check(unprivileged);
	return Success;
}

}
