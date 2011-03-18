<?php

include_module ('module_system_git');

if (!session::isAdmin()){
    return;
}

print_r($_SERVER);
moduleSystemGit::showModule();

$redirect = $_SERVER['HTTP_REFERER'];
header ("Location: $redirect");