<?php

class viewModuleSystemGit {

    public static function moduleFilterOption($selected = null){
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

        $str = '';
        $str.= '<div id="module_system_search">';
        $str.= '<form method="get" action="">';
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
        print $str;
    }

    public static function viewModules ($rows){
        foreach ($rows as $key => $val){
            print "<hr />\n";
            print "<h3>";
            echo $val['title'] . " ";

            echo "</h3>\n";
            
            if ($val['account_id'] == session::getUserId()){
                echo create_link("/module_system_git/user/edit/$val[id]", lang::translate('module_system_git_edit')) . " | ";
                echo create_link("/module_system_git/user/delete/$val[id]", lang::translate('module_system_git_delete'));
                echo "<br />\n";
            }

            print substr2 ($val['abstract'], 150);
                        echo create_link("/module_system_git/more/$val[id]", lang::translate('module_system_git_read_more'));
            print "<br />\n";
            print lang::translate('module_system_git_install') . " :: ";
            print "<br />\n";

            if ($val['type'] == '1'){
                print "./coscli.sh git --install $val[clone_url]";
            } else if ($val['type'] == '2') {
                print "./coscli.sh git --install-template $val[clone_url]";
            } else if ($val['type'] == '3') {
                print "./coscli.sh git --install-profile $val[clone_url]";
            }
        }        
    }

    public static function viewMore (){

    }
}