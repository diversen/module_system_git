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