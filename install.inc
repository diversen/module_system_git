<?php

/**
 * installer file for modules module
 *
 * @package    modules
 */
$_INSTALL['MAIN_MENU_ITEM'] = array (
    'title' => lang::translate('Modules', null, array ('no_translate' => true)),
    'url' => '/module_system_git/index',
    'module_name' => 'module_system_git',
    'auth' => '',
    'parent' => 0,
    'weight' => 7,
);

$_INSTALL['VERSION'] = 3.01;
$_INSTALL['PUBLIC_CLONE_URL'] = 'git://github.com/diversen/module_system_git.git';
$_INSTALL['PRIVATE_CLONE_URL'] = 'git@github.com:diversen/module_system_git.git';

