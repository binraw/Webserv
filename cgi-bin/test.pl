#!/usr/bin/perl

# TEST JUSTE POUR VERIFIER ENV
print "Content-Type: text/plain\r\n\r\n";


print "============================================\n";
print "VARIABLES D'ENVIRONNEMENT DISPONIBLES\n";
print "============================================\n\n";

foreach my $key (sort keys %ENV) {
    my $value = $ENV{$key};
   
    if (length($value) > 1000) {
        $value = substr($value, 0, 997) . "...";
    }
    print "$key = $value\n";
}


my $count = scalar keys %ENV;
print "\n============================================\n";
print "Total: $count variables d'environnement\n";
print "============================================\n";

print "\nVARIABLES CGI IMPORTANTES:\n";
print "----------------------------------------\n";

my @important_vars = (
    "REQUEST_METHOD",
    "QUERY_STRING",
    "CONTENT_TYPE",
    "CONTENT_LENGTH",
    "PATH_INFO",
    "SCRIPT_NAME",
    "SCRIPT_FILENAME",
    "SERVER_NAME",
    "SERVER_PORT",
    "REMOTE_ADDR",
    "REMOTE_HOST"
);

foreach my $var (@important_vars) {
    my $status = exists $ENV{$var} ? "PRÉSENTE" : "MANQUANTE";
    my $value = $ENV{$var} || "(non définie)";
    print "$var: $status = $value\n";
}

print "\nANALYSE DE QUERY_STRING:\n";
print "----------------------------------------\n";
if (exists $ENV{'QUERY_STRING'} && $ENV{'QUERY_STRING'} ne '') {
    my $query_string = $ENV{'QUERY_STRING'};
    print "QUERY_STRING brut: $query_string\n\n";
    
    print "Paramètres individuels:\n";
    my @pairs = split(/&/, $query_string);
    foreach my $pair (@pairs) {
        my ($key, $val) = split(/=/, $pair, 2);
        $key = "(vide)" if !defined $key || $key eq '';
        $val = "(vide)" if !defined $val || $val eq '';
        print "  - $key = $val\n";
    }
} else {
    print "QUERY_STRING n'est pas définie ou est vide\n";
}

print "\n============================================\n";
print "FIN DU RAPPORT\n";
print "============================================\n";