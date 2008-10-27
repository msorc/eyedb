/* 
This file is part of the PolePosition database benchmark
http://www.polepos.org

This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License
as published by the Free Software Foundation; either version 2
of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public
License along with this program; if not, write to the Free
Software Foundation, Inc., 59 Temple Place - Suite 330, Boston,
MA  02111-1307, USA. */


package org.polepos.framework;

import java.io.*;
import java.net.*;
import java.util.*;

/**
 * This classloader allows to run the same team against multiple versions of a 
 * candidate library in a single polepos season.
 * 
 * NOTE: This classloader is *not* a good Java citizen - it doesn't respect the
 * conventional delegation order but tries to load classes with the configured
 * prefixes prior to asking its parent classloader. Use with care!
 */
public class VersionClassLoader extends URLClassLoader {
    private final Map<String,Class> cache=new HashMap<String,Class>();
    
    private final String[] _prefixes;
    
    /**
     * @param urls The URLs to be included in this classloader's path
     * @param prefixes The package prefixes to be handled without delegation 
     * to the parent classloader
     */
    public VersionClassLoader(URL[] urls,String[] prefixes) {
        super(urls);
        _prefixes=prefixes;
    }

    protected Class<?> loadClass(String name,boolean resolve) throws ClassNotFoundException {
        try {
            if(cache.containsKey(name)) {
                return cache.get(name);
            }
            if(!knownPrefix(name)) {
                return super.loadClass(name,resolve);
            }
            String resName=name.replace('.','/')+".class";
            URL resURL=findResource(resName);
            if(resURL==null) {
                return super.loadClass(name,resolve);
            }
            //System.out.println(resURL);
            byte[] full = readBytes(resURL);
            Class clazz=defineClass(name, full, 0, full.length);
            if(resolve) {
                resolveClass(clazz);
            }
            cache.put(name,clazz);
            return clazz;
        } catch (Exception exc) {
            return super.loadClass(name,resolve);
        }
    }

    private byte[] readBytes(URL resURL) throws IOException {
        InputStream in=resURL.openStream();
        ByteArrayOutputStream out=new ByteArrayOutputStream();
        byte[] buf=new byte[1024];
        int bytesRead=0;
        while((bytesRead=in.read(buf))>=0) {
            out.write(buf,0,bytesRead);
        }
        in.close();
        out.close();
        byte[] full=out.toByteArray();
        return full;
    }
    
    private boolean knownPrefix(String className) {
        for (int idx = 0; idx < _prefixes.length; idx++) {
            if(className.startsWith(_prefixes[idx])) {
                return true;
            }
        }
        return false;
    }
}
