<?php

function steps_debug() {

  steps_enable();
  
  $results = steps_get_steps();
  
  $output = "Output empty <br />";
  foreach($results as $result) {
    $output .= $result->title . '<br />';
    $output .= $result->wizard;
  }
  
  
  dsm($results);
  
  return $output;
 
}
