/* Copyright (C) 2004 - 2006  db4objects Inc.  http://www.db4o.com */

package org.polepos.teams.jorm.data;

import de.ama.db.PersistentMarker;


public class N1 implements PersistentMarker{

    public String s0;
    
    public String s1;
    
    public String s2;
    
    public String s3;
    
    public String s4;
    
    public String s5;
    
    public String s6;
    
    public String s7;
    
    public String s8;
    
    public String s9;

    
    public N1(){
        
    }
    
    
    public static N1 generate(int index){
        N1 n1 = new N1();
        n1.setStrings(index);
        return n1;
    }
    
    private void setStrings(int index){
        String str = "N" + index + "aabbccddeeffgghhjjKK";
        str = str.substring(0,20);
        s0 = str;
        s1 = str;
        s2 = str;
        s3 = str;
        s4 = str;
        s5 = str;
        s6 = str;
        s7 = str;
        s8 = str;
        s9 = str;
    }

    
    public String getS0() {
        return s0;
    }

    
    public void setS0(String s0) {
        this.s0 = s0;
    }

    
    public String getS1() {
        return s1;
    }

    
    public void setS1(String s1) {
        this.s1 = s1;
    }

    
    public String getS2() {
        return s2;
    }

    
    public void setS2(String s2) {
        this.s2 = s2;
    }

    
    public String getS3() {
        return s3;
    }

    
    public void setS3(String s3) {
        this.s3 = s3;
    }

    
    public String getS4() {
        return s4;
    }

    
    public void setS4(String s4) {
        this.s4 = s4;
    }

    
    public String getS5() {
        return s5;
    }

    
    public void setS5(String s5) {
        this.s5 = s5;
    }

    
    public String getS6() {
        return s6;
    }

    
    public void setS6(String s6) {
        this.s6 = s6;
    }

    
    public String getS7() {
        return s7;
    }

    
    public void setS7(String s7) {
        this.s7 = s7;
    }

    
    public String getS8() {
        return s8;
    }

    
    public void setS8(String s8) {
        this.s8 = s8;
    }

    
    public String getS9() {
        return s9;
    }

    
    public void setS9(String s9) {
        this.s9 = s9;
    }

    
}
