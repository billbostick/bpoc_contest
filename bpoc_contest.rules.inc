<?php
/**
 * Implementation of hook_rules_event_info().
 * @ingroup rules
 */
function bpoc_contest_rules_event_info() {
  return array(
    'bpoc_contest_vote_cast' => array(
      'label' => t('A vote was cast'),
      'module' => 'bpoc_contest',
      'arguments' => array(
        'voting_user' => array('type' => 'user', 'label' => t('The user who voted.')),
        'content' => array('type' => 'value', 'label' => t('The content voted on.')),
      ),
    ),
  );
}

