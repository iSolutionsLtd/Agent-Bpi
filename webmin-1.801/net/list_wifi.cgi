#!/usr/bin/perl
# list_hosts.cgi
# List hosts from /etc/hosts

require './net-lib.pl';
$access{'wifi'} || &error($text{'wifi_ecannot'});
&ui_print_header(undef, $text{'wifi_title'}, "");

if ($access{'wifi'} == 2) {
	print &ui_form_start("wifi_manage.cgi", "post");
	@links = ( 
		   "<a href=\"wifi_manage.cgi?new=1\">$text{'wifi_manage'}</a>" );
	print &ui_links_row(\@links);
	@tds = ( "width=5" );
	}
my $out;
my $ex = &execute_command("iwconfig wlan0", undef, \$out, undef, 0, 1);
$out = "Interface:".$out;
#$out =~ s/\r\n//g;
my @lines = split (/\s\s+/,$out);
# $out =~ s/\s\s+/\<br\>/g;
print &ui_table_start($text{'wifi_sheader'}, 'width=33%', 1);

foreach $line (@lines){
print &ui_table_span($line);

}
print &ui_table_end();
# print join("<br>",@lines);
if ($access{'wifi'} == 2) {
	print &ui_links_row(\@links);
	
	}

&ui_print_footer("", $text{'index_return'});

