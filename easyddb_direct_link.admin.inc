<?php
/**
 * @file
 * easyOPAC direct link admin interface.
 */

/**
 * Admin form defining link redirection rules.
 */
function easyddb_direct_link_admin_form($form, &$form_state) {
  $form['links_rows'] = array(
    '#type' => 'container',
    '#tree' => TRUE,
  );

  // Fill existing settings.
  $existing = variable_get('links_rows', array());
  $existing_count = count($existing);
  for ($i = 0; $i < $existing_count; $i++) {
    $form['links_rows'][$i] = _easyddb_direct_link_form_row($form_state, $existing[$i]);
  }

  // Create empty rows.
  $new_rows = isset($form_state['link_rows_count']) ? $form_state['link_rows_count'] : 1;
  $form_state['link_rows_count'] = $new_rows;
  for ($i = $existing_count; $i < $existing_count + $new_rows; $i++) {
    $form['links_rows'][$i] = _easyddb_direct_link_form_row($form_state);
  }

  $form['links_add'] = array(
    '#type' => 'submit',
    '#value' => t('Add new row'),
    '#ajax' => array(
      'callback' => '_easyddb_direct_link_row_ajax_callback',
      'wrapper' => 'easyddb-direct-links',
      'method' => 'replace',
      'effect' => 'fade',
    ),
    '#submit' => array('easyddb_direct_link_admin_form_row_submit'),
  );

  $form['links_save'] = array(
    '#type' => 'submit',
    '#value' => t('Save configuration'),
  );

  $form['#prefix'] = '<div id="easyddb-direct-links">';
  $form['#suffix'] = '</div>';
  $form['#theme'] = array('easyddb_direct_link_admin_form');
  $form['#attached'] = array(
    'css' => array(
      drupal_get_path('module', 'easyddb_direct_link') . '/css/easyddb_direct_link.styles.css',
    ),
  );

  return $form;
}

/**
 * Custom submit handler for new row button.
 *
 * @see easyddb_direct_link_admin_form()
 */
function easyddb_direct_link_admin_form_row_submit($form, &$form_state) {
  $form_state['link_rows_count']++;
  $form_state['rebuild'] = TRUE;
}

/**
 * Custom submit handler for saving row values.
 *
 * @see easyddb_direct_link_admin_form()
 */
function easyddb_direct_link_admin_form_submit($form, &$form_state) {
  $input = $form_state['input']['links_rows'];
  // Skip settings that lack either path, or target value.
  $clean = array_filter($input, function($v) {
    return (!empty($v['link_path']) && !empty($v['link_target']));
  });

  variable_set('links_rows', $clean);

  drupal_set_message(t('Configuration options saved.'));
}

/**
 * Custom ajax callback handler for new row button.
 *
 * @see easyddb_direct_link_admin_form()
 */
function _easyddb_direct_link_row_ajax_callback($form, &$form_state) {
  return $form;
}

/**
 * Define a single row fields.
 * @param array $form_state
 *   The form state.
 *
 * @return array
 *   Form API array.
 *
 * @see easyddb_direct_link_admin_form()
 */
function _easyddb_direct_link_form_row($form_state, $default = array()) {
  global $base_url;
  $url = parse_url($base_url);
  $form = array();

  $form['link_enabled'] = array(
    '#type' => 'checkbox',
    '#default_value' => isset($default['link_enabled']) ? $default['link_enabled'] : '',
  );

  $form['link_path'] = array(
    '#type' => 'textfield',
    '#default_value' => isset($default['link_path']) ? $default['link_path'] : '',
    '#size' => 32,
    '#prefix' => '<div class="form-item-prefix">' . $url['scheme'] . '://</div>',
    '#suffix' => '<div class="form-item-suffix">.' . $url['host'] . '</div>',
  );

  $form['link_target'] = array(
    '#type' => 'textfield',
    '#default_value' => isset($default['link_target']) ? $default['link_target'] : '',
    '#size' => 32,
    '#prefix' => '<div class="form-item-prefix">' . $base_url . '/user/%/</div>',
  );

  return $form;
}

/**
 * Custom theming function for whole backend settings form.
 *
 * Main purpose is to place fields in a table view.
 *
 * @see easyddb_direct_link_theme()
 * @see easyddb_direct_link_admin_form()
 */
function theme_easyddb_direct_link_admin_form($variables) {
  $form = $variables['form'];

  $header = array(
    t('Enabled'),
    t('Sub-domain'),
    t('Redirect to'),
  );
  $rows = array();

  foreach (element_children($form['links_rows']) as $key) {
    $row = array(
      drupal_render($form['links_rows'][$key]['link_enabled']),
      drupal_render($form['links_rows'][$key]['link_path']),
      drupal_render($form['links_rows'][$key]['link_target']),
    );

    $rows[] = $row;
  }

  $table = array(
    'header' => $header,
    'rows' => $rows,
    'attributes' => array(),
    'caption' => '',
    'colgroups' => array(),
    'sticky' => FALSE,
    'empty' => 'EMPTY',
  );

  $output = theme_table($table);
  $output .= drupal_render_children($form);

  return $output;
}
