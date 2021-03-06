<?php
// $Id: theme.inc,v 1.1.2.22 2010/02/03 13:15:50 swentel Exp $

/**
 * @file
 * Theming functions for ds.
 */

/**
 * Theme function to render the field content.
 *
 * @param string $content The content to render.
 * @param string $field_key The name of the field key.
 * @param array $field The current field.
 */
function theme_ds_field($content, $field_key, $field) {

  $output = '';
  if (!empty($content)) {
    if ($field['type'] == 'ds') {
      $output .= '<div class="field field-'. $field_key .'">';
      // Above label.
      if ($field['labelformat'] == 'above') {
        $output .= '<div class="field-label">'. $field['title'] .': </div>';
      }
      // Inline label
      if ($field['labelformat'] == 'inline') {
        $output .= '<div class="field-label-inline-first">'. $field['title'] .': </div>';
      }
      $output .= $content;
      $output .= '</div>';
    }
    else {
      $output = $content;
    }
  }

  return $output;
}

/**
 * Evaluate custom code.
 *
 * @param array $field The field with all its properties.
 */
function theme_ds_eval_code($field) {
  if (isset($field['properties']['code'])) {
    $value = ds_eval($field['properties']['code'], $field['object']);
    // Token support - check on token property so we don't run every single field through token.
    if (module_exists('token') && isset($field['properties']['token']) && $field['properties']['token'] == TRUE) {
      $value = token_replace($value, $field['object_type'], $field['object']);
    }
    return $value;
  }
}

/**
 * Evaluate block field.
 *
 * @param array $field The field array.
 */
function theme_ds_eval_block($field) {
  if (isset($field['properties'])) {
    list($module, $delta) = explode('|', $field['properties']['block']);
    $block = module_invoke($module, 'block', 'view', $delta);
    if (!empty($block)) {
      switch ($field['properties']['render']) {
        case DS_BLOCK_TEMPLATE:
          global $theme_key;
          $block = (object) $block;
          $block->region = NULL;
          $block->module = $module;
          $block->delta = $delta;
          $block_title = db_result(db_query("SELECT title FROM {blocks} WHERE module = '%s' AND delta = '%s' AND theme = '%s'", $module, $delta, $theme_key));
          if (!empty($block_title)) {
            $block->subject = $block_title == '<none>' ? '' : check_plain($block_title);
          }
          $content = theme('block', $block);
          break;
        case DS_BLOCK_TITLE_CONTENT:
          $content = '<h2 class="block-title">'. $block['subject'] .'</h2>';
          $content .= $block['content'];
          break;
        case DS_BLOCK_CONTENT:
          $content = $block['content'];
          break;
      }
      return $content;
    }
  }
}

/**
 * Theme author name.
 */
function theme_ds_author_nolink($field) {
  return $field['object']->name;
}

/**
 * Theme author name linked.
 */
function theme_ds_author_link($field) {
  return theme('username', $field['object']);
}

/**
 * Theme the user picture.
 */
function theme_ds_picture($field) {
  return theme('user_picture', $field['object']);
}

/**
 * Template preprocess function for theme_views_view_row_object().
 */
function template_preprocess_ds_row_object(&$vars) {

  $options = $vars['options'];
  $vars['object'] = ''; // make sure var is defined.

  // We delegate the manipulation of the object to a function
  // which every module implementing the ds_api should define.
  // This can either be the simple selector or the advanced selector.
  // The advanced selector searches for a function build like this:
  // ds_views_row_adv_VIEWSNAME.
  if ($options['advanced']) {
    $row_function = 'ds_views_row_adv_'. $vars['view']->name;
    $row_function($vars);
  }
  // Simple selector.
  else {
    $row_function = 'ds_views_row_'. $vars['view']->base_table;
    $build_mode = $options['build_mode'];
    $row_function($vars, $build_mode);
  }
}

/**
 * Theme preprocess function for ds-display-overview-form.tpl.php.
 */
function template_preprocess_ds_display_overview_form(&$vars) {
  $form = &$vars['form'];

  $layout = $form['#layout'];
  $build_mode = $form['#build_mode'];
  $regions = ds_regions(variable_get('ds_build_mode_'. $build_mode, 'all'), $layout == 'tabledrag' ? FALSE : TRUE);

  // Determine which css and js to load.
  $vars['dev_preview'] = '<p>'. l('Table drag style', $_GET['q']) .' - '. l('Dashboard style', $_GET['q'], array('query' => array('layout' => '1'))) .'</p>';
  _template_preprocess_ds_display_overview_form($vars, $regions, $layout);

  // Order the fields.
  $order = array();
  foreach ($form['#fields'] as $key => $field) {
    $order[$field] = $form[$field]['ds_weight']['#default_value'];
  }
  asort($order);

  $rows = array();
  foreach ($order as $key => $field) {
    $element = &$form[$key];
    $row = new stdClass();

    // Each field will have a region, store it temporarily
    $region = $element[$build_mode]['region']['#default_value'];

    // Add field_id key
    $row->field_id = $key;

    foreach (element_children($element) as $child) {

      // Render the display fields
      if ($child == $build_mode) {
        $row->{$child}->label = drupal_render($element[$child]['label']);
        $row->{$child}->label_edit = drupal_render($element[$child]['label_edit']);
        $row->{$child}->label_value = drupal_render($element[$child]['label_value']);
        $row->{$child}->format = drupal_render($element[$child]['format']);
        $row->{$child}->region = drupal_render($element[$child]['region']);
      }
      // Render the rest of the fields
      else {
        // Process weight.
        if ($child == 'ds_weight') {
          $element['ds_weight']['#attributes']['class'] = 'field-weight field-weight-'. $region;
          $element['ds_weight'] = process_weight($element['ds_weight']);
        }
        $row->{$child} = drupal_render($element[$child]);
      }
    }

    // Add draggable.
    if ($layout == 'tabledrag') {
      $row->class = 'draggable';
      if ($region == 'disabled') {
        $row->class .= ' region-css-'. $region;
      }
    }

    $row->label_class = 'label-field';
    $rows[$region][] = $row;
  }

  // Plugins available.
  $vars['plugins_tabs'] = array();
  $vars['plugins_content'] = '';
  if ($form['#plugins'] == TRUE) {
    foreach ($form['#plugin_keys'] as $key => $title) {
      $vars['plugins_tabs'][$key] = $title;
      $vars['plugins_content'][$key] = drupal_render($form[$key]);
    }
  }

  $vars['rows'] = $rows;
  $vars['submit'] = drupal_render($form);
  $vars['regions'] = $regions;
  $vars['build_mode'] = $build_mode;
}

/**
 * Helper function to determine which layout we need to load. Can either
 * be the tabledrag screen or the dashboard style layout.
 *
 * @param array $vars All variables build right now.
 * @param array $regions All regions available.
 * @param string $layout The layout to load.
 */
function _template_preprocess_ds_display_overview_form(&$vars, $regions, $layout) {

  drupal_add_css(drupal_get_path('module', 'ds') .'/css/ds.css');
  drupal_add_js(drupal_get_path('module', 'ds') .'/js/ds.js');

  // Tabledrag layout.
  if ($layout == 'tabledrag') {

    drupal_add_js('misc/tableheader.js');
    drupal_add_js(drupal_get_path('module', 'ds') .'/js/ds-tabledrag.js');

    foreach ($regions as $region => $title) {
      drupal_add_tabledrag('fields', 'match', 'sibling', 'field-region-select', 'field-region-'. $region, NULL, FALSE);
      // We need to have subgroups to make the dragging available within parents
      drupal_add_tabledrag('fields', 'order', 'sibling', 'field-weight', 'field-weight-'. $region);
    }
  }

  // Dashboard layout.
  if ($layout == 'dashboard') {
    $vars['template_files'][] = 'ds-display-overview-dashboard-form';
    drupal_add_js(drupal_get_path('module', 'ds') .'/js/interface.js');
    drupal_add_js(drupal_get_path('module', 'ds') .'/js/ds-dashboard.js');
  }
}

