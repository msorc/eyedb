#!/usr/bin/ruby
require 'eyedb'
require 'eyedbmap'

eye=EyeDB.new("Library","zeta")
puts eye.query("select Document.title;").inspect
puts eye.query("select MP3.bitrate;").inspect
#eye.query(nil);
#eye.query(12);
eye.close

eye2=EyeDBMap.new(eye)
puts eye2.query("select Document.title;").inspect
a=eye2.query("select Document;")
puts a.inspect
puts eye2.query("select "+a[0].oid).inspect
puts a.first
puts a.first['title'].inspect
puts a.first.title


