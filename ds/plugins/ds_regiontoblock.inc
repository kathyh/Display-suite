<?php
// $Id$

/**
 * @file
 * DS_RegionToBlock: Plugin to make a region available as a block.
 */

class ds_regiontoblock {

  /**
   * Constructor().
   */
  function __construct() {

  }

  /**
   * plugin_form().
   */
  public function plugin_form(&$form, $display_settings) {

    $form['regiontoblock'] = array(
      '#type' => 'fieldset',
      '#collapsible' => FALSE,
      '#collapsed' => FALSE,
      '#description' => t('Toggle regions you want to make available as a block.'),
    );

    $build_mode = $form['#build_mode'];
    $regions = ds_regions();
    foreach ($regions as $region => $title) {
      if ($region != 'disabled') {
        $form['regiontoblock']['region-block-'. $region] = array(
          '#title' => $title,
          '#type' => 'checkbox',
          '#default_value' => ds_default_value($display_settings, $build_mode, 'regiontoblock', 'block', $region, ''),
        );
      }
    }
  }

  /**
   * plugin_form_submit().
   */
  public function plugin_form_submit($form, $form_state, &$display_settings) {

    $regions = ds_regions();
    $regions_save = array();
    $build_mode = $form['#build_mode'];

    foreach ($regions as $region => $title) {
      if (isset($form_state['values']['regiontoblock']['region-block-'. $region]))
      $regions_save['block'][$region] = $form_state['values']['regiontoblock']['region-block-'. $region];
    }

    $display_settings[$build_mode]['regiontoblock'] = $regions_save;
  }

  /**
   * plugin block listing.
   */
  public function block_list($module, &$blocks) {
    $api_info = ds_api_info($module);
    $types = $api_info['types']();
    if (!empty($types)) {
      foreach ($types as $type_key => $object) {
        $display_settings = variable_get($module .'_display_settings_'. $type_key, array());
        foreach ($display_settings as $build_mode => $settings) {
          if (isset($settings['regiontoblock'])) {
            foreach ($settings['regiontoblock']['block'] as $region => $value) {
              if ($value) {
                $block_key = 'ds_regiontoblock_'. $module .'_'. $object->type .'_'. $build_mode .'_'. $region;
                $block_name = $api_info['title'] .': '. $object->name .', '. $build_mode .', '.$region;
                $blocks[$block_key] = array(
                  'info' => $block_name,
                );
              }
            }
          }
        }
      }
    }
  }

  /**
   * execute().
   */
  public function execute(&$vars, &$display, $display_settings, $object_type) {

    /*foreach ($display->all_regions as $region_name => $region_title) {

      $render = ds_default_value($display_settings, $display->build_mode, 'regiontoblock', 'block', $region_name, FALSE);

      if ($render == TRUE && !isset($display->themed_regions[$region_name])) {

        $display->themed_regions[$region_name] = array('content' => '&nbsp;');

        if ($region_name == 'left' || $region_name == 'right') {
          $display->region_classes[$region_name] = $region_name;
        }
      }
    }*/
  }
}
