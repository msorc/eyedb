
//
// class ImageType
//
// package org.eyedb.utils
//
// Generated by eyedbodl at Sun Feb 11 18:15:46 2007
//

package org.eyedb.utils;

import org.eyedb.utils.*;
import org.eyedb.syscls.*;

public class ImageType extends org.eyedb.Enum {

  ImageType(org.eyedb.Database db)
  {
    super(db);
  }

  ImageType()
  {
    super();
  }

  public static final int GIF_IMG_TYPE = 1;
  public static final int JPEG_IMG_TYPE = 2;
  public static final int PM_IMG_TYPE = 3;
  public static final int PBM_IMG_TYPE = 4;
  public static final int X11BITMAP_IMG_TYPE = 5;
  public static final int BMP_IMG_TYPE = 6;
  public static final int TIFF_IMG_TYPE = 7;

  static org.eyedb.EnumClass make(org.eyedb.EnumClass ImageType_class, org.eyedb.Schema m)
  {
    if (ImageType_class == null)
      return new org.eyedb.EnumClass("image_type");

    org.eyedb.EnumItem[] en = new org.eyedb.EnumItem[7];
    en[0] = new org.eyedb.EnumItem("GIF_IMG_TYPE", 1, 0);
    en[1] = new org.eyedb.EnumItem("JPEG_IMG_TYPE", 2, 1);
    en[2] = new org.eyedb.EnumItem("PM_IMG_TYPE", 3, 2);
    en[3] = new org.eyedb.EnumItem("PBM_IMG_TYPE", 4, 3);
    en[4] = new org.eyedb.EnumItem("X11BITMAP_IMG_TYPE", 5, 4);
    en[5] = new org.eyedb.EnumItem("BMP_IMG_TYPE", 6, 5);
    en[6] = new org.eyedb.EnumItem("TIFF_IMG_TYPE", 7, 6);

    ImageType_class.setEnumItems(en);

    return ImageType_class;
  }

  static void init_p()
  {
    idbclass = make(null, null);
  }

  static void init()
  {
    make((org.eyedb.EnumClass)idbclass, null);
  }
  public static org.eyedb.Class idbclass;
}

