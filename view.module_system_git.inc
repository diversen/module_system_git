<?php

class viewModuleSystemGit {

    public static function moduleFilterOption($selected = null, $action = null){
        // remember the filtering
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

        if (!$action){
            $action = "/module_system_git/index";
            //$action = '';
        }

        $str = '';
        $str.= '<div id="module_system_search">';
        $str.= "<form method=\"get\" action=\"$action\">";
        $extras = array();
        $extras[0] = array('id' => '0', 'title' => lang::translate('Show All'));
        $str.= view_drop_down_db(
            'module_system_filter',
            'module_system_git_type', 'title', 'id', $selected, $extras,
            'onChange="this.form.submit()"');
        $str.= '<input type="submit" name="submit_form" value="' . lang::translate('Search') . '" />';
        $str.= '</form>';
        $str.= '</div>';
        //return $str;
        echo $str;
        self::suggestBox();
    }

    public static function viewModules ($rows){
        foreach ($rows as $val){
            
            // title
            echo "<hr />\n";
            echo "<h3>";
            echo html::createLink("/module_system_git/more/$val[id]", $val['title']);
            echo "</h3>\n";

            // abstract - more link
            echo "<p>\n";
            
            
            $str = strings::substr2 ($val['abstract'], 150);
            echo get_filtered_content(
                    config::getModuleIni('module_system_git_filters'), 
                    $str
            );
            echo html::createLink("/module_system_git/more/$val[id]", lang::translate('module_system_git_read_more'));
            echo "</p>\n";
            
            // install info
            self::installInfo($val);

            // admin options
            if ( ($val['account_id'] == session::getUserId()) || session::isAdmin()){
                self::adminLinks($val);
            }
        }        
    }

    public static function installInfo(&$val){
        echo "<p>\n";
        echo lang::translate('module_system_git_install') . " :: ";
        echo "<pre>\n";
        if ($val['type'] == '1'){
            echo "./coscli.sh git --mod-in $val[clone_url]";
        } else if ($val['type'] == '2') {
            echo "./coscli.sh git --temp-in $val[clone_url]";
        } else if ($val['type'] == '3') {
            echo "./coscli.sh git --pro-in $val[clone_url]";
        }
        echo "</pre>\n";
        echo "Install master:";
        echo "<pre>\n";
        if ($val['type'] == '1'){
            echo "./coscli.sh git --mod-in --master $val[clone_url]";
        } else if ($val['type'] == '2') {
            echo "./coscli.sh git --temp-in --master $val[clone_url]";
        } else if ($val['type'] == '3') {
            echo "./coscli.sh git --pro-in --master $val[clone_url]";
        }
        echo "</pre>\n";
        
        echo "</p>\n";
    }

    public static function adminLinks(&$val){
        echo "<p>\n";
        echo html::createLink("/module_system_git/user/edit/$val[id]", lang::translate('module_system_git_edit'));
        echo MENU_SUB_SEPARATOR;
        echo html::createLink("/module_system_git/user/delete/$val[id]", lang::translate('module_system_git_delete'));

        if (session::isAdmin() && ($val['published'] == 1) ){
            echo MENU_SUB_SEPARATOR;
            echo html::createLink("/module_system_git/admin/hide/$val[id]", lang::translate('module_system_git_hide'));
            //echo html::createLink("/module_system_git/admin/delete/$val[id]", lang::translate('module_system_git_delete'));
        } else {
            echo MENU_SUB_SEPARATOR;
            echo html::createLink("/module_system_git/admin/show/$val[id]", lang::translate('module_system_git_show'));

        }

        echo "<p/>\n";
    }

    public static function viewMore (){
        $val = moduleSystemGit::getModule();
        
        
        // title
        echo "<h3>";
        echo $val['title'] . " ";
        echo "</h3>\n";

        // abstract - more link
        echo "<p>\n";
        echo ($val['abstract']);
        echo "</p>\n";
        echo "<p>\n";
        echo lang::translate('module_system_git_visit_website') . " :: ";


        echo html::createLink($val['url'], $val['url']);
        echo "</p>\n";

        // install info
        self::installInfo($val);

        // admin options
        if ( ($val['account_id'] == session::getUserId()) || session::isAdmin()){
            self::adminLinks($val);
        }
    }

    public static function suggestBox (){ ?>

<br />Quick lookup:
<script type="text/javascript">
$(function() {

            $("#auto").autocomplete({
                source: "/module_system_git/rpc",
                minLength: 0,
                select: function(event, ui) {
                    var redirect = ui.item.id;
                    location.href = "/module_system_git/more/" +redirect;
                }
            });
        });

</script>
<input type="text" id="auto" />
<?
    }
}