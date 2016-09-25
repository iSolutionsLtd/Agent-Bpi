#!/usr/bin/perl
# save_wifi.cgi
# Create, update or delete a wifi networks

require './net-lib.pl';
require './wifi_func.pl';
use Data::Dumper;

$access{'wifi'} == 2 || &error($text{'hosts_ecannot'});

#&ui_print_header(undef, "Save Wifi", "");

&ReadParse();


#print "save=".$in{'save'};
if ($in{'delete'})
{
	@d = split(/\0/, $in{'d'});
	@d || &error($text{'wdelete_enone'});
	#print "Form Delete ".$in{'d'};
	if (@d)
	{
		my @nets = getwpaNetworks();
		foreach $d (@d) 
		{
			my $index = -1;
			for (my $i=0; $i < @nets; $i++) 
			{
	   			if (@nets[$i]->{'ssid'} eq $d)
				{
					$index=$i;
					break;
				}
			}
			if ($index>-1)
			{
				splice(@nets, $index, 1);
			}
		}
		#print Dumper(\@nets) . "\n";
		savewifiNetorks(\@nets);
	}
	&webmin_log("delete", "wifi", scalar(@d));
	&redirect("wifi_manage.cgi");
}elsif ($in{'save'})
{
	if (length $in{'ssid'})
	{
		my @nets = getwpaNetworks();
		my $net;
		my $index = -1;
		for (my $i=0; $i < @nets; $i++) 
		{
   			if (@nets[$i]->{'ssid'} eq $in{'ssid'})
			{
				$index=$i;
				break;
			}
		}
		if ($index>-1)
		{
			splice(@nets, $index, 1);
		}
		$net->{'ssid'}= $in{'ssid'};
		$net->{'psk'}= $in{'key'};
		if ($in{'hidden'}==1)
		{
			$net->{'scan_ssid'}= 1;
		}
		
		push(@nets,$net);
		#print Dumper(\@nets) . "\n";
		savewifiNetorks(\@nets);
		&webmin_log("Save", "wifi", $in{'ssid'});
		&redirect("wifi_manage.cgi");
	}

}


