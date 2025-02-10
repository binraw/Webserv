#!/usr/bin/perl
use POSIX 'strftime';
print "HTTP/1.1 200 OK\r\n";
print "Content-Type: text/html\n\n";
print "<HTML><HEAD><TITLE>Time</TITLE></HEAD><BODY>\n";
print "<h1>Hello, World.</h1>\n";
my $date_hours = strftime "%Y-%m-%d %H:%M:%S", localtime;
print "$date_hours\n";
print "</BODY></HTML>\n";

#sert a rien mais permet de capter comment ca fonctionne