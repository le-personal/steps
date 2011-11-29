<?php 

/**
 * Get the values of all steps using the same token
 */
function steps_value_get($token) {
  $results = db_query("SELECT * FROM {steps_values} WHERE token = :token", array(':token' => $token))->fetchAll();
  $data = array();
  foreach($results as $result) {
    $data[$result->step] = array(
      'token' => $result->token,
      'wizard' => $result->wizard,
      'uid' => $result->uid,
      'step' => $result->step,
      'value' => $result->value
    );
  }
  
  return $data;
}

/**
 * Perform operations on the steps_values table
 * 
 * @param $op
 * -- insert Insert a value into the steps_value table
 * -- delete Delete a value into the steps_value table
 */
function steps_value_op($op, $object) {
  switch ($op) {
    case "insert":
       db_insert("steps_values")
        ->fields(array(
          'token' => $object['token'],
          'wizard' => $object['wizard'],
          'uid' => $object['uid'],
          'step' => $object['step'],
          'value' => $object['value'],
        ))
        ->execute();
      break;

    case "delete":
      db_delete("steps_values")
        ->condition("token", $object['token'])
        ->execute();
      break;
  } 
}

/**
 * Get all the steps values for a wizard
 */
function steps_value_get_by_wizard($wizard) {
  return db_query("SELECT * FROM {steps_values} WHERE wizard = :wizard", array(':wizard' => $wizard))->fetchAssoc();
}

/**
 * Get all steps values for a wizard in a given step
 */
function steps_value_get_by_wizard_and_step($wizard, $step) {
  return db_query("SELECT * FROM {steps_values} WHERE wizard = :wizard AND step = :step", array(':wizard' => $wizard, ':step' => $step))->fetchAssoc();
}