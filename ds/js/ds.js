// $Id: ds.js,v 1.1.2.7 2010/01/29 14:22:40 swentel Exp $

/**
 * Drupal ds object.
 */
Drupal.DisplaySuite = Drupal.DisplaySuite || {};

/**
 * Show / hide fields or plugins content.
 */
Drupal.DisplaySuite.toggleDisplayTab = function(element) {
  $('#ds-tabs .tab').each(function() {
    var tab_id = $(this).attr('id');
    var content_id = tab_id.replace('-tab', '-content');
	if (tab_id == element) {
	  // Tabs.
      $(this).addClass('selected');
      $(this).removeClass('not-selected');
      // Content.
      $('#'+ content_id).show();
    }
    else {
      // Tabs.
      $(this).addClass('not-selected');
      $(this).removeClass('selected');
      // Content.
      $('#'+ content_id).hide();
    }
  });	
}
 
/**
 * Change the label of a field instance in a build mode.
 */
Drupal.DisplaySuite.changeLabel = function(element, title) {
 
  var changed = prompt(Drupal.t("Edit label"), title);
   
  if (changed == '') {
    alert(Drupal.t('Field can not be empty'));
    return false;
  }
   
  var labelcell = $(element).parents(".ds-label");
  labelcell.find(".label-field").text(changed);
  labelcell.find("input").val(changed);
}
