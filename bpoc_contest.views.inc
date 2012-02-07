<?php

function bpoc_contest_views_api() {
  return array(
    'api' => 2,
    'path' => drupal_get_path('module', 'bpoc_contest'),
  );
}

function bpoc_contest_views_data() {

  $data['bpoc_contest_votes']['table']['group'] = t('BPOC Contest');

  $data['bpoc_contest_votes']['vote_value'] = array(
    'title' => t('Value of vote'),
    'help' => t('The value of the vote'),
    'field' => array(
      'handler' => 'bpoc_contest_views_handler_field_value',
      'click sortable' => TRUE,
    ),
    'filter' = array(
      'handler' => 'views_handler_filter_numeric',
      'allow_empty' => TRUE,
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
  );
  return $data;
}

function bpoc_contest_views_data_alter(&$views_data) {
  $default_relationships = module_invoke_all('bpoc_contest_relationships');
  $default_relationships[] = array(
    'description' => t('nodes'),
    'content_type' => 'node',
    'base_table' => 'node',
    'content_id_column' => 'nid',
  );
}

