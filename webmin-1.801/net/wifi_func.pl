# wifi func

$wpa_conf="/etc/wpa_supplicant.conf";
use Data::Dumper;
sub getwpaNetworks
{
my @nets;
local *CFGFILE;
&open_readfile(CFGFILE, $wpa_conf) ||
	error("Unable to open $wpa_conf");
$line = <CFGFILE>;
while (defined $line) 
{
	chomp($line);
	# skip comments
	if ($line =~ /^\s*#/ || $line =~ /^\s*$/) 
	{
		$line = <CFGFILE>;
		next;
	}elsif ($line =~ /^\s*network=/)
	{
		
		my $net;
		$line = <CFGFILE>;
		while (defined $line && ! ($line =~ /^\s*\}/)) 
		{
			if ($line =~ /^\s*#/ || $line =~ /^\s*$/)
			{
				$line = <CFGFILE>;
				next;
			}
			
			my ($param, $value);
			$line =~ s/^\s+//;
			if ( ($param, $value) = ($line =~ (/(.*)=(.*)/) ) )
			{
				$value=~ s/"//g;
				$param=~ s/"//g;
				$net->{$param}= $value;
			}
			$line = <CFGFILE>;
		}	
		push(@nets, $net);
	}else 
	{
		#error("Error reading file $pathname: unexpected line '$line'");
		$line = <CFGFILE>;
	}
	
	
}
close(CFGFILE);
return @nets;
}

sub getwifiscan
{
	
	my @nets;

	local *SCAN;
	my $ex = &open_execute_command(SCAN,"iwlist wlan0 scan", 1, 1);
	
	$line = <SCAN>;
	while (defined $line) 
	{
		#print "Line ".$line;
		chomp($line);
		# skip comments
		if ($line =~ /.*Address\:(.*)/)
		{
			my $net;
			do
			{
				
				#/(.*):(.*)(  |\b)/
				my ($param, $value);
				if ( ($param, $value) = ($line =~ (/(Address):( \S+)/) ) )
				{
					$value=~ s/^\s+//;
					$net->{$param}= $value;
				}
				elsif ($line =~ /.*Quality:/)
				{
					if ( ($param, $value) = ($line =~ (/(Quality):(\S+)/) ) )
					{
						$value=~ s/^\s+//;
						$net->{$param}= $value;
					}
					if ( ($param, $value) = ($line =~ (/(Signal level):(\S+)/) ) )
					{
						$value=~ s/^\s+//;
						$net->{$param}= $value;
					}
					if ( ($param, $value) = ($line =~ (/(Noise level):(\S+)/) ) )
					{
						$value=~ s/^\s+//;
						$net->{$param}= $value;
					}		
				}elsif ( ($param, $value) = ($line =~ (/(.*):(.*)/) ) )
				{
					$value=~ s/^\s+//;
					$value=~ s/"//g;
					$param=~ s/^\s+//;
					$param=~ s/"//g;
					$net->{$param}= $value;
				}
				$line = <SCAN>;
			}while (defined $line && ! ($line =~ /.*Address\:(.*)/));	
			push(@nets, $net);
		}else 
		{
			#error("Error reading file $pathname: unexpected line '$line'");
			$line = <SCAN>;
		}
	 
	} 
	return @nets;
}

sub savewifiNetorks
{
	my $nets = @_[0];
	#print "<br>" . Dumper(\@nets) . "\n";
	$wpa_conf2="/etc/wpa_supplicantTT.conf";
	open(my $fh, '>', $wpa_conf);
	print $fh "ctrl_interface=/var/run/wpa_supplicant\nap_scan=1\n\n";
	#print "<br>";
	foreach $h (@$nets) 
	{
		#print "save ".$h->{'ssid'}."<br>";
		print $fh "network={\n";
		print $fh "\tssid=\"".$h->{'ssid'}."\"\n";
		if ($h->{'psk'} eq '')
		{
			print $fh "\tkey_mgmt=NONE\n";
		}else
		{
			print $fh "\tpsk=\"".$h->{'psk'}."\"\n";
		}	
		if ($h->{'scan_ssid'})
		{
			print $fh "\tscan_ssid=".$h->{'scan_ssid'}."\n";
		}
		print $fh "}\n";
	}
	close $fh;

}
