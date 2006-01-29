</td> <!-- end of center column -->

<td id="NewsCol">
<div id="News">
<span class="ColTitle">&gt;&nbsp;<a href="http://sourceforge.net/news/?group_id=127988">SOURCEFORGE</a></span><br/>

<? includeRSS( 'http://sourceforge.net/export/rss2_projnews.php?group_id=127988', 'sourceforge_news.html', 'printNews'); ?>

<span class="ColTitle">&gt;&nbsp;<a href="http://blog.eyedb.org">EYEDB BLOG</a></span><br/>
<? includeRSS( 'http://blog.eyedb.org/?feed=rss2', 'blog_news.html', 'printNews'); ?>
</div>
</td>

<? $rightColumnDone = true; ?>
