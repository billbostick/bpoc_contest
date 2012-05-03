<?php

function bpoc_contest_views_data() {

  $data = array();
  $data['bpoc_contest_votes']['table']['group'] = t('Votes');

  $data['bpoc_contest_votes']['table']['join']['votingapi_vote'] = array(
    'left_field' => 'vote_id',
    'field' => 'vote_id',
  );

  $data['bpoc_contest_votes']['vote_id'] = array(
    'title' => t('ID'),
    'help' => t('The ID'),
    'field' => array(
      'handler' => 'views_handler_field_numeric',
      'click sortable' => TRUE,
    ),
    'relationship' => array(
      'base' => 'votingapi_vote',
      'field' => 'vote_id',
      'handler' => 'views_handler_relationship',
      'label' => t('BPOC Vote'),
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

  $data['bpoc_contest_votes']['email_address'] = array(
    'title' => t('Email Address'), 
    'help' => t('The Email address of the user who cast the vote.'), 
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
    'argument' => array(
      'handler' => 'views_handler_argument_string',
    ),
  );

  return $data;
}

