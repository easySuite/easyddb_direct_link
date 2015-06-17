<?php
/**
 * @file
 * easyddb_direct_link.features.user_permission.inc
 */

/**
 * Implements hook_user_default_permissions().
 */
function easyddb_direct_link_user_default_permissions() {
  $permissions = array();

  // Exported permission: 'administer easyddb direct'.
  $permissions['administer easyddb direct'] = array(
    'name' => 'administer easyddb direct',
    'roles' => array(
      'administrators' => 'administrators',
      'local administrator' => 'local administrator',
    ),
    'module' => 'easyddb_direct_link',
  );

  return $permissions;
}
