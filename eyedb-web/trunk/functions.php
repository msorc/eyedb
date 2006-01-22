<?

class RSSParser {

  var $insideitem;
  var $tag;
  var $title;
  var $description;
  var $link;
  var $out;
  var $empty;

  function parse( $url, $file)
  {
    $this->insideitem = false;
    $this->tag = "";
    $this->title = "";
    $this->description = "";
    $this->link = "";
    $this->empty = true;

    $xml_parser = xml_parser_create();
    xml_set_object($xml_parser,&$this);
    xml_set_element_handler($xml_parser, "startElement", "endElement");
    xml_set_character_data_handler($xml_parser, "characterData");

    $fp = fopen( $url,"r");

    if (!$fp)
      return;

    $this->out = fopen( $file, "w");

    while ($data = fread($fp, 4096))
      {
	$res = xml_parse($xml_parser, $data, feof($fp));
	if (!$res)
	  return;
      }

    fclose($fp);
    xml_parser_free($xml_parser);

    if ($this->empty == false)
      fwrite( $this->out, "</dl>\n");

    fclose( $this->out);
  }

  function startElement($parser, $tagName, $attrs) {

    // debug
    //fwrite( $this->out, "<!-- ".$tagName." -->\n");

    if ($this->insideitem) 
      {
	$this->tag = $tagName;
      } 
    elseif ($tagName == "ITEM") 
      {
	$this->insideitem = true;
      }
  }

  function endElement($parser, $tagName) {
    if ($tagName == "ITEM") 
      {
	if ($this->empty) {
	  fwrite( $this->out, "<dl class=\"NewsList\">\n");
	  $this->empty = false;
	}

	$s = sprintf( "<dt>%s</dt>\n<dd><a href='%s' class=\"NewsTitleLink\">%s</a></dd>\n<dd>%s</dd>\n",
		      trim($this->date),
		      trim($this->link),
		      htmlspecialchars(trim($this->title)),
		      trim($this->description));

	fwrite( $this->out, $s);

	$this->title = "";
	$this->description = "";
	$this->link = "";
	$this->date = "";

	$this->insideitem = false;
    }
  }

  function characterData($parser, $data) {
    if ($this->insideitem) 
      {
	switch ($this->tag) {
	case "TITLE":
	  $this->title .= $data;
	  break;
	case "DESCRIPTION":
	  $this->description .= $data;
	  break;
	case "LINK":
	  $this->link .= $data;
	  break;
	case "PUBDATE":
	  $this->date .= $data;
	  break;
	}
    }
  }

}



function regenerateNewsCache( $url, $cache_file)
{
  $rss_parser = new RSSParser();

  $rss_parser->parse( $url, $cache_file);
}

/*
 - test if news cache file needs regeneration
 - includes it
*/
function includeNews( $url, $cache_file, $cache_lifetime) {
  /* check if cache file does not exist or has a modification time that is older that cache lifetime */
  if (!file_exists( $cache_file) 
      || (time() - filemtime ($cache_file)) >= $cache_lifetime)
    regenerateNewsCache( $url, $cache_file);

  include( $cache_file);
}



?>
