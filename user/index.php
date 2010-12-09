<?php

include_module('module_system_git');

$num_rows = moduleSystemGit::getNumRows('user');

$pager = new pearPager($num_rows);

viewModuleSystemGit::moduleFilterOption();
$rows = moduleSystemGit::getAllUserModules($pager->from);
viewModuleSystemGit::viewModules($rows);


$pager->pearPage();
