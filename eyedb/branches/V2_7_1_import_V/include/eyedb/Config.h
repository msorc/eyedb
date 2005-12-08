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


#ifndef _EYEDB_CONFIG_H
#define _EYEDB_CONFIG_H

namespace eyedb {

  /**
     @addtogroup eyedb
     @{
  */

  /**
     Not yet documented.
  */
  class Config {

    friend ostream& operator<<( ostream&, const Config&);

  public:
    /**
       Not yet documented
       @param file
    */
    Config(const char *file = 0);

    /**
       Not yet documented
       @param config
    */
    Config(const Config &config);

    /**
       Not yet documented
       @param config
       @return
    */
    Config& operator=(const Config &config);

    /**
       Not yet documented
       @param name
       @return
    */
    const char *getValue(const char *name);

    /**
       Not yet documented
       @param name
       @param value
    */
    void setValue(const char *name, const char *value);

    /**
       Not yet documented
       @param file
    */
    void add(const char *file);

    /**
       Not yet documented
       @return
    */
    static Config *getDefaultConfig();

    /**
       Not yet documented
       @param config
    */
    static void setDefaultConfig(Config *config);

    struct Item {
      char *name;
      char *value;

      /**
	 Not yet documented
      */
      Item();

      /**
	 Not yet documented
	 @param name
	 @param value
      */
      Item(const char *name, const char *value);

      /**
	 Not yet documented
	 @param item
      */
      Item(const Item &item);

      /**
	 Not yet documented
	 @param item
	 @return
      */
      Item& operator=(const Item &item);

      ~Item();
    };

    Item *getValues(int &item_cnt) const;

  public: // package level
    static void init();
    static void _release();

    ~Config();

  private:

    LinkedList list;
    void add(const char *name, const char *value);
    friend class Object;
    void garbage();
  };

  const char *getConfigValue(const char *name);

  /**
     @}
  */

}

#endif
