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
<ul>
<li>&raquo;<a href="index.php">Home</a></li>
<li>&raquo;<a href="keyfeatures.php">Key features</a></li>
<li>&raquo;<a href="quicktour.php">Quick tour</a></li>
<li>&raquo;<a href="history.php">History</a></li>
<li>&raquo;<a href="publis.php">Publications</a></li>
<li>&raquo;<a href="platforms.php">Platforms</a></li>
<li>&raquo;<a href="licensing.php">Licensing</a></li>
<!-- TBD -->
<!-- <li>&raquo;<a href="devteam.php">Development team</a></li> -->
<li>&raquo;<a href="mailinglists.php">Mailing Lists</a></li>
<li>&raquo;<a href="http://sourceforge.net/projects/eyedb">Sourceforge</a></li>
<li>&raquo;<a href="http://wiki.eyedb.org">Wiki</a></li>
<li>&raquo;<a href="http://blog.eyedb.org">Blog</a></li>
<!-- TBD -->
<!-- <li>&raquo;<a href="irc.php">IRC</a></li> -->
<li id="LastNav">&raquo;<a href="http://doc.eyedb.org">Documentation</a></li>
<!-- TBD -->
<!-- <li>&raquo;<a href="links.php">Links</a></li> -->
</ul>

<div id="Download">
<span class="ColTitle">&gt;&nbsp;DOWNLOAD</span>
<ul>

<?
function printDownload( $item_array, $out)
{
  $link = 'http://sourceforge.net/project/showfiles.php?group_id=127988';
  $count = 3;

  foreach ($item_array as $item)
    {
      if( ereg( "^[A-Za-z ]+[0-9]+.[0-9]+.[0-9]+", $item->title, $res))
	{
	  $s = sprintf( "<li><a href=\"%s\" class=\"DownloadLink\">%s</a></li>\n",
			$link,
			$res[0]);

	  fwrite( $out, $s);

	  if (--$count <= 0)
	    break;
	}
    }
}
includeRSS( 'http://sourceforge.net/export/rss2_projfiles.php?group_id=127988', 'sourceforge_download.html', 60, 'printDownload'); 

?>
</ul>
</div>

</td>
<td id="CenterCol">
