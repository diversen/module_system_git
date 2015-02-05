<?php

moduleloader::includeModule('module_system_git');

if (!session::isUser()){
    return;
}

if (!empty($_POST['submit'])){
    moduleSystemGit::sanitize();
    moduleSystemGit::validate();
    if (moduleSystemGit::$errors == null){
        $res = moduleSystemGit::insert();
        if ($res){
            session::setActionMessage(
                lang::translate('module_system_git_module_created')
            );
            header("Location: /module_system_git/user/index");
        }
    } else {
        html::errors(moduleSystemGit::$errors);
        moduleSystemGit::Form('insert');
    }
} else {
    moduleSystemGit::Form('insert');
}

