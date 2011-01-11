<?php
function traffy_admin_settings() {
  $form['traffy_appid'] = array(
    '#type' => 'textfield', 
    '#title' => t('AppId'), 
    '#default_value' => variable_get('traffy_appid', 0), 
    '#description' => t('Enter your appid'),
  );
  $form['traffy_apikey'] = array(
    '#type' => 'textfield', 
    '#title' => t('ApiKey'), 
    '#default_value' => variable_get('traffy_apikey', 0), 
    '#description' => t('Enter your apikey'),
  );
  $form['traffy_service_url'] = array(
    '#type' => 'textfield', 
    '#title' => t('Service URL'), 
    '#default_value' => variable_get('traffy_service_url', 'http://api.traffy.in.th/apis/apitraffy.php'), 
    '#description' => t('Enter your apikey'),
  );
  $form = system_settings_form($form);

  return $form; 
}

