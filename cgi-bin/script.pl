#!/usr/bin/perl 

use CGI;
use JSON;

my $query = CGI->new;

my $body = do { local $/; <STDIN> };
my $json = JSON->new;
my $data = $json->decode($body);

my $name = $data->{name};
my $age = $data->{age};

print "Body : $body\n";
print "Data : $data\n";
print "Name : $name\n";
print "Age : $age\n";

print "Content-Type: text/html\n\n";
print "<HTML><HEAD><TITLE>Test</TITLE></HEAD><BODY>\n";
print "<h1>Informations</h1>\n";
print "<p>Nom : $name</p>\n";
print "<p>Age : $age</p>\n";
print "</BODY></HTML>\n";