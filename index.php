<?php

use diversen\pagination as pearPager;

$filters = config::getModuleIni('module_system_git_filters');
moduleloader::includeFilters($filters);
$num_rows = moduleSystemGit::getNumRows();
$pager = new pearPager($num_rows);
viewModuleSystemGit::moduleFilterOption();

$rows = moduleSystemGit::getAllModules($pager->from);
viewModuleSystemGit::viewModules($rows);

$pager->pearPage();
