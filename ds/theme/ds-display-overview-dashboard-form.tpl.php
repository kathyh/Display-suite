<?php
// $Id: ds-display-overview-form.tpl.php,v 1.1.2.12 2010/01/29 14:22:40 swentel Exp $

/**
 * @file
 * Template file for the display settings overview form, dashboard style.
 */

if ($rows):

print $dev_preview;
?>

<!-- of course we will remove this message later on -->
Developer preview of dashboard style - this is not included in an official release yet.
This is development code which you can find on <a href="http://github.com/swentel/Display-suite">http://github.com/swentel/Display-suite</a>

<div id="ds-display-content">
  <?php if (!empty($plugins_tabs)): ?>
    <div id="ds-tabs">
      <div id="field-tab" class="tab selected"><a href="javascript:;" onClick="Drupal.DisplaySuite.toggleDisplayTab('field-tab'); return false;"><?php print t('Fields'); ?></a></div>
      <?php foreach ($plugins_tabs as $key => $title): ?>
      <div id="<?php print $key; ?>-tab" class="tab"><a href="javascript:;" onClick="Drupal.DisplaySuite.toggleDisplayTab('<?php print $key; ?>-tab'); return false;"><?php print $title; ?></a></div>
      <?php endforeach; ?>
    </div>
    <div style="clear: both"></div>
  <?php endif; ?>

  <div id="field-content" class="ds-display">

      <!-- Regions -->
      <?php foreach ($regions as $region => $title): ?>
        <div id="ds-db-region-<?php print $region;?>" class="groupWrapper"><?php print $title; ?>

        <!-- fields -->
        <?php
        if (!empty($rows[$region])):
          $count = 0;
          foreach ($rows[$region] as $row): ?>
            <div id="<?php print $row->field_id; ?>" class="groupItem ds-label">
              <div class="itemHeader">
                <span class="<?php print $row->label_class; ?>"><?php print $row->human_name; ?></span>
                <?php print $row->{$build_mode}->label_value; ?>
                <a href="#" class="closeEl">[+]</a>
              </div>
              <div class="itemContent" style="display: none">
                <?php
                  print '<div class="ds-dashboard-label"><span class="label-edit">'. t('Label') .' : '. $row->{$build_mode}->label_edit .'</span><span class="label-select">'. $row->{$build_mode}->label .'</span></div>';
                  print '<div class="ds-dashboard-format">'. $row->{$build_mode}->format .'</div>';
                  print '<div class="ds-dashboard-region">'. $row->{$build_mode}->region .'</div>';
                  print '<div class="ds-dashboard-region">'. $row->ds_weight .'</div>';
                ?>
              </div>
            </div>
            <?php
            $count++;
          endforeach;
        endif;
        ?></div><?php
      endforeach;
      ?>
  </div>
  <?php if (!empty($plugins_tabs)): ?>
    <?php foreach ($plugins_content as $key => $form): ?>
      <div id="<?php print $key; ?>-content" class="ds-hidden"><?php print $form; ?></div>
    <?php endforeach; ?>
  <?php endif; ?>
</div>
<?php
print '<div style="clear: both">'. $submit .'</div>';
endif;
