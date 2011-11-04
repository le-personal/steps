<?php 

/**
 * Returns a table with all wizards
 * @todo detect wizards in code
 */
function steps_ui_wizard_page() {
  $header = array();
  $header[] = array(
    'data' => t('Wizard'),
    'field' => 'wizard',
    'sort' => 'asc',
  );
  $header[] = array(
    'data' => t('Main path'),
    'field' => 'main_path',
  );
  $header[] = array(
    'data' => t('Start path'),
    'field' => 'start_path',
  );
  $header[] = array(
    'data' => t('End path'),
    'field' => 'end_path',
    'sort' => 'asc',
  );
  $header[] = array("data" => t("Wizard operations"));
  $header[] = array('data' => t('Steps operations'));
  
  $query = db_select('steps_wizard')->extend('PagerDefault')->extend('TableSort');
  $result = $query
    ->fields('steps_wizard')
    ->orderByHeader($header)
    ->limit(50)
    ->execute();
  
  $rows = array();
  $destination = drupal_get_destination();
  foreach ($result as $data) {
    $row = array();
    $row['data']['wizard'] = l(check_plain($data->title), "admin/structure/steps/wizard/$data->wizard/steps");
    $row['data']['main_path'] = check_plain($data->main_path);
    $row['data']['start_path'] = check_plain($data->start_path);
    $row['data']['end_path'] = check_plain($data->end_path);
    
    $operations_wizard = array();
    $operations_wizard['edit-wizard'] = array(
      'title' => t('edit wizard'), 
      'href' => "admin/structure/steps/wizard/$data->wizard/edit", 
      'query' => $destination,
    );
    $operations_wizard['delete-wizard'] = array(
      'title' => t('delete wizard'), 
      'href' => "admin/structure/steps/wizard/$data->wizard/delete", 
      'query' => $destination,
    );
    
    $row['data']['operations_wizard'] = array(
      'data' => array(
        '#theme' => 'links', 
        '#links' => $operations_wizard, 
        '#attributes' => array('class' => array('links', 'inline', 'nowrap')),
      ),
    );

    $operations_steps = array();
    $operations_steps['view-steps'] = array(
      'title' => t('view steps'), 
      'href' => "admin/structure/steps/wizard/$data->wizard/steps", 
      'query' => $destination,
    );
    $operations_steps['add-steps'] = array(
      'title' => t('add steps'), 
      'href' => "admin/structure/steps/wizard/$data->wizard/steps/new", 
      'query' => $destination,
    );
    
    $row['data']['operations'] = array(
      'data' => array(
        '#theme' => 'links', 
        '#links' => $operations_steps, 
        '#attributes' => array('class' => array('links', 'inline', 'nowrap')),
      ),
    );
    
    $rows[] = $row;
  }
  
  $build['path_table'] = array(
    '#theme' => 'table', 
    '#header' => $header, 
    '#rows' => $rows, 
    '#empty' => t('No wizards available'),
  );
  $build['path_pager'] = array('#theme' => 'pager');

  return $build;
}

/**
 * Create wizards form
 */
function steps_ui_wizard_form($form, $args) {
  $form = array();
  
  if(isset($args['build_info']['args'][0])) {
    $wizard = steps_get_wizard($args['build_info']['args'][0]);  
  }
    
  $form['wizard'] = array(
    '#type' => 'textfield',
    '#title' => t("Wizard unique key"),
    '#required' => TRUE,
    '#default_value' => isset($wizard['wizard']) ? $wizard['wizard'] : NULL,
  );
  
  if(isset($wizard['wizard'])) {
    $form['wizard']['#disabled'] = TRUE;
  }
  
  $form['title'] = array(
    '#type' => 'textfield',
    '#title' => t("Wizard title"),
    '#required' => TRUE,
    '#default_value' => isset($wizard['title']) ? $wizard['title'] : NULL,
  );
  
  $form['main_path'] = array(
    '#type' => 'textfield',
    '#title' => t("Main path"),
    '#description' => t("Steps will use the main path to create a menu link which can be used to start the wizard"),
    '#required' => TRUE,
  	'#default_value' => isset($wizard['main_path']) ? $wizard['main_path'] : NULL,
  );
  
  $form['start_path'] = array(
    '#type' => 'textfield',
    '#title' => t("Start path"),
    '#description' => t("This is the first step that will be called"),
    '#required' => TRUE,
  	'#default_value' => isset($wizard['start_path']) ? $wizard['start_path'] : NULL,
  );
  
  $form['end_path'] = array(
    '#type' => 'textfield',
    '#title' => t("End path"),
    '#description' => t("This is the last step. usually, to provent problems, this page is a thanks page or a confirmation page, your wizard must end in this path and the previous to last step must redirect here"),
    '#required' => TRUE,
  	'#default_value' => isset($wizard['end_path']) ? $wizard['end_path'] : NULL,
  );
  
  $form['actions']['#type'] = 'actions';
  
  $form['actions']['submit'] = array(
    '#type' => 'submit',
    '#value' => t("Save"),
  );
  
  return $form;
}

/**
 * Submit callback for creating wizards
 */
function steps_ui_wizard_form_submit(&$form, &$form_state) {
  $values = $form_state['values'];
  
  // Build the wizard object
  $wizard = new stdClass();
  $wizard->wizard = check_plain($values['wizard']);
  $wizard->title = check_plain($values['title']);
  $wizard->main_path = check_plain($values['main_path']);
  $wizard->start_path = check_plain($values['start_path']);
  $wizard->end_path = check_plain($values['end_path']);
  
  // Save the wizard
  steps_wizard_ops($wizard);
  
  // Alert the user that the wizard was created
  drupal_set_message(
    t("The wizard !wizard has been saved, now !add steps", 
      array(
      	'!wizard' => $wizard->title,
      	'!add' => l(t("add"), "admin/structure/steps/wizard/$wizard->wizard/steps/new")
      )
    )
  );
  
  // Redirect to the steps page for the newly created wizard
  drupal_goto("admin/structure/steps/wizard/$wizard->wizard/steps");  
}

/**
 * Delete steps
 */
function steps_ui_wizard_delete($form, $args) {
  $form = array();
  
  if(isset($args['build_info']['args'][0])) {
    $wizard = steps_get_wizard($args['build_info']['args'][0]);  
  }
    
  $form['_wizard'] = array(
    '#type' => 'hidden',
    '#value' => $wizard,
  );
  
  drupal_set_title(t("Are you sure you want to delete the wizard !wizard?", array('!wizard' => $wizard['wizard'])));
  
  $form['message'] = array(
    '#markup' => t("This will also delete all steps related to this wizard.")
  );
  
  $form['actions']['#type'] = 'actions';
  
  $form['actions']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Delete'),
    '#submit' => array("steps_ui_wizard_delete_delete"),
  );
  
  $form['actions']['cancel'] = array(
    '#type' => 'submit',
    '#value' => t("Cancel"),
  	'#submit' => array("steps_ui_wizard_delete_cancel"),
  );
  
  return $form;
}

/**
 * Submit callback for delete operation
 */
function steps_ui_wizard_delete_delete(&$form, &$form_state) {
  $values = $form_state['values'];
    
  steps_delete_wizard($values['_wizard']["wizard"]);
  
  drupal_goto("admin/structure/steps");
}

/**
 * Submit callback for cancel operation
 */
function steps_ui_wizard_delete_cancel(&$form, &$form_state) {
  $values = $form_state['values'];
  drupal_goto("admin/structure/steps");
}

/**
 * Get all steps for a given wizard
 * 
 * @param $wizard (string) the wizard key
 */
function steps_ui_steps_page($wizard) {
  drupal_set_title(t("Steps for wizard !wizard", array('!wizard' => $wizard)));
  
  $header = array();
  $header[] = array(
    'data' => t('Step'),
    'field' => 'step',
    'sort' => 'asc',
  );
  $header[] = array(
    'data' => t('Title'),
    'field' => 'title',
  );
  $header[] = array(
    'data' => t('Path'),
    'field' => 'path',
  );
  $header[] = array(
    'data' => t('Type'),
    'field' => 'component_type',
    'sort' => 'asc',
  );
  $header[] = array('data' => t('Operations'));
  
  $query = db_select('steps')->extend('PagerDefault')->extend('TableSort');
  $result = $query
    ->fields('steps')
    ->condition("wizard", $wizard, "=")
    ->orderByHeader($header)
    ->limit(50)
    ->execute();
  
  $rows = array();
  $destination = drupal_get_destination();
  foreach ($result as $data) {
    $row = array();
    $row['data']['step'] = check_plain($data->step);
    $row['data']['title'] = check_plain($data->title);
    $row['data']['path'] = check_plain($data->path);
    $row['data']['type'] = check_plain($data->component_type);
    $operations = array();
    $operations['view-step'] = array(
      'title' => t('view step'), 
      'href' => "admin/structure/steps/wizard/$data->wizard/steps/$data->step/view",
    );
    $operations['edit-step'] = array(
      'title' => t('edit step'), 
      'href' => "admin/structure/steps/wizard/$data->wizard/steps/$data->step/edit", 
      'query' => $destination,
    );
    $operations['remove-step'] = array(
      'title' => t('remove step'), 
      'href' => "admin/structure/steps/wizard/$data->wizard/steps/$data->step/delete"
      // 'query' => $destination,
    );
    $row['data']['operations'] = array(
      'data' => array(
        '#theme' => 'links', 
        '#links' => $operations, 
        '#attributes' => array('class' => array('links', 'inline', 'nowrap')),
      ),
    );
    
    $rows[] = $row;
  }
  
  $build['path_table'] = array(
    '#theme' => 'table', 
    '#header' => $header, 
    '#rows' => $rows, 
    '#empty' => t('No steps available for this wizard'),
  );
  $build['path_pager'] = array('#theme' => 'pager');

  return $build;
}

/**
 * Create new step
 */
function steps_ui_step_form($form, $args) {
  $form = array();
  
  if(isset($args['build_info']['args'][1])) {
    $step = steps_get_step($args['build_info']['args'][1]);  
  }
    
  $form['wizard'] = array(
    '#type' => 'textfield',
    '#title' => t("Wizard key"),
    '#required' => TRUE,
    '#disabled' => TRUE,
    '#default_value' => isset($step['wizard']) ? $step['wizard'] : $args['build_info']['args'][0],
  );
  
  $form['step'] = array(
    '#type' => 'textfield',
    '#title' => t("Step unique key"),
    '#required' => TRUE,
    '#default_value' => isset($step['step']) ? $step['step'] : NULL,
  );
  if(isset($step->step)) {
    $form['step']['#disabled'] = TRUE;  
  }  
  
  $form['title'] = array(
    '#type' => 'textfield',
    '#title' => t("Title"),
    '#description' => t("Step title"),
    '#required' => TRUE,
    '#default_value' => isset($step['title']) ? $step['title'] : NULL,
  );
  
  $form['path'] = array(
    '#type' => 'textfield',
    '#title' => t("Path"),
    '#description' => t("This is the step path"),
    '#required' => TRUE,
  	'#default_value' => isset($step['path']) ? $step['path'] : NULL,
  );
  
  $form['description'] = array(
    '#type' => 'textfield',
    '#title' => t("Description"),
    '#description' => t("The description will be used as a help message"),
    '#required' => FALSE,
  	'#default_value' => isset($step['description']) ? $step['description'] : NULL,
  );
  
  $form['component_type'] = array(
    '#type' => 'select',
    '#title' => t("Component Type"),
    '#description' => t("Select a component type"),
    '#required' => TRUE,
    '#options' => array(
      'radios' => t("Radios"),
      'select' => t("Select"),
      'textfield' => t("Textfield"),
      'textarea' => t("Textarea"),
      'checkboxes' => t("Checkboxes"),
      'markup' => t("Markup"),
      'form' => t("Custom Form"),
    ),
    '#default_value' => isset($step['component_type']) ? $step['component_type'] : NULL,
  );
  
  $form['component_options'] = array(
    '#type' => 'textarea',
    '#rows' => 10,
    '#title' => t("Component options"),
    '#description' => t("Enter the array of options, one per line, piped. Example: no|Disabled yes|Enabled"),
    '#default_value' => isset($step['component_options']) ? $step['component_options'] : NULL,
  );
  
  $form['component_validation'] = array(
    '#type' => 'textarea',
    '#rows' => 10,
    '#title' => t("Component validation"),
    '#description' => t("Enter the array of validation options, one per line, piped. Example: no|path1 yes|path2"),
    '#default_value' => isset($step['component_validation']) ? $step['component_validation'] : NULL,
  );
  
  $form['component_default'] = array(
    '#type' => 'textfield',
    '#title' => t("Default"),
    '#description' => t("Write the default option"),
  	'#default_value' => isset($step['component_default']) ? $step['component_default'] : NULL,
  );
  
  $form['component_redirect'] = array(
    '#type' => 'textfield',
    '#title' => t("Redirect path"),
    '#description' => t("Write the path to redirect"),
    '#default_value' => isset($step['component_redirect']) ? $step['component_redirect'] : NULL,
  );
  
  $form['component_value'] = array(
    '#type' => 'textarea',
    '#title' => t("Value"),
  	'#rows' => 10,
    '#description' => t("Value of markup"),
  	'#default_value' => isset($step['component_value']) ? $step['component_value'] : NULL,
  );
  
  $form['component_callback'] = array(
    '#type' => 'textfield',
    '#title' => t("Callback name"),
    '#description' => t("Write the name of the function to use. It needs to be in a .module file"),
  	'#default_value' => isset($step['component_callback']) ? $step['component_callback'] : NULL,
  );
  
  $form['actions']['#type'] = 'actions';
  
  $form['actions']['submit'] = array(
    '#type' => 'submit',
    '#value' => t("Save"),
  );
  
  return $form;
}

/**
 * Submit callback for steps_ui_new_step_page()
 */
function steps_ui_step_form_submit(&$form, &$form_state) {
  $values = $form_state['values'];
  
  $step = new stdClass();
  $step->step = $values['step'];
  $step->wizard = $values['wizard'];
  $step->title = $values['title'];
  $step->path = $values['path'];
  $step->description = $values['description'];
  $step->component_type = $values['component_type'];
  $step->component_options = $values['component_options'];
  $step->component_validation = $values['component_validation'];
  $step->component_default = $values['component_default'];
  $step->component_redirect = $values['component_redirect'];
  $step->component_value = $values['component_value'];
  $step->component_callback = $values['component_callback'];
  
  steps_steps_ops($step);
  
  drupal_set_message("Step was saved");
}

/**
 * View steps
 * @todo this should go throught the theme layer
 */
function steps_ui_step_view($wizard, $key) {
  drupal_set_title(t("Step !step in wizard !wizard", array('!step' => $key, '!wizard' => $wizard)));
    
  $step = steps_get_step($key);
  
  $output = '';
  foreach($step as $key => $element) {
    if(isset($element)) { 
       $output .= '<strong>' . str_replace("_", " ", ucfirst($key)) . ": </strong>" . $element . '<br />'; 
    }
  }
  
  return $output;
}

/**
 * Delete steps
 */
function steps_ui_step_delete_form($form, $args) {
  $form = array();
  
  if(isset($args['build_info']['args'][1])) {
    $step = steps_get_step($args['build_info']['args'][1]);  
  }
  
  $form['_step'] = array(
    '#type' => 'hidden',
    '#value' => $step,
  );
  
  drupal_set_title(t("Are you sure you want to delete the step !step?", array('!step' => $step['step'])));
  
  $form['actions']['#type'] = 'actions';
  
  $form['actions']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Delete'),
    '#submit' => array("steps_ui_step_delete_delete"),
  );
  
  $form['actions']['cancel'] = array(
    '#type' => 'submit',
    '#value' => t("Cancel"),
  	'#submit' => array("steps_ui_step_delete_cancel"),
  );
  
  return $form;
}

/**
 * Submit callback for delete operation
 */
function steps_ui_step_delete_delete(&$form, &$form_state) {
  $values = $form_state['values'];
  
  steps_delete_step($values['_step']["step"]);
  
  drupal_goto("admin/structure/steps/wizard/" . $values['_step']['wizard'] . "/steps");
}

/**
 * Submit callback for cancel operation
 */
function steps_ui_step_delete_cancel(&$form, &$form_state) {
  $values = $form_state['values'];
  drupal_goto("admin/structure/steps/wizard/" . $values['_step']['wizard'] . "/steps");
}