<? include( 'functions.php'); ?>

<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Strict//EN"
   "http://www.w3.org/TR/xhtml1/DTD/xhtml1-strict.dtd">
<html>
<head>
<title><?= 'EyeDB - '.$title; ?></title>
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

<ul class="List">
<li class="FirstItem">&raquo;<a href="index.php">Home</a></li>
<li class="Item">&raquo;<a href="keyfeatures.php">Key features</a></li>
<li class="Item">&raquo;<a href="quicktour.php">Quick tour</a></li>
<li class="Item">&raquo;<a href="history.php">History</a></li>
<li class="Item">&raquo;<a href="publis.php">Publications</a></li>
<li class="Item">&raquo;<a href="platforms.php">Platforms</a></li>
<li class="Item">&raquo;<a href="licensing.php">Licensing</a></li>
<li class="LastItem">&raquo;<a href="mailinglists.php">Mailing Lists</a></li>
<!-- Development team -->
<!-- IRC -->
<!-- Links -->
</ul>

<div class="BlockTitle">&gt;&nbsp;EYEDB SITES</div>
<ul class="List">
<li class="FirstItem">&raquo;<a href="http://sourceforge.net/projects/eyedb">Sourceforge</a></li>
<li class="Item">&raquo;<a href="http://wiki.eyedb.org">Wiki</a></li>
<li class="Item">&raquo;<a href="http://blog.eyedb.org">Blog</a></li>
<li class="LastItem">&raquo;<a href="http://doc.eyedb.org">Documentation</a></li>
</ul>

<div class="BlockTitle">&gt;&nbsp;DOWNLOAD</div>

<?
includeRSS( 'http://sourceforge.net/export/rss2_projfiles.php?group_id=127988', 'sourceforge_download.html', 'printDownload'); 
?>

</td>
<td id="CenterCol">
