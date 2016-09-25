#!/usr/bin/perl

require './mcgl-lib.pl';

use Data::Dumper;

&ReadParse();
#&ui_print_header(undef, "Save ext", "");
my $prop=readMcglProp();
#print Dumper(\$in) . "<br>";
if ($in{'save'})
{
	$prop->{'host_name'}=$in{'host_name'};
	$prop->{'storage_device'}=$in{'storage_device'};
	$prop->{'scan_time'}=$in{'scan_time'};
	$prop->{'upload_time'}=$in{'upload_time'};
	$prop->{'max_resumes'}=$in{'max_resumes'};
	$prop->{'tmp_folder'}=$in{'tmp_folder'};
	$prop->{'watch_folder'}=$in{'watch_folder'};

	$prop->{'ftp'}->{'host'}=$in{'host'};
	$prop->{'ftp'}->{'user'}=$in{'user'};
	$prop->{'ftp'}->{'pass'}=$in{'pass'};

	$prop->{'rest'}->{'premeasurements'}=$in{'premeasurements'};
	$prop->{'rest'}->{'prefile'}=$in{'prefile'};
	$prop->{'rest'}->{'files'}=$in{'files'};

	createMcglProp($prop);
	&webmin_log("Save", "Mcgl Configuration", \$prop);
	&redirect("index.cgi");
}
