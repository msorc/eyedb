<?php
/*
Template Name: HomeTemplate
*/
?>
<?php get_header(); ?>

<?php get_sidebar(); ?>
<div id="content">
<h2>Welcome to EyeDB</h2>
<?php if (have_posts()) : while (have_posts()) : the_post(); ?>
<div class="entrytext">
<?php the_content('<p class="serif">Read the rest of this page &raquo;</p>'); ?>
<?php link_pages('<p><strong>Pages:</strong> ', '</p>', 'number'); ?>
</div>
<?php endwhile; endif; ?>
	
<?php query_posts('category_name=News'); ?>

<?php if (have_posts()) : echo '<h2>' . __('News') . '</h2>'; while (have_posts()) : the_post(); ?>
<div id="firstpagenews">
<h3 class="posttitle"><a href="<?php the_permalink() ?>"><?php the_title(); ?></a></h3>
<p class="storycontent">
<?php the_content(__('(Read on ...)')); ?>
</p>
<p><?php echo __('Posted on '); the_date(); /* echo ' - '; comments_number(); */ ?></p>
</div>
<?php endwhile; endif; ?>
</div><!-- /content -->
<?php get_footer(); ?>
