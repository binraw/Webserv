#!/usr/bin/perl

use strict;
use warnings;
use CGI;

# Créer un nouvel objet CGI
my $cgi = CGI->new;

my $objectif = $cgi->param('objectif') || "";
# le body qui est necessaire pour le fonctionnnement du script

print "<p>Nous avons bien reçu votre demande concernant: <strong>$objectif</strong>.</p>\n";