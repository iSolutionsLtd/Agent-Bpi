# net-lib.pl
# Common local networking functions

BEGIN { push(@INC, ".."); };
use WebminCore;
&init_config();

$mcgl_stat="/usr/mcgl/mcglQ.xml";

use Data::Dumper;
use XML::LibXML;




sub readMcglStat
{
	my $parser = XML::LibXML->new();
	my $xmldoc = $parser->parse_file($mcgl_stat);
	
	my @items;
	my $itemsNode=$xmldoc->documentElement()->getChildrenByTagName("ITEMS");
	#print $itemsNode->nodeName."<BR>";
	foreach my $child ( $itemsNode->get_nodelist() ) 
	{
		#print "child=".$child->size."<BR>";
		foreach my $item ($child->getChildnodes ) 
		{
			if ( $item->nodeType() == XML_ELEMENT_NODE ) 
			{
				if ($item->findnodes('*')->size)
				{
					my $itemPar=readMcglStatNode($item);
					push(@items,$itemPar);
				}
			}
		}
		
	}
	#print "<br>" . Dumper(\@items) . "<br>";
	return @items;
}

sub readMcglStatNode
{
	my $node = @_[0];
	my $hash;
	foreach my $child ( $node->getChildnodes ) 
	{
		
		if ( $child->nodeType() == XML_ELEMENT_NODE ) 
		{
			if ($child->findnodes('*')->size)
			{
				print "ITEM<BR>";
				my $childParam=readMcglStatNode($child);
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

sub getStateName
{
	my $type=@_[0];
	my $typeName="Pending";
	if ($type==1)
	{
		$typeName="Prepare";
	}elsif ($type==2)
	{
		$typeName="Uploading";
	}elsif ($type==3)
	{
		$typeName="Error";
	}elsif ($type==4)
	{
		$typeName="Finished";
	}
	return $typeName;
}

sub getStatusName
{
	my $type=@_[0];
	my $typeName="File Detected";
	if ($type==1)
	{
		$typeName="Premeasurements Sended";
	}elsif ($type==2)
	{
		$typeName="Pre File Sended";
	}elsif ($type==3)
	{
		$typeName="File Uploaded";
	}elsif ($type==4)
	{
		$typeName="File Sended";
	}
	return $typeName;
}

1;
