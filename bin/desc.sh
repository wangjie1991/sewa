
awk -F '\t' '{
	an = split($6, a, ".");
	f = a[1];
	print "\"speaker\":\""$1"\"" >> f".des";
	print "\"office\":\""$2"\"" >> f".des";
	print "\"platform\":\""$3"\"" >> f".des";
	print "\"format\":\""$4"\"" >> f".des";
	print "\"business\":\""$5"\"" >> f".des";
}' $1


