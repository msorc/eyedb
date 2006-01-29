</td> <!-- end of center column -->

<td id="NewsCol">
<div id="News">
<span class="ColTitle">&gt;&nbsp;<a href="http://sourceforge.net/news/?group_id=127988">SOURCEFORGE</a></span><br/>

<?
function printNews( $item_array, $out)
{
  foreach ($item_array as $item)
    {
      $s = sprintf( "
<p>
<span class=\"NewsDate\">%s</span><br/>
<a href=\"%s\" class=\"NewsLink\">%s</a>
</p>
",
		    convertDate( $item->date),
		    $item->link,
		    $item->title);

      fwrite( $out, $s);
    }
}
?>

<? includeRSS( 'http://sourceforge.net/export/rss2_projnews.php?group_id=127988', 'sourceforge_news.html', 2*60, 'printNews'); ?>

<span class="ColTitle">&gt;&nbsp;<a href="http://blog.eyedb.org">EYEDB BLOG</a></span><br/>
<? includeRSS( 'http://blog.eyedb.org/?feed=rss2', 'blog_news.html', 2*60, 'printNews'); ?>
</div>
</td>

<? $rightColumnDone = true; ?>
