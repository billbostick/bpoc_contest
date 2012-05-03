<?php

/**
 * @file bpoc_contest_voting.inc - implements voting api functions for bpoc_contest module
 */

/**
 * save user vote to the database 
 */
function bpoc_contest_vote($nid, $email) {
  global $user;

  // prepare the vote structure
  $votes = array();
  $vote = array(
    'content_id' => $nid,
    'value' => 1,
    'tag' => 'bpoc_contest',
    'uid' => $user->uid,
    'value_type' => 'points',
  );
  $votes[] = $vote;
  $criteria = array();

  if (bpoc_contest_validate_vote($nid, $email)) {

    // save the email address to our own table
    // we don't know the vote_id yet, so we'll write a 0 there for now
    // and update the table in the insert hook
    db_query("INSERT INTO {bpoc_contest_votes} (vote_id, email_address) VALUES (%d, '%s')", 0, htmlentities($email));

    // call voting API function to save vote
    votingapi_set_votes($votes, $criteria);

    // write an entry to the watchdog log
    watchdog('bpoc_contest', 'Vote by @user (@email - @ipaddress) on node @nid.', array('@user' => $user->name, '@email' => $email, '@ipaddress' => ip_address(), '@nid' => $nid));

    // invoke our custom rules hook
    rules_invoke_event('bpoc_contest_vote_cast', $user, $nid);
  }

  return;
}

function bpoc_contest_votingapi_insert($votes) {
  // extract the vote_id value
  $vote_id = $votes[0]['vote_id'];

  // update the record in the bpoc_contest_votes table with the correct vote_id
  db_query("UPDATE {bpoc_contest_votes} SET vote_id = %d WHERE vote_id = %d", $vote_id, 0);
  return;  
}

function bpoc_contest_validate_vote($nid, $email) {
  $criteria = array();
  $criteria['vote_source'] = ip_address();
  $criteria['timestamp'] = time();

  $votes = votingapi_select_votes($criteria);
//  dpm($votes);

  return TRUE;
}

/**
 * given a node id, return the vote count for that node
 */
function bpoc_contest_get_submission_vote_count($nid) {
//  return db_result(db_query("SELECT COUNT({bpoc_contest_votes}.nid) FROM {bpoc_contest_votes} WHERE {bpoc_contest_votes}.nid = %d", $nid));
}

/*
 * given a contest node id, return an array of submission node ids for that contest
 */
function bpoc_contest_get_submissions($nid) {
  $node = node_load($nid);
  $contest_type = $node->field_contest_type[0]['value'];
  if ($contest_type == 'Photo') {
    $result = db_query("SELECT n.nid FROM {node} n LEFT JOIN {content_type_photo_submission} s ON n.nid = s.nid WHERE s.field_contest_ref_nid = %d", $nid);
  }
  else if ($contest_type == 'Video') {
    $result = db_query("SELECT n.nid FROM {node} n LEFT JOIN {content_type_video_submission} s ON n.nid = s.nid WHERE s.field_contest_ref_nid = %d", $nid);
  }
  $submissions = array();
  while ($record = db_fetch_object($result)) {
    $submissions[] = $record->nid;
  }
  return $submissions;
}

/**
 * given a contest node id, return an array of votes placed for that contest
 */
function bpoc_contest_get_votes($nid) {
//  $result = db_query("SELECT v.uid, v.email_address, v.timestamp, v.source FROM {bpoc_contest_votes} v WHERE v.nid = %d", $nid);
  $votes = array();
  $vote = array();
  while ($record = db_fetch_object($result)) {
    $vote['uid'] = $record->uid;
    $vote['email_address'] = $record->email_address;
    $vote['timestamp'] = $record->timestamp;
    $vote['source'] = $record->source;
    $votes[] = $vote;
  }
 return $votes; 
}

/**
 * menu handler to display votes for both contests and submissions
 */
function bpoc_contest_show_votes($node) {
  if ($node->type == 'contest') {
    return bpoc_contest_show_contest_votes($node);
  }
  if ($node->type == 'photo_submission') {
    return bpoc_contest_show_submission_votes($node);
  }
  if ($node->type == 'video_submission') {
    return bpoc_contest_show_submission_votes($node);
  }
}

/**
 * admin page to display all contest votes
 * will replace with a view when views integration is complete
 */
function bpoc_contest_show_contest_votes($node) {
  $output = views_embed_view('bpoc_photo_contest','page_2', $node->nid);
  return $output;
}

/**
 * admin page to display all votes for a submission
 * will replace with a view when views integration is complete
 */
function bpoc_contest_show_submission_votes($node) {
  $output = views_embed_view('bpoc_photo_contest','block_2', $node->nid);
  return $output;
}

// utility functions

/**
 * multi-dimensional array sort
 */
function array_sort_by_column(&$arr, $col, $dir = SORT_DESC) {
  $sort_col = array();
  foreach ($arr as $key=> $row) {
    $sort_col[$key] = $row[$col];
  }
  array_multisort($sort_col, $dir, $arr);
}
