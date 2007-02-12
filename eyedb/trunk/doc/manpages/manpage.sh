#!/bin/bash

function manpage() {
    $1 --help 2>&1 | grep usage | awk --assign cmd=$1 '
BEGIN {
print "<?xml version=\"1.0\"?>";
print "<!DOCTYPE refentry PUBLIC \"-//OASIS//DTD DocBook XML V4.1.2//EN\"";
print "  \"http://www.oasis-open.org/docbook/xml/4.1.2/docbookx.dtd\" [";
printf("  <!ENTITY %s \"<command>%s</command>\">\n", cmd, cmd);
print "]>"
print "";
printf( "<refentry id=\"%s\">\n", cmd);
print "  <refentryinfo>";
print "    <date>12 February 2007</date>";
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
{
print "  <refsynopsisdiv>";
print "    <cmdsynopsis>";
printf( "      <code><![CDATA[%s]]></code>\n", $0);
printf( "      %s\n", cmd);
# TODO
for (i = 3; i <= NF; i++) {
#      <arg choice="req"><replaceable>dbname</replaceable></arg>
#      <arg choice="req">r|rw|rx|rwx|admin|no</arg>
  a = $i
  gsub( "<", "@lt;replaceable@gt;", a);
  gsub( ">", "@lt;/replaceable@gt;", a);
  gsub( "@lt;", "<", a);
  gsub( "@gt;", ">", a);
  printf( "      <arg>%s</arg>\n", a);
}
# /TODO
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
print "      <varlistentry>";
# TODO
print "	<term>--option</term>";
print "	<listitem>";
print "	  <para>option_description</para>";
print "	</listitem>";
# /TODO
print "      </varlistentry>";
print "    </variablelist>";
print "";
print "  </refsect1>";
print "";
}
END {
print "  <refsect1>";
print "    <title>Environment</title>";
print "    See eyedb-options";
print "  </refsect1>";
print "";
print "  <refsect1>";
print "    <title>Exit status</title>";
print "  </refsect1>";
print "";
print "  <refsect1>";
print "    <title>Examples</title>";
print "  </refsect1>";
print "";
print "  <refsect1>";
print "    <title>See also</title>";
print "  </refsect1>";
print "";
print "  <refsect1>";
print "    <title>Bugs</title>";
print "  </refsect1>";
print "";
print "  <refsect1>";
print "    <title>Authors</title>";
print "  </refsect1>";
print "";
print "</refentry>";
}
'
}

manpage $1 > $1.new.xml

