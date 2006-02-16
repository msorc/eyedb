/* 
   EyeDB Object Database Management System
   Copyright (C) 1994-1999,2004-2006 SYSRA
   
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


#ifndef _EYEDB_CONFIG_H
#define _EYEDB_CONFIG_H

namespace eyedb {

  /**
     @addtogroup eyedb
     @{
  */

  /**
     A class storing the configuration variables values for the client and the server.
  */
  class Config {

    friend std::ostream& operator<<( std::ostream&, const Config&);

  public:
    /**
       Get a client configuration variable value.

       @param name the variable name
       @return the variable value, or null if not mapped
    */
    static const char* getClientValue( const char *name);

    /**
       Get a server configuration variable value.

       @param name the variable name
       @return the variable value, or null if not mapped
    */
    static const char* getServerValue( const char *name);

  public: // package level

    Config(const Config &config);

    Config::Config(const char *file);

    struct Item {
      char *name;
      char *value;

      Item();
      Item(const char *name, const char *value);
      Item(const Item &item);
      Item& operator=(const Item &item);
      ~Item();
    };

    static Config* getClientConfig();
    static Config* getServerConfig();

    static Status setClientConfigFile(const std::string &);
    static Status setServerConfigFile(const std::string &);

    void add(const char *file, int quietFileNotFoundError = 0);

    const char *getValue(const char *name);
    void setValue(const char *name, const char *value);

    Item *getValues(int &item_cnt) const;

    static void init();
    static void _release();

    ~Config();

  private:
    friend class Object;

    Config();
    Config& operator=(const Config &config);

    void setClientDefaults();
    void setServerDefaults();

    void garbage();

    static Config *theClientConfig;
    static Config *theServerConfig;

    static std::string client_config_file;
    static std::string server_config_file;

    LinkedList list;
  };

  /**
     @}
  */

}

#endif
