<?php

/** 
 * credits: 
 * example found on the following site:
 * 
 * http://www.nodstrum.com/2007/09/19/autocompleter/
 */

$db = new db();

if(isset($_POST['queryString'])) {
    $queryString = cos_htmlentities($_POST['queryString']);
    if(strlen($queryString) > 2) {
        $query = "SELECT id, title FROM module_system_git WHERE title LIKE ". db::$dbh->quote("%" . $queryString . "%");
        $rows = $db->selectQuery($query);
        if($rows) {
            foreach ($rows as $key => $val ) {
                echo "<a href=\"/module_system_git/more/$val[id]\"><li onClick=\"fill(\''.$val[title].'\');\">$val[title]</li></a>";
            }
        }
    }
}

die;