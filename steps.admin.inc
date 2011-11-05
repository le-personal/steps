<?php

function steps_debug() {

  /*steps_enable();
  
  $results = steps_get_steps();
  
  $output = "Output empty <br />";
  foreach($results as $result) {
    $output .= $result->title . '<br />';
    $output .= $result->wizard;
  }
  
  
  dsm($results);
  */
  $token = "GKYr_XpCzCIFM0QgAa5SzI0QWQb7l7ByC6yth72vTqo";
  $object = steps_value_get($token);
  dsm($object);
  
  return "HELLO";
 
}
