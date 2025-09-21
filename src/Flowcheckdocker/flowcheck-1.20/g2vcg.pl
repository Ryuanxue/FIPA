#!/usr/bin/perl

my %nodes;
my @edges;

while (<>) {
    chomp;
    my($from, $to, $cap) = split;
    $nodes{$from} = $nodes{$to} = 1;
    push @edges, [$from, $to, $cap];
}

print "graph: {\n";
print "display_edge_labels: yes\n";
for my $n (sort keys %nodes) {
    print qq/node: { title: "l$n" label: "$n" }\n/;
}
for my $e (@edges) {
    print qq/edge: { sourcename: "l$e->[0]" targetname: "l$e->[1]"/;
    print qq/ label: "$e->[2]" }\n/;
}
print "}\n";
