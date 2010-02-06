// $Id$

/**
 * Bind on dashboard divs.
 */
$(document).ready(
  function () {
	$('a.closeEl').bind('click', toggleContent);
	
	$('div.groupWrapper').Sortable( {
	  accept: 'groupItem',
	  helperclass: 'sortHelper',
	  activeclass : 	'sortableactive',
	  hoverclass : 	'sortablehover',
	  handle: 'div.itemHeader',
	  tolerance: 'pointer',
	  
	  onChange : function(ser) {
	  },
	  
	  onStart : function() {
	    $.iAutoscroller.start(this, document.getElementsByTagName('body'));
	  },
	  
	  onStop : function() {
	    $.iAutoscroller.stop();		
	  }
	});
  }
);

var toggleContent = function(e) {
  var targetContent = $('div.itemContent', this.parentNode.parentNode);
  if (targetContent.css('display') == 'none') {
	targetContent.slideDown(300);
	$(this).html('[-]');
  } 
  else {
	targetContent.slideUp(300);
	$(this).html('[+]');
  }
  return false;
};

function saveLayoutSettings() {
  var settings = $.SortSerialize();
  var regions = settings.o;
  var build_mode = $('#edit-build-mode').val();
  for (region in regions) {
	var region_value = region.replace('ds-db-region-', '');
	var fields = regions[region];
	for (var i = 0; i < fields.length; i++) {
	  var field_replace = fields[i].replace("_", "-"); 
	  var weight_id = 'edit-'+ field_replace +'-ds-weight';
	  var region_id = 'edit-'+ field_replace +'-'+ build_mode +'-region';
	  $('#'+ weight_id).val(i);
  	  $('#'+ region_id).val(region_value);
    }
  }
}
