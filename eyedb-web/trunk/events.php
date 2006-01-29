<div id="Events">

<span class="ColTitle">&gt;&nbsp;EVENTS</span><br/>

<?
function printEvents( $item_array, $out)
{
  foreach ($item_array as $item)
    {
      $s = sprintf( "
<p>
<span class=\"NewsDate\">%s</span><br/>
<a href=\"%s\" class=\"NewsLink\">%s</a><br/>
<span class=\"Event\">%s</span>
</p>
",
		    convertDate( $item->date),
		    $item->link,
		    $item->title,
		    $item->description);

      fwrite( $out, $s);
    }
}
?>

<? includeRSS( 'events/eyedbrss.php', 'eyedb_events.html', 1*60, 'printEvents'); ?>

</div>
