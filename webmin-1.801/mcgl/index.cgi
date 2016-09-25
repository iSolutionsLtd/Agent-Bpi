#!/usr/bin/perl

require './mcgl-lib.pl';
use Data::Dumper;

ui_print_header(undef, "MCGL Configurations", "", undef, 1, 1);

@links = ( 
		   "<a href=\"list_ext.cgi\">Manage MCGL Extensions</a>" );
	print &ui_links_row(\@links);

my $prop=readMcglProp();

#print "<br>" . Dumper(\$prop) . "<br>";
print &text('index_root', $dir),"<p>\n";

print &ui_form_start("save_mcgl.cgi","post");

print &ui_table_start("Mcgl Parameters","width=600", 4);
print &ui_table_row("Host name:", &ui_textbox("host_name",$prop->{'host_name'}, 30),2);
print &ui_table_row("Storage device:", &ui_textbox("storage_device",$prop->{'storage_device'}, 30),2);
print &ui_table_row("Scan period:", &ui_textbox("scan_time", $prop->{'scan_time'}, 5),1);
print &ui_table_row("Seconds");
print &ui_table_row("Upload period:", &ui_textbox("upload_time", $prop->{'upload_time'}, 5),1);
print &ui_table_row("Seconds");
print &ui_table_row("Max upload resumes:", &ui_textbox("max_resumes", $prop->{'max_resumes'}, 5),2);
print &ui_table_row("Temp folder:", &ui_textbox("tmp_folder", $prop->{'tmp_folder'}, 30),2);
print &ui_table_row("Watch folder:", &ui_textbox("watch_folder", $prop->{'watch_folder'}, 30),2);


print &ui_table_end();

print &ui_table_start("Ftp Parameters","width=600", 4);

print &ui_table_row("Host:", &ui_textbox("host", $prop->{'ftp'}->{'host'}, 60),2);
print &ui_table_row("User:", &ui_textbox("user", $prop->{'ftp'}->{'user'}, 30),2);
print &ui_table_row("Password:", &ui_textbox("pass", $prop->{'ftp'}->{'pass'}, 30),2);


print &ui_table_end();

print &ui_table_start("Rest Parameters","width=600", 4);

print &ui_table_row("Pre-Measurements:", &ui_textbox("premeasurements", $prop->{'rest'}->{'premeasurements'}, 60),2);
print &ui_table_row("Pre-file:", &ui_textbox("prefile", $prop->{'rest'}->{'prefile'}, 60),2);
print &ui_table_row("Files:", &ui_textbox("files", $prop->{'rest'}->{'files'}, 60),2);


print &ui_table_end();

# End of the form
@buts = ( [ 'save', $text{'save'} ] );

print &ui_form_end(\@buts);

print &ui_links_row(\@links);
ui_print_footer("/", "return to MCGL Configurations");
