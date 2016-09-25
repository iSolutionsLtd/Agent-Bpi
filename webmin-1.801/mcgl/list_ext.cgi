#!/usr/bin/perl

require './mcgl-lib.pl';
use Data::Dumper;

&ui_print_header(undef, "Manage Mcgl extensions", "");

my $prop=readMcglProp();
my $exts = $prop->{'extensions'};

@links = ( 
		   "<a href=\"edit_ext.cgi\">Add new extension</a>" );
	print &ui_links_row(\@links);

print &ui_form_start("save_ext.cgi");
print &ui_columns_start(["","Extension","Type"], 20, 0, \@tds);

while ( my ($key,$val) = each %$exts ) 
{
	local @cols;
	push(@cols,"<a href=\"edit_ext.cgi?ext=".$key."\">".$key."</a>");
	push(@cols,"<a href=\"edit_ext.cgi?ext=".$key."\">".getTypeName($val->{'type'})."</a>");
	if ($key eq "default")
	{
		unshift(@cols,"");
		print &ui_columns_row(\@cols,undef,"d",$key);
	}else
	{
		print &ui_checked_columns_row(\@cols,undef,"d",$key);
	}

}
print &ui_columns_end();
print &ui_form_end([ [ "delete", "Delete selected extensions" ] ]);
print &ui_links_row(\@links);
ui_print_footer("/", "return to MCGL Configurations");
