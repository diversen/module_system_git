<?php

use diversen\valid as cosValidate; 

include_once _COS_PATH . "/modules/module_system_git/views.php";
moduleloader::includeTemplateCommon('jquery-markedit');
jquery_markedit_load_assets();

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

   /**
    * method for creating a form for insert, update and deleting entries
    * in module_system module
    *
    *
    * @param string    method (update, delete or insert)
    * @param int       id (if delete or update)
    */
    public static function Form($method, $id = null, $values = array(), $caption = null){
        
        $form = new HTML();
        
        if ($method == 'delete') {
            $form->formStart('module_system_git');
            $form->legend(lang::translate('Delete Module'));
            $form->label('submit', '');
            $form->submit('submit', lang::system('submit')); 
            $form->formEnd();
            echo $form->getStr();
            return;
        }
        if ($method == 'update') {
            $db = new db();
            $values = $db->selectOne('module_system_git', 'id', $id);
            $form->init($values, 'submit');
            $legend = lang::translate('Edit Module');
        }
        
        if ($method =='insert') {
            $form->init(array(), 'submit');
            $legend = lang::translate('Add Module');
        }
        
        $form->formStart('module_system_git');
        $form->legend($legend);
        $form->label('title', lang::translate('module_system_git_form_title'));
        $form->text('title');
    
        $form->label('abstract', lang::translate('module_system_git_form_abstract'));
        $form->textareaMed('abstract', null, array ('class' => 'markdown'));
        //$form->textareaMed($name, $value, $extra)
        
        $form->label('url', lang::translate('module_system_git_form_url'));
        $form->text('url');
        
        $form->label('clone_url', lang::translate('module_system_git_form_clone_url'));
        $form->text('clone_url');
        
       
        $db = new db();
        $rows = $db->selectAll('module_system_git_type');

        $form->label('type', lang::translate('module_system_git_form_type'));
        $form->select('type', $rows, 'title', 'id', null, null, null);
        
        $form->label('submit', '');
        $form->submit('submit', lang::system('submit'));
        
        $form->formEnd();
        echo $form->getStr();
    }


    public static function sanitize (){
        $_POST = html::specialEncode($_POST);

    }

    public static function validate ($method = 'insert') {
        if (!empty($_POST)){
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
                //self::$errors[] = lang::translate('module_system_git_error_url');
                if (!cosValidate::url($_POST['url'])){
                    self::$errors[] = lang::translate('module_system_git_error_url');
                }
            }


            if (empty($_POST['clone_url'])){
                self::$errors[] = lang::translate('module_system_git_error_clone_url');
            } else {


                $row = $db->selectOne('module_system_git', 'clone_url', $_POST['clone_url']);

                if ($row && $method == 'insert'){
                    self::$errors[] = lang::translate('module_system_git_error_clone_url_exists');
                }

                if ($row && $method == 'update'){
                    if ($row['id'] != self::getUserModuleId()){
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
        if (session::isAdmin()){
            $values['published'] = 1;
        }
        return $db->insert('module_system_git', $values);
    }

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
        $res = $db->update('module_system_git', $values, self::getUserModuleId());
        return $res;
    }

    /**
     * method for deleting a module
     * (access control is cheched in controller file)
     * @return boolean true on success and false on failure
     */
    public function deleteModule () {
        $db = new db();
        $res = $db->delete('module_system_git', 'id', self::getUserModuleId());
        return $res;
    }

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

    /**
     * method for getting all modules
     *
     * @param   int from
     * @param   int limit
     * @return  array   with latest modules rows
     */
    public static function getAllModules($from = 0, $limit = 10){
        $selected = self::getModuleFilter();

        $search = array ();
        if (!session::isAdmin()){
            $search['published'] = 1;
        }
        if ($selected){
            $search['type'] = $selected;
        } 
        $db = new db();
        $rows = $db->selectAll('module_system_git', null, $search, $from, $limit, 'title', true);
        return $rows;
    }

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
            $where = array ( 'account_id' => session::getUserId());
        }
        $db = new db();
        $rows = $db->selectAll('module_system_git', null, $where, $from, $limit, 'title', true);
        return $rows;
    }


    public static function getAllAdminModules($from = 0, $limit = 10){
        $selected = self::getModuleFilter();
        if ($selected){
            $where = array ('type' => $selected);
        } else {
            $where = array ();
        }
        $db = new db();
        $where['published'] = 0;
        $rows = $db->selectAll('module_system_git', null, $where, $from, $limit, 'created', true);
        return $rows;
    }

    public static function getNumRows ($user = false, $admin = false){
        $db = new db();

        $type = self::getModuleFilter();
        //$search =  array('type' => $type);

        if ($admin){
            $search = array ('published' => 0);
            return $db->getNumRows('module_system_git', $search);
        }

        if (!$user){
            if ($type == 0){
                $search = array ('published' => 1);
                return $db->getNumRows('module_system_git', $search);
            } else {
                
                $search = array ('published' => 1, 'type' => $type);
                return $db->getNumRows('module_system_git', $search);
            }
        } else {
            // users personal modules - no need to be published
            $account_id = session::getUserId();
            if ($type == 0 ){
                return $db->getNumRows('module_system_git', array ('account_id' => $account_id));
            } else {
                return $db->getNumRows('module_system_git', array('type' => $type, 'account_id' => $account_id));
            }
        }
    }

    public static function hideModule (){
        $id = uri::$fragments[3];
        $db = new db();
        $values = array ('published' => 0);
        return $db->update('module_system_git', $values, $id);
    }

    public static function showModule (){
        echo $id = uri::$fragments[3];
        $db = new db();
        $values = array ('published' => 1);
        return $db->update('module_system_git', $values, $id);
    }

    /**
     * method for checking if a module belongs to logged in user or not
     *
     * @return boolean true on success and false on failure
     */
    public static function checkModuleOwner (){
        // checks that user owns the module in question.
        $search = array(
            'account_id' => session::getUserId(),
            'id' => self::getUserModuleId(),
        );
        $db = new db();
        $rows = $db->select('module_system_git', null, $search);
        if (empty($rows)) {
            return false;
        } else {
            return true;
        }
    }

    public static function getModule (){
        $id = moduleSystemGit::getModuleId();


        $db = new db();
        $val = $db->selectOne('module_system_git', 'id', $id);
        if (empty($val)){
            session::setActionMessage(lang::translate('module_system_moved_permanently'));
            header ("HTTP/1.1 301 Moved Permanently");
            header ("Location: /module_system_git/index");
        }

        $filters = config::getModuleIni('module_system_git_filters');

        $val['abstract'] = html::specialDecode($val['abstract']);
        $val['abstract'] = moduleloader::getFilteredContent($filters, $val['abstract']);

        return $val;

    }
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
