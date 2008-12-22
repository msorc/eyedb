<?php 

/* ======================================== 
 * Quicktour navigation links
 * ======================================== */
function quicktour_nav($id)
{
  $quicktour_pages = get_pages( 'sort_column=menu_order&child_of=6');

//   echo "<pre>\n";
//   print_r( $quicktour_pages);
//   echo "</pre>\n";

  $index = -1;
  foreach ($quicktour_pages as $k => $v) {
    if ($v->ID == $id)
      $index = $k;
  }

  if ($index == -1) {
    echo "<!-- error in quicktour_nav -->\n";
    return;
  }

  $s = "<p class=\"quicktournav\">\n";
  // seems 'guid' fields are not updated w.r.t. permalinks
  if ($index != 0) {
    $s .= '<a href="'.str_replace('/home','',$quicktour_pages[$index-1]->guid).'">'.__('Previous').'</a> | ';
  }    
  $s .= '<a href="/quick-tour">'.__('Top').'</a>';
  if ($index != count($quicktour_pages)-1) {
    $s .= ' | <a href="'.str_replace('/home','',$quicktour_pages[$index+1]->guid).'">Next</a>';
  }

  $s .= "\n</p>\n";

  echo $s;
}

/* ======================================== 
 * RSS parser
 * ======================================== */

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

    $fp = @fopen( $url, "r");

    if (!$fp)
      return null;

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


/* ======================================== 
 * Sidebar functions
 * ======================================== */
if ( function_exists('register_sidebar') )
    register_sidebar();

function eyedb_widget_pages() {
  $out = wp_list_pages( array('title_li' => '', 'echo' => 0, 'sort_column' => 'menu_order', 'depth' => 1) );
  //  wp_list_pages('title_li=&child_of=3&depth=1&sort_column=menu_order' );
  echo '<li id="pages" class="widget widget_pages">';
  echo "\n";
  echo "<ul>\n";
  echo $out;
  echo "</ul>\n";
}

if ( function_exists('register_sidebar_widget') ) {
  register_sidebar_widget(__('Pages'), 'eyedb_widget_pages');
  unregister_widget_control(__('Pages'));
}

function eyedb_widget_download() {

  echo '<li id="download" class="widget widget_downloads"><h2 class="widgettitle">'.__('Download')."</h2>\n";
  echo "<ul>\n";

  $eyedb_group_id = 127988;
  $eyedb_src_package_id = 140123;
  $eyedb_doc_package_id = 177749;

  $rss_url = "http://sourceforge.net/export/rss2_projfiles.php?group_id=$eyedb_group_id";
  $rss_parser = new RSSParser();
  $item_array = $rss_parser->parse( $rss_url);

  $download = 'http://sourceforge.net/project/showfiles.php';
  if ($item_array != null) {
    $src_done = false;
    $doc_done = false;
    foreach ($item_array as $item)
      {
	if( !$src_done && ereg( "EyeDB [0-9]+.[0-9]+.[0-9]+", $item->title, $release)) {
	  ereg( "[0-9]+.[0-9]+.[0-9]+", $release[0], $release_number_a);
	  ereg( "release_id=([0-9]+)", $item->link, $release_id_a);
	  $src_release_number = $release_number_a[0];
	  $src_release_id = $release_id_a[1];
	  $src_link = "{$download}?group_id={$eyedb_group_id}&package_id={$eyedb_src_package_id}&release_id={$src_release_id}";
	  printf( "<li><a href=\"%s\">Sources: %s</a></li>\n", $src_link, $src_release_number);
	  $src_done = true;
	}
	if( !$doc_done && ereg( "EyeDB documentation [0-9]+.[0-9]+.[0-9]+", $item->title, $release)) {
	  ereg( "[0-9]+.[0-9]+.[0-9]+", $release[0], $release_number_a);
	  ereg( "release_id=([0-9]+)", $item->link, $release_id_a);
	  $doc_release_number = $release_number_a[0];
	  $doc_release_id = $release_id_a[1];
	  $doc_link = "{$download}?group_id={$eyedb_group_id}&package_id={$eyedb_doc_package_id}&release_id={$doc_release_id}";
	  printf( "<li><a href=\"%s\">Documentation: %s</a></li>\n", $doc_link, $doc_release_number);
	  $doc_done = true;
	}
      }
  }
  
  $download_link = 'http://sourceforge.net/project/showfiles.php?group_id=127988';
  printf( "<li><a href=\"%s\">%s</a></li>\n", $download_link, "All downloads");

  echo "</ul>\n";
}

if ( function_exists('register_sidebar_widget') ) {
  register_sidebar_widget(__('Download'), 'eyedb_widget_download');
//   unregister_widget_control(__('Download'));
}

?>
