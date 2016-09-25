# wifi func

$dhcpcd_config = '/etc/dhcpcd.conf';
use Data::Dumper;
sub getIfcs
{
my @ifcs;
local *CFGFILE;
&open_readfile(CFGFILE, $dhcpcd_config) ||
	error("Unable to open $wpa_conf");
$line = <CFGFILE>;
my $foundISec=0;
while (defined $line) 
{
	chomp($line);
	# skip comments
	if (($foundISec) || ($line =~/^\s*#@@@ INTERFACES @@@$/))
	{
		$foundISec=1;
		#print "found interfaces sec!<br>";
		if ($line =~ /^\s*#/ || $line =~ /^\s*$/) 
		{
			#print "skip Empty <br>";
			$line = <CFGFILE>;
			next;
		}elsif ($line =~ /^\s*interface /)
		{
			#print "found interfaces!<br>";

			my $ifc;
			my $ifc_name;
			if (($ifc_name) = ($line =~ /^\s*interface\s*(.*)$/))
			{
				#print "ifc name=".$ifc_name."<br>";
				$ifc->{'name'}=$ifc_name;
			}
			do
			{
				
				my ($param, $value,$mask);
				$line =~ s/^\s+//;
				if ( ($param, $value,$mask) = ($line =~ (/^\s*static (.*)=(.*)\/(.*)/) ) )
				{
					#print "par=".$param." val=".$value." mask=".prefix_to_mask($mask)."<br>";
					$ifc->{$param}= $value;
					$ifc->{'netmask'}= prefix_to_mask($mask);
				}elsif ( ($param, $value) = ($line =~ (/^\s*static (.*)=(.*)/) ) )
				{
					#print "par=".$param." val=".$value."<br>";
					$ifc->{$param}= $value;
				} 
				$line = <CFGFILE>;
			}while (defined $line && ! ($line =~ /^\s*interface/));
			#print "push ifc ".$ifc->{'name'}."<br>";
			push(@ifcs, $ifc);
			#$line = <CFGFILE>;
		}
	}else
	{
		#print "skip row ".$line."<br>";
		$line = <CFGFILE>;
		next;
	}		
		
	
}
close(CFGFILE);
#print "<br>" . Dumper(\ @ifcs) . "\n";
return @ifcs;
}

sub saveDhcpcd
{
	my $ifcs = @_[0];
	$lref = read_file_lines($dhcpcd_config);
	for (my $i=0; $i < @$lref; $i++) 
	{
		#print $i."  ".@$lref[$i]."<br>";
		if (@$lref[$i] =~ /^\s*#@@@ INTERFACES @@@$/)
		{
			$i=$i+2;
			#print "last index ".$i."<br>";
			splice @$lref, $i;
			break;
		}
	}
	#print "<br>" . Dumper(\@$ifcs) . "\n";
	foreach $h (@$ifcs) 
	{
		push(@$lref,'');
		push(@$lref,"interface ".$h->{'name'});
		if (length $h->{'ip_address'})
		{
			push(@$lref,"static ip_address=".$h->{'ip_address'});
		}
		if (length $h->{'routers'})
		{
			push(@$lref,"static routers=".$h->{'routers'});
		}
		if (length $h->{'dns'})
		{
			push(@$lref,"static domain_name_servers=".$h->{'domain_name_servers'});
		}
	}
	#print "<br>" . Dumper(\ @$lref) . "\n";
	flush_file_lines($dhcpcd_config);


}
