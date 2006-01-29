<? include( 'functions.php'); ?>

<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Strict//EN"
   "http://www.w3.org/TR/xhtml1/DTD/xhtml1-strict.dtd">
<html>
<head>
<title><? echo 'EyeDB - '.$title; ?></title>
    <link href="eyedb.css" rel="stylesheet" type="text/css" />
    <meta http-equiv="Content-Type" content="text/html; charset=iso-8859-15" />
</head>
<body>

<table id="Container" cellspacing=0 cellpadding=0;>

<tr id="HeaderRow">
<td id="HeaderCol" colspan=3>
<img src="images/eyedb-logo.png" width="766" height="75" alt="EyeDB" usemap ="#headermap"/>
<map id ="headermap" name="headermap">
  <area shape ="rect" coords ="5,5,175,55" href ="index.php" alt="EyeDB" />
</map> 
</td>
</tr>
<tr id="CenterRow">
<td id="NavCol" height="700">
<ul>
<li>&gt; <a href="index.php">Home</a></li>
<li>&gt; <a href="keyfeatures.php">Key features</a></li>
<li>&gt; <a href="quicktour.php">Quick tour</a></li>
<li>&gt; <a href="history.php">History</a></li>
<li>&gt; <a href="platforms.php">Platforms</a></li>
<li>&gt; <a href="licensing.php">Licensing</a></li>
<!-- TBD -->
<!-- <li>&gt; <a href="devteam.php">Development team</a></li> -->
<li>&gt; <a href="mailinglists.php">Mailing Lists</a></li>
<li>&gt; <a href="http://sourceforge.net/projects/eyedb">Sourceforge</a></li>
<li>&gt; <a href="http://wiki.eyedb.org">Wiki</a></li>
<li>&gt; <a href="http://blog.eyedb.org">Blog</a></li>
<!-- TBD -->
<!-- <li>&gt; <a href="irc.php">IRC</a></li> -->
<li>&gt; <a href="http://doc.eyedb.org">Documentation</a></li>
<!-- TBD -->
<!-- <li>&gt; <a href="links.php">Links</a></li> -->
</ul>

<div id="Download">
<div id="DownloadTitle">Download EyeDB</div>
<? includeRSS( 'http://sourceforge.net/export/rss2_projfiles.php?group_id=127988', 'sourceforge_download.html', 60, 'printDownload'); ?>
</div>

</td>
<td id="CenterCol">
