# net-lib.pl
# Common local networking functions

BEGIN { push(@INC, ".."); };
use WebminCore;
&init_config();

$mcgl_conf="/usr/mcgl/mcglProp.xml";

use Data::Dumper;
use XML::LibXML;




sub readMcglProp
{
	my $parser = XML::LibXML->new();
	my $xmldoc = $parser->parse_file($mcgl_conf);
	
	my $propTree;
	$propTree = readMcglPropNode($xmldoc->documentElement());
#	print "<br>" . Dumper(\@propTree) . "<br>";
	return $propTree;
}

sub readMcglPropNode
{
	my $node = @_[0];
	my $hash;
	foreach my $child ( $node->getChildnodes ) 
	{
		if ( $child->nodeType() == XML_ELEMENT_NODE ) 
		{
			if ($child->findnodes('*')->size)
			{
				my $childParam=readMcglPropNode($child);
				$hash->{$child->nodeName()}=$childParam;
			}else
			{
				$hash->{$child->nodeName()}=$child->textContent();
			}
		}
	}
	return $hash;
}

sub createMcglProp
{	
	my $hash = @_[0];
	my $newDoc = XML::LibXML::Document->new('1.0', 'utf-8');
	#print "<br>" . Dumper(\$hash) . "<br>";
	my $root = $newDoc->createElement("mcgl");
	createMcglPropNode($hash,$root,$newDoc);
	$newDoc->setDocumentElement($root);
	open(my $fh, '>', $mcgl_conf);
	print $fh $newDoc->toString(2);
	close $fh;
	system("/usr/mcgl/McglMng p");
        #print "<br><br>NewXml!<br>".$newDoc->toString(2)."<br>";

	
}

sub createMcglPropNode
{
	my $hash = @_[0];
	my $node = @_[1];
	my $newDoc = @_[2];
	while ( my ($key,$val) = each %$hash ) 
	{
		my $newNode = $newDoc->createElement($key);
		if (%$val)
		{
			createMcglPropNode($val,$newNode,$newDoc);
		}else
		{
			$newNode->appendTextNode($val);
		}
		$node->appendChild($newNode);
	}

}

sub getTypeName
{
	my $type=@_[0];
	my $typeName="Bulk";
	if ($type==1)
	{
		$typeName="Over";
	}elsif ($type==2)
	{
		$typeName="Append";
	}
	return $typeName;
}
1;
