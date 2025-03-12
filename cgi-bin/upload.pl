#!/usr/bin/perl

use strict;
use warnings;
use CGI;


my $query = CGI->new;

my $upload_dir = "./upload";
my $filename = $query->param('fichier')
my $email_adress = $query->param('email')
my $upload_filehandle = $query->upload('fichier')

open (UPLOADFILE, ">$upload_dir/$filename");
binmode UPLOADFILE;

while (my $ligne = <$upload_filehandle>) 
{
    print UPLOADFILE $ligne;
}
close UPLOADFILE;
close $upload_filehandle;



my $body = "<HTML>
<HEAD>
<TITLE>Thanks!</TITLE>
</HEAD>

<BODY>

<P>Thanks for uploading your photo!</P>
<P>Your email address: $email_address</P>
<P>Your photo:</P>
<img src="$upload_dir/$filename" border="0">

</BODY>
</HTML>";
print $query->header(-type => 'text/html', -status => '200 OK', -http11 => 'HTTP/1.1', -length => length($body));
print $body;



