/* Copyright (C) 2004   db4objects Inc.   http://www.db4o.com */
package org.polepos.teams.db4o;


public class Db4oOptions {
    
    public static final int NO_FLUSH = 1;
    public static final int CLIENT_SERVER = 2;
    public static final int CLIENT_SERVER_TCP = 3;
    public static final int MEMORY_IO = 4;
    public static final int CACHED_BTREE_ROOT = 5;
    public static final int LAZY_QUERIES = 6;
    public static final int SNAPSHOT_QUERIES = 7;
    public static final int CONCURRENT_COUNT = 10;
	public static final int NORMAL_COLLECTION = 11;
	public static final int P1FAST_COLLECTION = 12;
    public static final int INDEX_FREESPACE = 13;
    public static final int BTREE_FREESPACE = 14;
	
	public static boolean containsOption(int[] options, int value) {
		if(options == null) {
			return false;
		}
		for (int opt : options) {
			if(opt == value) {
				return true;
			}
		}
		return false;
	}
}
