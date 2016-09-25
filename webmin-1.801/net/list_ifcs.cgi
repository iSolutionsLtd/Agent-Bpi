#!/usr/bin/perl
# list_ifcs.cgi
# List active and boot-time interfaces

require './net-lib.pl';
require './dhcpcd_func.pl';
&ReadParse();
$access{'ifcs'} || &error($text{'ifcs_ecannot'});
$allow_add = &can_create_iface() && !$noos_support_add_ifcs;
&ui_print_header(undef, $text{'ifcs_title'}, "");

# Start tabs for active/boot time interfaces
@tabs = ( [ "active", $text{'ifcs_now'}, "list_ifcs.cgi?mode=active" ] );
$defmode = "active";
if (!$access{'bootonly'}) {
	push(@tabs, [ "boot", $text{'ifcs_boot'}, "list_ifcs.cgi?mode=boot" ] );
	$defmode = "boot";
	}
print &ui_tabs_start(\@tabs, "mode", $in{'mode'} || $defmode, 1);

# Show interfaces that are currently active
@act = &active_interfaces(1);
if (!$access{'bootonly'}) {
	# Table heading and links
	print &ui_tabs_start_tab("mode", "active");
	print $text{'ifcs_activedesc'},"<p>\n";
	local @tds;
	@links = ( );
	if ($access{'ifcs'} >= 2) {
		print &ui_form_start("delete_aifcs.cgi", "post");
		push(@links, &select_all_link("d"),
			     &select_invert_link("d") );
		push(@tds, "width=5 valign=top");
		}
	push(@tds, "width=20% valign=top", "width=20% valign=top",
		   "width=20% valign=top", "width=20% valign=top");
	push(@tds, "width=20% valign=top") if (&supports_address6());
	push(@tds, "width=5% valign=top");
	if ($allow_add) {
		push(@links,
		     &ui_link("edit_aifc.cgi?new=1",$text{'ifcs_add'}));
		}
	print &ui_links_row(\@links);
	print &ui_columns_start([ $access{'ifcs'} >= 2 ? ( "" ) : ( ),
				  $text{'ifcs_name'},
				  $text{'ifcs_type'},
				  $text{'ifcs_ip'},
				  $text{'ifcs_mask'},
				  &supports_address6() ?
					( $text{'ifcs_ip6'} ) : ( ),
				  $text{'ifcs_status'} ], 100, 0, \@tds);

	# Show table of interfaces
	@act = sort iface_sort @act;
	foreach $a (@act) {
		next if ($access{'hide'} &&	# skip hidden
			 (!$a->{'edit'} || !&can_iface($a)));
		local $mod = &module_for_interface($a);
		local %minfo = $mod ? &get_module_info($mod->{'module'}) : ( );
		local @cols;
		if ($a->{'edit'} && &can_iface($a) && $a->{'address'}) {
			push(@cols,
			    "<a href=\"edit_aifc.cgi?idx=$a->{'index'}\">".
			    &html_escape($a->{'fullname'})."</a>");
			}
		elsif (!$a->{'edit'} && $mod) {
			push(@cols,
			   "<a href=\"mod_aifc.cgi?idx=$a->{'index'}\">".
			   &html_escape($a->{'fullname'})."</a>");
			}
		else {
			push(@cols, &html_escape($a->{'fullname'}));
			}
		if ($a->{'virtual'} ne "") {
			$cols[0] = "&nbsp;&nbsp;".$cols[0];
			}
		if (%minfo && $minfo{'dir'} eq 'virtual-server') {
			# Shorten name
			$minfo{'desc'} = $text{'index_vmin'};
			}
		push(@cols, &iface_type($a->{'name'}).
		      ($a->{'virtual'} eq "" ||
		       $mod ? "" : " ($text{'ifcs_virtual'})").
		      (%minfo ? " ($minfo{'desc'})" : "").
		      ($a->{'speed'} ? " ".$a->{'speed'} : ""));
		push(@cols, &html_escape($a->{'address'}) ||
			    $text{'ifcs_noaddress'});
		push(@cols, &html_escape($a->{'netmask'}) ||
			    $text{'ifcs_nonetmask'});
		if (&supports_address6()) {
			push(@cols, join("<br>\n", map { &html_escape($_) }
						    @{$a->{'address6'}}));
			}
		push(@cols, $a->{'up'} ? $text{'ifcs_up'} :
			"<font color=#ff0000>$text{'ifcs_down'}</font>");
		if ($a->{'edit'} && &can_iface($a)) {
			print &ui_checked_columns_row(\@cols, \@tds, "d",
						      $a->{'fullname'});
			}
		else {
			print &ui_columns_row([ "", @cols ], \@tds);
			}
		}
	print &ui_columns_end();
	print &ui_links_row(\@links);
	if ($access{'ifcs'} >= 2) {
		print &ui_form_end([ [ "delete", $text{'index_delete1'} ] ]);
		}
	print &ui_tabs_end_tab();
	}

# Show interfaces that get activated at boot
print &ui_tabs_start_tab("mode", "boot");
print $text{'ifcs_bootdesc'},"<p>\n";

@tds = ( "width=5 valign=top", "width=20% valign=top", "width=20% valign=top",
	 "width=20% valign=top" );

print &ui_columns_start([ "",
			  $text{'ifcs_name'},
			  $text{'ifcs_type'},
			  $text{'ifcs_ip'},
			  $text{'ifcs_mask'},"",""
			   ], 100, 0, \@tds);

@ifcs = getIfcs();
foreach $a (@ifcs) {
	local @cols;
	local @mytds = @tds;

		# A normal single interface

		push(@cols, "<a href=\"edit_dhcpcd.cgi?name=".$a->{'name'}."\">".&html_escape($a->{'name'})."</a>");
		push(@cols, &iface_type($a->{'name'}));
		push(@cols, $a->{'ip_address'} ? &html_escape($a->{'ip_address'}) : "DHCP") ;
		push(@cols, $a->{'netmask'} ? &html_escape($a->{'netmask'}) : "DHCP") ;
		
	#if ($can) {
		#print &ui_checked_columns_row(\@cols, \@mytds, "b",
					      #$a->{'fullname'});
		#}
	#else {
		print &ui_columns_row([ "", @cols ], \@tds);
		#}
	}
print &ui_columns_end();

print &ui_tabs_end_tab();

print &ui_tabs_end(1);

&ui_print_footer("", $text{'index_return'});

sub iface_sort
{
return $a->{'name'} cmp $b->{'name'} if ($a->{'name'} cmp $b->{'name'});
return $a->{'virtual'} eq '' ? -1 :
       $b->{'virtual'} eq '' ? 1 : $a->{'virtual'} <=> $b->{'virtual'};
}

