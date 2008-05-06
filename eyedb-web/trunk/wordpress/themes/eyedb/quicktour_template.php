<?php
/*
Template Name: QuicktourTemplate
*/
?>
<?php get_header(); ?>

<?php get_sidebar(); ?>

<div id="content">
<?php if (have_posts()) : while (have_posts()) : the_post(); ?>
<div class="post">
<h2 id="post-<?php the_ID(); ?>"><?php the_title(); ?></h2>
<div class="entrytext">
<?php the_content('<p class="serif">Read the rest of this page &raquo;</p>'); ?>
<?php link_pages('<p><strong>Pages:</strong> ', '</p>', 'number'); ?>
</div>
<?php quicktour_nav($post->ID) ?>
</div>
<?php endwhile; endif; ?>
</div> <!-- /content -->

<?php get_footer(); ?>
