<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Strict//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-strict.dtd">
<html xmlns="http://www.w3.org/1999/xhtml" <?php language_attributes(); ?>>

<head>
<meta http-equiv="Content-Type" content="<?php bloginfo('html_type'); ?>; charset=<?php bloginfo('charset'); ?>" />
<meta name="generator" content="WordPress <?php bloginfo('version'); ?>" /> <!-- leave this for stats -->
<meta name="keywords" content="eyedb, database, object" />

<title><?php bloginfo('name'); ?><?php wp_title(); ?></title>

<link rel="stylesheet" type="text/css" media="screen" href="<?php bloginfo('stylesheet_url'); ?>" />

</head>

<body>

<div id="Header">
<img src="<?php bloginfo('stylesheet_directory'); ?>/images/eyedb-logo.png" width="766" height="75" alt="EyeDB" usemap ="#headermap"/>
<map id ="headermap" name="headermap"><area shape ="rect" coords ="5,5,175,55" href ="index.php" alt="EyeDB" /></map> 
</div>

<div id="Nav">
<?php 
  /* Home page is id 3 */
  wp_list_pages('title_li=&child_of=3&depth=1&sort_column=menu_order' ); 
?>
</div>


