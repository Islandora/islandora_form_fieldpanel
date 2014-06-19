<?php

/**
 * @file
 *
 *  Allows forms to dynamically add new fieldpanes to a fieldpanel in a form.
 */
module_load_include('inc', 'php_lib', 'DrupalFormHelpers');

/**
 * A collection of static functions that allow for theming and processing fieldpanes.
 */
class FieldPane {
  const DeleteButton = 'delete-fieldpane';
  const MoveUpButton = 'move-up-fieldpane';
  const MoveDownButton = 'move-down-fieldpane';

  /**
   * FieldPane's theme hook.
   *
   * @param array $element
   * @return string
   */
  public static function Theme($element) {
    $children = isset($element['#children']) ? $element['#children'] : '';
    $description = isset($element['#description']) ? "<div class='description'>{$element['#description']}</div>" : '';
    return "<div id='{$element['#hash']}' class='islandora-form-fieldpanel-pane'>{$description}{$children}</div>";
  }

  /**
   * The default #process function for fieldpane.
   *
   * Adds elements that allow for adding/remove form elements.
   *
   * @param array $element
   * @param mixed $edit
   * @param array $form_state
   * @param array $complete_form
   * @return array
   */
  public static function Process(array $element, array &$form_state, array $complete_form = NULL) {
    $panel = get_form_element_parent($element, $complete_form);
    $children = element_children($panel);
    $delete = isset($element['#user_data']['delete']) ? $element['#user_data']['delete'] : TRUE; // Defaults to TRUE
    if (count($children) > 2 && $delete) {
      $delete_label = isset($element['#user_data']['delete_label']) ? $element['#user_data']['delete_label'] : t('Delete');
      $element[self::DeleteButton] = self::createRemoveButton($element, $complete_form, $delete_label);
    }
    $weight = $element['#weight'] * 1000;
    $number_children = count($children) - 3;
    if ($weight > 0) {
      $element[self::MoveUpButton] = self::createMoveUpButton($element, $complete_form, t('Move Up'));
    }
    if ($weight < $number_children) {
      $element[self::MoveDownButton] = self::createMoveDownButton($element, $complete_form, t('Move Down'));
    }
    return $element;
  }

  /**
   * Creates a remove button that allows the user to remove this fieldpane.
   *
   * @param FormElement $fieldpane
   * @return FormElement
   */
  private static function createRemoveButton(array &$element, array & $complete_form, $label) {
    $tabs = get_form_element_parent($element, $complete_form);
    $button['#type'] = 'button';
    $button['#weight'] = 10;
    $button['#size'] = 30;
    $button['#id'] = $button['#name'] = $element['#hash'] . '-remove';
    $button['#value'] = $label;
    $button['#prefix'] = '<div class="ui-fieldpane-delete-button">';
    $button['#suffix'] = '</div>';
    $button['#limit_validation_errors'][] = $element['#parents'];
    $button['#ajax'] = array(
      'callback' => 'xml_form_elements_ajax_callback',
      'params' => array(
        'target' => $tabs['#hash'],
        'render' => $tabs['#hash'],
        'action' => 'delete',
        'child' => $element['#hash']
      ),
      'wrapper' => $tabs['#hash'], // The parents wrapper is set to the parents hash.
      'method' => 'replaceWith',
      'effect' => 'fade',
    );
    return $button;
  }

  /**
   * Creates a move up button that allows the user to move this element up by one.
   *
   * @param FormElement $fieldpane
   * @return FormElement
   */
  private static function createMoveUpButton(array &$element, array & $complete_form, $label) {
    $tabs = get_form_element_parent($element, $complete_form);
    $button['#type'] = 'button';
    $button['#weight'] = 4;
    $button['#size'] = 30;
    $button['#id'] = $button['#name'] = $element['#hash'] . '-move_up';
    $button['#attributes']['data-position'] = $element['#weight'] * 1000;
    $button['#value'] = $label;
    $button['#prefix'] = '<div class="ui-fieldpane-move-up-button">';
    $button['#suffix'] = '</div>';
    $button['#limit_validation_errors'][] = $element['#parents'];
    return $button;
  }

  /**
   * Creates a move up button that allows the user to move this element down by one.
   *
   * @param FormElement $fieldpane
   * @return FormElement
   */
  private static function createMoveDownButton(array &$element, array & $complete_form, $label) {
    $tabs = get_form_element_parent($element, $complete_form);
    $button['#type'] = 'button';
    $button['#weight'] = 5;
    $button['#size'] = 30;
    $button['#id'] = $button['#name'] = $element['#hash'] . '-move_down';
    $button['#attributes']['data-position'] = $element['#weight'] * 1000;
    $button['#value'] = $label;
    $button['#prefix'] = '<div class="ui-fieldpane-move-down-button">';
    $button['#suffix'] = '</div>';
    $button['#limit_validation_errors'][] = $element['#parents'];
    return $button;
  }
}

/**
 * Theme hook for a fieldpane
 */
function theme_fieldpane($vars) {
  return FieldPane::Theme($vars['element']);
}
