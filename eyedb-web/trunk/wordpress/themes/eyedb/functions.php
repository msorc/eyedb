<?php 

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
  // seems guid fields are not updated w.r.t. permalinks
  if ($index != 0) {
    $s .= "<a href=\"".str_replace('/home','',$quicktour_pages[$index-1]->guid)."\">Previous</a> | ";
  }    
  $s .= "<a href=\"/quick-tour\">Top</a>";
  if ($index != count($quicktour_pages)-1) {
    $s .= " | <a href=\"".str_replace('/home','',$quicktour_pages[$index+1]->guid)."\">Next</a>";
  }

  $s .= "\n</p>\n";

  echo $s;
}

?>
