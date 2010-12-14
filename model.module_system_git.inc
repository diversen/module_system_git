<?php

include_once "mySqlForm.php";
include_once "pearPager.php";

class moduleSystemGit {
    public static $errors = null;

    public static function getModuleId (){
        $uri = uri::getInstance();
        return (int)$uri->fragment(2);
    }

    public static function getUserModuleId (){
        $uri = uri::getInstance();
        return (int)$uri->fragment(3);
    }



     // {{{ viewModuleForm($method, $id = null, $values = array(), $caption = null)
   /**
    * method for creating a form for insert, update and deleting entries
    * in module_system module
    *
    *
    * @param string    method (update, delete or insert)
    * @param int       id (if delete or update)
    */
    public static function Form($method, $id = null, $values = array(), $caption = null){
        $fields_captions =
            array(
                'title' => lang::translate('module_system_git_form_title'),
                'abstract' => lang::translate('module_system_git_form_abstract'),
                'url' => lang::translate('module_system_git_form_url'),
                'clone_url' => lang::translate('module_system_git_form_clone_url'),
                'type' => lang::translate('module_system_git_form_type'),

        );
        
        $fields = array('id', 'title', 'abstract', 'url','clone_url', 'type');
        $tf = new mySqlForm('module_system_git', $fields, $values);

        $tf->setToMany('type', 'module_system_git_type', 'id', 'title', 'dropdown');
        $tf->setLabels($fields_captions);

        if (isset($id)){
            if ($method == 'delete'){
                if (empty($caption)) $caption = lang::translate('Delete Module');
            } else {
                // no title if updating
                $fields = array('id', 'abstract', 'url', 'clone_url', 'type');
                $tf = new mySqlForm('module_system_git', $fields, $values);

                $tf->setToMany('type', 'module_system_git_type', 'id', 'title', 'dropdown');
                $tf->setLabels($fields_captions);
                if (empty($caption)){
                    $caption = lang::translate('Edit Module');
                }
            }
            $tf->setMethod($method, $id);

        } else {
            $tf->setMethod($method);
            if (empty($caption)) $caption = lang::translate('Add Module');
        }
        $tf->createForm('', 'post',  'module_system_git', '', 'multipart/form-data',  $caption);
    }
    // }}}

    public static function sanitize (){
        $_POST = get_filtered_content('safe', $_POST);
        
    }

    public static function validate ($method = 'insert') {
        if (!empty($_POST['submit'])){
            $db = new db();
            if ($method == 'insert'){
                if (empty($_POST['title'])){
                    self::$errors[] = lang::translate('module_system_git_error_title');
                }

                
                $row = $db->selectOne('module_system_git', 'title', $_POST['title']);

                if ($row){
                    self::$errors[] = lang::translate('module_system_git_error_title_exists');
                }
            }

            if (empty($_POST['abstract'])){
                self::$errors[] = lang::translate('module_system_git_error_abstract');
            }



            if (empty($_POST['url'])){
                self::$errors[] = lang::translate('module_system_git_error_url');
            }

            $row = $db->selectOne('module_system_git', 'url', $_POST['url']);
            if ($row){
                if ($method == 'insert'){
                    self::$errors[] = lang::translate('module_system_git_error_url_exists');
                }
                if ($method == 'update'){
                    if ($row['id'] != self::getModuleId()){
                        self::$errors[] = lang::translate('module_system_git_error_url_exists');
                    }
                }
            }

            if (empty($_POST['clone_url'])){
                self::$errors[] = lang::translate('module_system_git_error_clone_url');
            }

            $row = $db->selectOne('module_system_git', 'clone_url', $_POST['clone_url']);
            if ($row){
                 if ($method == 'insert'){
                    self::$errors[] = lang::translate('module_system_git_error_clone_url_exists');
                }
                if ($method == 'update'){
                    if ($row['id'] != self::getModuleId()){
                        self::$errors[] = lang::translate('module_system_git_error_clone_url_exists');
                    }
                }
                
            }

            if (!empty($_POST['clone_url'])){
                $tag = latest_tag($_POST['clone_url']);
                if (!$tag){
                    self::$errors[] = lang::translate('module_system_git_error_clone_url_no_tag');
                } else {
                    if (!isfloat($tag)){
                        self::$errors[] = lang::translate('module_system_git_error_clone_url_tag_not_float');
                    }
                }
            }
        }
    }

    public static function insert(){
        $db = new db();
        $_POST['account_id'] = session::getUserId();
        $values = db::prepareToPost();
        return $db->insert('module_system_git', $values);
    }

    // {{{ updateModule()
    /**
     * method for updating a module in database
     * (access control is cheched in controller file)
     *
     * @return boolean true on success or false on failure
     */
    public function updateModule () {
        $values = db::prepareToPost();
        $values['account_id'] = session::getUserId();
        
        $db = new db();
        $res = $db->update('module_system_git', $values, self::getModuleId());
        return $res;
    }
    // }}}
    // {{{ deleteModule()
    /**
     * method for deleting a module
     * (access control is cheched in controller file)
     * @return boolean true on success and false on failure
     */
    public function deleteModule () {
        $db = new db();
        $res = $db->delete('module_system_git', 'id', self::getModuleId());
        return $res;
    }
    // }}}

    // {{{ getUserModules()
    /**
     * method for fetching modules belonging to a user
     * @return array assoc rows of modules belonging to user
     */
    public function getUserModules(){
        $id = session::getUserId();
        $db = new db();
        $rows = $db->select('module_system_git', 'account_id', $id);
        return $rows;
    }

    // }}}
    
    // {{{ getModuleFilter
    /**
     * @return  int  category_id (module type) for filtereing modules.
     */
    public static function getModuleFilter (){
        $selected = 0;
        if (isset($_GET['module_system_filter'])){
            $_GET['module_system_filter'] = filter_input(
                INPUT_GET,
                "module_system_filter",
                FILTER_SANITIZE_NUMBER_INT
            );
            $selected = $_SESSION['module_system_filter'] = $_GET['module_system_filter'];
        }
        if (isset($_SESSION['module_system_filter'])){
            $selected = $_SESSION['module_system_filter'];
        }
        return $selected;
    }
    // }}}
    // {{{ getAllModules($from = 0, $limit = 10)
    /**
     * method for getting all modules
     *
     * @param   int from
     * @param   int limit
     * @return  array   with latest modules rows
     */
    public static function getAllModules($from = 0, $limit = 10){
        $selected = self::getModuleFilter();
        if ($selected){
            $where = array ('type' => $selected);
        } else {
            $where = null;
        }
        $db = new db();
        $rows = $db->selectAll('module_system_git', null, $where, $from, $limit, 'title', true);
        return $rows;
    }
    // }}}
        // {{{ getAllModules($from = 0, $limit = 10)
    /**
     * method for getting all modules
     *
     * @param   int from
     * @param   int limit
     * @return  array   with latest modules rows
     */
    public static function getAllUserModules($from = 0, $limit = 10){
        $selected = self::getModuleFilter();
        if ($selected){
            $where = array ('type' => $selected, 'account_id' => session::getUserId());
        } else {
            $where = null;
        }
        $db = new db();
        $rows = $db->selectAll('module_system_git', null, $where, $from, $limit, 'title', true);
        return $rows;
    }
    // }}}
    public static function getNumRows ($user = false){
        $db = new db();

        
        $type = self::getModuleFilter();
        if (!$user){
            if ($type == 0){
                return $db->getNumRows('module_system_git');
            } else {
                return $db->getNumRows('module_system_git', array('type' => $type));
            }
        } else {
        
            $account_id = session::getUserId();
            if ($type == 0 ){
                return $db->getNumRows('module_system_git', array ('account_id' => $account_id));
            } else {
                return $db->getNumRows('module_system_git', array('type' => $type, 'account_id' => $account_id));
            }
        }
    }

    // {{{ checkModuleOwner
    /**
     * method for checking if a module belongs to logged in user or not
     *
     * @return boolean true on success and false on failure
     */
    public function checkModuleOwner (){
        // checks that user owns the module in question.
        $search = array(
            'account_id' => session::getUserId(),
            'id' => self::getModuleId(),
        );
        $db = new db();
        $rows = $db->select('module_system_git', null, $search);
        if (empty($rows)) {
            return false;
        } else {
            return true;
        }
    }
    // }}}
}


/**
 * following two functions are sligtly modified from:
 * https://github.com/troelskn/pearhub
 *
 * @param   string  a git url url
 * @return  array   array of remote tags
 */
function get_tags($url = null) {
    static $tags;
    if ($tags == null) {
        $tags = array();
        $output = array();
        $ret = 0;

        $command = "git ls-remote --tags " . escapeshellarg($url);
        exec($command.' 2>&1', $output, $ret);

        foreach ($output as $line) {
            trim($line);
            if (preg_match('~^[0-9a-f]{40}\s+refs/tags/(([a-zA-Z_-]+)?([0-9]+)(\.([0-9]+))?(\.([0-9]+))?([A-Za-z]+[0-9A-Za-z-]*)?)$~', $line, $reg)) {
                $tags[] = $reg[1];
            }
        }
    }
    return $tags;


}

function latest_tag($url = null) {
    $tags = get_tags($url);
    if (count($tags) > 0) {
        sort($tags);
        return $tags[count($tags) - 1];
    }
}

// found in comments about is_float on php.net
function isfloat($f) {
    return ($f == (string)(float)$f);
}