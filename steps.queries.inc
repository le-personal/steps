<?php 

/**
 *************************************************
 * STEPS OPERATIONS
 *************************************************   
 */

/**
 * Get all steps from the database or code
 */
function steps_get_steps($wizard = NULL) {
  ctools_include("export");
  if(isset($wizard)) {
    // $result = db_query("SELECT * FROM {steps} WHERE wizard = :wizard", array(':wizard' => $wizard->wizard))->fetchAll();
    $result = ctools_export_load_object("steps", "conditions", array('wizard' => $wizard));
  }
  else {
    // $result = db_query("SELECT * FROM {steps}")->fetchAll();
    $result = ctools_export_load_object("steps", "all");
  }
  
  if(isset($result[$wizard])) {
    return $result[$wizard];
  }
}

/**
 * Get a single step wherever it is, db or code
 */
function steps_get_step($key) { 
  ctools_include("export");
  $result = ctools_export_load_object("steps", "names", array($key));
  if(isset($result[$key])) {
    return $result[$key];
  }
   
  // return db_query("SELECT * FROM {steps} WHERE step = :step", array(':step' => $key))->fetchAssoc();
}

/**
 * Get current step by path
 */
function steps_get_step_by_path($path) {
  ctools_include("export");
  // return db_query("SELECT * FROM {steps} WHERE path = :path", array(':path' => $path))->fetchAssoc();
  $result = ctools_export_load_object("steps", "conditions", array('path' => $path));
  if(isset($result[$key])) {
    return $result[$key];
  }
}

/**
 * Delete step
 */
function steps_delete_step($step) {
  db_delete("steps")
  ->condition("step", $step)
  ->execute();
}

/**
 * 
 * Handle insert and update operations
 * @param object $step
 */
function steps_steps_ops($step) {
  // Check if the current wizard object exists
  $result = db_select("steps")
            ->fields("steps")
            ->condition("step", $step->step, "=")
            ->execute()
            ->fetchAssoc();  
  
  // If it does exists then update it, else, insert it
  if($result) {
    db_update("steps")
      ->fields(array(
        'wizard' => $step->wizard,
        'title' => $step->title,
        'path' => $step->path,
        'description' => $step->description,
        'component_type' => $step->component_type,
        'component_options' => $step->component_options,
        'component_validation' => $step->component_validation,
        'component_default' => $step->component_default,
        'component_redirect' => $step->component_redirect,
        'component_value' => $step->component_value,
        'component_callback' => $step->component_callback,
      ))
      ->condition("step", $step->step, "=")
      ->execute();
  }
  else {
    // error here
    db_insert("steps")
      ->fields(array(
        'step' => $step->step,
        'wizard' => $step->wizard,
        'title' => $step->title,
        'path' => $step->path,
        'description' => $step->description,
        'component_type' => $step->component_type,
        'component_options' => $step->component_options,
        'component_validation' => $step->component_validation,
        'component_default' => $step->component_default,
        'component_redirect' => $step->component_redirect,
        'component_value' => $step->component_value,
        'component_callback' => $step->component_callback,
      ))
      ->execute();
  }
}

