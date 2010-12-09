<?php

$num_rows = moduleSystemGit::getNumRows();
$pager = new pearPager($num_rows);
viewModuleSystemGit::moduleFilterOption();

$rows = moduleSystemGit::getAllModules($pager->from);
viewModuleSystemGit::viewModules($rows);

$pager->pearPage();
