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
  
  $values = "
  one|Hello World\n
  two|Hola Mundo\n
  ";
  $output = "Hello";
  
$lines = explode("\n", trim($values));
$matches = array();
foreach ($lines as $line) {
  $line = trim($line);

  if (preg_match('/^([^|]*)\|(.*)$/', $line, $matches)) {
    $key = $matches[1];
    $vals = $matches[2];
    
    $arr[$key] = $vals;
  }
}


  dsm($arr);
  
  
  return $output;
 
}
