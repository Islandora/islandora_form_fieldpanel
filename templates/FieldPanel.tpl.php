<?php
/**
 * @file
 * Template for the Tabs Element.
 */
$classes = ($collapsible) ? 'islandora-form-fieldpanel-collapsible ' : '';
$classes .= ($collapsed) ? 'islandora-form-fieldpanel-collapsed' : '';
?>
<div class="islandora-form-fieldpanel-pane <?php print $classes ?>">
  <!-- Content  -->
  <?php print $content ?>
</div>

