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

function quicktour_nav()
{
  $quicktour_pages = get_pages( 'sort_column=menu_order&child_of=6');

  foreach ($quicktour_pages as $k => $v) {
    print( "****************************************\n"); 
    print( $k); 
    if ($k == 0 && $v->ID == the_ID())
      print ( "=== FIRST\n");
    if ($k == (count($quicktour_pages) - 1)  && $v->ID == the_ID())
      print ( "=== LAST\n");
    print( "****************************************\n"); 
  }

  return;

  $s = "<p class=\"quicktournav\">";
  if ($previous)
    $s .= "<a href=\"/home/quick-tour/$previous\">Previous</a> | ";
  $s .= "<a href=\"/home/quick-tour\">Quick</a>";
  if ($next)
    $s .= " | <a href=\"/home/quick-tour/$next\">Next</a>";
  $s .= "</p>\n";

  echo $s;
}

?>
