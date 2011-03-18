<?php

include_module('module_system_git');

$num_rows = moduleSystemGit::getNumRows($user = false, $admin = true);

$pager = new pearPager($num_rows);

viewModuleSystemGit::moduleFilterOption(null, "/module_system_git/admin/index");
$rows = moduleSystemGit::getAllAdminModules($pager->from);
viewModuleSystemGit::viewModules($rows);

$pager->pearPage();

