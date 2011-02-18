<?php

include_model('module_system_git');

if (!moduleSystemGit::checkModuleOwner()){
    if ( !session::isAdmin()){
        moduleLoader::$status[403] = 1;
        return;
    }
}

if (!empty($_POST)){
    moduleSystemGit::sanitize();
    moduleSystemGit::validate('update');
    if (moduleSystemGit::$errors == null){
        $res = moduleSystemGit::updateModule();
        if ($res){
            //$insertId = self::$dbh->lastInsertId();
            session::setActionMessage(
                lang::translate('module_system_git_module_updated')
            );
            header("Location: /module_system_git/user/index");
        }
    } else {
        view_form_errors(moduleSystemGit::$errors);
        moduleSystemGit::Form('update', moduleSystemGit::getUserModuleId());
    }
} else {
    moduleSystemGit::Form('update', moduleSystemGit::getUserModuleId());
}