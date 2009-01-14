#!/usr/bin/ruby

require 'eyedb'

class EyeDBMap

@connection=nil

def initialize(b=nil,l=nil,p=nil,s=nil)

if !b.nil? && b.class==EyeDB then 
@connection=b
else
@connection=EyeDB.new(b,l,p,s)
end

puts "Init Run"

end


def query(q)

ret=[]
@connection.query(q).each{|f| ret.push(value(f))}

return ret

end

private
def value(v)

case(v.instance_variable_get('@rtype'))
when "string" then return v.instance_variable_get('@string')
when "int" then return v.instance_variable_get('@int')
when "float" then return v.instance_variable_get('@float')
when "nil" then return nil
when "OID" then return EyeDBAutoObject.new(v.instance_variable_get('@oid'),self)
else
	return v 
end

end


end


class EyeDBAutoObject
private 
@oid=nil
@connectionmap=nil
public
def initialize(oid,connection)
@oid=oid
@connectionmap=connection
end

def oid
return @oid
end

def [](name)
 @connectionmap.query("a:=#{@oid}")
a= @connectionmap.query("a.#{name}")
return a.first unless a.empty?
end


def method_missing(v)
self[v.to_s]
end

def to_s
"#<EyeDBAutoObject oid=#{@oid}>"
end

end
