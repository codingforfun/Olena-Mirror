#!/usr/bin/perl -w

my @words = ();

while (<>) {
    $_ .= <> while s/\\$//;
    if (s/^\s*\#\s*(if|ifdef|ifndef|elif)\s+//) {
	s(/\*.*?\*/)();
	s(/\*.*)();
	foreach my $word (split (/\W+/))
	{
	    next if $word eq 'defined' || $word !~ /^[a-zA-Z_]/;
	    push @words, $word;
	}
    }
}

print "cat \${1+\"\$\@\"} | egrep '^#.*(" . join ('|', @words), ")'\n";
