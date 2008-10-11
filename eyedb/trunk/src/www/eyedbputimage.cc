/* 
   EyeDB Object Database Management System
   Copyright (C) 1994-2008 SYSRA
   
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


#include <eyedb/eyedb.h>
#include <fcntl.h>
#include <sys/stat.h>

using namespace eyedb;
using namespace std;

static const char *db_str;
static const char *classname;
static const char *attrname;
static const Class *image_class;
static const Attribute *image_item;
static int img_start;
static Status status;

#include "../eyedb/GetOpt.h"

#define PROG "eyedbputimage"

static Connection *conn;
static Database *db;

static int
usage()
{
  fprintf(stderr, "usage: " PROG " DBNAME IMAGE... "
	  "[--class=CLASSNAME ATTRNAME]\n");
  return 1;
}

#define PRS() fprintf(stderr, PROG ": %s\n", status->getDesc())

static ImageType::Type
get_image_type(const char *im)
{
  int len = strlen(im);
  if (len < 4)
    return (ImageType::Type)0;

  if (!strncmp(&im[len-4], ".jpg", 4) ||
      !strncmp(&im[len-4], ".JPG", 4) ||
      !strncmp(&im[len-5], ".jpeg", 5) ||
      !strncmp(&im[len-5], ".JPEG", 5))
    return ImageType::JPEG_IMG_TYPE;

  if (!strncmp(&im[len-4], ".gif", 4) ||
      !strncmp(&im[len-4], ".GIF", 4))
    return ImageType::GIF_IMG_TYPE;

  return (ImageType::Type)0;
}

static void
put_image(const char *im)
{
  ImageType::Type im_type = get_image_type(im);
  static const Attribute *item;

  if (im_type == (ImageType::Type)0)
    {
      fprintf(stderr, PROG ": unknown image type for '%s'\n", im);
      return;
    }

  int fd = open(im, O_RDONLY);
  if (fd < 0)
    {
      fprintf(stderr, PROG ": cannot open image '%s'\n", im);
      return;
    }

  struct stat st;
  fstat(fd, &st);
  char *buf = (char *)malloc(st.st_size+1);
  if (read(fd, buf, st.st_size) != st.st_size)
    {
      fprintf(stderr, PROG ": cannot read image '%s'\n", im);
      free(buf);
      close(fd);
      return;
    }

  Image *image;

  Object *oim;
  if  (image_class)
    {
      oim = image_class->newObj(db);
      Status status = image_item->getValue(oim, (Data *)&image, 1, 0);
      if (status)
	{
	  fprintf(stderr, PROG ": %s\n", status->getDesc());
	  close(fd);
	  return;
	}
      image = (Image *)oim;
  }
  else
    oim = image = (Image *)db->getSchema()->getClass("image")->newObj(db);

  image->setType(im_type);
  image->setName(im);

  if (!item)
    item = image->getClass()->getAttribute("data");

  eyedb_CHECK(item->setSize(image, st.st_size));

  eyedb_CHECK(item->setValue(image, (Data)buf, st.st_size, 0));

  eyedb_CHECK(oim->realize());

  free(buf);
  close(fd);
}

int
getopts(int argc, char *argv[])
{
  if (argc < 3)
    return usage();

  for (int i = 1; i < argc; )
    {
      const char *s = argv[i];
      string value;
      if (!strcmp(s, "--class")) {
	if (argc - i < 2)
	  return usage();
	classname = argv[++i];
	attrname = argv[++i];
	++i;
      }
      else if (!db_str)
	db_str = argv[i++];
      else if (!img_start)
	img_start = i++;
      else
	i++;
    }

  if (!db_str)
    return usage();

  return 0;
}

int
main(int argc, char *argv[])
{
  eyedb::init(argc, argv); 

  if (getopts(argc, argv))
    return 1;

  conn = new Connection();

  if (status = conn->open())
    {
      PRS();
      return 1;
    }

  db = new Database(db_str);
  
  if (status = db->open(conn, Database::DBRW))
    {
      PRS();
      return 1;
    }

  if (status = db->transactionBegin())
    {
      PRS();
      return 1;
    }

  if (classname && attrname)
    {
      image_class = db->getSchema()->getClass(classname);
      if (!image_class)
	{
	  fprintf(stderr, PROG ": cannot find class `%s' in database `%s'\n",
		  classname, db_str);
	  return 1;
	}

      image_item = image_class->getAttribute(attrname);
      if (!image_item)
	{
	  fprintf(stderr, PROG ": cannot find item `%s' in class `%s' "
		  "in database `%s'\n",
		  attrname, classname, db_str);
	  return 1;
	}
    }

  for (int i = img_start; i < argc; i++)
    put_image(argv[i]);

  db->transactionCommit();

  return 0;
}
