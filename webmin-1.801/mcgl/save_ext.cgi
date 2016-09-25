#!/usr/bin/perl

require './mcgl-lib.pl';

use Data::Dumper;

&ReadParse();
#&ui_print_header(undef, "Save ext", "");
my $prop=readMcglProp();
my $exts = $prop->{'extensions'};
#print Dumper(\$in) . "<br>";
if ($in{'delete'})
{
	@d = split(/\0/, $in{'d'});
	@d || &error($text{'wdelete_enone'});
	if (@d)
	{	
		foreach $d (@d) 
		{
			#print "delete ".$d." <br>";
			delete $exts->{$d};
			&webmin_log("Delete", "Mcgl extension", $d);
		}
		#print Dumper(\$prop) . "<br>";
	}
	createMcglProp($prop);
	&redirect("list_ext.cgi");

}elsif ($in{'save'} && $in{'ext'})
{
	my $ext = lc $in{'ext'};
	delete $exts->{$ext };
	my $newExt;
	$newExt->{'type'}=$in{'type'};
	$newExt->{'EofTime'}=$in{'EofTime'};
	$newExt->{'MaxHoldTime'}=$in{'MaxHoldTime'};
	$newExt->{'MaxHoldSize'}=$in{'MaxHoldSize'};
	$exts->{$ext }=$newExt;
	createMcglProp($prop);
	&webmin_log("Save", "Mcgl extension", $in{'ext'});
	&redirect("list_ext.cgi");
}
