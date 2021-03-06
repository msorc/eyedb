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


#include <config.h>

#include "eyedb_p.h"

namespace eyedb {

  const char Connection::LocalHost[]      = "-idb-local-host";
  const char Connection::DefaultHost[]    = "-idb-default-host";
  const char Connection::DefaultIDBPort[] = "-idb-default-idb-port";

  const char Connection::DefaultIDBPortValue[] = "6123";

#ifdef HAVE_UNISTD_H
#include <unistd.h>
#endif

  Connection::Connection()
  {
    host     = 0;
    port = 0;
    connh    = 0;
    oql_info = 0;
    user_data = 0;
  }

  const char *Connection::getHost(void) const
  {
    return host;
  }

  const char *Connection::getIDBPort(void) const
  {
    return port;
  }

  int Connection::getServerPid(void) const
  {
    return sv_pid;
  }

  int Connection::getServerUid(void) const
  {
    return sv_uid;
  }

  extern char *prog_name;

#include <pwd.h>

  static const char *
  getUserName()
  {
    struct passwd *p = getpwuid(getuid());
    if (!p)
      return "<unknown>";
    return p->pw_name;
  }

  Status Connection::open(const char *_host, const char *_port)
  {
    if (connh)
      return Exception::make(IDB_CONNECTION_FAILURE, "connection already made");

    if (!_host)
      return Exception::make(IDB_CONNECTION_FAILURE, "cannot connect on not specified host");
    if (!_port)
      return Exception::make(IDB_CONNECTION_FAILURE, "cannot connect on not specified eyedb port");

    if (!strcmp(_host, LocalHost))
      _host = "localhost";
    else if (!strcmp(_host, DefaultHost))
      _host = getDefaultHost();

    if (!_host)
      return Exception::make(IDB_CONNECTION_FAILURE, "default host is not set for connection");

    if (!strcmp(_port, DefaultIDBPort))
      _port = getDefaultIDBPort();

    if (!_port)
      return Exception::make(IDB_CONNECTION_FAILURE, "default eyedb port is not set for connection");

    if ((connOpen(_host, _port, &connh, 0)) == RPCSuccess)
      {
	if (host)
	  host = strdup(_host);
	else
	  host = strdup("localhost");

	port = strdup(_port);

	char hostname[256];
	gethostname(hostname, sizeof(hostname)-1);
	RPCStatus rpc_status = 
	  set_conn_info(connh, hostname, getUserName(), prog_name,
			    &sv_pid, &sv_uid, getVersionNumber());
	if (rpc_status)
	  return Exception::make(IDB_CONNECTION_FAILURE, rpc_status->err_msg);

	if (getenv("EYEDBWAIT")) {
	  printf("### Connection Established for PID %d ###\n", getpid());
	  printf("Continue? ");
	  getchar();
	}

	return Success;
      }

    return Exception::make(IDB_CONNECTION_FAILURE,
			   "connection refused to '%s', eyedb port '%s'",
			   (_host ? _host : "localhost"), _port);
  }

  Status Connection::close(void)
  {
    if (!connh)
      return Exception::make(IDB_CONNECTION_FAILURE, "connection not opened");

    Status status = StatusMake(connClose(connh));
    if (status)
      return status;

    connh = 0;
    return Success;
  }

  Status
  Connection::sendInterrupt()
  {
    return StatusMake(backendInterrupt(connh, 0));
  }

  Connection::~Connection()
  {
    if (connh)
      close();
    free(host);
    free(port);
    free(connh);
  }

  char *Connection::default_host;
  char *Connection::default_port;
  char *Connection::default_user;
  char *Connection::default_passwd;

  Bool Connection::set_auth_required = False;

  void Connection::init()
  {
    const char *s;

    if (!default_host)
      default_host = strdup( (s = getConfigValue("host")) ?
			     s : "localhost" );
    if (!default_port)
      default_port = strdup( (s = getConfigValue("port")) ?
				 s : DefaultIDBPortValue);

    if (!set_auth_required)
      {
	if (!default_user)
	  default_user = ((s = getConfigValue("user")) ?
			  strdup(s) : strdup(""));
	if (!default_passwd)
	  default_passwd = ((s = getConfigValue("passwd")) ?
			    strdup(s) : strdup(""));
      }
  }

  void Connection::_release()
  {
    free(default_host);
    free(default_port);

    free(default_user);
    free(default_passwd);
  }

  void Connection::setAuthRequired(Bool _set_auth_required)
  {
    set_auth_required = _set_auth_required;
  }

  void Connection::setDefaultHost(const char *_host)
  {
    free(default_host);
    default_host = strdup(_host);
  }

  const char *Connection::getDefaultHost()
  {
    return default_host;
  }

  void Connection::setDefaultIDBPort(const char *_port)
  {
    free(default_port);
    default_port = strdup(_port);
  }

  const char *Connection::getDefaultIDBPort()
  {
    return default_port;
  }

  void Connection::setDefaultUser(const char *_user)
  {
    free(default_user);
    default_user = strdup(_user);
  }

  const char *Connection::getDefaultUser()
  {
    return default_user;
  }

  void Connection::setDefaultPasswd(const char *_passwd)
  {
    free(default_passwd);
    default_passwd = strdup(_passwd);
  }

  const char *Connection::getDefaultPasswd()
  {
    return default_passwd;
  }

  void *Connection::setUserData(void *ud)
  {
    void *oud = user_data;
    user_data = ud;
    return oud;
  }

  void *Connection::getUserData()
  {
    return user_data;
  }

  const void *Connection::getUserData() const
  {
    return user_data;
  }

  void *Connection::setOQLInfo(void *noql_info)
  {
    void *x = oql_info;
    oql_info = noql_info;
    return x;
  }

  void *Connection::getOQLInfo()
  {
    return oql_info;
  }

  //
  // Server message management
  //

  eyedblib::Thread *srv_msg_thr;

  struct Connection::ServerMessageContext {
    ConnHandle *connh;
    const ServerMessageDisplayer &dsp;

    ServerMessageContext(ConnHandle *_connh, 
			 const ServerMessageDisplayer &_dsp) :
      connh(_connh), dsp(_dsp)
    {
    }
  };

  static void *
  srv_msg_listen(void *x)
  {
    Connection::ServerMessageContext *srv_msg_ctx = (Connection::ServerMessageContext *)x;

    ConnHandle *connh = srv_msg_ctx->connh;

    for (;;) {
      int type = IDB_SERVER_MESSAGE;
      unsigned int size;
      Data data;
      RPCStatus rpc_status = getServerOutOfBandData(connh, &type, &data,
							&size);
      if (rpc_status) {
	std::string msg =
	  std::string("Thread for echoing server messages got an "
		      "unexepected error: #") + str_convert((long)rpc_status->err) +
	  rpc_status->err_msg + "\n";
	srv_msg_ctx->dsp.display(msg.c_str());
	break;
      }

      assert(type == IDB_SERVER_MESSAGE);
      if (data) {
	srv_msg_ctx->dsp.display((const char *)data);
	delete [] data;
      }
    }
    return 0;
  }

  Status
  Connection::echoServerMessages(const ServerMessageDisplayer &dsp) const
  {
    if (srv_msg_thr)
      return Exception::make(IDB_ERROR,
			     "a thread is already echoing server messages");

    // 1st thread for remote listening
    ServerMessageContext *srv_msg_ctx = new ServerMessageContext(connh, dsp);
    srv_msg_thr = new eyedblib::Thread();
    srv_msg_thr->execute(srv_msg_listen, srv_msg_ctx);

    // 2nd thread for local listening
    srv_msg_ctx = new ServerMessageContext(0, dsp);
    srv_msg_thr = new eyedblib::Thread();
    srv_msg_thr->execute(srv_msg_listen, srv_msg_ctx);
    return Success;
  }

  void
  Connection::setServerMessage(const char *msg)
  {
    setServerMessage(msg);
  }

  void
  Connection::setOutOfBandData(unsigned int type, unsigned char *data,
			       unsigned int len)
  {
    setServerOutOfBandData(type, data, len);
  }

  Bool
  Connection::isBackendInterrupted()
  {
    return isBackendInterrupted();
  }

  StdServerMessageDisplayer::StdServerMessageDisplayer(FILE *_fd)
  {
    fd = _fd;
    os = 0;
  }

  StdServerMessageDisplayer::StdServerMessageDisplayer(ostream &_os)
  {
    fd = 0;
    os = &_os;
  }

  void StdServerMessageDisplayer::display(const char *msg) const
  {
    if (fd) {
      /*
	fprintf(fd, msg);
	fflush(fd);
      */
      write(fileno(fd), msg, strlen(msg));
      return;
    }
    (*os) << msg << flush;
  }
}
