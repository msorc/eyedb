<? include( 'functions.php'); ?>
<? include( 'header.php'); ?>
<? include( 'nav.php'); ?>

<div id="CenterBlock">
<p>
bla bla bla
</p>

<div id="NewsBlock">
> LATEST NEWS 
<div class="News">
From <a href="http://sourceforge.net/news/?group_id=127988">Sourceforge</a>:
<? includeNews( 'http://sourceforge.net/export/rss2_projnews.php?group_id=127988', 'news_cache_sourceforge.net_news.html', 30); ?>
</div>
<div class="News">
From <a href="http://blog.eyedb.org">EyEDB blog</a>:
<? includeNews( 'http://blog.eyedb.org/?feed=rss2', 'news_cache_blog.eyedb.org_2.html', 60); ?>
</div>
</div>

</div>

<? include( 'footer.php'); ?>
