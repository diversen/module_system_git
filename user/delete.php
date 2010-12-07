<?php

/**
 * controller for module_system/delete
 *
 * @package    module_system
 */
include_module ("module_system_git");
template::setTitle(lang::translate('module_system_git_delete_module'));

if (!session::isUser()){
    moduleLoader::$status['403'] = 1;
    return;
}

// checks that user owns module
if (!moduleSystemGit::checkModuleOwner()){
    moduleLoader::$status['403'] = 1;
    return;
}

if (isset($_POST['submit'])){
    $res = moduleSystemGit::deleteModule();
    if ($res){
        session::setActionMessage(
            lang::translate('module_system_git_module_deleted')
        );
        header("Location: /module_system_git/user/index");
    }
}

moduleSystemGit::Form('delete', moduleSystemGit::getModuleId());