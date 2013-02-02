<?php

moduleloader::includeModule ('module_system_git');

if (!session::isAdmin()){
    return;
}

print_r($_SERVER);
moduleSystemGit::hideModule();
$redirect = $_SERVER['HTTP_REFERER'];
header ("Location: $redirect");
