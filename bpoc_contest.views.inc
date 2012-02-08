<?php

function bpoc_contest_views_api() {
  return array(
    'api' => 2,
    'path' => drupal_get_path('module', 'bpoc_contest'),
  );
}

function bpoc_contest_views_data() {

  $data['bpoc_contest_votes']['table']['group'] = t('Votes');

  $data['bpoc_contest_votes']['table']['base'] = array(
    'field' => 'vid',
    'title' => t('Votes'),
    'help' => t('BPOC Votes'),
    'weight' => -9;
  );

  $data['bpoc_contest_votes']['table']['join'] = array(
    'node' => array(
      'left_field' => 'nid',
      'field' => 'nid',
    ),
  );

  $data['bpoc_contest_votes']['vote_value'] = array(
    'title' => t('Value'),
    'help' => t('The value of the vote'),
    'field' => array(
      'handler' => 'views_handler_field_numeric',
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

  $data['bpoc_contest_votes']['vote_id'] = array(
    'title' => t('ID'),
    'help' => t('The ID'),
    'field' => array(
      'handler' => 'views_handler_field_comment',
      'click sortable' => TRUE,
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_numeric',
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_numeric',
    ),
  );

  $data['bpoc_contest_votes']['nid'] = array(
    'title' => t('Node'),
    'help' => t('The particular node the vote is attached to'),
    'relationship' => array(
      'label' => t('Node'),
      'base' => 'node',
      'base field' => 'nid',
      'handler' => 'views_handler_relationship',
    ),
  );

  $data['bpoc_contest_votes']['uid'] = array(
    'title' => t('User'), 
    'help' => t('The user who cast the vote.'), 
    'field' => array(
      'handler' => 'views_handler_field_user',
      'click sortable' => TRUE,
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_user_name',
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_user_uid',
    ),
    'relationship' => array(
      'handler' => 'views_handler_relationship',
      'base' => 'users',
      'field' => 'uid',
      'label' => t('Individual voter'),
    ),
  );

  $data['bpoc_contest_votes']['current_uid'] = array(
    'title' => t('Current user'), 
    'help' => t('Restrict votes to those cast by the current user.'), 
    'real field' => 'uid',
    'filter' => array(
      'handler' => 'views_handler_filter_user_current',
    ),
  );

  $data['bpoc_contest_votes']['vote_source'] = array(
    'title' => t('IP Address'), 
    'help' => t('The IP address of the user who cast the vote.'), 
    'field' => array(
      'handler' => 'views_handler_field',
      'click sortable' => TRUE,
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_string',
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
  );

  $data['bpoc_contest_votes']['timestamp'] = array(
    'title' => t('Timestamp'), 
    'help' => t('The time the vote was cast.'), 
    'field' => array(
      'handler' => 'views_handler_field_date',
      'click sortable' => TRUE,
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_date',
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

