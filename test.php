<?php


simple_prg();

// a post form
function form_test (){ ?>
<form method ="post" action = "/module_system_git/test">
<input type ="text" name="test" value="<?=@$_POST['test']?>" /> <br />
<input type ="submit" name="submit" value="Send!" />
</form>
<?php }

// call post form
form_test();
