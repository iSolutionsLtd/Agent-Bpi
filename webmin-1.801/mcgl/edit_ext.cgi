#!/usr/bin/perl

require './mcgl-lib.pl';
use Data::Dumper;
&ReadParse();

ui_print_header(undef, "Edit Mcgl Extenstion ".$in{'ext'}, "", undef, 1, 1);

@links = ( 
		   "<a href=\"index.cgi\">MCGL Configuration</a>" );
	print &ui_links_row(\@links);

#my $prop=readMcglProp();
my $exts = readMcglProp()->{'extensions'};
my $ext=$exts->{$in{'ext'}};
#print "<br>" . Dumper(\$prop) . "<br>";
print &text('index_root', $dir),"<p>\n";
print &ui_form_start("save_ext.cgi","post");
print &ui_table_start("Mcgl Parameters","width=600", 4);

if ($in{'ext'} eq "default")
{
	print &ui_table_row("Extension:", &ui_textbox("ext",$in{'ext'}, 30,0,10,"readonly=readonly"),3);
}else
{
	print &ui_table_row("Extension:", &ui_textbox("ext",$in{'ext'}, 30),3);
}
print &ui_table_row("Type:", &ui_select("type",$ext?$ext->{'type'}:0,[[0,"Bulk"],[1,"Over"],[2,"Append"]]),2);
print &ui_table_row("EOF Time:", &ui_textbox("EofTime", $ext->{'EofTime'}, 5),1);
print &ui_table_row("Seconds");
print &ui_table_row("Max hold time:", &ui_textbox("MaxHoldTime", $ext->{'MaxHoldTime'}, 5),1);
print &ui_table_row("Seconds");
print &ui_table_row("Max hold size:", &ui_textbox("MaxHoldSize", $ext->{'MaxHoldSize'}, 5),1);
print &ui_table_row("KB");
print &ui_table_end();

# End of the form
@buts = ( [ 'save', $text{'save'} ] );

print &ui_form_end(\@buts);

print &ui_links_row(\@links);
ui_print_footer("/", "return to MCGL Configurations");
