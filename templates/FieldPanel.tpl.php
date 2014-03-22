<?php
/**
 * @file
 * Template for the Tabs Element.
 */
$classes = ($collapsible) ? 'xml-form-elements-tabs-collapsible ' : '';
$classes .= ($collapsed) ? 'xml-form-elements-tabs-collapsed' : '';
?>
<div class="xml-form-elements-tabs <?php print $classes ?>">
  <!-- Content  -->
  <?php print $content ?>
</div>

