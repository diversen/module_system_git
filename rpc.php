<?php

/** 
 * credits: 
 * example found on the following site:
 * 
 * http://www.nodstrum.com/2007/09/19/autocompleter/
 */

$db = new db();

if(isset($_GET['term']) && ( strlen($_GET['term']) >= 1)) {
    $queryString = html::specialEncode($_GET['term']);


    //if(strlen($queryString) >= 0) {
        $filter = moduleSystemGit::getModuleFilter();

        if ($filter == 0){
            $query = "SELECT id, title as value FROM module_system_git WHERE title LIKE ". db::$dbh->quote("" . $queryString . "%");
        } else {
            $query = "SELECT id, title as value FROM module_system_git WHERE type = " . db::$dbh->quote($filter). " AND title LIKE ". db::$dbh->quote("" . $queryString . "%");
        }
        $query.= " AND published = 1";
        
        $rows = $db->selectQuery($query);
        $json = json_encode($rows);
        echo $json;
    //}
}

die;
