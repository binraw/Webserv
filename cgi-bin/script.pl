#!/usr/bin/perl

use CGI;



my $cgi = CGI->new();

my $obj = $cgi->param('objectif');

print $obj;


# le body qui est necessaire pour le fonctionnnement du script
# "objectif=Creation+de+site+web+pour+entreprise&design=oui&rdv=non&delai=2+mois&maintenance=oui&SEO=non"