<?php

/**
 * @file bpoc_contest_voting.inc - implements voting api functions for bpoc_contest module
 */

/**
 * save user vote to the database 
 */
function bpoc_contest_vote($nid, $email='') {
  global $user;
  db_query("INSERT INTO {bpoc_contest_votes} (uid, nid, vote_value, email_address, timestamp, source) VALUES (%d, %d, %d, '%s', %d, '%s')", $user->uid, $nid, 1, htmlentities($email), time(), ip_address());
  watchdog('bpoc_contest', 'Vote by @user (@email - @ipaddress) on node @nid.', array('@user' => $user->name, '@email' => $email, '@ipaddress' => ip_address(), '@nid' => $nid));
  return;
}

/**
 * given a node id, return the vote count for that node
 */
function bpoc_contest_get_submission_vote_count($nid) {
  return db_result(db_query("SELECT COUNT({bpoc_contest_votes}.nid) FROM {bpoc_contest_votes} WHERE {bpoc_contest_votes}.nid = %d", $nid));
}

/*
 * given a contest node id, return an array of submission node ids for that contest
 */
function bpoc_contest_get_submissions($nid) {
  $result = db_query("SELECT n.nid FROM {node} n LEFT JOIN {content_type_photo_submission} s ON n.nid = s.nid WHERE s.field_contest_ref_nid = %d", $nid);
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
  $result = db_query("SELECT v.uid, v.email_address, v.timestamp, v.source FROM {bpoc_contest_votes} v WHERE v.nid = %d", $nid);
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
}

/**
 * admin page to display all contest votes
 * will replace with a view when views integration is complete
 */
function bpoc_contest_show_contest_votes($node) {
  $sub_ids = bpoc_contest_get_submissions($node->nid);
  $header = array('Thumbnail', 'Title', 'Votes');
  $rows = array();
  foreach ($sub_ids as $sub) {
    $subnode = node_load($sub);
    $img = $subnode->field_contest_photo[0]['filepath'];
    $alias = drupal_lookup_path('alias', 'node/' . $subnode->nid); 
    if (!$alias) {
      $alias = 'node/' . $subnode->nid;
    }
    $thumb = '<a href="/' . $alias . '">' . 
             theme('imagecache', 'contest_admin_thumbnail', $img) .
             '</a>'; 
    $title = '<a href="/' . $alias . '">' . $subnode->title . '</a>';
    $rows[] = array($thumb, $title, bpoc_contest_get_submission_vote_count($sub));
  }
  array_sort_by_column($rows, '2');
  $output .= theme('table', $header, $rows);
  return $output;
}
/**
 * admin page to display all votes for a submission
 * will replace with a view when views integration is complete
 */
function bpoc_contest_show_submission_votes($node) {
  $votes = bpoc_contest_get_votes($node->nid);
  $header = array('User', 'Email', 'Date', 'Source');
  $rows = array();
  foreach ($votes as $vote) {
    $rows[] = array(
      theme('username', user_load($vote['uid'])),
      $vote['email_address'],
      date('d-M-Y g:ha', $vote['timestamp']),
      $vote['source'],
    );
  }
  $output = theme('table', $header, $rows);
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
