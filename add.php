<?php



if (!empty($_POST['insert'])){
    moduleSystemGit::sanitize();
    moduleSystemGit::validate();
    if (moduleSystemGit::$errors == null){
        moduleSystemGit::insert();
    } else {
        html::errors(moduleSystemGit::$errors);
        moduleSystemGit::Form('insert');
    }
} else {
    moduleSystemGit::Form('insert');
}