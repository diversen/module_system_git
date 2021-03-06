<?php

use diversen\pagination as pearPager;
moduleloader::includeModule('module_system_git');

$num_rows = moduleSystemGit::getNumRows($user = true);

$pager = new pearPager($num_rows);

viewModuleSystemGit::moduleFilterOption();
$rows = moduleSystemGit::getAllUserModules($pager->from);
viewModuleSystemGit::viewModules($rows);


$pager->pearPage();
