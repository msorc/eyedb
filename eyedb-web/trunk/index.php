<? include( 'functions.php'); ?>
<? include( 'header.php'); ?>
<? include( 'nav.php'); ?>

<div id="CenterBlock">
<p>
bla bla bla
</p>

<p>
<? includeNews( 'http://blog.eyedb.org/?feed=rss2', 'news_cache_blog.eyedb.org_2.html', 60); ?>
<? includeNews( 'http://sourceforge.net/export/rss2_projnews.php?group_id=127988', 'news_cache_sourceforge.net_news.html', 60); ?>
</p>

</div>

<? include( 'footer.php'); ?>
