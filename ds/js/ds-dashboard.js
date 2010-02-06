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
	serial = $.SortSerialize();
	alert(serial.hash);
};

