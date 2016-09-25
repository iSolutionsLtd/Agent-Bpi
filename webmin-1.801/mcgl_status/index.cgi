#!/usr/bin/perl

require './mcgl_status-lib.pl';
use Data::Dumper;
use POSIX 'strftime';
#use POSIX qw[tzset];
#$ENV{'TZ'} = 'Asia/Jerusalem';

&ui_print_header(undef, "Manage Mcgl status", "");

my @Items=readMcglStat();


#@links = ( 
#		   "<a href=\"edit_ext.cgi\">Add new extension</a>" );
#	print &ui_links_row(\@links);

print "<script language=\"javascript\">setTimeout(function(){window.location.reload(1);}, 10000);</script>\n";
print &ui_form_start("save_ext.cgi");
print &ui_columns_start(["#","File","Folder","State","Status","Date","Size","Resumes"], 50, 0, \@tds);


my $ind=1;
foreach my $Item (@Items)
{
	local @cols;
	push(@cols,$ind);
	push(@cols,$Item->{'SRCFILE'});
	push(@cols,$Item->{'SRCPATH'});
	push(@cols,getStateName($Item->{'STATE'}));
	push(@cols,getStatusName($Item->{'S'}));
	push(@cols,strftime('%d/%m/%Y %T', localtime($Item->{'MODTIME'})));
	push(@cols,int($Item->{'SIZE'}/1024). " KB");
	push(@cols,$Item->{'RESUMES'});
	print &ui_columns_row(\@cols,undef,"d",$key);
	$ind++;

}
print &ui_columns_end();
#print &ui_form_end([ [ "delete", "Delete selected extensions" ] ]);
print &ui_form_end();
#print &ui_links_row(\@links);
ui_print_footer("/", "return to MCGL Configurations");
