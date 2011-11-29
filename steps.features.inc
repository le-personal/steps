<?php
/*
function wizard_features_export($data, &$export, $module_name) {
  $export['dependencies']['steps'] = "steps";
  
  $pipe = array();
  foreach($data as $wizard) {
    $export['features']['wizard'][$wizard] = $wizard;
    $wizard = steps_get_wizard($wizard);
  }
  
  return $pipe;
}

function wizard_features_export_options() {
  $wizards = steps_get_wizard();
  
  foreach($wizards as $wizard) {
    $options[$wizard->wizard] = $wizard->title;
  }
  
  return $options;
}

function wizard_features_export_render($module_name, $data, $export = NULL) {
  $code = array();
  $code[] = '  $wizards = array();';
  foreach($data as $name) {
    $export = features_var_export(steps_get_wizard($name), '  ');
    $code[] = "  // Exported wizard: {$name}";
    $code[] = "  \$wizards['{$name}'] = {$export};";
  }
  $code[] = "  return \$wizards;";
  $code = implode("\n", $code);
  return array("wizard_default_wizards" => $code);
}

function wizard_features_revert($module) {
  if($defaults = features_get_default("wizard", $module)) {
    foreach($defaults as $wizard) {
      $existing = steps_get_wizard($wizard['wizard']);
      drupal_write_record("steps_wizard", $wizard, $existing ? array("wizard") : array());
    }
  }
}

function wizard_features_rebuild($module) {
  $items = module_invoke_all($module, "wizard_default_wizards");
  if(!empty($items)) {
    foreach($items as $item) {
      drupal_write_record("steps_wizard", $item);
    }
  }
}
*/