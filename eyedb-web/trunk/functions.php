<?

class Item {
  var $title;
  var $description;
  var $link;
  var $date;

  function Item( $title, $description, $link, $date)
  {
    $this->title = $title;
    $this->description = $description;
    $this->link = $link;
    $this->date = $date;
  }
}

class RSSParser {

  var $insideitem;
  var $tag;

  var $title;
  var $description;
  var $link;
  var $date;

  var $item_array;

  function parse( $url)
  {
    $this->insideitem = false;
    $this->tag = "";

    $this->title = "";
    $this->description = "";
    $this->link = "";
    $this->date = "";

    $this->item_array = array();

    $xml_parser = xml_parser_create();
    xml_set_object($xml_parser,&$this);
    xml_set_element_handler($xml_parser, "startElement", "endElement");
    xml_set_character_data_handler($xml_parser, "characterData");

    $fp = fopen( $url,"r");

    if (!$fp)
      return;

    while ($data = fread($fp, 4096))
      {
	$res = xml_parse($xml_parser, $data, feof($fp));
	if (!$res)
	  return;
      }

    fclose($fp);
    xml_parser_free($xml_parser);

    return $this->item_array;
  }

  function startElement($parser, $tagName, $attrs) {
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
	$this->item_array[] = new Item( htmlspecialchars(trim($this->title)),
					trim($this->description),
					trim($this->link),
					trim($this->date));

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

  $item_array = $rss_parser->parse( $url);

  if (count($item_array) == 0)
    return;

  $out = fopen( $cache_file, "w");
  fwrite( $out, "<dl class=\"NewsList\">\n");
    
  foreach ($item_array as $item)
    {
      $s = sprintf( "<dt>%s</dt>\n<dd><a href='%s' class=\"NewsTitleLink\">%s</a></dd>\n<dd>%s</dd>\n",
		    $item->date,
		    $item->link,
		    $item->title,
		    $item->description);

      fwrite( $out, $s);
    }

  fwrite( $out, "</dl>\n");
  fclose( $out);
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
