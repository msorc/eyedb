<html>
<body>
<? 

error_reporting( E_ALL);


include( 'functions.php');

includeRSS( 'http://localhost/eyedb/rss.php', 'eyedb_events.html', 'printEvents', true, 60);

includeRSS( 'http://sourceforge.net/export/rss2_projfiles.php?group_id=127988', 'sourceforge_download.html', 'printDownload', true, 60);

includeRSS( 'http://sourceforge.net/export/rss2_projnews.php?group_id=127988', 'sourceforge_news.html', 'printNews', true, 60);

includeRSS( 'http://blog.eyedb.org/?feed=rss2', 'blog_news.html', 'printNews', true, 60);

?>
</body>
</html>

