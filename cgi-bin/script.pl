#!/usr/bin/perl
use POSIX 'strftime';

print "<HTML><HEAD><TITLE>Time</TITLE></HEAD><BODY>\n";

my $query_string = $ENV{'QUERY_STRING'};



print "<h1>Le Calcul de votre Devis revient a : </h1>\n";
print "<h2>$query_string</h2>\n";

print "</BODY></HTML>\n";
