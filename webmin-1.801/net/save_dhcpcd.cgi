#!/usr/bin/perl
# save_wifi.cgi
# Create, update or delete a wifi networks

require './net-lib.pl';
require './dhcpcd_func.pl';
use Data::Dumper;

$access{'wifi'} == 2 || &error($text{'hosts_ecannot'});
#&ui_print_header(undef, "Save DHCPCD", "");

&ReadParse();

#print "<br>" . Dumper(\$in) . "\n";
#print "save=".$in{'save'};
if ($in{'save'})
{
	@ifcs = getIfcs();
	my $ifc;
	my $index = -1;
	for (my $i=0; $i < @ifcs; $i++) 
	{
		if (@ifcs[$i]->{'name'} eq $in{'name'})
		{
			$index=$i;
			break;
		}
	}
	if ($index>-1)
	{
		splice(@ifcs, $index, 1);
	}
	
	$ifc->{'name'}=$in{'name'};
	if (($in{'dhcp'}==0) && (length $in{'address'}))
	{
		my $ip=$in{'address'};
		if (length $in{'netmask'})
		{
			$ip=$ip."/".mask_to_prefix($in{'netmask'});
		}else
		{
			$ip=$ip."/24";
		}
		$ifc->{'ip_address'}=$ip;
		if (length $in{'gateway'})
		{
			$ifc->{'routers'}=$in{'gateway'};
		}
		if (length $in{'dns'})
		{
			$ifc->{'domain_name_servers'}=$in{'dns'};
		}
	}
	push(@ifcs,$ifc);
	saveDhcpcd(\@ifcs);
	&webmin_log("Save", "Dhcpcd", @ifcs);
	&redirect("list_ifcs.cgi");
}


