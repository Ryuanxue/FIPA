#!/usr/bin/perl

my %use;
my %capacity;
my %edge;

my $orig_graph;
if (@ARGV == 2) {
    $orig_graph = pop @ARGV;
}

while (<>) {
    die unless m[^(\d+) (-?\d+) (\d+)/(\d+)$];
    my($from, $to, $use, $capacity) = ($1, $2, $3, $4);
    my $edge = "$from->$to";
    $use{$edge} += $use;
    $capacity{$edge} += $capacity;
    $edge = "$to->$from";
    $edge{$from}{$to} = 1;
    $edge{$to}{$from} = 1;
}

my %reach = (0 => 1);
my @fringe = (0);

my @candidates;

while (@fringe) {
    my $u = pop @fringe;
    for my $v (keys %{$edge{$u}}) {
	next if $reach{$v};
	my $edge = "$u->$v";
	my $anti_edge = "$v->$u";
	if ($capacity{$edge} and $use{$edge} < $capacity{$edge}) {
	    $reach{$v} = 1;
	    push @fringe, $v;
	} elsif ($capacity{$anti_edge} and $use{$anti_edge}) {
	    $reach{$v} = 1;
	    push @fringe, $v;
	} else {
	    push @candidates, [$u, $v];
	}
    }
}

my $total = 0;

my @cut_edges = ();

for my $e (@candidates) {
    my($u, $v) = @$e;
    next if $reach{$v};
    my $edge = "$u->$v";
    push @cut_edges, [$u, $v, $use{$edge}];
    $total += $use{$edge};
    #delete $edge{$from}{$to};
}

if (not defined $orig_graph) {
    for my $e (@cut_edges) {
	print "$e->[0] -> $e->[1] $e->[2]\n";
    }
} else {
    # Print corresponding lines from the original graph file, which
    # have location information
    my %want;
    for my $e (@cut_edges) {
	$want{"$e->[0] $e->[1]"} = $e->[2];
    }
    open(G, "<$orig_graph") or die "Can't open $orig_graph: $!";
    while (<G>) {
	if (/^(\d+ \d+) /) {
	    print $_ if $want{$1};
	}
    }
}

print "Total: $total bits\n";
