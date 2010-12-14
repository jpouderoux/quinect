import os, glob;
print "<!DOCTYPE RCC>"
print "<RCC version='1.0'>"
print "\t<qresource prefix='/icons'>"
for fname in glob.glob( os.path.join(".", '*.png') ):
	(filepath, filename) = os.path.split(fname)
	(shortname, extension) = os.path.splitext(filename)   
	print "\t\t<file alias='" + shortname + "'>data/" + filename + "</file>"
print "\t</qresource>"
print "\t<qresource prefix='/images'>\n\t\t<file alias='splash'>data/splash.png</file>\n\t</qresource>"
print "</RCC>"
