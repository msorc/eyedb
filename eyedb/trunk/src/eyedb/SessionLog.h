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


#ifndef _eyedb_sesslog_
#define _eyedb_sesslog_

#include <eyedb/eyedb.h>
#include <eyedbsm/eyedbsm.h>
#include "eyedbsm/transaction.h"
#include "eyedbsm/mutex.h"
#include <eyedblib/m_mem.h>

namespace eyedb {

  class SessionHead;
  class ClientInfo;

  class ClientSessionLog {

  public:
    ClientSessionLog(ClientInfo *);

    ClientInfo *getClientInfo() {return clinfo;}

    void addDatabase(const char *name, const char *userauth, int flags);
    void suppressDatabase(const char *name, const char *userauth, int flags);

    ~ClientSessionLog();

  private:
    ClientInfo *clinfo;
  };

  class SessionLog {

  public:
    // open constructor
    SessionLog(const char *port, const char *logdir);

    // create constructor
    SessionLog(const char *logdir, const char *version,
	       int nports, const char *ports[], const char *voldir,
	       const char *logdev, int loglevel);

    Status add(const char *hostname, const char *username,
	       const char *progname, int pid,
	       ClientSessionLog*&);

    Bool isUp() const;

    void suppress(ClientSessionLog *);

    Status stopServers(Bool force);

    void display(FILE *fd, Bool nolock = False);

    void remove();

    Status getStatus() const {return status;}

    static void release();

    ~SessionLog();

  private:
    char *idbport;
    eyedbsm::XMHandle *xm_connlog;
    m_Map *m_connlog;
    char *addr_connlog;
    SessionHead *connhead;
    Status status;
    Bool islocked;
    static SessionLog *sesslog;
    int file_cnt;
    char **files;
    eyedbsm::Mutex mp;

    Status openRealize(const char *idbport, const char *logdir,
		       Bool create);
    char *makeFile(const char *_idbport, const char *_logdir);
    int get_nb_clients();
  };

}

#endif
