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
        $str.= '<form method="get" action="/module_system_git/index">';
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
        foreach ($rows as $key => $val){
            
            // title
            echo "<hr />\n";
            echo "<h3>";
            echo create_link("/module_system_git/more/$val[id]", $val['title']);
            echo "</h3>\n";

            // abstract - more link
            echo "<p>\n";
            print substr2 ($val['abstract'], 150);

            echo create_link("/module_system_git/more/$val[id]", lang::translate('module_system_git_read_more'));
            echo "</p>\n";
            
            // install info
            self::installInfo($val);

            // admin options
            if ($val['account_id'] == session::getUserId()){
                self::adminLinks($val['id']);
            }
        }        
    }

    public static function installInfo(&$val){
        print "<p>\n";
        print lang::translate('module_system_git_install') . " :: ";
        print "<pre>\n";
        if ($val['type'] == '1'){
            print "./coscli.sh git --mod-in $val[clone_url]";
        } else if ($val['type'] == '2') {
            print "./coscli.sh git --temp-in $val[clone_url]";
        } else if ($val['type'] == '3') {
            print "./coscli.sh git --pro-in $val[clone_url]";
        }
        print "</pre>\n";
        print "</p>\n";
    }

    public static function adminLinks($id){
        echo "<p>\n";
        echo create_link("/module_system_git/user/edit/$id", lang::translate('module_system_git_edit')) . " | ";
        echo create_link("/module_system_git/user/delete/$id", lang::translate('module_system_git_delete'));
        echo "<p/>\n";
    }

    public static function viewMore (){
        $val = moduleSystemGit::getModule();
        
        
        // title
        print "<h3>";
        echo $val['title'] . " ";
        echo "</h3>\n";

        // abstract - more link
        echo "<p>\n";
        print ($val['abstract']);
        print "</p>\n";
        print "<p>\n";
        print lang::translate('module_system_git_visit_website') . " :: ";


        echo create_link($val['url'], $val['url']);
        echo "</p>\n";

        // install info
        self::installInfo($val);

        // admin options
        if ($val['account_id'] == session::getUserId()){
            self::adminLinks($val['id']);
        }
    }

    public static function suggestBox (){ ?>

<br />Quick lookup (min 3 chars):
<script type="text/javascript">
$(function() {

            $("#auto").autocomplete({
                source: "/module_system_git/rpc",
                minLength: 2,
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