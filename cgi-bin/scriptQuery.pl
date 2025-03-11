#!/usr/bin/perl

my $query = $ENV{'QUERY_STRING'};

if ($query)
{
    my @pairs = split(/&/, $query);
    foreach my $pair (@pairs)
    {
        my ($key, $val) = split(/=/, $pair);
        if ($key eq "value1")
        {
            $value1 = $val;
            last;
        }
    }
}
print "La valeur de value1 est: $value1";
print "La valeur de query: $query";