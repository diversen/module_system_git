<?php


$_MODULE_MENU = array (
    1 => array(
        'title' => lang::translate('module_system_git_menu_list'),
        'url' => '/module_system_git/index',
    ),

    3 => array(
        'title' => lang::translate('module_system_git_menu_user'),
        'url' => '/module_system_git/user/index',
        'auth' => 'user',
    ),

    5 => array(
        'title' => lang::translate('module_system_git_menu_admin'),
        'url' => '/module_system_git/admin/index',
        'auth' => 'admin',
    ),

);