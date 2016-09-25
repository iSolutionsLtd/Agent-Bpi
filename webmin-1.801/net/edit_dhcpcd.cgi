#!/usr/bin/perl
# edit_aifc.cgi
# Edit or create an active interface

require './net-lib.pl';
require './dhcpcd_func.pl';
@ifcs = getIfcs();

&ReadParse();
my $ifc;
foreach $i (@ifcs)
{
	if ($i->{'name'} eq $in{'name'})
	{
		$ifc=$i;
		break;
	}
}
	
if ($ifc)
{
	ui_print_header(undef, $text{'dhcpcd_edit'}.$ifc->{'name'}, "");
}else
{
	ui_print_header(undef, $text{'dhcpcd_edit'}, "");
}
# Form start
print &ui_form_start("save_dhcpcd.cgi");
print &ui_hidden("name",$ifc->{'name'});
print &ui_table_start($text{'dhcpcd_desc'},undef, 2);

# Interface name, perhaps editable
$namefield = "<tt>$ifc->{'name'}</tt>";

print &ui_table_row($text{'ifcs_name'}, $namefield,"","");

print &ui_table_row("Address Type:",ui_radio("dhcp",$ifc->{'ip_address'}?0:1,[[1,"DHCP"],[0,"Static"]]));
# IP address
print &ui_table_row($text{'ifcs_ip'},
	&ui_textbox("address", $ifc ? $ifc->{'ip_address'} : "", 20));

# Netmask field

# Allow editing
$netmaskfield = &ui_textbox("netmask", $ifc->{'netmask'}, 20);
	
print &ui_table_row($text{'ifcs_mask'}, $netmaskfield);

# Broadcast address field

# Allow editing
$broadfield = &ui_textbox("gateway", $ifc->{'routers'}, 20);
	
print &ui_table_row("Gateway", $broadfield);

print &ui_table_row("DNS", &ui_textbox("dns", $ifc->{'domain_name_servers'}, 20));

     
# End of the form
print &ui_table_end();

@buts = ( [ 'save', $text{'save'} ] );

print &ui_form_end(\@buts);

&ui_print_footer("list_ifcs.cgi?mode=active", $text{'ifcs_return'});

