<?php 

/**
 *************************************************
 * WIZARD OPERATIONS
 *************************************************   
 */

/**
 * Get a given wizard from the database or code
 */
function steps_get_wizard($wizard) {
  ctools_include("export");
  $result = ctools_export_load_object("steps_wizard", "names", array($wizard));
  if(isset($result[$wizard])) {
    return $result[$wizard];
  }
}

/**
 * Get all wizards from table or code
 */
function steps_get_wizards() {
  ctools_include("export");
  return ctools_export_crud_load_all("steps_wizard");
}

/**
 * Delete wizard
 */
function steps_delete_wizard($wizard) {  
  // Delete all steps
  $steps = db_query("SELECT * FROM {steps} WHERE wizard = :wizard", array(':wizard' => $wizard))->fetchAll();
  foreach($steps as $step) {
    steps_delete_step($step->step);
  }
  
  db_delete("steps_wizard")
  ->condition("wizard", $wizard, "=")
  ->execute();
}

/**
 * Insert and update operations for wizards
 */
function steps_wizard_ops($wizard) {
  // Check if the current wizard object exists
  $result = db_select("steps_wizard", "w")
            ->fields("w", array("wizard"))
            ->condition("wizard", $wizard->wizard, "=")
            ->execute()
            ->fetchAssoc();  
  
  // If it does exists then update it, else, insert it
  if($result) {
    db_update("steps_wizard")
      ->fields(array(
        'title' => $wizard->title,
        'main_path' => $wizard->main_path,
        'start_path' => $wizard->start_path,
        'end_path' => $wizard->end_path,
      ))
      ->condition("wizard", $wizard->wizard, "=")
      ->execute();
  }
  else {
    db_insert("steps_wizard")
      ->fields(array(
        'wizard' => $wizard->wizard,
        'title' => $wizard->title,
        'main_path' => $wizard->main_path,
        'start_path' => $wizard->start_path,
        'end_path' => $wizard->end_path,
      ))
      ->execute();
  }
}