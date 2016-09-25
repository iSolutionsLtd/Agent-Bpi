#!/usr/bin/perl
# list_hosts.cgi
# List hosts from /etc/hosts

require './net-lib.pl';
require './wifi_func.pl';
use Data::Dumper;

$access{'wifimanage'} || &error($text{'hosts_ecannot'});
&ui_print_header(undef, $text{'wifi_manage'}, "");

if ($access{'wifi'} == 2) {
	print &ui_form_start("save_wifi.cgi", "post");
	@links = ("<a href=\"edit_wifi.cgi?new=1\">$text{'wifi_add'}</a>", "<a href=\"scan_wifi.cgi\">$text{'wifi_scan'}</a>",);
	print &ui_links_row(\@links);
	@tds = ( "width=5" );
	}
print &ui_columns_start([$access{'wifi'} == 2?(""):(), "SSID" ], undef, 0, \@tds);

my @nets = getwpaNetworks();
#print Dumper(\@nets) . "\n";
foreach $h (@nets) {
	local @cols;
	my $ssid=$h->{'ssid'};
	if ($access{'wifi'} == 2) {
		push(@cols, "<a href=\"edit_wifi.cgi?ssid=$h->{'ssid'}\">".
			    &html_escape($h->{'ssid'})."</a>");
		}
	else {
		push(@cols, &html_escape($h->{'ssid'}));
		}
	
	if ($access{'wifi'} == 2) {
		print &ui_checked_columns_row(\@cols, \@tds, "d",$ssid);
		}
	else {
		print &ui_columns_row(\@cols);
		}
	}
print &ui_columns_end();
if ($access{'wifi'} == 2) {
	print &ui_links_row(\@links);
	print &ui_form_end([ [ "delete", $text{'wifi_delete'} ] ]);
	}

&ui_print_footer("", $text{'index_return'});

