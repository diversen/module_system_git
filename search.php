<script type="text/javascript">
    // example found on:
    // http://www.nodstrum.com/2007/09/19/autocompleter/
	function lookup(inputString) {
		if(inputString.length == 0) {
			// Hide the suggestion box.
			$('#suggestions').hide();
		} else {
			$.post("/module_system_git/rpc", {queryString: ""+inputString+""}, function(data){
				if(data.length > 0) {
					$('#suggestions').show();
					$('#autoSuggestionsList').html(data);
				}
			});
		}
	} // lookup
	
	function fill(thisValue) {
		$('#inputString').val(thisValue);
		setTimeout("$('#suggestions').hide();", 200);
	}
</script>

<style type="text/css">


	.suggestionsBox {
		position: relative;
		left: 0px;
		margin: 10px 0px 0px 0px;
		width: 200px;

		-moz-border-radius: 7px;
		-webkit-border-radius: 7px;
		border: 2px solid #000;	

	}
	
	.suggestionList {
		margin: 0px;
		padding: 0px;
	}
	
	.suggestionList li {
		
		margin: 0px 0px 3px 0px;
		padding: 3px;
		cursor: pointer;
                list-style:none;
	}
	
	.suggestionList li:hover {
		background-color: #659CD8;
	}
</style>




	<div>
		<form>
			<div>
				Type your county:
				<br />
				<input type="text" size="30" value="" id="inputString" onkeyup="lookup(this.value);" onblur="fill();" />
			</div>

			
			<div class="suggestionsBox" id="suggestions" style="display: none;">

				<div class="suggestionList" id="autoSuggestionsList">
					&nbsp;
				</div>
			</div>
		</form>
	</div>


