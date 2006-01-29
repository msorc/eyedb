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

function convertDate( $date, $sep = "-")
{
  static $months = array( "Jan" => "01", 
			  "Feb" => "02", 
			  "Mar" => "03", 
			  "Apr" => "04", 
			  "May" => "05", 
			  "Jun" => "06", 
			  "Jul" => "07", 
			  "Aug" => "08", 
			  "Sep" => "09", 
			  "Oct" => "10", 
			  "Nov" => "11", 
			  "Dec" => "12");

  /* Thu, 26 Jan 2006 21:55:46 GMT -> 2006-01-26 */
  $arr = explode( " ", $date);

  return $arr[3].$sep.$months[$arr[2]].$sep.$arr[1];
}

function printNews( $item_array, $out)
{
  foreach ($item_array as $item)
    {
      $s = sprintf( "<p><span class=\"NewsDate\">%s</span><br/>\n<a href=\"%s\" class=\"NewsLink\">%s</a></p>\n",
		    convertDate( $item->date),
		    $item->link,
		    $item->title);

      fwrite( $out, $s);
    }
}

function printDownload( $item_array, $out)
{
  $link = 'http://sourceforge.net/project/showfiles.php?group_id=127988';
  $count = 3;

  foreach ($item_array as $item)
    {
      if( ereg( "^[A-Za-z ]+[0-9]+.[0-9]+.[0-9]+", $item->title, $res))
	{
	  $s = sprintf( "<p><a href=\"%s\" class=\"NewsLink\">%s</a></p>\n",
			$link,
			$res[0]);

	  fwrite( $out, $s);

	  if (--$count <= 0)
	    break;
	}
    }
}

/*
 - test if news cache file needs regeneration
 - includes it
*/
function includeRSS( $url, $cache_file, $cache_lifetime, $rss_function) {
  $cache_dir = 'cache';
  $cache_file = $cache_dir.'/'.$cache_file;
  
  /* check if cache file does not exist or has a modification time that is older that cache lifetime */
  if (!file_exists( $cache_file) 
      || (time() - filemtime ($cache_file)) >= $cache_lifetime)
    {
      $rss_parser = new RSSParser();
      $item_array = $rss_parser->parse( $url);

      if (count($item_array) == 0)
	return;

      if (!file_exists( $cache_dir))
	mkdir ( $cache_dir);

      $out = fopen( $cache_file, "w");
    
      call_user_func( $rss_function, $item_array, $out);
      
      fclose( $out);
    }

  include( $cache_file);
}

?>
