#!/usr/bin/perl
# edit_aifc.cgi
# Edit or create an active interface

require './net-lib.pl';
require './wifi_func.pl';



my @nets;
&ReadParse();

	# Editing existing interface
	@nets = getwpaNetworks();
	my $net;
	my $ssid;
	foreach $n (@nets)
	{
		if ($n->{'ssid'} eq $in{'ssid'})
		{
			$net=$n;
			break;
		}
	}
	
	if ($net)
	{
		#print "param=".$net->[0]->[0]." val=".$net->[0]->[1];
		$ssid=$net->{'ssid'};
		ui_print_header(undef, $text{'wifi_editnet'}, "");
	}else
	{
		ui_print_header(undef, $text{'wifi_newnet'}, "");
		$ssid=$in{'ssid'};
	}
		
     

# Form start
print &ui_form_start("save_wifi.cgi");
print &ui_table_start($ssid ? $ssid:"");
print &ui_table_row("SSID",
	&ui_textbox("ssid", $ssid , 32));
print &ui_table_row("Key",
	&ui_textbox("key", $net ? $net->{'psk'} : "", 32));
print &ui_table_row("",
	&ui_checkbox("hidden", "1", "Hidden network",$net ? $net->{'scan_ssid'} : 0));
print &ui_table_end();
print &ui_submit("Save", "save");
@buts = ( [ 'save', $text{'save'} ] );
print &ui_form_end();
&ui_print_footer("", $text{'index_return'});
