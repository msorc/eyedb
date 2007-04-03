#!/bin/bash

function manpage() {
    $1 --help 2>&1 | grep usage | awk --assign cmd=$1 '
function token( c, n) {
      r["tok"] = c;
      r["n"] = n;

      return 1;
}

function next_token( line, n) {
  word = "";
  for ( ;;n++) {
    c = substr( line, n, 1);

    if (n > length(line)) {
      if (word)
        return token(word,n);
      else
        return "";
    }

    if (c == " ") {
      if (word)
        return token(word,n);
      else
        continue;
    }
    if (c == "[" || c == "]") {
      if (word)
        return token(word,n);
      else
        return token(c,n+1);
      }
    else
      word = word c;
  }
}

BEGIN {
print "<?xml version=\"1.0\"?>";
print "<!DOCTYPE refentry PUBLIC \"-//OASIS//DTD DocBook XML V4.1.2//EN\"";
print "  \"http://www.oasis-open.org/docbook/xml/4.1.2/docbookx.dtd\" [";
printf("  <!ENTITY %s \"<command>%s</command>\">\n", cmd, cmd);
print "]>"
print "";
printf( "<refentry id=\"%s\">\n", cmd);
print "  <refentryinfo>";
print "    <date>March 2007</date>";
print "  </refentryinfo>";
print "";
print "  <refmeta>";
printf( "    <refentrytitle>%s</refentrytitle>\n", cmd);
print "    <manvolnum>1</manvolnum>";
print "    <refmiscinfo>eyedb database</refmiscinfo>";
print "  </refmeta>";
print "";
print "  <refnamediv>";
printf( "    <refname>&%s;</refname>\n", cmd);
print "    <refpurpose>command purpose</refpurpose>";
print "  </refnamediv>";
print "";
}

/usage:/ {
print "  <refsynopsisdiv>";
print "    <cmdsynopsis>";
printf( "      <![CDATA[%s]]>\n", $0);
printf( "      %s\n", cmd);

line = "";
for (i = 3; i <= NF; i++)
  line = line $i;

n = 1;
optional = 0;
opt_cnt = 0;
print "line" line > /dev/stderr;
for (;;) {
  t = next_token( line, n);
  print "token " r["tok"] > /dev/stderr;
  if (!t)
    break;
  n = r["n"];

  if (r["tok"] == "[")
    optional++;
  else if (r["tok"] == "]")
    optional--;
  else {
    opt[opt_cnt] = r["tok"];
    opt_type[opt_cnt] = optional;
    opt_cnt++;
  }    
}

for ( n = 0; n < opt_cnt; n++) {
  arg_type = (opt_type[n]) ? "optional" : "plain";
  gsub( "<", "@lt;replaceable@gt;", opt[n]);
  gsub( ">", "@lt;/replaceable@gt;", opt[n]);
  gsub( "@lt;", "<", opt[n]);
  gsub( "@gt;", ">", opt[n]);
  print "<arg choice=\"" arg_type "\">" opt[n] "</arg>";
}

print "    </cmdsynopsis>";
print "  </refsynopsisdiv>";
print "";
print "  <refsect1>";
print "    <title>Description</title>";
print "";
printf( "    <para>&%s; ...</para>\n", cmd);
print "  </refsect1>";
print "";
print "  <refsect1>";
print "    <title>Options</title>";
print "";
print "    <variablelist>";
print ""

for ( n = 0; n < opt_cnt; n++) {
  arg_type = (opt_type[n]) ? "optional" : "plain";
  print "      <varlistentry>";
  print "	<term>" "<arg choice=\"" arg_type "\">" opt[n] "</arg>" "</term>";
  print "	<listitem>";
  print "	  <para>option_description</para>";
  print "	</listitem>";
  print "      </varlistentry>";
  print ""
}

print "    </variablelist>";
print "";
print "  </refsect1>";
print "";
}
END {
print "<refsect1>";
print "  <title>Environment</title>";
print "  See eyedb-options(7)";
print "</refsect1>";
print "";
print "<refsect1>";
print "  <title>Exit status</title>";
print "</refsect1>";
print "";
print "<refsect1>";
print "  <title>Examples</title>";
print "</refsect1>";
print "";
print "<refsect1>";
print "  <title>See also</title>";
print "</refsect1>";
print "";
print "<refsect1>";
print "  <title>Bugs</title>";
print "</refsect1>";
print "";
print "<refsect1>";
print "  <title>Authors</title>";
print "</refsect1>";
print "";
print "</refentry>";
}
'
}

for i in $*
do
  type $i > /dev/null 2>&1
  if [ $? = 0 ] ; then 
     if [ -r $i.xml ] ; then mv -f $i.xml $i.xml.bak; fi
     echo Generate $i.xml...
     manpage $i > $i.xml
  fi
done
