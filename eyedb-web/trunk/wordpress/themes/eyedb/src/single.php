<?php get_header(); ?>

<?php if (have_posts()) : while (have_posts()) : the_post(); ?>
<div class="post">
<h3><?php the_title(); ?></h3>
<div class="storycontent">
<?php the_content(__('Read more')); ?>
</div>
</div>
<?php endwhile; endif; ?>
	
<?php get_footer(); ?>
