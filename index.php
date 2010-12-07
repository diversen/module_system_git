<?php

viewModuleSystemGit::moduleFilterOption();
$rows = moduleSystemGit::getAllModules();
viewModuleSystemGit::viewModules($rows);
