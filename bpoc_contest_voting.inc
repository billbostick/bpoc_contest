<?php

/*
 * @file bpoc_contest_voting.inc - implements voting api functions for bpoc_contest module
 */

function bpoc_contest_vote($nid, $email='') {
  global $user;
  db_query("INSERT INTO {bpoc_contest_votes} (uid, nid, vote_value, email_address, timestamp, source) VALUES (%d, %d, %d, '%s', %d, '%s')", $user->uid, $nid, 1, htmlentities($email), time(), ip_address());
  watchdog('bpoc_contest', 'Vote by @user (@email - @ipaddress) on node @nid.', array('@user' => $user->name, '@email' => $email, '@ipaddress' => ip_address(), '@nid' => $nid));
  return;
}


