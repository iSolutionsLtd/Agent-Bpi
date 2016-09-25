#!/usr/bin/perl
# list_hosts.cgi
# List hosts from /etc/hosts

require './net-lib.pl';
require './wifi_func.pl';
use Data::Dumper;

$access{'wifimanage'} || &error($text{'hosts_ecannot'});
&ui_print_header(undef, $text{'wifi_manage'}, "");

if ($access{'wifi'} == 2) {
	print &ui_form_start("delete_hosts.cgi", "post");
	@links = ("<a href=\"edit_wifi.cgi?new=1\">$text{'wifi_add'}</a>", "<a href=\"scan_wifi.cgi\">$text{'wifi_scan'}</a>",);
	print &ui_links_row(\@links);
	@tds = ( "width=5" );
	}
print &ui_columns_start([ "SSID","Quality","Mode","Level"], undef, 0, \@tds);

my @nets = getwifiscan();
#print Dumper(\@nets) . "\n";
foreach $h (@nets) {
	local @cols;
	my $ssid=$h->{'ESSID'};
	
		my $lnk="<a href=\"edit_wifi.cgi?ssid=$h->{'ESSID'}\">";
		if (length $h->{'ESSID'})
		{	
			$lnk=$lnk.&html_escape($h->{'ESSID'});
		}else
		{
			$lnk=$lnk."{Hidden}";
		}
		$lnk=$lnk."</a>";
		push(@cols, $lnk);
	
	
	
	push(@cols, &html_escape($h->{'Quality'}));
	push(@cols, &html_escape($h->{'IE'}));
	push(@cols, &html_escape($h->{'Signal level'}." dBm"));
	print &ui_columns_row(\@cols);
		
	}
print &ui_columns_end();
if ($access{'wifi'} == 2) {
	print &ui_links_row(\@links);
	print &ui_form_end();
	}

&ui_print_footer("", $text{'index_return'});

