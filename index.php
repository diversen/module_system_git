<?php

include_once "pearPager.php";
viewModuleSystemGit::moduleFilterOption();
$rows = moduleSystemGit::getAllModules();
viewModuleSystemGit::viewModules($rows);
$num_rows = moduleSystemGit::getNumRows();

$pager = new pearPager($num_rows);
$pager->pearPage();
