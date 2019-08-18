/*
    NanoGUI was developed by Wenzel Jakob <wenzel.jakob@epfl.ch>.
    The widget drawing code is based on the NanoVG demo application
    by Mikko Mononen.

    All rights reserved. Use of this source code is governed by a
    BSD-style license that can be found in the LICENSE.txt file.
*/

/* Developer note: need to make a change to this file?
 * Please raise an Issue on GitHub describing what needs to change.  This file
 * was generated, so the scripts that generated it needs to update as well.
 */

/**
 * \file nanogui/icons.h
 *
 * \brief This is a list of icon codes for the free variant of the
 * FontAwesome 5.10.1 font.
 *
 * \rst
 *
 * This file defines the full listing of
 * `FontAwesome <https://raw.githubusercontent.com/FortAwesome/Font-Awesome>`_
 * icons available in NanoGUI.  Please note that if viewing the documentation
 * on the web, your browser may display the icons differently than what they
 * look like in NanoGUI.  Run the one of the :ref:`nanogui_example_icons`
 * executables to see what they all look like in NanoGUI.
 *
 * .. tip::
 *
 *    In C++, ``#include <nanogui/icons.h>`` to gain access to the ``#define``
 *    shown in these docs and simply reference the names as listed in the
 *    documentation.  To use the icons from Python, declare
 *    ``from nanogui import icons`` and reference the icons as follows:
 *    ``icons.FA_ASTERISK``.
 *
 * The following icons are available:
 *
 * .. raw:: html
 *
 *    <center>
 *      <div class="wy-table-responsive">
 *        <table class="docutils" border=1>
 *          <colgroup>
 *            <col width="90%" align="center"/>
 *            <col width="10%" align="center" />
 *          </colgroup>
 *          <thead valign="bottom">
 *            <tr class="row-odd">
 *              <th class="head" align="center">Definition</th>
 *              <th class="head" align="center">Icon</th>
 *            </tr>
 *          </thead>
 *          <tbody valign="top">
 *            <tr class="row-even">
 *              <td align="right"><code>FA_AD</code></td>
 *              <td align="center"><span class="fa-ad"></span></td>
 *            </tr>
 *            <tr class="row-odd">
 *              <td align="right"><code>FA_ADDRESS_BOOK</code></td>
 *              <td align="center"><span class="fa-address-book"></span></td>
 *            </tr>
 *            <tr class="row-even">
 *              <td align="right"><code>FA_ADDRESS_CARD</code></td>
 *              <td align="center"><span class="fa-address-card"></span></td>
 *            </tr>
 *            <tr class="row-odd">
 *              <td align="right"><code>FA_ADJUST</code></td>
 *              <td align="center"><span class="fa-adjust"></span></td>
 *            </tr>
 *            <tr class="row-even">
 *              <td align="right"><code>FA_AIR_FRESHENER</code></td>
 *              <td align="center"><span class="fa-air-freshener"></span></td>
 *            </tr>
 *            <tr class="row-odd">
 *              <td align="right"><code>FA_ALIGN_CENTER</code></td>
 *              <td align="center"><span class="fa-align-center"></span></td>
 *            </tr>
 *            <tr class="row-even">
 *              <td align="right"><code>FA_ALIGN_JUSTIFY</code></td>
 *              <td align="center"><span class="fa-align-justify"></span></td>
 *            </tr>
 *            <tr class="row-odd">
 *              <td align="right"><code>FA_ALIGN_LEFT</code></td>
 *              <td align="center"><span class="fa-align-left"></span></td>
 *            </tr>
 *            <tr class="row-even">
 *              <td align="right"><code>FA_ALIGN_RIGHT</code></td>
 *              <td align="center"><span class="fa-align-right"></span></td>
 *            </tr>
 *            <tr class="row-odd">
 *              <td align="right"><code>FA_ALLERGIES</code></td>
 *              <td align="center"><span class="fa-allergies"></span></td>
 *            </tr>
 *            <tr class="row-even">
 *              <td align="right"><code>FA_AMBULANCE</code></td>
 *              <td align="center"><span class="fa-ambulance"></span></td>
 *            </tr>
 *            <tr class="row-odd">
 *              <td align="right"><code>FA_AMERICAN_SIGN_LANGUAGE_INTERPRETING</code></td>
 *              <td align="center"><span class="fa-american-sign-language-interpreting"></span></td>
 *            </tr>
 *            <tr class="row-even">
 *              <td align="right"><code>FA_ANCHOR</code></td>
 *              <td align="center"><span class="fa-anchor"></span></td>
 *            </tr>
 *            <tr class="row-odd">
 *              <td align="right"><code>FA_ANGLE_DOUBLE_DOWN</code></td>
 *              <td align="center"><span class="fa-angle-double-down"></span></td>
 *            </tr>
 *            <tr class="row-even">
 *              <td align="right"><code>FA_ANGLE_DOUBLE_LEFT</code></td>
 *              <td align="center"><span class="fa-angle-double-left"></span></td>
 *            </tr>
 *            <tr class="row-odd">
 *              <td align="right"><code>FA_ANGLE_DOUBLE_RIGHT</code></td>
 *              <td align="center"><span class="fa-angle-double-right"></span></td>
 *            </tr>
 *            <tr class="row-even">
 *              <td align="right"><code>FA_ANGLE_DOUBLE_UP</code></td>
 *              <td align="center"><span class="fa-angle-double-up"></span></td>
 *            </tr>
 *            <tr class="row-odd">
 *              <td align="right"><code>FA_ANGLE_DOWN</code></td>
 *              <td align="center"><span class="fa-angle-down"></span></td>
 *            </tr>
 *            <tr class="row-even">
 *              <td align="right"><code>FA_ANGLE_LEFT</code></td>
 *              <td align="center"><span class="fa-angle-left"></span></td>
 *            </tr>
 *            <tr class="row-odd">
 *              <td align="right"><code>FA_ANGLE_RIGHT</code></td>
 *              <td align="center"><span class="fa-angle-right"></span></td>
 *            </tr>
 *            <tr class="row-even">
 *              <td align="right"><code>FA_ANGLE_UP</code></td>
 *              <td align="center"><span class="fa-angle-up"></span></td>
 *            </tr>
 *            <tr class="row-odd">
 *              <td align="right"><code>FA_ANGRY</code></td>
 *              <td align="center"><span class="fa-angry"></span></td>
 *            </tr>
 *            <tr class="row-even">
 *              <td align="right"><code>FA_ANKH</code></td>
 *              <td align="center"><span class="fa-ankh"></span></td>
 *            </tr>
 *            <tr class="row-odd">
 *              <td align="right"><code>FA_APPLE_ALT</code></td>
 *              <td align="center"><span class="fa-apple-alt"></span></td>
 *            </tr>
 *            <tr class="row-even">
 *              <td align="right"><code>FA_ARCHIVE</code></td>
 *              <td align="center"><span class="fa-archive"></span></td>
 *            </tr>
 *            <tr class="row-odd">
 *              <td align="right"><code>FA_ARCHWAY</code></td>
 *              <td align="center"><span class="fa-archway"></span></td>
 *            </tr>
 *            <tr class="row-even">
 *              <td align="right"><code>FA_ARROW_ALT_CIRCLE_DOWN</code></td>
 *              <td align="center"><span class="fa-arrow-alt-circle-down"></span></td>
 *            </tr>
 *            <tr class="row-odd">
 *              <td align="right"><code>FA_ARROW_ALT_CIRCLE_LEFT</code></td>
 *              <td align="center"><span class="fa-arrow-alt-circle-left"></span></td>
 *            </tr>
 *            <tr class="row-even">
 *              <td align="right"><code>FA_ARROW_ALT_CIRCLE_RIGHT</code></td>
 *              <td align="center"><span class="fa-arrow-alt-circle-right"></span></td>
 *            </tr>
 *            <tr class="row-odd">
 *              <td align="right"><code>FA_ARROW_ALT_CIRCLE_UP</code></td>
 *              <td align="center"><span class="fa-arrow-alt-circle-up"></span></td>
 *            </tr>
 *            <tr class="row-even">
 *              <td align="right"><code>FA_ARROW_CIRCLE_DOWN</code></td>
 *              <td align="center"><span class="fa-arrow-circle-down"></span></td>
 *            </tr>
 *            <tr class="row-odd">
 *              <td align="right"><code>FA_ARROW_CIRCLE_LEFT</code></td>
 *              <td align="center"><span class="fa-arrow-circle-left"></span></td>
 *            </tr>
 *            <tr class="row-even">
 *              <td align="right"><code>FA_ARROW_CIRCLE_RIGHT</code></td>
 *              <td align="center"><span class="fa-arrow-circle-right"></span></td>
 *            </tr>
 *            <tr class="row-odd">
 *              <td align="right"><code>FA_ARROW_CIRCLE_UP</code></td>
 *              <td align="center"><span class="fa-arrow-circle-up"></span></td>
 *            </tr>
 *            <tr class="row-even">
 *              <td align="right"><code>FA_ARROW_DOWN</code></td>
 *              <td align="center"><span class="fa-arrow-down"></span></td>
 *            </tr>
 *            <tr class="row-odd">
 *              <td align="right"><code>FA_ARROW_LEFT</code></td>
 *              <td align="center"><span class="fa-arrow-left"></span></td>
 *            </tr>
 *            <tr class="row-even">
 *              <td align="right"><code>FA_ARROW_RIGHT</code></td>
 *              <td align="center"><span class="fa-arrow-right"></span></td>
 *            </tr>
 *            <tr class="row-odd">
 *              <td align="right"><code>FA_ARROW_UP</code></td>
 *              <td align="center"><span class="fa-arrow-up"></span></td>
 *            </tr>
 *            <tr class="row-even">
 *              <td align="right"><code>FA_ARROWS_ALT</code></td>
 *              <td align="center"><span class="fa-arrows-alt"></span></td>
 *            </tr>
 *            <tr class="row-odd">
 *              <td align="right"><code>FA_ARROWS_ALT_H</code></td>
 *              <td align="center"><span class="fa-arrows-alt-h"></span></td>
 *            </tr>
 *            <tr class="row-even">
 *              <td align="right"><code>FA_ARROWS_ALT_V</code></td>
 *              <td align="center"><span class="fa-arrows-alt-v"></span></td>
 *            </tr>
 *            <tr class="row-odd">
 *              <td align="right"><code>FA_ASSISTIVE_LISTENING_SYSTEMS</code></td>
 *              <td align="center"><span class="fa-assistive-listening-systems"></span></td>
 *            </tr>
 *            <tr class="row-even">
 *              <td align="right"><code>FA_ASTERISK</code></td>
 *              <td align="center"><span class="fa-asterisk"></span></td>
 *            </tr>
 *            <tr class="row-odd">
 *              <td align="right"><code>FA_AT</code></td>
 *              <td align="center"><span class="fa-at"></span></td>
 *            </tr>
 *            <tr class="row-even">
 *              <td align="right"><code>FA_ATLAS</code></td>
 *              <td align="center"><span class="fa-atlas"></span></td>
 *            </tr>
 *            <tr class="row-odd">
 *              <td align="right"><code>FA_ATOM</code></td>
 *              <td align="center"><span class="fa-atom"></span></td>
 *            </tr>
 *            <tr class="row-even">
 *              <td align="right"><code>FA_AUDIO_DESCRIPTION</code></td>
 *              <td align="center"><span class="fa-audio-description"></span></td>
 *            </tr>
 *            <tr class="row-odd">
 *              <td align="right"><code>FA_AWARD</code></td>
 *              <td align="center"><span class="fa-award"></span></td>
 *            </tr>
 *            <tr class="row-even">
 *              <td align="right"><code>FA_BABY</code></td>
 *              <td align="center"><span class="fa-baby"></span></td>
 *            </tr>
 *            <tr class="row-odd">
 *              <td align="right"><code>FA_BABY_CARRIAGE</code></td>
 *              <td align="center"><span class="fa-baby-carriage"></span></td>
 *            </tr>
 *            <tr class="row-even">
 *              <td align="right"><code>FA_BACKSPACE</code></td>
 *              <td align="center"><span class="fa-backspace"></span></td>
 *            </tr>
 *            <tr class="row-odd">
 *              <td align="right"><code>FA_BACKWARD</code></td>
 *              <td align="center"><span class="fa-backward"></span></td>
 *            </tr>
 *            <tr class="row-even">
 *              <td align="right"><code>FA_BACON</code></td>
 *              <td align="center"><span class="fa-bacon"></span></td>
 *            </tr>
 *            <tr class="row-odd">
 *              <td align="right"><code>FA_BALANCE_SCALE</code></td>
 *              <td align="center"><span class="fa-balance-scale"></span></td>
 *            </tr>
 *            <tr class="row-even">
 *              <td align="right"><code>FA_BALANCE_SCALE_LEFT</code></td>
 *              <td align="center"><span class="fa-balance-scale-left"></span></td>
 *            </tr>
 *            <tr class="row-odd">
 *              <td align="right"><code>FA_BALANCE_SCALE_RIGHT</code></td>
 *              <td align="center"><span class="fa-balance-scale-right"></span></td>
 *            </tr>
 *            <tr class="row-even">
 *              <td align="right"><code>FA_BAN</code></td>
 *              <td align="center"><span class="fa-ban"></span></td>
 *            </tr>
 *            <tr class="row-odd">
 *              <td align="right"><code>FA_BAND_AID</code></td>
 *              <td align="center"><span class="fa-band-aid"></span></td>
 *            </tr>
 *            <tr class="row-even">
 *              <td align="right"><code>FA_BARCODE</code></td>
 *              <td align="center"><span class="fa-barcode"></span></td>
 *            </tr>
 *            <tr class="row-odd">
 *              <td align="right"><code>FA_BARS</code></td>
 *              <td align="center"><span class="fa-bars"></span></td>
 *            </tr>
 *            <tr class="row-even">
 *              <td align="right"><code>FA_BASEBALL_BALL</code></td>
 *              <td align="center"><span class="fa-baseball-ball"></span></td>
 *            </tr>
 *            <tr class="row-odd">
 *              <td align="right"><code>FA_BASKETBALL_BALL</code></td>
 *              <td align="center"><span class="fa-basketball-ball"></span></td>
 *            </tr>
 *            <tr class="row-even">
 *              <td align="right"><code>FA_BATH</code></td>
 *              <td align="center"><span class="fa-bath"></span></td>
 *            </tr>
 *            <tr class="row-odd">
 *              <td align="right"><code>FA_BATTERY_EMPTY</code></td>
 *              <td align="center"><span class="fa-battery-empty"></span></td>
 *            </tr>
 *            <tr class="row-even">
 *              <td align="right"><code>FA_BATTERY_FULL</code></td>
 *              <td align="center"><span class="fa-battery-full"></span></td>
 *            </tr>
 *            <tr class="row-odd">
 *              <td align="right"><code>FA_BATTERY_HALF</code></td>
 *              <td align="center"><span class="fa-battery-half"></span></td>
 *            </tr>
 *            <tr class="row-even">
 *              <td align="right"><code>FA_BATTERY_QUARTER</code></td>
 *              <td align="center"><span class="fa-battery-quarter"></span></td>
 *            </tr>
 *            <tr class="row-odd">
 *              <td align="right"><code>FA_BATTERY_THREE_QUARTERS</code></td>
 *              <td align="center"><span class="fa-battery-three-quarters"></span></td>
 *            </tr>
 *            <tr class="row-even">
 *              <td align="right"><code>FA_BED</code></td>
 *              <td align="center"><span class="fa-bed"></span></td>
 *            </tr>
 *            <tr class="row-odd">
 *              <td align="right"><code>FA_BEER</code></td>
 *              <td align="center"><span class="fa-beer"></span></td>
 *            </tr>
 *            <tr class="row-even">
 *              <td align="right"><code>FA_BELL</code></td>
 *              <td align="center"><span class="fa-bell"></span></td>
 *            </tr>
 *            <tr class="row-odd">
 *              <td align="right"><code>FA_BELL_SLASH</code></td>
 *              <td align="center"><span class="fa-bell-slash"></span></td>
 *            </tr>
 *            <tr class="row-even">
 *              <td align="right"><code>FA_BEZIER_CURVE</code></td>
 *              <td align="center"><span class="fa-bezier-curve"></span></td>
 *            </tr>
 *            <tr class="row-odd">
 *              <td align="right"><code>FA_BIBLE</code></td>
 *              <td align="center"><span class="fa-bible"></span></td>
 *            </tr>
 *            <tr class="row-even">
 *              <td align="right"><code>FA_BICYCLE</code></td>
 *              <td align="center"><span class="fa-bicycle"></span></td>
 *            </tr>
 *            <tr class="row-odd">
 *              <td align="right"><code>FA_BIKING</code></td>
 *              <td align="center"><span class="fa-biking"></span></td>
 *            </tr>
 *            <tr class="row-even">
 *              <td align="right"><code>FA_BINOCULARS</code></td>
 *              <td align="center"><span class="fa-binoculars"></span></td>
 *            </tr>
 *            <tr class="row-odd">
 *              <td align="right"><code>FA_BIOHAZARD</code></td>
 *              <td align="center"><span class="fa-biohazard"></span></td>
 *            </tr>
 *            <tr class="row-even">
 *              <td align="right"><code>FA_BIRTHDAY_CAKE</code></td>
 *              <td align="center"><span class="fa-birthday-cake"></span></td>
 *            </tr>
 *            <tr class="row-odd">
 *              <td align="right"><code>FA_BLENDER</code></td>
 *              <td align="center"><span class="fa-blender"></span></td>
 *            </tr>
 *            <tr class="row-even">
 *              <td align="right"><code>FA_BLENDER_PHONE</code></td>
 *              <td align="center"><span class="fa-blender-phone"></span></td>
 *            </tr>
 *            <tr class="row-odd">
 *              <td align="right"><code>FA_BLIND</code></td>
 *              <td align="center"><span class="fa-blind"></span></td>
 *            </tr>
 *            <tr class="row-even">
 *              <td align="right"><code>FA_BLOG</code></td>
 *              <td align="center"><span class="fa-blog"></span></td>
 *            </tr>
 *            <tr class="row-odd">
 *              <td align="right"><code>FA_BOLD</code></td>
 *              <td align="center"><span class="fa-bold"></span></td>
 *            </tr>
 *            <tr class="row-even">
 *              <td align="right"><code>FA_BOLT</code></td>
 *              <td align="center"><span class="fa-bolt"></span></td>
 *            </tr>
 *            <tr class="row-odd">
 *              <td align="right"><code>FA_BOMB</code></td>
 *              <td align="center"><span class="fa-bomb"></span></td>
 *            </tr>
 *            <tr class="row-even">
 *              <td align="right"><code>FA_BONE</code></td>
 *              <td align="center"><span class="fa-bone"></span></td>
 *            </tr>
 *            <tr class="row-odd">
 *              <td align="right"><code>FA_BONG</code></td>
 *              <td align="center"><span class="fa-bong"></span></td>
 *            </tr>
 *            <tr class="row-even">
 *              <td align="right"><code>FA_BOOK</code></td>
 *              <td align="center"><span class="fa-book"></span></td>
 *            </tr>
 *            <tr class="row-odd">
 *              <td align="right"><code>FA_BOOK_DEAD</code></td>
 *              <td align="center"><span class="fa-book-dead"></span></td>
 *            </tr>
 *            <tr class="row-even">
 *              <td align="right"><code>FA_BOOK_MEDICAL</code></td>
 *              <td align="center"><span class="fa-book-medical"></span></td>
 *            </tr>
 *            <tr class="row-odd">
 *              <td align="right"><code>FA_BOOK_OPEN</code></td>
 *              <td align="center"><span class="fa-book-open"></span></td>
 *            </tr>
 *            <tr class="row-even">
 *              <td align="right"><code>FA_BOOK_READER</code></td>
 *              <td align="center"><span class="fa-book-reader"></span></td>
 *            </tr>
 *            <tr class="row-odd">
 *              <td align="right"><code>FA_BOOKMARK</code></td>
 *              <td align="center"><span class="fa-bookmark"></span></td>
 *            </tr>
 *            <tr class="row-even">
 *              <td align="right"><code>FA_BORDER_ALL</code></td>
 *              <td align="center"><span class="fa-border-all"></span></td>
 *            </tr>
 *            <tr class="row-odd">
 *              <td align="right"><code>FA_BORDER_NONE</code></td>
 *              <td align="center"><span class="fa-border-none"></span></td>
 *            </tr>
 *            <tr class="row-even">
 *              <td align="right"><code>FA_BORDER_STYLE</code></td>
 *              <td align="center"><span class="fa-border-style"></span></td>
 *            </tr>
 *            <tr class="row-odd">
 *              <td align="right"><code>FA_BOWLING_BALL</code></td>
 *              <td align="center"><span class="fa-bowling-ball"></span></td>
 *            </tr>
 *            <tr class="row-even">
 *              <td align="right"><code>FA_BOX</code></td>
 *              <td align="center"><span class="fa-box"></span></td>
 *            </tr>
 *            <tr class="row-odd">
 *              <td align="right"><code>FA_BOX_OPEN</code></td>
 *              <td align="center"><span class="fa-box-open"></span></td>
 *            </tr>
 *            <tr class="row-even">
 *              <td align="right"><code>FA_BOXES</code></td>
 *              <td align="center"><span class="fa-boxes"></span></td>
 *            </tr>
 *            <tr class="row-odd">
 *              <td align="right"><code>FA_BRAILLE</code></td>
 *              <td align="center"><span class="fa-braille"></span></td>
 *            </tr>
 *            <tr class="row-even">
 *              <td align="right"><code>FA_BRAIN</code></td>
 *              <td align="center"><span class="fa-brain"></span></td>
 *            </tr>
 *            <tr class="row-odd">
 *              <td align="right"><code>FA_BREAD_SLICE</code></td>
 *              <td align="center"><span class="fa-bread-slice"></span></td>
 *            </tr>
 *            <tr class="row-even">
 *              <td align="right"><code>FA_BRIEFCASE</code></td>
 *              <td align="center"><span class="fa-briefcase"></span></td>
 *            </tr>
 *            <tr class="row-odd">
 *              <td align="right"><code>FA_BRIEFCASE_MEDICAL</code></td>
 *              <td align="center"><span class="fa-briefcase-medical"></span></td>
 *            </tr>
 *            <tr class="row-even">
 *              <td align="right"><code>FA_BROADCAST_TOWER</code></td>
 *              <td align="center"><span class="fa-broadcast-tower"></span></td>
 *            </tr>
 *            <tr class="row-odd">
 *              <td align="right"><code>FA_BROOM</code></td>
 *              <td align="center"><span class="fa-broom"></span></td>
 *            </tr>
 *            <tr class="row-even">
 *              <td align="right"><code>FA_BRUSH</code></td>
 *              <td align="center"><span class="fa-brush"></span></td>
 *            </tr>
 *            <tr class="row-odd">
 *              <td align="right"><code>FA_BUG</code></td>
 *              <td align="center"><span class="fa-bug"></span></td>
 *            </tr>
 *            <tr class="row-even">
 *              <td align="right"><code>FA_BUILDING</code></td>
 *              <td align="center"><span class="fa-building"></span></td>
 *            </tr>
 *            <tr class="row-odd">
 *              <td align="right"><code>FA_BULLHORN</code></td>
 *              <td align="center"><span class="fa-bullhorn"></span></td>
 *            </tr>
 *            <tr class="row-even">
 *              <td align="right"><code>FA_BULLSEYE</code></td>
 *              <td align="center"><span class="fa-bullseye"></span></td>
 *            </tr>
 *            <tr class="row-odd">
 *              <td align="right"><code>FA_BURN</code></td>
 *              <td align="center"><span class="fa-burn"></span></td>
 *            </tr>
 *            <tr class="row-even">
 *              <td align="right"><code>FA_BUS</code></td>
 *              <td align="center"><span class="fa-bus"></span></td>
 *            </tr>
 *            <tr class="row-odd">
 *              <td align="right"><code>FA_BUS_ALT</code></td>
 *              <td align="center"><span class="fa-bus-alt"></span></td>
 *            </tr>
 *            <tr class="row-even">
 *              <td align="right"><code>FA_BUSINESS_TIME</code></td>
 *              <td align="center"><span class="fa-business-time"></span></td>
 *            </tr>
 *            <tr class="row-odd">
 *              <td align="right"><code>FA_CALCULATOR</code></td>
 *              <td align="center"><span class="fa-calculator"></span></td>
 *            </tr>
 *            <tr class="row-even">
 *              <td align="right"><code>FA_CALENDAR</code></td>
 *              <td align="center"><span class="fa-calendar"></span></td>
 *            </tr>
 *            <tr class="row-odd">
 *              <td align="right"><code>FA_CALENDAR_ALT</code></td>
 *              <td align="center"><span class="fa-calendar-alt"></span></td>
 *            </tr>
 *            <tr class="row-even">
 *              <td align="right"><code>FA_CALENDAR_CHECK</code></td>
 *              <td align="center"><span class="fa-calendar-check"></span></td>
 *            </tr>
 *            <tr class="row-odd">
 *              <td align="right"><code>FA_CALENDAR_DAY</code></td>
 *              <td align="center"><span class="fa-calendar-day"></span></td>
 *            </tr>
 *            <tr class="row-even">
 *              <td align="right"><code>FA_CALENDAR_MINUS</code></td>
 *              <td align="center"><span class="fa-calendar-minus"></span></td>
 *            </tr>
 *            <tr class="row-odd">
 *              <td align="right"><code>FA_CALENDAR_PLUS</code></td>
 *              <td align="center"><span class="fa-calendar-plus"></span></td>
 *            </tr>
 *            <tr class="row-even">
 *              <td align="right"><code>FA_CALENDAR_TIMES</code></td>
 *              <td align="center"><span class="fa-calendar-times"></span></td>
 *            </tr>
 *            <tr class="row-odd">
 *              <td align="right"><code>FA_CALENDAR_WEEK</code></td>
 *              <td align="center"><span class="fa-calendar-week"></span></td>
 *            </tr>
 *            <tr class="row-even">
 *              <td align="right"><code>FA_CAMERA</code></td>
 *              <td align="center"><span class="fa-camera"></span></td>
 *            </tr>
 *            <tr class="row-odd">
 *              <td align="right"><code>FA_CAMERA_RETRO</code></td>
 *              <td align="center"><span class="fa-camera-retro"></span></td>
 *            </tr>
 *            <tr class="row-even">
 *              <td align="right"><code>FA_CAMPGROUND</code></td>
 *              <td align="center"><span class="fa-campground"></span></td>
 *            </tr>
 *            <tr class="row-odd">
 *              <td align="right"><code>FA_CANDY_CANE</code></td>
 *              <td align="center"><span class="fa-candy-cane"></span></td>
 *            </tr>
 *            <tr class="row-even">
 *              <td align="right"><code>FA_CANNABIS</code></td>
 *              <td align="center"><span class="fa-cannabis"></span></td>
 *            </tr>
 *            <tr class="row-odd">
 *              <td align="right"><code>FA_CAPSULES</code></td>
 *              <td align="center"><span class="fa-capsules"></span></td>
 *            </tr>
 *            <tr class="row-even">
 *              <td align="right"><code>FA_CAR</code></td>
 *              <td align="center"><span class="fa-car"></span></td>
 *            </tr>
 *            <tr class="row-odd">
 *              <td align="right"><code>FA_CAR_ALT</code></td>
 *              <td align="center"><span class="fa-car-alt"></span></td>
 *            </tr>
 *            <tr class="row-even">
 *              <td align="right"><code>FA_CAR_BATTERY</code></td>
 *              <td align="center"><span class="fa-car-battery"></span></td>
 *            </tr>
 *            <tr class="row-odd">
 *              <td align="right"><code>FA_CAR_CRASH</code></td>
 *              <td align="center"><span class="fa-car-crash"></span></td>
 *            </tr>
 *            <tr class="row-even">
 *              <td align="right"><code>FA_CAR_SIDE</code></td>
 *              <td align="center"><span class="fa-car-side"></span></td>
 *            </tr>
 *            <tr class="row-odd">
 *              <td align="right"><code>FA_CARET_DOWN</code></td>
 *              <td align="center"><span class="fa-caret-down"></span></td>
 *            </tr>
 *            <tr class="row-even">
 *              <td align="right"><code>FA_CARET_LEFT</code></td>
 *              <td align="center"><span class="fa-caret-left"></span></td>
 *            </tr>
 *            <tr class="row-odd">
 *              <td align="right"><code>FA_CARET_RIGHT</code></td>
 *              <td align="center"><span class="fa-caret-right"></span></td>
 *            </tr>
 *            <tr class="row-even">
 *              <td align="right"><code>FA_CARET_SQUARE_DOWN</code></td>
 *              <td align="center"><span class="fa-caret-square-down"></span></td>
 *            </tr>
 *            <tr class="row-odd">
 *              <td align="right"><code>FA_CARET_SQUARE_LEFT</code></td>
 *              <td align="center"><span class="fa-caret-square-left"></span></td>
 *            </tr>
 *            <tr class="row-even">
 *              <td align="right"><code>FA_CARET_SQUARE_RIGHT</code></td>
 *              <td align="center"><span class="fa-caret-square-right"></span></td>
 *            </tr>
 *            <tr class="row-odd">
 *              <td align="right"><code>FA_CARET_SQUARE_UP</code></td>
 *              <td align="center"><span class="fa-caret-square-up"></span></td>
 *            </tr>
 *            <tr class="row-even">
 *              <td align="right"><code>FA_CARET_UP</code></td>
 *              <td align="center"><span class="fa-caret-up"></span></td>
 *            </tr>
 *            <tr class="row-odd">
 *              <td align="right"><code>FA_CARROT</code></td>
 *              <td align="center"><span class="fa-carrot"></span></td>
 *            </tr>
 *            <tr class="row-even">
 *              <td align="right"><code>FA_CART_ARROW_DOWN</code></td>
 *              <td align="center"><span class="fa-cart-arrow-down"></span></td>
 *            </tr>
 *            <tr class="row-odd">
 *              <td align="right"><code>FA_CART_PLUS</code></td>
 *              <td align="center"><span class="fa-cart-plus"></span></td>
 *            </tr>
 *            <tr class="row-even">
 *              <td align="right"><code>FA_CASH_REGISTER</code></td>
 *              <td align="center"><span class="fa-cash-register"></span></td>
 *            </tr>
 *            <tr class="row-odd">
 *              <td align="right"><code>FA_CAT</code></td>
 *              <td align="center"><span class="fa-cat"></span></td>
 *            </tr>
 *            <tr class="row-even">
 *              <td align="right"><code>FA_CERTIFICATE</code></td>
 *              <td align="center"><span class="fa-certificate"></span></td>
 *            </tr>
 *            <tr class="row-odd">
 *              <td align="right"><code>FA_CHAIR</code></td>
 *              <td align="center"><span class="fa-chair"></span></td>
 *            </tr>
 *            <tr class="row-even">
 *              <td align="right"><code>FA_CHALKBOARD</code></td>
 *              <td align="center"><span class="fa-chalkboard"></span></td>
 *            </tr>
 *            <tr class="row-odd">
 *              <td align="right"><code>FA_CHALKBOARD_TEACHER</code></td>
 *              <td align="center"><span class="fa-chalkboard-teacher"></span></td>
 *            </tr>
 *            <tr class="row-even">
 *              <td align="right"><code>FA_CHARGING_STATION</code></td>
 *              <td align="center"><span class="fa-charging-station"></span></td>
 *            </tr>
 *            <tr class="row-odd">
 *              <td align="right"><code>FA_CHART_AREA</code></td>
 *              <td align="center"><span class="fa-chart-area"></span></td>
 *            </tr>
 *            <tr class="row-even">
 *              <td align="right"><code>FA_CHART_BAR</code></td>
 *              <td align="center"><span class="fa-chart-bar"></span></td>
 *            </tr>
 *            <tr class="row-odd">
 *              <td align="right"><code>FA_CHART_LINE</code></td>
 *              <td align="center"><span class="fa-chart-line"></span></td>
 *            </tr>
 *            <tr class="row-even">
 *              <td align="right"><code>FA_CHART_PIE</code></td>
 *              <td align="center"><span class="fa-chart-pie"></span></td>
 *            </tr>
 *            <tr class="row-odd">
 *              <td align="right"><code>FA_CHECK</code></td>
 *              <td align="center"><span class="fa-check"></span></td>
 *            </tr>
 *            <tr class="row-even">
 *              <td align="right"><code>FA_CHECK_CIRCLE</code></td>
 *              <td align="center"><span class="fa-check-circle"></span></td>
 *            </tr>
 *            <tr class="row-odd">
 *              <td align="right"><code>FA_CHECK_DOUBLE</code></td>
 *              <td align="center"><span class="fa-check-double"></span></td>
 *            </tr>
 *            <tr class="row-even">
 *              <td align="right"><code>FA_CHECK_SQUARE</code></td>
 *              <td align="center"><span class="fa-check-square"></span></td>
 *            </tr>
 *            <tr class="row-odd">
 *              <td align="right"><code>FA_CHEESE</code></td>
 *              <td align="center"><span class="fa-cheese"></span></td>
 *            </tr>
 *            <tr class="row-even">
 *              <td align="right"><code>FA_CHESS</code></td>
 *              <td align="center"><span class="fa-chess"></span></td>
 *            </tr>
 *            <tr class="row-odd">
 *              <td align="right"><code>FA_CHESS_BISHOP</code></td>
 *              <td align="center"><span class="fa-chess-bishop"></span></td>
 *            </tr>
 *            <tr class="row-even">
 *              <td align="right"><code>FA_CHESS_BOARD</code></td>
 *              <td align="center"><span class="fa-chess-board"></span></td>
 *            </tr>
 *            <tr class="row-odd">
 *              <td align="right"><code>FA_CHESS_KING</code></td>
 *              <td align="center"><span class="fa-chess-king"></span></td>
 *            </tr>
 *            <tr class="row-even">
 *              <td align="right"><code>FA_CHESS_KNIGHT</code></td>
 *              <td align="center"><span class="fa-chess-knight"></span></td>
 *            </tr>
 *            <tr class="row-odd">
 *              <td align="right"><code>FA_CHESS_PAWN</code></td>
 *              <td align="center"><span class="fa-chess-pawn"></span></td>
 *            </tr>
 *            <tr class="row-even">
 *              <td align="right"><code>FA_CHESS_QUEEN</code></td>
 *              <td align="center"><span class="fa-chess-queen"></span></td>
 *            </tr>
 *            <tr class="row-odd">
 *              <td align="right"><code>FA_CHESS_ROOK</code></td>
 *              <td align="center"><span class="fa-chess-rook"></span></td>
 *            </tr>
 *            <tr class="row-even">
 *              <td align="right"><code>FA_CHEVRON_CIRCLE_DOWN</code></td>
 *              <td align="center"><span class="fa-chevron-circle-down"></span></td>
 *            </tr>
 *            <tr class="row-odd">
 *              <td align="right"><code>FA_CHEVRON_CIRCLE_LEFT</code></td>
 *              <td align="center"><span class="fa-chevron-circle-left"></span></td>
 *            </tr>
 *            <tr class="row-even">
 *              <td align="right"><code>FA_CHEVRON_CIRCLE_RIGHT</code></td>
 *              <td align="center"><span class="fa-chevron-circle-right"></span></td>
 *            </tr>
 *            <tr class="row-odd">
 *              <td align="right"><code>FA_CHEVRON_CIRCLE_UP</code></td>
 *              <td align="center"><span class="fa-chevron-circle-up"></span></td>
 *            </tr>
 *            <tr class="row-even">
 *              <td align="right"><code>FA_CHEVRON_DOWN</code></td>
 *              <td align="center"><span class="fa-chevron-down"></span></td>
 *            </tr>
 *            <tr class="row-odd">
 *              <td align="right"><code>FA_CHEVRON_LEFT</code></td>
 *              <td align="center"><span class="fa-chevron-left"></span></td>
 *            </tr>
 *            <tr class="row-even">
 *              <td align="right"><code>FA_CHEVRON_RIGHT</code></td>
 *              <td align="center"><span class="fa-chevron-right"></span></td>
 *            </tr>
 *            <tr class="row-odd">
 *              <td align="right"><code>FA_CHEVRON_UP</code></td>
 *              <td align="center"><span class="fa-chevron-up"></span></td>
 *            </tr>
 *            <tr class="row-even">
 *              <td align="right"><code>FA_CHILD</code></td>
 *              <td align="center"><span class="fa-child"></span></td>
 *            </tr>
 *            <tr class="row-odd">
 *              <td align="right"><code>FA_CHURCH</code></td>
 *              <td align="center"><span class="fa-church"></span></td>
 *            </tr>
 *            <tr class="row-even">
 *              <td align="right"><code>FA_CIRCLE</code></td>
 *              <td align="center"><span class="fa-circle"></span></td>
 *            </tr>
 *            <tr class="row-odd">
 *              <td align="right"><code>FA_CIRCLE_NOTCH</code></td>
 *              <td align="center"><span class="fa-circle-notch"></span></td>
 *            </tr>
 *            <tr class="row-even">
 *              <td align="right"><code>FA_CITY</code></td>
 *              <td align="center"><span class="fa-city"></span></td>
 *            </tr>
 *            <tr class="row-odd">
 *              <td align="right"><code>FA_CLINIC_MEDICAL</code></td>
 *              <td align="center"><span class="fa-clinic-medical"></span></td>
 *            </tr>
 *            <tr class="row-even">
 *              <td align="right"><code>FA_CLIPBOARD</code></td>
 *              <td align="center"><span class="fa-clipboard"></span></td>
 *            </tr>
 *            <tr class="row-odd">
 *              <td align="right"><code>FA_CLIPBOARD_CHECK</code></td>
 *              <td align="center"><span class="fa-clipboard-check"></span></td>
 *            </tr>
 *            <tr class="row-even">
 *              <td align="right"><code>FA_CLIPBOARD_LIST</code></td>
 *              <td align="center"><span class="fa-clipboard-list"></span></td>
 *            </tr>
 *            <tr class="row-odd">
 *              <td align="right"><code>FA_CLOCK</code></td>
 *              <td align="center"><span class="fa-clock"></span></td>
 *            </tr>
 *            <tr class="row-even">
 *              <td align="right"><code>FA_CLONE</code></td>
 *              <td align="center"><span class="fa-clone"></span></td>
 *            </tr>
 *            <tr class="row-odd">
 *              <td align="right"><code>FA_CLOSED_CAPTIONING</code></td>
 *              <td align="center"><span class="fa-closed-captioning"></span></td>
 *            </tr>
 *            <tr class="row-even">
 *              <td align="right"><code>FA_CLOUD</code></td>
 *              <td align="center"><span class="fa-cloud"></span></td>
 *            </tr>
 *            <tr class="row-odd">
 *              <td align="right"><code>FA_CLOUD_DOWNLOAD_ALT</code></td>
 *              <td align="center"><span class="fa-cloud-download-alt"></span></td>
 *            </tr>
 *            <tr class="row-even">
 *              <td align="right"><code>FA_CLOUD_MEATBALL</code></td>
 *              <td align="center"><span class="fa-cloud-meatball"></span></td>
 *            </tr>
 *            <tr class="row-odd">
 *              <td align="right"><code>FA_CLOUD_MOON</code></td>
 *              <td align="center"><span class="fa-cloud-moon"></span></td>
 *            </tr>
 *            <tr class="row-even">
 *              <td align="right"><code>FA_CLOUD_MOON_RAIN</code></td>
 *              <td align="center"><span class="fa-cloud-moon-rain"></span></td>
 *            </tr>
 *            <tr class="row-odd">
 *              <td align="right"><code>FA_CLOUD_RAIN</code></td>
 *              <td align="center"><span class="fa-cloud-rain"></span></td>
 *            </tr>
 *            <tr class="row-even">
 *              <td align="right"><code>FA_CLOUD_SHOWERS_HEAVY</code></td>
 *              <td align="center"><span class="fa-cloud-showers-heavy"></span></td>
 *            </tr>
 *            <tr class="row-odd">
 *              <td align="right"><code>FA_CLOUD_SUN</code></td>
 *              <td align="center"><span class="fa-cloud-sun"></span></td>
 *            </tr>
 *            <tr class="row-even">
 *              <td align="right"><code>FA_CLOUD_SUN_RAIN</code></td>
 *              <td align="center"><span class="fa-cloud-sun-rain"></span></td>
 *            </tr>
 *            <tr class="row-odd">
 *              <td align="right"><code>FA_CLOUD_UPLOAD_ALT</code></td>
 *              <td align="center"><span class="fa-cloud-upload-alt"></span></td>
 *            </tr>
 *            <tr class="row-even">
 *              <td align="right"><code>FA_COCKTAIL</code></td>
 *              <td align="center"><span class="fa-cocktail"></span></td>
 *            </tr>
 *            <tr class="row-odd">
 *              <td align="right"><code>FA_CODE</code></td>
 *              <td align="center"><span class="fa-code"></span></td>
 *            </tr>
 *            <tr class="row-even">
 *              <td align="right"><code>FA_CODE_BRANCH</code></td>
 *              <td align="center"><span class="fa-code-branch"></span></td>
 *            </tr>
 *            <tr class="row-odd">
 *              <td align="right"><code>FA_COFFEE</code></td>
 *              <td align="center"><span class="fa-coffee"></span></td>
 *            </tr>
 *            <tr class="row-even">
 *              <td align="right"><code>FA_COG</code></td>
 *              <td align="center"><span class="fa-cog"></span></td>
 *            </tr>
 *            <tr class="row-odd">
 *              <td align="right"><code>FA_COGS</code></td>
 *              <td align="center"><span class="fa-cogs"></span></td>
 *            </tr>
 *            <tr class="row-even">
 *              <td align="right"><code>FA_COINS</code></td>
 *              <td align="center"><span class="fa-coins"></span></td>
 *            </tr>
 *            <tr class="row-odd">
 *              <td align="right"><code>FA_COLUMNS</code></td>
 *              <td align="center"><span class="fa-columns"></span></td>
 *            </tr>
 *            <tr class="row-even">
 *              <td align="right"><code>FA_COMMENT</code></td>
 *              <td align="center"><span class="fa-comment"></span></td>
 *            </tr>
 *            <tr class="row-odd">
 *              <td align="right"><code>FA_COMMENT_ALT</code></td>
 *              <td align="center"><span class="fa-comment-alt"></span></td>
 *            </tr>
 *            <tr class="row-even">
 *              <td align="right"><code>FA_COMMENT_DOLLAR</code></td>
 *              <td align="center"><span class="fa-comment-dollar"></span></td>
 *            </tr>
 *            <tr class="row-odd">
 *              <td align="right"><code>FA_COMMENT_DOTS</code></td>
 *              <td align="center"><span class="fa-comment-dots"></span></td>
 *            </tr>
 *            <tr class="row-even">
 *              <td align="right"><code>FA_COMMENT_MEDICAL</code></td>
 *              <td align="center"><span class="fa-comment-medical"></span></td>
 *            </tr>
 *            <tr class="row-odd">
 *              <td align="right"><code>FA_COMMENT_SLASH</code></td>
 *              <td align="center"><span class="fa-comment-slash"></span></td>
 *            </tr>
 *            <tr class="row-even">
 *              <td align="right"><code>FA_COMMENTS</code></td>
 *              <td align="center"><span class="fa-comments"></span></td>
 *            </tr>
 *            <tr class="row-odd">
 *              <td align="right"><code>FA_COMMENTS_DOLLAR</code></td>
 *              <td align="center"><span class="fa-comments-dollar"></span></td>
 *            </tr>
 *            <tr class="row-even">
 *              <td align="right"><code>FA_COMPACT_DISC</code></td>
 *              <td align="center"><span class="fa-compact-disc"></span></td>
 *            </tr>
 *            <tr class="row-odd">
 *              <td align="right"><code>FA_COMPASS</code></td>
 *              <td align="center"><span class="fa-compass"></span></td>
 *            </tr>
 *            <tr class="row-even">
 *              <td align="right"><code>FA_COMPRESS</code></td>
 *              <td align="center"><span class="fa-compress"></span></td>
 *            </tr>
 *            <tr class="row-odd">
 *              <td align="right"><code>FA_COMPRESS_ARROWS_ALT</code></td>
 *              <td align="center"><span class="fa-compress-arrows-alt"></span></td>
 *            </tr>
 *            <tr class="row-even">
 *              <td align="right"><code>FA_CONCIERGE_BELL</code></td>
 *              <td align="center"><span class="fa-concierge-bell"></span></td>
 *            </tr>
 *            <tr class="row-odd">
 *              <td align="right"><code>FA_COOKIE</code></td>
 *              <td align="center"><span class="fa-cookie"></span></td>
 *            </tr>
 *            <tr class="row-even">
 *              <td align="right"><code>FA_COOKIE_BITE</code></td>
 *              <td align="center"><span class="fa-cookie-bite"></span></td>
 *            </tr>
 *            <tr class="row-odd">
 *              <td align="right"><code>FA_COPY</code></td>
 *              <td align="center"><span class="fa-copy"></span></td>
 *            </tr>
 *            <tr class="row-even">
 *              <td align="right"><code>FA_COPYRIGHT</code></td>
 *              <td align="center"><span class="fa-copyright"></span></td>
 *            </tr>
 *            <tr class="row-odd">
 *              <td align="right"><code>FA_COUCH</code></td>
 *              <td align="center"><span class="fa-couch"></span></td>
 *            </tr>
 *            <tr class="row-even">
 *              <td align="right"><code>FA_CREDIT_CARD</code></td>
 *              <td align="center"><span class="fa-credit-card"></span></td>
 *            </tr>
 *            <tr class="row-odd">
 *              <td align="right"><code>FA_CROP</code></td>
 *              <td align="center"><span class="fa-crop"></span></td>
 *            </tr>
 *            <tr class="row-even">
 *              <td align="right"><code>FA_CROP_ALT</code></td>
 *              <td align="center"><span class="fa-crop-alt"></span></td>
 *            </tr>
 *            <tr class="row-odd">
 *              <td align="right"><code>FA_CROSS</code></td>
 *              <td align="center"><span class="fa-cross"></span></td>
 *            </tr>
 *            <tr class="row-even">
 *              <td align="right"><code>FA_CROSSHAIRS</code></td>
 *              <td align="center"><span class="fa-crosshairs"></span></td>
 *            </tr>
 *            <tr class="row-odd">
 *              <td align="right"><code>FA_CROW</code></td>
 *              <td align="center"><span class="fa-crow"></span></td>
 *            </tr>
 *            <tr class="row-even">
 *              <td align="right"><code>FA_CROWN</code></td>
 *              <td align="center"><span class="fa-crown"></span></td>
 *            </tr>
 *            <tr class="row-odd">
 *              <td align="right"><code>FA_CRUTCH</code></td>
 *              <td align="center"><span class="fa-crutch"></span></td>
 *            </tr>
 *            <tr class="row-even">
 *              <td align="right"><code>FA_CUBE</code></td>
 *              <td align="center"><span class="fa-cube"></span></td>
 *            </tr>
 *            <tr class="row-odd">
 *              <td align="right"><code>FA_CUBES</code></td>
 *              <td align="center"><span class="fa-cubes"></span></td>
 *            </tr>
 *            <tr class="row-even">
 *              <td align="right"><code>FA_CUT</code></td>
 *              <td align="center"><span class="fa-cut"></span></td>
 *            </tr>
 *            <tr class="row-odd">
 *              <td align="right"><code>FA_DATABASE</code></td>
 *              <td align="center"><span class="fa-database"></span></td>
 *            </tr>
 *            <tr class="row-even">
 *              <td align="right"><code>FA_DEAF</code></td>
 *              <td align="center"><span class="fa-deaf"></span></td>
 *            </tr>
 *            <tr class="row-odd">
 *              <td align="right"><code>FA_DEMOCRAT</code></td>
 *              <td align="center"><span class="fa-democrat"></span></td>
 *            </tr>
 *            <tr class="row-even">
 *              <td align="right"><code>FA_DESKTOP</code></td>
 *              <td align="center"><span class="fa-desktop"></span></td>
 *            </tr>
 *            <tr class="row-odd">
 *              <td align="right"><code>FA_DHARMACHAKRA</code></td>
 *              <td align="center"><span class="fa-dharmachakra"></span></td>
 *            </tr>
 *            <tr class="row-even">
 *              <td align="right"><code>FA_DIAGNOSES</code></td>
 *              <td align="center"><span class="fa-diagnoses"></span></td>
 *            </tr>
 *            <tr class="row-odd">
 *              <td align="right"><code>FA_DICE</code></td>
 *              <td align="center"><span class="fa-dice"></span></td>
 *            </tr>
 *            <tr class="row-even">
 *              <td align="right"><code>FA_DICE_D20</code></td>
 *              <td align="center"><span class="fa-dice-d20"></span></td>
 *            </tr>
 *            <tr class="row-odd">
 *              <td align="right"><code>FA_DICE_D6</code></td>
 *              <td align="center"><span class="fa-dice-d6"></span></td>
 *            </tr>
 *            <tr class="row-even">
 *              <td align="right"><code>FA_DICE_FIVE</code></td>
 *              <td align="center"><span class="fa-dice-five"></span></td>
 *            </tr>
 *            <tr class="row-odd">
 *              <td align="right"><code>FA_DICE_FOUR</code></td>
 *              <td align="center"><span class="fa-dice-four"></span></td>
 *            </tr>
 *            <tr class="row-even">
 *              <td align="right"><code>FA_DICE_ONE</code></td>
 *              <td align="center"><span class="fa-dice-one"></span></td>
 *            </tr>
 *            <tr class="row-odd">
 *              <td align="right"><code>FA_DICE_SIX</code></td>
 *              <td align="center"><span class="fa-dice-six"></span></td>
 *            </tr>
 *            <tr class="row-even">
 *              <td align="right"><code>FA_DICE_THREE</code></td>
 *              <td align="center"><span class="fa-dice-three"></span></td>
 *            </tr>
 *            <tr class="row-odd">
 *              <td align="right"><code>FA_DICE_TWO</code></td>
 *              <td align="center"><span class="fa-dice-two"></span></td>
 *            </tr>
 *            <tr class="row-even">
 *              <td align="right"><code>FA_DIGITAL_TACHOGRAPH</code></td>
 *              <td align="center"><span class="fa-digital-tachograph"></span></td>
 *            </tr>
 *            <tr class="row-odd">
 *              <td align="right"><code>FA_DIRECTIONS</code></td>
 *              <td align="center"><span class="fa-directions"></span></td>
 *            </tr>
 *            <tr class="row-even">
 *              <td align="right"><code>FA_DIVIDE</code></td>
 *              <td align="center"><span class="fa-divide"></span></td>
 *            </tr>
 *            <tr class="row-odd">
 *              <td align="right"><code>FA_DIZZY</code></td>
 *              <td align="center"><span class="fa-dizzy"></span></td>
 *            </tr>
 *            <tr class="row-even">
 *              <td align="right"><code>FA_DNA</code></td>
 *              <td align="center"><span class="fa-dna"></span></td>
 *            </tr>
 *            <tr class="row-odd">
 *              <td align="right"><code>FA_DOG</code></td>
 *              <td align="center"><span class="fa-dog"></span></td>
 *            </tr>
 *            <tr class="row-even">
 *              <td align="right"><code>FA_DOLLAR_SIGN</code></td>
 *              <td align="center"><span class="fa-dollar-sign"></span></td>
 *            </tr>
 *            <tr class="row-odd">
 *              <td align="right"><code>FA_DOLLY</code></td>
 *              <td align="center"><span class="fa-dolly"></span></td>
 *            </tr>
 *            <tr class="row-even">
 *              <td align="right"><code>FA_DOLLY_FLATBED</code></td>
 *              <td align="center"><span class="fa-dolly-flatbed"></span></td>
 *            </tr>
 *            <tr class="row-odd">
 *              <td align="right"><code>FA_DONATE</code></td>
 *              <td align="center"><span class="fa-donate"></span></td>
 *            </tr>
 *            <tr class="row-even">
 *              <td align="right"><code>FA_DOOR_CLOSED</code></td>
 *              <td align="center"><span class="fa-door-closed"></span></td>
 *            </tr>
 *            <tr class="row-odd">
 *              <td align="right"><code>FA_DOOR_OPEN</code></td>
 *              <td align="center"><span class="fa-door-open"></span></td>
 *            </tr>
 *            <tr class="row-even">
 *              <td align="right"><code>FA_DOT_CIRCLE</code></td>
 *              <td align="center"><span class="fa-dot-circle"></span></td>
 *            </tr>
 *            <tr class="row-odd">
 *              <td align="right"><code>FA_DOVE</code></td>
 *              <td align="center"><span class="fa-dove"></span></td>
 *            </tr>
 *            <tr class="row-even">
 *              <td align="right"><code>FA_DOWNLOAD</code></td>
 *              <td align="center"><span class="fa-download"></span></td>
 *            </tr>
 *            <tr class="row-odd">
 *              <td align="right"><code>FA_DRAFTING_COMPASS</code></td>
 *              <td align="center"><span class="fa-drafting-compass"></span></td>
 *            </tr>
 *            <tr class="row-even">
 *              <td align="right"><code>FA_DRAGON</code></td>
 *              <td align="center"><span class="fa-dragon"></span></td>
 *            </tr>
 *            <tr class="row-odd">
 *              <td align="right"><code>FA_DRAW_POLYGON</code></td>
 *              <td align="center"><span class="fa-draw-polygon"></span></td>
 *            </tr>
 *            <tr class="row-even">
 *              <td align="right"><code>FA_DRUM</code></td>
 *              <td align="center"><span class="fa-drum"></span></td>
 *            </tr>
 *            <tr class="row-odd">
 *              <td align="right"><code>FA_DRUM_STEELPAN</code></td>
 *              <td align="center"><span class="fa-drum-steelpan"></span></td>
 *            </tr>
 *            <tr class="row-even">
 *              <td align="right"><code>FA_DRUMSTICK_BITE</code></td>
 *              <td align="center"><span class="fa-drumstick-bite"></span></td>
 *            </tr>
 *            <tr class="row-odd">
 *              <td align="right"><code>FA_DUMBBELL</code></td>
 *              <td align="center"><span class="fa-dumbbell"></span></td>
 *            </tr>
 *            <tr class="row-even">
 *              <td align="right"><code>FA_DUMPSTER</code></td>
 *              <td align="center"><span class="fa-dumpster"></span></td>
 *            </tr>
 *            <tr class="row-odd">
 *              <td align="right"><code>FA_DUMPSTER_FIRE</code></td>
 *              <td align="center"><span class="fa-dumpster-fire"></span></td>
 *            </tr>
 *            <tr class="row-even">
 *              <td align="right"><code>FA_DUNGEON</code></td>
 *              <td align="center"><span class="fa-dungeon"></span></td>
 *            </tr>
 *            <tr class="row-odd">
 *              <td align="right"><code>FA_EDIT</code></td>
 *              <td align="center"><span class="fa-edit"></span></td>
 *            </tr>
 *            <tr class="row-even">
 *              <td align="right"><code>FA_EGG</code></td>
 *              <td align="center"><span class="fa-egg"></span></td>
 *            </tr>
 *            <tr class="row-odd">
 *              <td align="right"><code>FA_EJECT</code></td>
 *              <td align="center"><span class="fa-eject"></span></td>
 *            </tr>
 *            <tr class="row-even">
 *              <td align="right"><code>FA_ELLIPSIS_H</code></td>
 *              <td align="center"><span class="fa-ellipsis-h"></span></td>
 *            </tr>
 *            <tr class="row-odd">
 *              <td align="right"><code>FA_ELLIPSIS_V</code></td>
 *              <td align="center"><span class="fa-ellipsis-v"></span></td>
 *            </tr>
 *            <tr class="row-even">
 *              <td align="right"><code>FA_ENVELOPE</code></td>
 *              <td align="center"><span class="fa-envelope"></span></td>
 *            </tr>
 *            <tr class="row-odd">
 *              <td align="right"><code>FA_ENVELOPE_OPEN</code></td>
 *              <td align="center"><span class="fa-envelope-open"></span></td>
 *            </tr>
 *            <tr class="row-even">
 *              <td align="right"><code>FA_ENVELOPE_OPEN_TEXT</code></td>
 *              <td align="center"><span class="fa-envelope-open-text"></span></td>
 *            </tr>
 *            <tr class="row-odd">
 *              <td align="right"><code>FA_ENVELOPE_SQUARE</code></td>
 *              <td align="center"><span class="fa-envelope-square"></span></td>
 *            </tr>
 *            <tr class="row-even">
 *              <td align="right"><code>FA_EQUALS</code></td>
 *              <td align="center"><span class="fa-equals"></span></td>
 *            </tr>
 *            <tr class="row-odd">
 *              <td align="right"><code>FA_ERASER</code></td>
 *              <td align="center"><span class="fa-eraser"></span></td>
 *            </tr>
 *            <tr class="row-even">
 *              <td align="right"><code>FA_ETHERNET</code></td>
 *              <td align="center"><span class="fa-ethernet"></span></td>
 *            </tr>
 *            <tr class="row-odd">
 *              <td align="right"><code>FA_EURO_SIGN</code></td>
 *              <td align="center"><span class="fa-euro-sign"></span></td>
 *            </tr>
 *            <tr class="row-even">
 *              <td align="right"><code>FA_EXCHANGE_ALT</code></td>
 *              <td align="center"><span class="fa-exchange-alt"></span></td>
 *            </tr>
 *            <tr class="row-odd">
 *              <td align="right"><code>FA_EXCLAMATION</code></td>
 *              <td align="center"><span class="fa-exclamation"></span></td>
 *            </tr>
 *            <tr class="row-even">
 *              <td align="right"><code>FA_EXCLAMATION_CIRCLE</code></td>
 *              <td align="center"><span class="fa-exclamation-circle"></span></td>
 *            </tr>
 *            <tr class="row-odd">
 *              <td align="right"><code>FA_EXCLAMATION_TRIANGLE</code></td>
 *              <td align="center"><span class="fa-exclamation-triangle"></span></td>
 *            </tr>
 *            <tr class="row-even">
 *              <td align="right"><code>FA_EXPAND</code></td>
 *              <td align="center"><span class="fa-expand"></span></td>
 *            </tr>
 *            <tr class="row-odd">
 *              <td align="right"><code>FA_EXPAND_ARROWS_ALT</code></td>
 *              <td align="center"><span class="fa-expand-arrows-alt"></span></td>
 *            </tr>
 *            <tr class="row-even">
 *              <td align="right"><code>FA_EXTERNAL_LINK_ALT</code></td>
 *              <td align="center"><span class="fa-external-link-alt"></span></td>
 *            </tr>
 *            <tr class="row-odd">
 *              <td align="right"><code>FA_EXTERNAL_LINK_SQUARE_ALT</code></td>
 *              <td align="center"><span class="fa-external-link-square-alt"></span></td>
 *            </tr>
 *            <tr class="row-even">
 *              <td align="right"><code>FA_EYE</code></td>
 *              <td align="center"><span class="fa-eye"></span></td>
 *            </tr>
 *            <tr class="row-odd">
 *              <td align="right"><code>FA_EYE_DROPPER</code></td>
 *              <td align="center"><span class="fa-eye-dropper"></span></td>
 *            </tr>
 *            <tr class="row-even">
 *              <td align="right"><code>FA_EYE_SLASH</code></td>
 *              <td align="center"><span class="fa-eye-slash"></span></td>
 *            </tr>
 *            <tr class="row-odd">
 *              <td align="right"><code>FA_FAN</code></td>
 *              <td align="center"><span class="fa-fan"></span></td>
 *            </tr>
 *            <tr class="row-even">
 *              <td align="right"><code>FA_FAST_BACKWARD</code></td>
 *              <td align="center"><span class="fa-fast-backward"></span></td>
 *            </tr>
 *            <tr class="row-odd">
 *              <td align="right"><code>FA_FAST_FORWARD</code></td>
 *              <td align="center"><span class="fa-fast-forward"></span></td>
 *            </tr>
 *            <tr class="row-even">
 *              <td align="right"><code>FA_FAX</code></td>
 *              <td align="center"><span class="fa-fax"></span></td>
 *            </tr>
 *            <tr class="row-odd">
 *              <td align="right"><code>FA_FEATHER</code></td>
 *              <td align="center"><span class="fa-feather"></span></td>
 *            </tr>
 *            <tr class="row-even">
 *              <td align="right"><code>FA_FEATHER_ALT</code></td>
 *              <td align="center"><span class="fa-feather-alt"></span></td>
 *            </tr>
 *            <tr class="row-odd">
 *              <td align="right"><code>FA_FEMALE</code></td>
 *              <td align="center"><span class="fa-female"></span></td>
 *            </tr>
 *            <tr class="row-even">
 *              <td align="right"><code>FA_FIGHTER_JET</code></td>
 *              <td align="center"><span class="fa-fighter-jet"></span></td>
 *            </tr>
 *            <tr class="row-odd">
 *              <td align="right"><code>FA_FILE</code></td>
 *              <td align="center"><span class="fa-file"></span></td>
 *            </tr>
 *            <tr class="row-even">
 *              <td align="right"><code>FA_FILE_ALT</code></td>
 *              <td align="center"><span class="fa-file-alt"></span></td>
 *            </tr>
 *            <tr class="row-odd">
 *              <td align="right"><code>FA_FILE_ARCHIVE</code></td>
 *              <td align="center"><span class="fa-file-archive"></span></td>
 *            </tr>
 *            <tr class="row-even">
 *              <td align="right"><code>FA_FILE_AUDIO</code></td>
 *              <td align="center"><span class="fa-file-audio"></span></td>
 *            </tr>
 *            <tr class="row-odd">
 *              <td align="right"><code>FA_FILE_CODE</code></td>
 *              <td align="center"><span class="fa-file-code"></span></td>
 *            </tr>
 *            <tr class="row-even">
 *              <td align="right"><code>FA_FILE_CONTRACT</code></td>
 *              <td align="center"><span class="fa-file-contract"></span></td>
 *            </tr>
 *            <tr class="row-odd">
 *              <td align="right"><code>FA_FILE_CSV</code></td>
 *              <td align="center"><span class="fa-file-csv"></span></td>
 *            </tr>
 *            <tr class="row-even">
 *              <td align="right"><code>FA_FILE_DOWNLOAD</code></td>
 *              <td align="center"><span class="fa-file-download"></span></td>
 *            </tr>
 *            <tr class="row-odd">
 *              <td align="right"><code>FA_FILE_EXCEL</code></td>
 *              <td align="center"><span class="fa-file-excel"></span></td>
 *            </tr>
 *            <tr class="row-even">
 *              <td align="right"><code>FA_FILE_EXPORT</code></td>
 *              <td align="center"><span class="fa-file-export"></span></td>
 *            </tr>
 *            <tr class="row-odd">
 *              <td align="right"><code>FA_FILE_IMAGE</code></td>
 *              <td align="center"><span class="fa-file-image"></span></td>
 *            </tr>
 *            <tr class="row-even">
 *              <td align="right"><code>FA_FILE_IMPORT</code></td>
 *              <td align="center"><span class="fa-file-import"></span></td>
 *            </tr>
 *            <tr class="row-odd">
 *              <td align="right"><code>FA_FILE_INVOICE</code></td>
 *              <td align="center"><span class="fa-file-invoice"></span></td>
 *            </tr>
 *            <tr class="row-even">
 *              <td align="right"><code>FA_FILE_INVOICE_DOLLAR</code></td>
 *              <td align="center"><span class="fa-file-invoice-dollar"></span></td>
 *            </tr>
 *            <tr class="row-odd">
 *              <td align="right"><code>FA_FILE_MEDICAL</code></td>
 *              <td align="center"><span class="fa-file-medical"></span></td>
 *            </tr>
 *            <tr class="row-even">
 *              <td align="right"><code>FA_FILE_MEDICAL_ALT</code></td>
 *              <td align="center"><span class="fa-file-medical-alt"></span></td>
 *            </tr>
 *            <tr class="row-odd">
 *              <td align="right"><code>FA_FILE_PDF</code></td>
 *              <td align="center"><span class="fa-file-pdf"></span></td>
 *            </tr>
 *            <tr class="row-even">
 *              <td align="right"><code>FA_FILE_POWERPOINT</code></td>
 *              <td align="center"><span class="fa-file-powerpoint"></span></td>
 *            </tr>
 *            <tr class="row-odd">
 *              <td align="right"><code>FA_FILE_PRESCRIPTION</code></td>
 *              <td align="center"><span class="fa-file-prescription"></span></td>
 *            </tr>
 *            <tr class="row-even">
 *              <td align="right"><code>FA_FILE_SIGNATURE</code></td>
 *              <td align="center"><span class="fa-file-signature"></span></td>
 *            </tr>
 *            <tr class="row-odd">
 *              <td align="right"><code>FA_FILE_UPLOAD</code></td>
 *              <td align="center"><span class="fa-file-upload"></span></td>
 *            </tr>
 *            <tr class="row-even">
 *              <td align="right"><code>FA_FILE_VIDEO</code></td>
 *              <td align="center"><span class="fa-file-video"></span></td>
 *            </tr>
 *            <tr class="row-odd">
 *              <td align="right"><code>FA_FILE_WORD</code></td>
 *              <td align="center"><span class="fa-file-word"></span></td>
 *            </tr>
 *            <tr class="row-even">
 *              <td align="right"><code>FA_FILL</code></td>
 *              <td align="center"><span class="fa-fill"></span></td>
 *            </tr>
 *            <tr class="row-odd">
 *              <td align="right"><code>FA_FILL_DRIP</code></td>
 *              <td align="center"><span class="fa-fill-drip"></span></td>
 *            </tr>
 *            <tr class="row-even">
 *              <td align="right"><code>FA_FILM</code></td>
 *              <td align="center"><span class="fa-film"></span></td>
 *            </tr>
 *            <tr class="row-odd">
 *              <td align="right"><code>FA_FILTER</code></td>
 *              <td align="center"><span class="fa-filter"></span></td>
 *            </tr>
 *            <tr class="row-even">
 *              <td align="right"><code>FA_FINGERPRINT</code></td>
 *              <td align="center"><span class="fa-fingerprint"></span></td>
 *            </tr>
 *            <tr class="row-odd">
 *              <td align="right"><code>FA_FIRE</code></td>
 *              <td align="center"><span class="fa-fire"></span></td>
 *            </tr>
 *            <tr class="row-even">
 *              <td align="right"><code>FA_FIRE_ALT</code></td>
 *              <td align="center"><span class="fa-fire-alt"></span></td>
 *            </tr>
 *            <tr class="row-odd">
 *              <td align="right"><code>FA_FIRE_EXTINGUISHER</code></td>
 *              <td align="center"><span class="fa-fire-extinguisher"></span></td>
 *            </tr>
 *            <tr class="row-even">
 *              <td align="right"><code>FA_FIRST_AID</code></td>
 *              <td align="center"><span class="fa-first-aid"></span></td>
 *            </tr>
 *            <tr class="row-odd">
 *              <td align="right"><code>FA_FISH</code></td>
 *              <td align="center"><span class="fa-fish"></span></td>
 *            </tr>
 *            <tr class="row-even">
 *              <td align="right"><code>FA_FIST_RAISED</code></td>
 *              <td align="center"><span class="fa-fist-raised"></span></td>
 *            </tr>
 *            <tr class="row-odd">
 *              <td align="right"><code>FA_FLAG</code></td>
 *              <td align="center"><span class="fa-flag"></span></td>
 *            </tr>
 *            <tr class="row-even">
 *              <td align="right"><code>FA_FLAG_CHECKERED</code></td>
 *              <td align="center"><span class="fa-flag-checkered"></span></td>
 *            </tr>
 *            <tr class="row-odd">
 *              <td align="right"><code>FA_FLAG_USA</code></td>
 *              <td align="center"><span class="fa-flag-usa"></span></td>
 *            </tr>
 *            <tr class="row-even">
 *              <td align="right"><code>FA_FLASK</code></td>
 *              <td align="center"><span class="fa-flask"></span></td>
 *            </tr>
 *            <tr class="row-odd">
 *              <td align="right"><code>FA_FLUSHED</code></td>
 *              <td align="center"><span class="fa-flushed"></span></td>
 *            </tr>
 *            <tr class="row-even">
 *              <td align="right"><code>FA_FOLDER</code></td>
 *              <td align="center"><span class="fa-folder"></span></td>
 *            </tr>
 *            <tr class="row-odd">
 *              <td align="right"><code>FA_FOLDER_MINUS</code></td>
 *              <td align="center"><span class="fa-folder-minus"></span></td>
 *            </tr>
 *            <tr class="row-even">
 *              <td align="right"><code>FA_FOLDER_OPEN</code></td>
 *              <td align="center"><span class="fa-folder-open"></span></td>
 *            </tr>
 *            <tr class="row-odd">
 *              <td align="right"><code>FA_FOLDER_PLUS</code></td>
 *              <td align="center"><span class="fa-folder-plus"></span></td>
 *            </tr>
 *            <tr class="row-even">
 *              <td align="right"><code>FA_FONT</code></td>
 *              <td align="center"><span class="fa-font"></span></td>
 *            </tr>
 *            <tr class="row-odd">
 *              <td align="right"><code>FA_FONT_AWESOME_LOGO_FULL</code></td>
 *              <td align="center"><span class="fa-font-awesome-logo-full"></span></td>
 *            </tr>
 *            <tr class="row-even">
 *              <td align="right"><code>FA_FOOTBALL_BALL</code></td>
 *              <td align="center"><span class="fa-football-ball"></span></td>
 *            </tr>
 *            <tr class="row-odd">
 *              <td align="right"><code>FA_FORWARD</code></td>
 *              <td align="center"><span class="fa-forward"></span></td>
 *            </tr>
 *            <tr class="row-even">
 *              <td align="right"><code>FA_FROG</code></td>
 *              <td align="center"><span class="fa-frog"></span></td>
 *            </tr>
 *            <tr class="row-odd">
 *              <td align="right"><code>FA_FROWN</code></td>
 *              <td align="center"><span class="fa-frown"></span></td>
 *            </tr>
 *            <tr class="row-even">
 *              <td align="right"><code>FA_FROWN_OPEN</code></td>
 *              <td align="center"><span class="fa-frown-open"></span></td>
 *            </tr>
 *            <tr class="row-odd">
 *              <td align="right"><code>FA_FUNNEL_DOLLAR</code></td>
 *              <td align="center"><span class="fa-funnel-dollar"></span></td>
 *            </tr>
 *            <tr class="row-even">
 *              <td align="right"><code>FA_FUTBOL</code></td>
 *              <td align="center"><span class="fa-futbol"></span></td>
 *            </tr>
 *            <tr class="row-odd">
 *              <td align="right"><code>FA_GAMEPAD</code></td>
 *              <td align="center"><span class="fa-gamepad"></span></td>
 *            </tr>
 *            <tr class="row-even">
 *              <td align="right"><code>FA_GAS_PUMP</code></td>
 *              <td align="center"><span class="fa-gas-pump"></span></td>
 *            </tr>
 *            <tr class="row-odd">
 *              <td align="right"><code>FA_GAVEL</code></td>
 *              <td align="center"><span class="fa-gavel"></span></td>
 *            </tr>
 *            <tr class="row-even">
 *              <td align="right"><code>FA_GEM</code></td>
 *              <td align="center"><span class="fa-gem"></span></td>
 *            </tr>
 *            <tr class="row-odd">
 *              <td align="right"><code>FA_GENDERLESS</code></td>
 *              <td align="center"><span class="fa-genderless"></span></td>
 *            </tr>
 *            <tr class="row-even">
 *              <td align="right"><code>FA_GHOST</code></td>
 *              <td align="center"><span class="fa-ghost"></span></td>
 *            </tr>
 *            <tr class="row-odd">
 *              <td align="right"><code>FA_GIFT</code></td>
 *              <td align="center"><span class="fa-gift"></span></td>
 *            </tr>
 *            <tr class="row-even">
 *              <td align="right"><code>FA_GIFTS</code></td>
 *              <td align="center"><span class="fa-gifts"></span></td>
 *            </tr>
 *            <tr class="row-odd">
 *              <td align="right"><code>FA_GLASS_CHEERS</code></td>
 *              <td align="center"><span class="fa-glass-cheers"></span></td>
 *            </tr>
 *            <tr class="row-even">
 *              <td align="right"><code>FA_GLASS_MARTINI</code></td>
 *              <td align="center"><span class="fa-glass-martini"></span></td>
 *            </tr>
 *            <tr class="row-odd">
 *              <td align="right"><code>FA_GLASS_MARTINI_ALT</code></td>
 *              <td align="center"><span class="fa-glass-martini-alt"></span></td>
 *            </tr>
 *            <tr class="row-even">
 *              <td align="right"><code>FA_GLASS_WHISKEY</code></td>
 *              <td align="center"><span class="fa-glass-whiskey"></span></td>
 *            </tr>
 *            <tr class="row-odd">
 *              <td align="right"><code>FA_GLASSES</code></td>
 *              <td align="center"><span class="fa-glasses"></span></td>
 *            </tr>
 *            <tr class="row-even">
 *              <td align="right"><code>FA_GLOBE</code></td>
 *              <td align="center"><span class="fa-globe"></span></td>
 *            </tr>
 *            <tr class="row-odd">
 *              <td align="right"><code>FA_GLOBE_AFRICA</code></td>
 *              <td align="center"><span class="fa-globe-africa"></span></td>
 *            </tr>
 *            <tr class="row-even">
 *              <td align="right"><code>FA_GLOBE_AMERICAS</code></td>
 *              <td align="center"><span class="fa-globe-americas"></span></td>
 *            </tr>
 *            <tr class="row-odd">
 *              <td align="right"><code>FA_GLOBE_ASIA</code></td>
 *              <td align="center"><span class="fa-globe-asia"></span></td>
 *            </tr>
 *            <tr class="row-even">
 *              <td align="right"><code>FA_GLOBE_EUROPE</code></td>
 *              <td align="center"><span class="fa-globe-europe"></span></td>
 *            </tr>
 *            <tr class="row-odd">
 *              <td align="right"><code>FA_GOLF_BALL</code></td>
 *              <td align="center"><span class="fa-golf-ball"></span></td>
 *            </tr>
 *            <tr class="row-even">
 *              <td align="right"><code>FA_GOPURAM</code></td>
 *              <td align="center"><span class="fa-gopuram"></span></td>
 *            </tr>
 *            <tr class="row-odd">
 *              <td align="right"><code>FA_GRADUATION_CAP</code></td>
 *              <td align="center"><span class="fa-graduation-cap"></span></td>
 *            </tr>
 *            <tr class="row-even">
 *              <td align="right"><code>FA_GREATER_THAN</code></td>
 *              <td align="center"><span class="fa-greater-than"></span></td>
 *            </tr>
 *            <tr class="row-odd">
 *              <td align="right"><code>FA_GREATER_THAN_EQUAL</code></td>
 *              <td align="center"><span class="fa-greater-than-equal"></span></td>
 *            </tr>
 *            <tr class="row-even">
 *              <td align="right"><code>FA_GRIMACE</code></td>
 *              <td align="center"><span class="fa-grimace"></span></td>
 *            </tr>
 *            <tr class="row-odd">
 *              <td align="right"><code>FA_GRIN</code></td>
 *              <td align="center"><span class="fa-grin"></span></td>
 *            </tr>
 *            <tr class="row-even">
 *              <td align="right"><code>FA_GRIN_ALT</code></td>
 *              <td align="center"><span class="fa-grin-alt"></span></td>
 *            </tr>
 *            <tr class="row-odd">
 *              <td align="right"><code>FA_GRIN_BEAM</code></td>
 *              <td align="center"><span class="fa-grin-beam"></span></td>
 *            </tr>
 *            <tr class="row-even">
 *              <td align="right"><code>FA_GRIN_BEAM_SWEAT</code></td>
 *              <td align="center"><span class="fa-grin-beam-sweat"></span></td>
 *            </tr>
 *            <tr class="row-odd">
 *              <td align="right"><code>FA_GRIN_HEARTS</code></td>
 *              <td align="center"><span class="fa-grin-hearts"></span></td>
 *            </tr>
 *            <tr class="row-even">
 *              <td align="right"><code>FA_GRIN_SQUINT</code></td>
 *              <td align="center"><span class="fa-grin-squint"></span></td>
 *            </tr>
 *            <tr class="row-odd">
 *              <td align="right"><code>FA_GRIN_SQUINT_TEARS</code></td>
 *              <td align="center"><span class="fa-grin-squint-tears"></span></td>
 *            </tr>
 *            <tr class="row-even">
 *              <td align="right"><code>FA_GRIN_STARS</code></td>
 *              <td align="center"><span class="fa-grin-stars"></span></td>
 *            </tr>
 *            <tr class="row-odd">
 *              <td align="right"><code>FA_GRIN_TEARS</code></td>
 *              <td align="center"><span class="fa-grin-tears"></span></td>
 *            </tr>
 *            <tr class="row-even">
 *              <td align="right"><code>FA_GRIN_TONGUE</code></td>
 *              <td align="center"><span class="fa-grin-tongue"></span></td>
 *            </tr>
 *            <tr class="row-odd">
 *              <td align="right"><code>FA_GRIN_TONGUE_SQUINT</code></td>
 *              <td align="center"><span class="fa-grin-tongue-squint"></span></td>
 *            </tr>
 *            <tr class="row-even">
 *              <td align="right"><code>FA_GRIN_TONGUE_WINK</code></td>
 *              <td align="center"><span class="fa-grin-tongue-wink"></span></td>
 *            </tr>
 *            <tr class="row-odd">
 *              <td align="right"><code>FA_GRIN_WINK</code></td>
 *              <td align="center"><span class="fa-grin-wink"></span></td>
 *            </tr>
 *            <tr class="row-even">
 *              <td align="right"><code>FA_GRIP_HORIZONTAL</code></td>
 *              <td align="center"><span class="fa-grip-horizontal"></span></td>
 *            </tr>
 *            <tr class="row-odd">
 *              <td align="right"><code>FA_GRIP_LINES</code></td>
 *              <td align="center"><span class="fa-grip-lines"></span></td>
 *            </tr>
 *            <tr class="row-even">
 *              <td align="right"><code>FA_GRIP_LINES_VERTICAL</code></td>
 *              <td align="center"><span class="fa-grip-lines-vertical"></span></td>
 *            </tr>
 *            <tr class="row-odd">
 *              <td align="right"><code>FA_GRIP_VERTICAL</code></td>
 *              <td align="center"><span class="fa-grip-vertical"></span></td>
 *            </tr>
 *            <tr class="row-even">
 *              <td align="right"><code>FA_GUITAR</code></td>
 *              <td align="center"><span class="fa-guitar"></span></td>
 *            </tr>
 *            <tr class="row-odd">
 *              <td align="right"><code>FA_H_SQUARE</code></td>
 *              <td align="center"><span class="fa-h-square"></span></td>
 *            </tr>
 *            <tr class="row-even">
 *              <td align="right"><code>FA_HAMBURGER</code></td>
 *              <td align="center"><span class="fa-hamburger"></span></td>
 *            </tr>
 *            <tr class="row-odd">
 *              <td align="right"><code>FA_HAMMER</code></td>
 *              <td align="center"><span class="fa-hammer"></span></td>
 *            </tr>
 *            <tr class="row-even">
 *              <td align="right"><code>FA_HAMSA</code></td>
 *              <td align="center"><span class="fa-hamsa"></span></td>
 *            </tr>
 *            <tr class="row-odd">
 *              <td align="right"><code>FA_HAND_HOLDING</code></td>
 *              <td align="center"><span class="fa-hand-holding"></span></td>
 *            </tr>
 *            <tr class="row-even">
 *              <td align="right"><code>FA_HAND_HOLDING_HEART</code></td>
 *              <td align="center"><span class="fa-hand-holding-heart"></span></td>
 *            </tr>
 *            <tr class="row-odd">
 *              <td align="right"><code>FA_HAND_HOLDING_USD</code></td>
 *              <td align="center"><span class="fa-hand-holding-usd"></span></td>
 *            </tr>
 *            <tr class="row-even">
 *              <td align="right"><code>FA_HAND_LIZARD</code></td>
 *              <td align="center"><span class="fa-hand-lizard"></span></td>
 *            </tr>
 *            <tr class="row-odd">
 *              <td align="right"><code>FA_HAND_MIDDLE_FINGER</code></td>
 *              <td align="center"><span class="fa-hand-middle-finger"></span></td>
 *            </tr>
 *            <tr class="row-even">
 *              <td align="right"><code>FA_HAND_PAPER</code></td>
 *              <td align="center"><span class="fa-hand-paper"></span></td>
 *            </tr>
 *            <tr class="row-odd">
 *              <td align="right"><code>FA_HAND_PEACE</code></td>
 *              <td align="center"><span class="fa-hand-peace"></span></td>
 *            </tr>
 *            <tr class="row-even">
 *              <td align="right"><code>FA_HAND_POINT_DOWN</code></td>
 *              <td align="center"><span class="fa-hand-point-down"></span></td>
 *            </tr>
 *            <tr class="row-odd">
 *              <td align="right"><code>FA_HAND_POINT_LEFT</code></td>
 *              <td align="center"><span class="fa-hand-point-left"></span></td>
 *            </tr>
 *            <tr class="row-even">
 *              <td align="right"><code>FA_HAND_POINT_RIGHT</code></td>
 *              <td align="center"><span class="fa-hand-point-right"></span></td>
 *            </tr>
 *            <tr class="row-odd">
 *              <td align="right"><code>FA_HAND_POINT_UP</code></td>
 *              <td align="center"><span class="fa-hand-point-up"></span></td>
 *            </tr>
 *            <tr class="row-even">
 *              <td align="right"><code>FA_HAND_POINTER</code></td>
 *              <td align="center"><span class="fa-hand-pointer"></span></td>
 *            </tr>
 *            <tr class="row-odd">
 *              <td align="right"><code>FA_HAND_ROCK</code></td>
 *              <td align="center"><span class="fa-hand-rock"></span></td>
 *            </tr>
 *            <tr class="row-even">
 *              <td align="right"><code>FA_HAND_SCISSORS</code></td>
 *              <td align="center"><span class="fa-hand-scissors"></span></td>
 *            </tr>
 *            <tr class="row-odd">
 *              <td align="right"><code>FA_HAND_SPOCK</code></td>
 *              <td align="center"><span class="fa-hand-spock"></span></td>
 *            </tr>
 *            <tr class="row-even">
 *              <td align="right"><code>FA_HANDS</code></td>
 *              <td align="center"><span class="fa-hands"></span></td>
 *            </tr>
 *            <tr class="row-odd">
 *              <td align="right"><code>FA_HANDS_HELPING</code></td>
 *              <td align="center"><span class="fa-hands-helping"></span></td>
 *            </tr>
 *            <tr class="row-even">
 *              <td align="right"><code>FA_HANDSHAKE</code></td>
 *              <td align="center"><span class="fa-handshake"></span></td>
 *            </tr>
 *            <tr class="row-odd">
 *              <td align="right"><code>FA_HANUKIAH</code></td>
 *              <td align="center"><span class="fa-hanukiah"></span></td>
 *            </tr>
 *            <tr class="row-even">
 *              <td align="right"><code>FA_HARD_HAT</code></td>
 *              <td align="center"><span class="fa-hard-hat"></span></td>
 *            </tr>
 *            <tr class="row-odd">
 *              <td align="right"><code>FA_HASHTAG</code></td>
 *              <td align="center"><span class="fa-hashtag"></span></td>
 *            </tr>
 *            <tr class="row-even">
 *              <td align="right"><code>FA_HAT_WIZARD</code></td>
 *              <td align="center"><span class="fa-hat-wizard"></span></td>
 *            </tr>
 *            <tr class="row-odd">
 *              <td align="right"><code>FA_HAYKAL</code></td>
 *              <td align="center"><span class="fa-haykal"></span></td>
 *            </tr>
 *            <tr class="row-even">
 *              <td align="right"><code>FA_HDD</code></td>
 *              <td align="center"><span class="fa-hdd"></span></td>
 *            </tr>
 *            <tr class="row-odd">
 *              <td align="right"><code>FA_HEADING</code></td>
 *              <td align="center"><span class="fa-heading"></span></td>
 *            </tr>
 *            <tr class="row-even">
 *              <td align="right"><code>FA_HEADPHONES</code></td>
 *              <td align="center"><span class="fa-headphones"></span></td>
 *            </tr>
 *            <tr class="row-odd">
 *              <td align="right"><code>FA_HEADPHONES_ALT</code></td>
 *              <td align="center"><span class="fa-headphones-alt"></span></td>
 *            </tr>
 *            <tr class="row-even">
 *              <td align="right"><code>FA_HEADSET</code></td>
 *              <td align="center"><span class="fa-headset"></span></td>
 *            </tr>
 *            <tr class="row-odd">
 *              <td align="right"><code>FA_HEART</code></td>
 *              <td align="center"><span class="fa-heart"></span></td>
 *            </tr>
 *            <tr class="row-even">
 *              <td align="right"><code>FA_HEART_BROKEN</code></td>
 *              <td align="center"><span class="fa-heart-broken"></span></td>
 *            </tr>
 *            <tr class="row-odd">
 *              <td align="right"><code>FA_HEARTBEAT</code></td>
 *              <td align="center"><span class="fa-heartbeat"></span></td>
 *            </tr>
 *            <tr class="row-even">
 *              <td align="right"><code>FA_HELICOPTER</code></td>
 *              <td align="center"><span class="fa-helicopter"></span></td>
 *            </tr>
 *            <tr class="row-odd">
 *              <td align="right"><code>FA_HIGHLIGHTER</code></td>
 *              <td align="center"><span class="fa-highlighter"></span></td>
 *            </tr>
 *            <tr class="row-even">
 *              <td align="right"><code>FA_HIKING</code></td>
 *              <td align="center"><span class="fa-hiking"></span></td>
 *            </tr>
 *            <tr class="row-odd">
 *              <td align="right"><code>FA_HIPPO</code></td>
 *              <td align="center"><span class="fa-hippo"></span></td>
 *            </tr>
 *            <tr class="row-even">
 *              <td align="right"><code>FA_HISTORY</code></td>
 *              <td align="center"><span class="fa-history"></span></td>
 *            </tr>
 *            <tr class="row-odd">
 *              <td align="right"><code>FA_HOCKEY_PUCK</code></td>
 *              <td align="center"><span class="fa-hockey-puck"></span></td>
 *            </tr>
 *            <tr class="row-even">
 *              <td align="right"><code>FA_HOLLY_BERRY</code></td>
 *              <td align="center"><span class="fa-holly-berry"></span></td>
 *            </tr>
 *            <tr class="row-odd">
 *              <td align="right"><code>FA_HOME</code></td>
 *              <td align="center"><span class="fa-home"></span></td>
 *            </tr>
 *            <tr class="row-even">
 *              <td align="right"><code>FA_HORSE</code></td>
 *              <td align="center"><span class="fa-horse"></span></td>
 *            </tr>
 *            <tr class="row-odd">
 *              <td align="right"><code>FA_HORSE_HEAD</code></td>
 *              <td align="center"><span class="fa-horse-head"></span></td>
 *            </tr>
 *            <tr class="row-even">
 *              <td align="right"><code>FA_HOSPITAL</code></td>
 *              <td align="center"><span class="fa-hospital"></span></td>
 *            </tr>
 *            <tr class="row-odd">
 *              <td align="right"><code>FA_HOSPITAL_ALT</code></td>
 *              <td align="center"><span class="fa-hospital-alt"></span></td>
 *            </tr>
 *            <tr class="row-even">
 *              <td align="right"><code>FA_HOSPITAL_SYMBOL</code></td>
 *              <td align="center"><span class="fa-hospital-symbol"></span></td>
 *            </tr>
 *            <tr class="row-odd">
 *              <td align="right"><code>FA_HOT_TUB</code></td>
 *              <td align="center"><span class="fa-hot-tub"></span></td>
 *            </tr>
 *            <tr class="row-even">
 *              <td align="right"><code>FA_HOTDOG</code></td>
 *              <td align="center"><span class="fa-hotdog"></span></td>
 *            </tr>
 *            <tr class="row-odd">
 *              <td align="right"><code>FA_HOTEL</code></td>
 *              <td align="center"><span class="fa-hotel"></span></td>
 *            </tr>
 *            <tr class="row-even">
 *              <td align="right"><code>FA_HOURGLASS</code></td>
 *              <td align="center"><span class="fa-hourglass"></span></td>
 *            </tr>
 *            <tr class="row-odd">
 *              <td align="right"><code>FA_HOURGLASS_END</code></td>
 *              <td align="center"><span class="fa-hourglass-end"></span></td>
 *            </tr>
 *            <tr class="row-even">
 *              <td align="right"><code>FA_HOURGLASS_HALF</code></td>
 *              <td align="center"><span class="fa-hourglass-half"></span></td>
 *            </tr>
 *            <tr class="row-odd">
 *              <td align="right"><code>FA_HOURGLASS_START</code></td>
 *              <td align="center"><span class="fa-hourglass-start"></span></td>
 *            </tr>
 *            <tr class="row-even">
 *              <td align="right"><code>FA_HOUSE_DAMAGE</code></td>
 *              <td align="center"><span class="fa-house-damage"></span></td>
 *            </tr>
 *            <tr class="row-odd">
 *              <td align="right"><code>FA_HRYVNIA</code></td>
 *              <td align="center"><span class="fa-hryvnia"></span></td>
 *            </tr>
 *            <tr class="row-even">
 *              <td align="right"><code>FA_I_CURSOR</code></td>
 *              <td align="center"><span class="fa-i-cursor"></span></td>
 *            </tr>
 *            <tr class="row-odd">
 *              <td align="right"><code>FA_ICE_CREAM</code></td>
 *              <td align="center"><span class="fa-ice-cream"></span></td>
 *            </tr>
 *            <tr class="row-even">
 *              <td align="right"><code>FA_ICICLES</code></td>
 *              <td align="center"><span class="fa-icicles"></span></td>
 *            </tr>
 *            <tr class="row-odd">
 *              <td align="right"><code>FA_ICONS</code></td>
 *              <td align="center"><span class="fa-icons"></span></td>
 *            </tr>
 *            <tr class="row-even">
 *              <td align="right"><code>FA_ID_BADGE</code></td>
 *              <td align="center"><span class="fa-id-badge"></span></td>
 *            </tr>
 *            <tr class="row-odd">
 *              <td align="right"><code>FA_ID_CARD</code></td>
 *              <td align="center"><span class="fa-id-card"></span></td>
 *            </tr>
 *            <tr class="row-even">
 *              <td align="right"><code>FA_ID_CARD_ALT</code></td>
 *              <td align="center"><span class="fa-id-card-alt"></span></td>
 *            </tr>
 *            <tr class="row-odd">
 *              <td align="right"><code>FA_IGLOO</code></td>
 *              <td align="center"><span class="fa-igloo"></span></td>
 *            </tr>
 *            <tr class="row-even">
 *              <td align="right"><code>FA_IMAGE</code></td>
 *              <td align="center"><span class="fa-image"></span></td>
 *            </tr>
 *            <tr class="row-odd">
 *              <td align="right"><code>FA_IMAGES</code></td>
 *              <td align="center"><span class="fa-images"></span></td>
 *            </tr>
 *            <tr class="row-even">
 *              <td align="right"><code>FA_INBOX</code></td>
 *              <td align="center"><span class="fa-inbox"></span></td>
 *            </tr>
 *            <tr class="row-odd">
 *              <td align="right"><code>FA_INDENT</code></td>
 *              <td align="center"><span class="fa-indent"></span></td>
 *            </tr>
 *            <tr class="row-even">
 *              <td align="right"><code>FA_INDUSTRY</code></td>
 *              <td align="center"><span class="fa-industry"></span></td>
 *            </tr>
 *            <tr class="row-odd">
 *              <td align="right"><code>FA_INFINITY</code></td>
 *              <td align="center"><span class="fa-infinity"></span></td>
 *            </tr>
 *            <tr class="row-even">
 *              <td align="right"><code>FA_INFO</code></td>
 *              <td align="center"><span class="fa-info"></span></td>
 *            </tr>
 *            <tr class="row-odd">
 *              <td align="right"><code>FA_INFO_CIRCLE</code></td>
 *              <td align="center"><span class="fa-info-circle"></span></td>
 *            </tr>
 *            <tr class="row-even">
 *              <td align="right"><code>FA_ITALIC</code></td>
 *              <td align="center"><span class="fa-italic"></span></td>
 *            </tr>
 *            <tr class="row-odd">
 *              <td align="right"><code>FA_JEDI</code></td>
 *              <td align="center"><span class="fa-jedi"></span></td>
 *            </tr>
 *            <tr class="row-even">
 *              <td align="right"><code>FA_JOINT</code></td>
 *              <td align="center"><span class="fa-joint"></span></td>
 *            </tr>
 *            <tr class="row-odd">
 *              <td align="right"><code>FA_JOURNAL_WHILLS</code></td>
 *              <td align="center"><span class="fa-journal-whills"></span></td>
 *            </tr>
 *            <tr class="row-even">
 *              <td align="right"><code>FA_KAABA</code></td>
 *              <td align="center"><span class="fa-kaaba"></span></td>
 *            </tr>
 *            <tr class="row-odd">
 *              <td align="right"><code>FA_KEY</code></td>
 *              <td align="center"><span class="fa-key"></span></td>
 *            </tr>
 *            <tr class="row-even">
 *              <td align="right"><code>FA_KEYBOARD</code></td>
 *              <td align="center"><span class="fa-keyboard"></span></td>
 *            </tr>
 *            <tr class="row-odd">
 *              <td align="right"><code>FA_KHANDA</code></td>
 *              <td align="center"><span class="fa-khanda"></span></td>
 *            </tr>
 *            <tr class="row-even">
 *              <td align="right"><code>FA_KISS</code></td>
 *              <td align="center"><span class="fa-kiss"></span></td>
 *            </tr>
 *            <tr class="row-odd">
 *              <td align="right"><code>FA_KISS_BEAM</code></td>
 *              <td align="center"><span class="fa-kiss-beam"></span></td>
 *            </tr>
 *            <tr class="row-even">
 *              <td align="right"><code>FA_KISS_WINK_HEART</code></td>
 *              <td align="center"><span class="fa-kiss-wink-heart"></span></td>
 *            </tr>
 *            <tr class="row-odd">
 *              <td align="right"><code>FA_KIWI_BIRD</code></td>
 *              <td align="center"><span class="fa-kiwi-bird"></span></td>
 *            </tr>
 *            <tr class="row-even">
 *              <td align="right"><code>FA_LANDMARK</code></td>
 *              <td align="center"><span class="fa-landmark"></span></td>
 *            </tr>
 *            <tr class="row-odd">
 *              <td align="right"><code>FA_LANGUAGE</code></td>
 *              <td align="center"><span class="fa-language"></span></td>
 *            </tr>
 *            <tr class="row-even">
 *              <td align="right"><code>FA_LAPTOP</code></td>
 *              <td align="center"><span class="fa-laptop"></span></td>
 *            </tr>
 *            <tr class="row-odd">
 *              <td align="right"><code>FA_LAPTOP_CODE</code></td>
 *              <td align="center"><span class="fa-laptop-code"></span></td>
 *            </tr>
 *            <tr class="row-even">
 *              <td align="right"><code>FA_LAPTOP_MEDICAL</code></td>
 *              <td align="center"><span class="fa-laptop-medical"></span></td>
 *            </tr>
 *            <tr class="row-odd">
 *              <td align="right"><code>FA_LAUGH</code></td>
 *              <td align="center"><span class="fa-laugh"></span></td>
 *            </tr>
 *            <tr class="row-even">
 *              <td align="right"><code>FA_LAUGH_BEAM</code></td>
 *              <td align="center"><span class="fa-laugh-beam"></span></td>
 *            </tr>
 *            <tr class="row-odd">
 *              <td align="right"><code>FA_LAUGH_SQUINT</code></td>
 *              <td align="center"><span class="fa-laugh-squint"></span></td>
 *            </tr>
 *            <tr class="row-even">
 *              <td align="right"><code>FA_LAUGH_WINK</code></td>
 *              <td align="center"><span class="fa-laugh-wink"></span></td>
 *            </tr>
 *            <tr class="row-odd">
 *              <td align="right"><code>FA_LAYER_GROUP</code></td>
 *              <td align="center"><span class="fa-layer-group"></span></td>
 *            </tr>
 *            <tr class="row-even">
 *              <td align="right"><code>FA_LEAF</code></td>
 *              <td align="center"><span class="fa-leaf"></span></td>
 *            </tr>
 *            <tr class="row-odd">
 *              <td align="right"><code>FA_LEMON</code></td>
 *              <td align="center"><span class="fa-lemon"></span></td>
 *            </tr>
 *            <tr class="row-even">
 *              <td align="right"><code>FA_LESS_THAN</code></td>
 *              <td align="center"><span class="fa-less-than"></span></td>
 *            </tr>
 *            <tr class="row-odd">
 *              <td align="right"><code>FA_LESS_THAN_EQUAL</code></td>
 *              <td align="center"><span class="fa-less-than-equal"></span></td>
 *            </tr>
 *            <tr class="row-even">
 *              <td align="right"><code>FA_LEVEL_DOWN_ALT</code></td>
 *              <td align="center"><span class="fa-level-down-alt"></span></td>
 *            </tr>
 *            <tr class="row-odd">
 *              <td align="right"><code>FA_LEVEL_UP_ALT</code></td>
 *              <td align="center"><span class="fa-level-up-alt"></span></td>
 *            </tr>
 *            <tr class="row-even">
 *              <td align="right"><code>FA_LIFE_RING</code></td>
 *              <td align="center"><span class="fa-life-ring"></span></td>
 *            </tr>
 *            <tr class="row-odd">
 *              <td align="right"><code>FA_LIGHTBULB</code></td>
 *              <td align="center"><span class="fa-lightbulb"></span></td>
 *            </tr>
 *            <tr class="row-even">
 *              <td align="right"><code>FA_LINK</code></td>
 *              <td align="center"><span class="fa-link"></span></td>
 *            </tr>
 *            <tr class="row-odd">
 *              <td align="right"><code>FA_LIRA_SIGN</code></td>
 *              <td align="center"><span class="fa-lira-sign"></span></td>
 *            </tr>
 *            <tr class="row-even">
 *              <td align="right"><code>FA_LIST</code></td>
 *              <td align="center"><span class="fa-list"></span></td>
 *            </tr>
 *            <tr class="row-odd">
 *              <td align="right"><code>FA_LIST_ALT</code></td>
 *              <td align="center"><span class="fa-list-alt"></span></td>
 *            </tr>
 *            <tr class="row-even">
 *              <td align="right"><code>FA_LIST_OL</code></td>
 *              <td align="center"><span class="fa-list-ol"></span></td>
 *            </tr>
 *            <tr class="row-odd">
 *              <td align="right"><code>FA_LIST_UL</code></td>
 *              <td align="center"><span class="fa-list-ul"></span></td>
 *            </tr>
 *            <tr class="row-even">
 *              <td align="right"><code>FA_LOCATION_ARROW</code></td>
 *              <td align="center"><span class="fa-location-arrow"></span></td>
 *            </tr>
 *            <tr class="row-odd">
 *              <td align="right"><code>FA_LOCK</code></td>
 *              <td align="center"><span class="fa-lock"></span></td>
 *            </tr>
 *            <tr class="row-even">
 *              <td align="right"><code>FA_LOCK_OPEN</code></td>
 *              <td align="center"><span class="fa-lock-open"></span></td>
 *            </tr>
 *            <tr class="row-odd">
 *              <td align="right"><code>FA_LONG_ARROW_ALT_DOWN</code></td>
 *              <td align="center"><span class="fa-long-arrow-alt-down"></span></td>
 *            </tr>
 *            <tr class="row-even">
 *              <td align="right"><code>FA_LONG_ARROW_ALT_LEFT</code></td>
 *              <td align="center"><span class="fa-long-arrow-alt-left"></span></td>
 *            </tr>
 *            <tr class="row-odd">
 *              <td align="right"><code>FA_LONG_ARROW_ALT_RIGHT</code></td>
 *              <td align="center"><span class="fa-long-arrow-alt-right"></span></td>
 *            </tr>
 *            <tr class="row-even">
 *              <td align="right"><code>FA_LONG_ARROW_ALT_UP</code></td>
 *              <td align="center"><span class="fa-long-arrow-alt-up"></span></td>
 *            </tr>
 *            <tr class="row-odd">
 *              <td align="right"><code>FA_LOW_VISION</code></td>
 *              <td align="center"><span class="fa-low-vision"></span></td>
 *            </tr>
 *            <tr class="row-even">
 *              <td align="right"><code>FA_LUGGAGE_CART</code></td>
 *              <td align="center"><span class="fa-luggage-cart"></span></td>
 *            </tr>
 *            <tr class="row-odd">
 *              <td align="right"><code>FA_MAGIC</code></td>
 *              <td align="center"><span class="fa-magic"></span></td>
 *            </tr>
 *            <tr class="row-even">
 *              <td align="right"><code>FA_MAGNET</code></td>
 *              <td align="center"><span class="fa-magnet"></span></td>
 *            </tr>
 *            <tr class="row-odd">
 *              <td align="right"><code>FA_MAIL_BULK</code></td>
 *              <td align="center"><span class="fa-mail-bulk"></span></td>
 *            </tr>
 *            <tr class="row-even">
 *              <td align="right"><code>FA_MALE</code></td>
 *              <td align="center"><span class="fa-male"></span></td>
 *            </tr>
 *            <tr class="row-odd">
 *              <td align="right"><code>FA_MAP</code></td>
 *              <td align="center"><span class="fa-map"></span></td>
 *            </tr>
 *            <tr class="row-even">
 *              <td align="right"><code>FA_MAP_MARKED</code></td>
 *              <td align="center"><span class="fa-map-marked"></span></td>
 *            </tr>
 *            <tr class="row-odd">
 *              <td align="right"><code>FA_MAP_MARKED_ALT</code></td>
 *              <td align="center"><span class="fa-map-marked-alt"></span></td>
 *            </tr>
 *            <tr class="row-even">
 *              <td align="right"><code>FA_MAP_MARKER</code></td>
 *              <td align="center"><span class="fa-map-marker"></span></td>
 *            </tr>
 *            <tr class="row-odd">
 *              <td align="right"><code>FA_MAP_MARKER_ALT</code></td>
 *              <td align="center"><span class="fa-map-marker-alt"></span></td>
 *            </tr>
 *            <tr class="row-even">
 *              <td align="right"><code>FA_MAP_PIN</code></td>
 *              <td align="center"><span class="fa-map-pin"></span></td>
 *            </tr>
 *            <tr class="row-odd">
 *              <td align="right"><code>FA_MAP_SIGNS</code></td>
 *              <td align="center"><span class="fa-map-signs"></span></td>
 *            </tr>
 *            <tr class="row-even">
 *              <td align="right"><code>FA_MARKER</code></td>
 *              <td align="center"><span class="fa-marker"></span></td>
 *            </tr>
 *            <tr class="row-odd">
 *              <td align="right"><code>FA_MARS</code></td>
 *              <td align="center"><span class="fa-mars"></span></td>
 *            </tr>
 *            <tr class="row-even">
 *              <td align="right"><code>FA_MARS_DOUBLE</code></td>
 *              <td align="center"><span class="fa-mars-double"></span></td>
 *            </tr>
 *            <tr class="row-odd">
 *              <td align="right"><code>FA_MARS_STROKE</code></td>
 *              <td align="center"><span class="fa-mars-stroke"></span></td>
 *            </tr>
 *            <tr class="row-even">
 *              <td align="right"><code>FA_MARS_STROKE_H</code></td>
 *              <td align="center"><span class="fa-mars-stroke-h"></span></td>
 *            </tr>
 *            <tr class="row-odd">
 *              <td align="right"><code>FA_MARS_STROKE_V</code></td>
 *              <td align="center"><span class="fa-mars-stroke-v"></span></td>
 *            </tr>
 *            <tr class="row-even">
 *              <td align="right"><code>FA_MASK</code></td>
 *              <td align="center"><span class="fa-mask"></span></td>
 *            </tr>
 *            <tr class="row-odd">
 *              <td align="right"><code>FA_MEDAL</code></td>
 *              <td align="center"><span class="fa-medal"></span></td>
 *            </tr>
 *            <tr class="row-even">
 *              <td align="right"><code>FA_MEDKIT</code></td>
 *              <td align="center"><span class="fa-medkit"></span></td>
 *            </tr>
 *            <tr class="row-odd">
 *              <td align="right"><code>FA_MEH</code></td>
 *              <td align="center"><span class="fa-meh"></span></td>
 *            </tr>
 *            <tr class="row-even">
 *              <td align="right"><code>FA_MEH_BLANK</code></td>
 *              <td align="center"><span class="fa-meh-blank"></span></td>
 *            </tr>
 *            <tr class="row-odd">
 *              <td align="right"><code>FA_MEH_ROLLING_EYES</code></td>
 *              <td align="center"><span class="fa-meh-rolling-eyes"></span></td>
 *            </tr>
 *            <tr class="row-even">
 *              <td align="right"><code>FA_MEMORY</code></td>
 *              <td align="center"><span class="fa-memory"></span></td>
 *            </tr>
 *            <tr class="row-odd">
 *              <td align="right"><code>FA_MENORAH</code></td>
 *              <td align="center"><span class="fa-menorah"></span></td>
 *            </tr>
 *            <tr class="row-even">
 *              <td align="right"><code>FA_MERCURY</code></td>
 *              <td align="center"><span class="fa-mercury"></span></td>
 *            </tr>
 *            <tr class="row-odd">
 *              <td align="right"><code>FA_METEOR</code></td>
 *              <td align="center"><span class="fa-meteor"></span></td>
 *            </tr>
 *            <tr class="row-even">
 *              <td align="right"><code>FA_MICROCHIP</code></td>
 *              <td align="center"><span class="fa-microchip"></span></td>
 *            </tr>
 *            <tr class="row-odd">
 *              <td align="right"><code>FA_MICROPHONE</code></td>
 *              <td align="center"><span class="fa-microphone"></span></td>
 *            </tr>
 *            <tr class="row-even">
 *              <td align="right"><code>FA_MICROPHONE_ALT</code></td>
 *              <td align="center"><span class="fa-microphone-alt"></span></td>
 *            </tr>
 *            <tr class="row-odd">
 *              <td align="right"><code>FA_MICROPHONE_ALT_SLASH</code></td>
 *              <td align="center"><span class="fa-microphone-alt-slash"></span></td>
 *            </tr>
 *            <tr class="row-even">
 *              <td align="right"><code>FA_MICROPHONE_SLASH</code></td>
 *              <td align="center"><span class="fa-microphone-slash"></span></td>
 *            </tr>
 *            <tr class="row-odd">
 *              <td align="right"><code>FA_MICROSCOPE</code></td>
 *              <td align="center"><span class="fa-microscope"></span></td>
 *            </tr>
 *            <tr class="row-even">
 *              <td align="right"><code>FA_MINUS</code></td>
 *              <td align="center"><span class="fa-minus"></span></td>
 *            </tr>
 *            <tr class="row-odd">
 *              <td align="right"><code>FA_MINUS_CIRCLE</code></td>
 *              <td align="center"><span class="fa-minus-circle"></span></td>
 *            </tr>
 *            <tr class="row-even">
 *              <td align="right"><code>FA_MINUS_SQUARE</code></td>
 *              <td align="center"><span class="fa-minus-square"></span></td>
 *            </tr>
 *            <tr class="row-odd">
 *              <td align="right"><code>FA_MITTEN</code></td>
 *              <td align="center"><span class="fa-mitten"></span></td>
 *            </tr>
 *            <tr class="row-even">
 *              <td align="right"><code>FA_MOBILE</code></td>
 *              <td align="center"><span class="fa-mobile"></span></td>
 *            </tr>
 *            <tr class="row-odd">
 *              <td align="right"><code>FA_MOBILE_ALT</code></td>
 *              <td align="center"><span class="fa-mobile-alt"></span></td>
 *            </tr>
 *            <tr class="row-even">
 *              <td align="right"><code>FA_MONEY_BILL</code></td>
 *              <td align="center"><span class="fa-money-bill"></span></td>
 *            </tr>
 *            <tr class="row-odd">
 *              <td align="right"><code>FA_MONEY_BILL_ALT</code></td>
 *              <td align="center"><span class="fa-money-bill-alt"></span></td>
 *            </tr>
 *            <tr class="row-even">
 *              <td align="right"><code>FA_MONEY_BILL_WAVE</code></td>
 *              <td align="center"><span class="fa-money-bill-wave"></span></td>
 *            </tr>
 *            <tr class="row-odd">
 *              <td align="right"><code>FA_MONEY_BILL_WAVE_ALT</code></td>
 *              <td align="center"><span class="fa-money-bill-wave-alt"></span></td>
 *            </tr>
 *            <tr class="row-even">
 *              <td align="right"><code>FA_MONEY_CHECK</code></td>
 *              <td align="center"><span class="fa-money-check"></span></td>
 *            </tr>
 *            <tr class="row-odd">
 *              <td align="right"><code>FA_MONEY_CHECK_ALT</code></td>
 *              <td align="center"><span class="fa-money-check-alt"></span></td>
 *            </tr>
 *            <tr class="row-even">
 *              <td align="right"><code>FA_MONUMENT</code></td>
 *              <td align="center"><span class="fa-monument"></span></td>
 *            </tr>
 *            <tr class="row-odd">
 *              <td align="right"><code>FA_MOON</code></td>
 *              <td align="center"><span class="fa-moon"></span></td>
 *            </tr>
 *            <tr class="row-even">
 *              <td align="right"><code>FA_MORTAR_PESTLE</code></td>
 *              <td align="center"><span class="fa-mortar-pestle"></span></td>
 *            </tr>
 *            <tr class="row-odd">
 *              <td align="right"><code>FA_MOSQUE</code></td>
 *              <td align="center"><span class="fa-mosque"></span></td>
 *            </tr>
 *            <tr class="row-even">
 *              <td align="right"><code>FA_MOTORCYCLE</code></td>
 *              <td align="center"><span class="fa-motorcycle"></span></td>
 *            </tr>
 *            <tr class="row-odd">
 *              <td align="right"><code>FA_MOUNTAIN</code></td>
 *              <td align="center"><span class="fa-mountain"></span></td>
 *            </tr>
 *            <tr class="row-even">
 *              <td align="right"><code>FA_MOUSE_POINTER</code></td>
 *              <td align="center"><span class="fa-mouse-pointer"></span></td>
 *            </tr>
 *            <tr class="row-odd">
 *              <td align="right"><code>FA_MUG_HOT</code></td>
 *              <td align="center"><span class="fa-mug-hot"></span></td>
 *            </tr>
 *            <tr class="row-even">
 *              <td align="right"><code>FA_MUSIC</code></td>
 *              <td align="center"><span class="fa-music"></span></td>
 *            </tr>
 *            <tr class="row-odd">
 *              <td align="right"><code>FA_NETWORK_WIRED</code></td>
 *              <td align="center"><span class="fa-network-wired"></span></td>
 *            </tr>
 *            <tr class="row-even">
 *              <td align="right"><code>FA_NEUTER</code></td>
 *              <td align="center"><span class="fa-neuter"></span></td>
 *            </tr>
 *            <tr class="row-odd">
 *              <td align="right"><code>FA_NEWSPAPER</code></td>
 *              <td align="center"><span class="fa-newspaper"></span></td>
 *            </tr>
 *            <tr class="row-even">
 *              <td align="right"><code>FA_NOT_EQUAL</code></td>
 *              <td align="center"><span class="fa-not-equal"></span></td>
 *            </tr>
 *            <tr class="row-odd">
 *              <td align="right"><code>FA_NOTES_MEDICAL</code></td>
 *              <td align="center"><span class="fa-notes-medical"></span></td>
 *            </tr>
 *            <tr class="row-even">
 *              <td align="right"><code>FA_OBJECT_GROUP</code></td>
 *              <td align="center"><span class="fa-object-group"></span></td>
 *            </tr>
 *            <tr class="row-odd">
 *              <td align="right"><code>FA_OBJECT_UNGROUP</code></td>
 *              <td align="center"><span class="fa-object-ungroup"></span></td>
 *            </tr>
 *            <tr class="row-even">
 *              <td align="right"><code>FA_OIL_CAN</code></td>
 *              <td align="center"><span class="fa-oil-can"></span></td>
 *            </tr>
 *            <tr class="row-odd">
 *              <td align="right"><code>FA_OM</code></td>
 *              <td align="center"><span class="fa-om"></span></td>
 *            </tr>
 *            <tr class="row-even">
 *              <td align="right"><code>FA_OTTER</code></td>
 *              <td align="center"><span class="fa-otter"></span></td>
 *            </tr>
 *            <tr class="row-odd">
 *              <td align="right"><code>FA_OUTDENT</code></td>
 *              <td align="center"><span class="fa-outdent"></span></td>
 *            </tr>
 *            <tr class="row-even">
 *              <td align="right"><code>FA_PAGER</code></td>
 *              <td align="center"><span class="fa-pager"></span></td>
 *            </tr>
 *            <tr class="row-odd">
 *              <td align="right"><code>FA_PAINT_BRUSH</code></td>
 *              <td align="center"><span class="fa-paint-brush"></span></td>
 *            </tr>
 *            <tr class="row-even">
 *              <td align="right"><code>FA_PAINT_ROLLER</code></td>
 *              <td align="center"><span class="fa-paint-roller"></span></td>
 *            </tr>
 *            <tr class="row-odd">
 *              <td align="right"><code>FA_PALETTE</code></td>
 *              <td align="center"><span class="fa-palette"></span></td>
 *            </tr>
 *            <tr class="row-even">
 *              <td align="right"><code>FA_PALLET</code></td>
 *              <td align="center"><span class="fa-pallet"></span></td>
 *            </tr>
 *            <tr class="row-odd">
 *              <td align="right"><code>FA_PAPER_PLANE</code></td>
 *              <td align="center"><span class="fa-paper-plane"></span></td>
 *            </tr>
 *            <tr class="row-even">
 *              <td align="right"><code>FA_PAPERCLIP</code></td>
 *              <td align="center"><span class="fa-paperclip"></span></td>
 *            </tr>
 *            <tr class="row-odd">
 *              <td align="right"><code>FA_PARACHUTE_BOX</code></td>
 *              <td align="center"><span class="fa-parachute-box"></span></td>
 *            </tr>
 *            <tr class="row-even">
 *              <td align="right"><code>FA_PARAGRAPH</code></td>
 *              <td align="center"><span class="fa-paragraph"></span></td>
 *            </tr>
 *            <tr class="row-odd">
 *              <td align="right"><code>FA_PARKING</code></td>
 *              <td align="center"><span class="fa-parking"></span></td>
 *            </tr>
 *            <tr class="row-even">
 *              <td align="right"><code>FA_PASSPORT</code></td>
 *              <td align="center"><span class="fa-passport"></span></td>
 *            </tr>
 *            <tr class="row-odd">
 *              <td align="right"><code>FA_PASTAFARIANISM</code></td>
 *              <td align="center"><span class="fa-pastafarianism"></span></td>
 *            </tr>
 *            <tr class="row-even">
 *              <td align="right"><code>FA_PASTE</code></td>
 *              <td align="center"><span class="fa-paste"></span></td>
 *            </tr>
 *            <tr class="row-odd">
 *              <td align="right"><code>FA_PAUSE</code></td>
 *              <td align="center"><span class="fa-pause"></span></td>
 *            </tr>
 *            <tr class="row-even">
 *              <td align="right"><code>FA_PAUSE_CIRCLE</code></td>
 *              <td align="center"><span class="fa-pause-circle"></span></td>
 *            </tr>
 *            <tr class="row-odd">
 *              <td align="right"><code>FA_PAW</code></td>
 *              <td align="center"><span class="fa-paw"></span></td>
 *            </tr>
 *            <tr class="row-even">
 *              <td align="right"><code>FA_PEACE</code></td>
 *              <td align="center"><span class="fa-peace"></span></td>
 *            </tr>
 *            <tr class="row-odd">
 *              <td align="right"><code>FA_PEN</code></td>
 *              <td align="center"><span class="fa-pen"></span></td>
 *            </tr>
 *            <tr class="row-even">
 *              <td align="right"><code>FA_PEN_ALT</code></td>
 *              <td align="center"><span class="fa-pen-alt"></span></td>
 *            </tr>
 *            <tr class="row-odd">
 *              <td align="right"><code>FA_PEN_FANCY</code></td>
 *              <td align="center"><span class="fa-pen-fancy"></span></td>
 *            </tr>
 *            <tr class="row-even">
 *              <td align="right"><code>FA_PEN_NIB</code></td>
 *              <td align="center"><span class="fa-pen-nib"></span></td>
 *            </tr>
 *            <tr class="row-odd">
 *              <td align="right"><code>FA_PEN_SQUARE</code></td>
 *              <td align="center"><span class="fa-pen-square"></span></td>
 *            </tr>
 *            <tr class="row-even">
 *              <td align="right"><code>FA_PENCIL_ALT</code></td>
 *              <td align="center"><span class="fa-pencil-alt"></span></td>
 *            </tr>
 *            <tr class="row-odd">
 *              <td align="right"><code>FA_PENCIL_RULER</code></td>
 *              <td align="center"><span class="fa-pencil-ruler"></span></td>
 *            </tr>
 *            <tr class="row-even">
 *              <td align="right"><code>FA_PEOPLE_CARRY</code></td>
 *              <td align="center"><span class="fa-people-carry"></span></td>
 *            </tr>
 *            <tr class="row-odd">
 *              <td align="right"><code>FA_PEPPER_HOT</code></td>
 *              <td align="center"><span class="fa-pepper-hot"></span></td>
 *            </tr>
 *            <tr class="row-even">
 *              <td align="right"><code>FA_PERCENT</code></td>
 *              <td align="center"><span class="fa-percent"></span></td>
 *            </tr>
 *            <tr class="row-odd">
 *              <td align="right"><code>FA_PERCENTAGE</code></td>
 *              <td align="center"><span class="fa-percentage"></span></td>
 *            </tr>
 *            <tr class="row-even">
 *              <td align="right"><code>FA_PERSON_BOOTH</code></td>
 *              <td align="center"><span class="fa-person-booth"></span></td>
 *            </tr>
 *            <tr class="row-odd">
 *              <td align="right"><code>FA_PHONE</code></td>
 *              <td align="center"><span class="fa-phone"></span></td>
 *            </tr>
 *            <tr class="row-even">
 *              <td align="right"><code>FA_PHONE_ALT</code></td>
 *              <td align="center"><span class="fa-phone-alt"></span></td>
 *            </tr>
 *            <tr class="row-odd">
 *              <td align="right"><code>FA_PHONE_SLASH</code></td>
 *              <td align="center"><span class="fa-phone-slash"></span></td>
 *            </tr>
 *            <tr class="row-even">
 *              <td align="right"><code>FA_PHONE_SQUARE</code></td>
 *              <td align="center"><span class="fa-phone-square"></span></td>
 *            </tr>
 *            <tr class="row-odd">
 *              <td align="right"><code>FA_PHONE_SQUARE_ALT</code></td>
 *              <td align="center"><span class="fa-phone-square-alt"></span></td>
 *            </tr>
 *            <tr class="row-even">
 *              <td align="right"><code>FA_PHONE_VOLUME</code></td>
 *              <td align="center"><span class="fa-phone-volume"></span></td>
 *            </tr>
 *            <tr class="row-odd">
 *              <td align="right"><code>FA_PHOTO_VIDEO</code></td>
 *              <td align="center"><span class="fa-photo-video"></span></td>
 *            </tr>
 *            <tr class="row-even">
 *              <td align="right"><code>FA_PIGGY_BANK</code></td>
 *              <td align="center"><span class="fa-piggy-bank"></span></td>
 *            </tr>
 *            <tr class="row-odd">
 *              <td align="right"><code>FA_PILLS</code></td>
 *              <td align="center"><span class="fa-pills"></span></td>
 *            </tr>
 *            <tr class="row-even">
 *              <td align="right"><code>FA_PIZZA_SLICE</code></td>
 *              <td align="center"><span class="fa-pizza-slice"></span></td>
 *            </tr>
 *            <tr class="row-odd">
 *              <td align="right"><code>FA_PLACE_OF_WORSHIP</code></td>
 *              <td align="center"><span class="fa-place-of-worship"></span></td>
 *            </tr>
 *            <tr class="row-even">
 *              <td align="right"><code>FA_PLANE</code></td>
 *              <td align="center"><span class="fa-plane"></span></td>
 *            </tr>
 *            <tr class="row-odd">
 *              <td align="right"><code>FA_PLANE_ARRIVAL</code></td>
 *              <td align="center"><span class="fa-plane-arrival"></span></td>
 *            </tr>
 *            <tr class="row-even">
 *              <td align="right"><code>FA_PLANE_DEPARTURE</code></td>
 *              <td align="center"><span class="fa-plane-departure"></span></td>
 *            </tr>
 *            <tr class="row-odd">
 *              <td align="right"><code>FA_PLAY</code></td>
 *              <td align="center"><span class="fa-play"></span></td>
 *            </tr>
 *            <tr class="row-even">
 *              <td align="right"><code>FA_PLAY_CIRCLE</code></td>
 *              <td align="center"><span class="fa-play-circle"></span></td>
 *            </tr>
 *            <tr class="row-odd">
 *              <td align="right"><code>FA_PLUG</code></td>
 *              <td align="center"><span class="fa-plug"></span></td>
 *            </tr>
 *            <tr class="row-even">
 *              <td align="right"><code>FA_PLUS</code></td>
 *              <td align="center"><span class="fa-plus"></span></td>
 *            </tr>
 *            <tr class="row-odd">
 *              <td align="right"><code>FA_PLUS_CIRCLE</code></td>
 *              <td align="center"><span class="fa-plus-circle"></span></td>
 *            </tr>
 *            <tr class="row-even">
 *              <td align="right"><code>FA_PLUS_SQUARE</code></td>
 *              <td align="center"><span class="fa-plus-square"></span></td>
 *            </tr>
 *            <tr class="row-odd">
 *              <td align="right"><code>FA_PODCAST</code></td>
 *              <td align="center"><span class="fa-podcast"></span></td>
 *            </tr>
 *            <tr class="row-even">
 *              <td align="right"><code>FA_POLL</code></td>
 *              <td align="center"><span class="fa-poll"></span></td>
 *            </tr>
 *            <tr class="row-odd">
 *              <td align="right"><code>FA_POLL_H</code></td>
 *              <td align="center"><span class="fa-poll-h"></span></td>
 *            </tr>
 *            <tr class="row-even">
 *              <td align="right"><code>FA_POO</code></td>
 *              <td align="center"><span class="fa-poo"></span></td>
 *            </tr>
 *            <tr class="row-odd">
 *              <td align="right"><code>FA_POO_STORM</code></td>
 *              <td align="center"><span class="fa-poo-storm"></span></td>
 *            </tr>
 *            <tr class="row-even">
 *              <td align="right"><code>FA_POOP</code></td>
 *              <td align="center"><span class="fa-poop"></span></td>
 *            </tr>
 *            <tr class="row-odd">
 *              <td align="right"><code>FA_PORTRAIT</code></td>
 *              <td align="center"><span class="fa-portrait"></span></td>
 *            </tr>
 *            <tr class="row-even">
 *              <td align="right"><code>FA_POUND_SIGN</code></td>
 *              <td align="center"><span class="fa-pound-sign"></span></td>
 *            </tr>
 *            <tr class="row-odd">
 *              <td align="right"><code>FA_POWER_OFF</code></td>
 *              <td align="center"><span class="fa-power-off"></span></td>
 *            </tr>
 *            <tr class="row-even">
 *              <td align="right"><code>FA_PRAY</code></td>
 *              <td align="center"><span class="fa-pray"></span></td>
 *            </tr>
 *            <tr class="row-odd">
 *              <td align="right"><code>FA_PRAYING_HANDS</code></td>
 *              <td align="center"><span class="fa-praying-hands"></span></td>
 *            </tr>
 *            <tr class="row-even">
 *              <td align="right"><code>FA_PRESCRIPTION</code></td>
 *              <td align="center"><span class="fa-prescription"></span></td>
 *            </tr>
 *            <tr class="row-odd">
 *              <td align="right"><code>FA_PRESCRIPTION_BOTTLE</code></td>
 *              <td align="center"><span class="fa-prescription-bottle"></span></td>
 *            </tr>
 *            <tr class="row-even">
 *              <td align="right"><code>FA_PRESCRIPTION_BOTTLE_ALT</code></td>
 *              <td align="center"><span class="fa-prescription-bottle-alt"></span></td>
 *            </tr>
 *            <tr class="row-odd">
 *              <td align="right"><code>FA_PRINT</code></td>
 *              <td align="center"><span class="fa-print"></span></td>
 *            </tr>
 *            <tr class="row-even">
 *              <td align="right"><code>FA_PROCEDURES</code></td>
 *              <td align="center"><span class="fa-procedures"></span></td>
 *            </tr>
 *            <tr class="row-odd">
 *              <td align="right"><code>FA_PROJECT_DIAGRAM</code></td>
 *              <td align="center"><span class="fa-project-diagram"></span></td>
 *            </tr>
 *            <tr class="row-even">
 *              <td align="right"><code>FA_PUZZLE_PIECE</code></td>
 *              <td align="center"><span class="fa-puzzle-piece"></span></td>
 *            </tr>
 *            <tr class="row-odd">
 *              <td align="right"><code>FA_QRCODE</code></td>
 *              <td align="center"><span class="fa-qrcode"></span></td>
 *            </tr>
 *            <tr class="row-even">
 *              <td align="right"><code>FA_QUESTION</code></td>
 *              <td align="center"><span class="fa-question"></span></td>
 *            </tr>
 *            <tr class="row-odd">
 *              <td align="right"><code>FA_QUESTION_CIRCLE</code></td>
 *              <td align="center"><span class="fa-question-circle"></span></td>
 *            </tr>
 *            <tr class="row-even">
 *              <td align="right"><code>FA_QUIDDITCH</code></td>
 *              <td align="center"><span class="fa-quidditch"></span></td>
 *            </tr>
 *            <tr class="row-odd">
 *              <td align="right"><code>FA_QUOTE_LEFT</code></td>
 *              <td align="center"><span class="fa-quote-left"></span></td>
 *            </tr>
 *            <tr class="row-even">
 *              <td align="right"><code>FA_QUOTE_RIGHT</code></td>
 *              <td align="center"><span class="fa-quote-right"></span></td>
 *            </tr>
 *            <tr class="row-odd">
 *              <td align="right"><code>FA_QURAN</code></td>
 *              <td align="center"><span class="fa-quran"></span></td>
 *            </tr>
 *            <tr class="row-even">
 *              <td align="right"><code>FA_RADIATION</code></td>
 *              <td align="center"><span class="fa-radiation"></span></td>
 *            </tr>
 *            <tr class="row-odd">
 *              <td align="right"><code>FA_RADIATION_ALT</code></td>
 *              <td align="center"><span class="fa-radiation-alt"></span></td>
 *            </tr>
 *            <tr class="row-even">
 *              <td align="right"><code>FA_RAINBOW</code></td>
 *              <td align="center"><span class="fa-rainbow"></span></td>
 *            </tr>
 *            <tr class="row-odd">
 *              <td align="right"><code>FA_RANDOM</code></td>
 *              <td align="center"><span class="fa-random"></span></td>
 *            </tr>
 *            <tr class="row-even">
 *              <td align="right"><code>FA_RECEIPT</code></td>
 *              <td align="center"><span class="fa-receipt"></span></td>
 *            </tr>
 *            <tr class="row-odd">
 *              <td align="right"><code>FA_RECYCLE</code></td>
 *              <td align="center"><span class="fa-recycle"></span></td>
 *            </tr>
 *            <tr class="row-even">
 *              <td align="right"><code>FA_REDO</code></td>
 *              <td align="center"><span class="fa-redo"></span></td>
 *            </tr>
 *            <tr class="row-odd">
 *              <td align="right"><code>FA_REDO_ALT</code></td>
 *              <td align="center"><span class="fa-redo-alt"></span></td>
 *            </tr>
 *            <tr class="row-even">
 *              <td align="right"><code>FA_REGISTERED</code></td>
 *              <td align="center"><span class="fa-registered"></span></td>
 *            </tr>
 *            <tr class="row-odd">
 *              <td align="right"><code>FA_REMOVE_FORMAT</code></td>
 *              <td align="center"><span class="fa-remove-format"></span></td>
 *            </tr>
 *            <tr class="row-even">
 *              <td align="right"><code>FA_REPLY</code></td>
 *              <td align="center"><span class="fa-reply"></span></td>
 *            </tr>
 *            <tr class="row-odd">
 *              <td align="right"><code>FA_REPLY_ALL</code></td>
 *              <td align="center"><span class="fa-reply-all"></span></td>
 *            </tr>
 *            <tr class="row-even">
 *              <td align="right"><code>FA_REPUBLICAN</code></td>
 *              <td align="center"><span class="fa-republican"></span></td>
 *            </tr>
 *            <tr class="row-odd">
 *              <td align="right"><code>FA_RESTROOM</code></td>
 *              <td align="center"><span class="fa-restroom"></span></td>
 *            </tr>
 *            <tr class="row-even">
 *              <td align="right"><code>FA_RETWEET</code></td>
 *              <td align="center"><span class="fa-retweet"></span></td>
 *            </tr>
 *            <tr class="row-odd">
 *              <td align="right"><code>FA_RIBBON</code></td>
 *              <td align="center"><span class="fa-ribbon"></span></td>
 *            </tr>
 *            <tr class="row-even">
 *              <td align="right"><code>FA_RING</code></td>
 *              <td align="center"><span class="fa-ring"></span></td>
 *            </tr>
 *            <tr class="row-odd">
 *              <td align="right"><code>FA_ROAD</code></td>
 *              <td align="center"><span class="fa-road"></span></td>
 *            </tr>
 *            <tr class="row-even">
 *              <td align="right"><code>FA_ROBOT</code></td>
 *              <td align="center"><span class="fa-robot"></span></td>
 *            </tr>
 *            <tr class="row-odd">
 *              <td align="right"><code>FA_ROCKET</code></td>
 *              <td align="center"><span class="fa-rocket"></span></td>
 *            </tr>
 *            <tr class="row-even">
 *              <td align="right"><code>FA_ROUTE</code></td>
 *              <td align="center"><span class="fa-route"></span></td>
 *            </tr>
 *            <tr class="row-odd">
 *              <td align="right"><code>FA_RSS</code></td>
 *              <td align="center"><span class="fa-rss"></span></td>
 *            </tr>
 *            <tr class="row-even">
 *              <td align="right"><code>FA_RSS_SQUARE</code></td>
 *              <td align="center"><span class="fa-rss-square"></span></td>
 *            </tr>
 *            <tr class="row-odd">
 *              <td align="right"><code>FA_RUBLE_SIGN</code></td>
 *              <td align="center"><span class="fa-ruble-sign"></span></td>
 *            </tr>
 *            <tr class="row-even">
 *              <td align="right"><code>FA_RULER</code></td>
 *              <td align="center"><span class="fa-ruler"></span></td>
 *            </tr>
 *            <tr class="row-odd">
 *              <td align="right"><code>FA_RULER_COMBINED</code></td>
 *              <td align="center"><span class="fa-ruler-combined"></span></td>
 *            </tr>
 *            <tr class="row-even">
 *              <td align="right"><code>FA_RULER_HORIZONTAL</code></td>
 *              <td align="center"><span class="fa-ruler-horizontal"></span></td>
 *            </tr>
 *            <tr class="row-odd">
 *              <td align="right"><code>FA_RULER_VERTICAL</code></td>
 *              <td align="center"><span class="fa-ruler-vertical"></span></td>
 *            </tr>
 *            <tr class="row-even">
 *              <td align="right"><code>FA_RUNNING</code></td>
 *              <td align="center"><span class="fa-running"></span></td>
 *            </tr>
 *            <tr class="row-odd">
 *              <td align="right"><code>FA_RUPEE_SIGN</code></td>
 *              <td align="center"><span class="fa-rupee-sign"></span></td>
 *            </tr>
 *            <tr class="row-even">
 *              <td align="right"><code>FA_SAD_CRY</code></td>
 *              <td align="center"><span class="fa-sad-cry"></span></td>
 *            </tr>
 *            <tr class="row-odd">
 *              <td align="right"><code>FA_SAD_TEAR</code></td>
 *              <td align="center"><span class="fa-sad-tear"></span></td>
 *            </tr>
 *            <tr class="row-even">
 *              <td align="right"><code>FA_SATELLITE</code></td>
 *              <td align="center"><span class="fa-satellite"></span></td>
 *            </tr>
 *            <tr class="row-odd">
 *              <td align="right"><code>FA_SATELLITE_DISH</code></td>
 *              <td align="center"><span class="fa-satellite-dish"></span></td>
 *            </tr>
 *            <tr class="row-even">
 *              <td align="right"><code>FA_SAVE</code></td>
 *              <td align="center"><span class="fa-save"></span></td>
 *            </tr>
 *            <tr class="row-odd">
 *              <td align="right"><code>FA_SCHOOL</code></td>
 *              <td align="center"><span class="fa-school"></span></td>
 *            </tr>
 *            <tr class="row-even">
 *              <td align="right"><code>FA_SCREWDRIVER</code></td>
 *              <td align="center"><span class="fa-screwdriver"></span></td>
 *            </tr>
 *            <tr class="row-odd">
 *              <td align="right"><code>FA_SCROLL</code></td>
 *              <td align="center"><span class="fa-scroll"></span></td>
 *            </tr>
 *            <tr class="row-even">
 *              <td align="right"><code>FA_SD_CARD</code></td>
 *              <td align="center"><span class="fa-sd-card"></span></td>
 *            </tr>
 *            <tr class="row-odd">
 *              <td align="right"><code>FA_SEARCH</code></td>
 *              <td align="center"><span class="fa-search"></span></td>
 *            </tr>
 *            <tr class="row-even">
 *              <td align="right"><code>FA_SEARCH_DOLLAR</code></td>
 *              <td align="center"><span class="fa-search-dollar"></span></td>
 *            </tr>
 *            <tr class="row-odd">
 *              <td align="right"><code>FA_SEARCH_LOCATION</code></td>
 *              <td align="center"><span class="fa-search-location"></span></td>
 *            </tr>
 *            <tr class="row-even">
 *              <td align="right"><code>FA_SEARCH_MINUS</code></td>
 *              <td align="center"><span class="fa-search-minus"></span></td>
 *            </tr>
 *            <tr class="row-odd">
 *              <td align="right"><code>FA_SEARCH_PLUS</code></td>
 *              <td align="center"><span class="fa-search-plus"></span></td>
 *            </tr>
 *            <tr class="row-even">
 *              <td align="right"><code>FA_SEEDLING</code></td>
 *              <td align="center"><span class="fa-seedling"></span></td>
 *            </tr>
 *            <tr class="row-odd">
 *              <td align="right"><code>FA_SERVER</code></td>
 *              <td align="center"><span class="fa-server"></span></td>
 *            </tr>
 *            <tr class="row-even">
 *              <td align="right"><code>FA_SHAPES</code></td>
 *              <td align="center"><span class="fa-shapes"></span></td>
 *            </tr>
 *            <tr class="row-odd">
 *              <td align="right"><code>FA_SHARE</code></td>
 *              <td align="center"><span class="fa-share"></span></td>
 *            </tr>
 *            <tr class="row-even">
 *              <td align="right"><code>FA_SHARE_ALT</code></td>
 *              <td align="center"><span class="fa-share-alt"></span></td>
 *            </tr>
 *            <tr class="row-odd">
 *              <td align="right"><code>FA_SHARE_ALT_SQUARE</code></td>
 *              <td align="center"><span class="fa-share-alt-square"></span></td>
 *            </tr>
 *            <tr class="row-even">
 *              <td align="right"><code>FA_SHARE_SQUARE</code></td>
 *              <td align="center"><span class="fa-share-square"></span></td>
 *            </tr>
 *            <tr class="row-odd">
 *              <td align="right"><code>FA_SHEKEL_SIGN</code></td>
 *              <td align="center"><span class="fa-shekel-sign"></span></td>
 *            </tr>
 *            <tr class="row-even">
 *              <td align="right"><code>FA_SHIELD_ALT</code></td>
 *              <td align="center"><span class="fa-shield-alt"></span></td>
 *            </tr>
 *            <tr class="row-odd">
 *              <td align="right"><code>FA_SHIP</code></td>
 *              <td align="center"><span class="fa-ship"></span></td>
 *            </tr>
 *            <tr class="row-even">
 *              <td align="right"><code>FA_SHIPPING_FAST</code></td>
 *              <td align="center"><span class="fa-shipping-fast"></span></td>
 *            </tr>
 *            <tr class="row-odd">
 *              <td align="right"><code>FA_SHOE_PRINTS</code></td>
 *              <td align="center"><span class="fa-shoe-prints"></span></td>
 *            </tr>
 *            <tr class="row-even">
 *              <td align="right"><code>FA_SHOPPING_BAG</code></td>
 *              <td align="center"><span class="fa-shopping-bag"></span></td>
 *            </tr>
 *            <tr class="row-odd">
 *              <td align="right"><code>FA_SHOPPING_BASKET</code></td>
 *              <td align="center"><span class="fa-shopping-basket"></span></td>
 *            </tr>
 *            <tr class="row-even">
 *              <td align="right"><code>FA_SHOPPING_CART</code></td>
 *              <td align="center"><span class="fa-shopping-cart"></span></td>
 *            </tr>
 *            <tr class="row-odd">
 *              <td align="right"><code>FA_SHOWER</code></td>
 *              <td align="center"><span class="fa-shower"></span></td>
 *            </tr>
 *            <tr class="row-even">
 *              <td align="right"><code>FA_SHUTTLE_VAN</code></td>
 *              <td align="center"><span class="fa-shuttle-van"></span></td>
 *            </tr>
 *            <tr class="row-odd">
 *              <td align="right"><code>FA_SIGN</code></td>
 *              <td align="center"><span class="fa-sign"></span></td>
 *            </tr>
 *            <tr class="row-even">
 *              <td align="right"><code>FA_SIGN_IN_ALT</code></td>
 *              <td align="center"><span class="fa-sign-in-alt"></span></td>
 *            </tr>
 *            <tr class="row-odd">
 *              <td align="right"><code>FA_SIGN_LANGUAGE</code></td>
 *              <td align="center"><span class="fa-sign-language"></span></td>
 *            </tr>
 *            <tr class="row-even">
 *              <td align="right"><code>FA_SIGN_OUT_ALT</code></td>
 *              <td align="center"><span class="fa-sign-out-alt"></span></td>
 *            </tr>
 *            <tr class="row-odd">
 *              <td align="right"><code>FA_SIGNAL</code></td>
 *              <td align="center"><span class="fa-signal"></span></td>
 *            </tr>
 *            <tr class="row-even">
 *              <td align="right"><code>FA_SIGNATURE</code></td>
 *              <td align="center"><span class="fa-signature"></span></td>
 *            </tr>
 *            <tr class="row-odd">
 *              <td align="right"><code>FA_SIM_CARD</code></td>
 *              <td align="center"><span class="fa-sim-card"></span></td>
 *            </tr>
 *            <tr class="row-even">
 *              <td align="right"><code>FA_SITEMAP</code></td>
 *              <td align="center"><span class="fa-sitemap"></span></td>
 *            </tr>
 *            <tr class="row-odd">
 *              <td align="right"><code>FA_SKATING</code></td>
 *              <td align="center"><span class="fa-skating"></span></td>
 *            </tr>
 *            <tr class="row-even">
 *              <td align="right"><code>FA_SKIING</code></td>
 *              <td align="center"><span class="fa-skiing"></span></td>
 *            </tr>
 *            <tr class="row-odd">
 *              <td align="right"><code>FA_SKIING_NORDIC</code></td>
 *              <td align="center"><span class="fa-skiing-nordic"></span></td>
 *            </tr>
 *            <tr class="row-even">
 *              <td align="right"><code>FA_SKULL</code></td>
 *              <td align="center"><span class="fa-skull"></span></td>
 *            </tr>
 *            <tr class="row-odd">
 *              <td align="right"><code>FA_SKULL_CROSSBONES</code></td>
 *              <td align="center"><span class="fa-skull-crossbones"></span></td>
 *            </tr>
 *            <tr class="row-even">
 *              <td align="right"><code>FA_SLASH</code></td>
 *              <td align="center"><span class="fa-slash"></span></td>
 *            </tr>
 *            <tr class="row-odd">
 *              <td align="right"><code>FA_SLEIGH</code></td>
 *              <td align="center"><span class="fa-sleigh"></span></td>
 *            </tr>
 *            <tr class="row-even">
 *              <td align="right"><code>FA_SLIDERS_H</code></td>
 *              <td align="center"><span class="fa-sliders-h"></span></td>
 *            </tr>
 *            <tr class="row-odd">
 *              <td align="right"><code>FA_SMILE</code></td>
 *              <td align="center"><span class="fa-smile"></span></td>
 *            </tr>
 *            <tr class="row-even">
 *              <td align="right"><code>FA_SMILE_BEAM</code></td>
 *              <td align="center"><span class="fa-smile-beam"></span></td>
 *            </tr>
 *            <tr class="row-odd">
 *              <td align="right"><code>FA_SMILE_WINK</code></td>
 *              <td align="center"><span class="fa-smile-wink"></span></td>
 *            </tr>
 *            <tr class="row-even">
 *              <td align="right"><code>FA_SMOG</code></td>
 *              <td align="center"><span class="fa-smog"></span></td>
 *            </tr>
 *            <tr class="row-odd">
 *              <td align="right"><code>FA_SMOKING</code></td>
 *              <td align="center"><span class="fa-smoking"></span></td>
 *            </tr>
 *            <tr class="row-even">
 *              <td align="right"><code>FA_SMOKING_BAN</code></td>
 *              <td align="center"><span class="fa-smoking-ban"></span></td>
 *            </tr>
 *            <tr class="row-odd">
 *              <td align="right"><code>FA_SMS</code></td>
 *              <td align="center"><span class="fa-sms"></span></td>
 *            </tr>
 *            <tr class="row-even">
 *              <td align="right"><code>FA_SNOWBOARDING</code></td>
 *              <td align="center"><span class="fa-snowboarding"></span></td>
 *            </tr>
 *            <tr class="row-odd">
 *              <td align="right"><code>FA_SNOWFLAKE</code></td>
 *              <td align="center"><span class="fa-snowflake"></span></td>
 *            </tr>
 *            <tr class="row-even">
 *              <td align="right"><code>FA_SNOWMAN</code></td>
 *              <td align="center"><span class="fa-snowman"></span></td>
 *            </tr>
 *            <tr class="row-odd">
 *              <td align="right"><code>FA_SNOWPLOW</code></td>
 *              <td align="center"><span class="fa-snowplow"></span></td>
 *            </tr>
 *            <tr class="row-even">
 *              <td align="right"><code>FA_SOCKS</code></td>
 *              <td align="center"><span class="fa-socks"></span></td>
 *            </tr>
 *            <tr class="row-odd">
 *              <td align="right"><code>FA_SOLAR_PANEL</code></td>
 *              <td align="center"><span class="fa-solar-panel"></span></td>
 *            </tr>
 *            <tr class="row-even">
 *              <td align="right"><code>FA_SORT</code></td>
 *              <td align="center"><span class="fa-sort"></span></td>
 *            </tr>
 *            <tr class="row-odd">
 *              <td align="right"><code>FA_SORT_ALPHA_DOWN</code></td>
 *              <td align="center"><span class="fa-sort-alpha-down"></span></td>
 *            </tr>
 *            <tr class="row-even">
 *              <td align="right"><code>FA_SORT_ALPHA_DOWN_ALT</code></td>
 *              <td align="center"><span class="fa-sort-alpha-down-alt"></span></td>
 *            </tr>
 *            <tr class="row-odd">
 *              <td align="right"><code>FA_SORT_ALPHA_UP</code></td>
 *              <td align="center"><span class="fa-sort-alpha-up"></span></td>
 *            </tr>
 *            <tr class="row-even">
 *              <td align="right"><code>FA_SORT_ALPHA_UP_ALT</code></td>
 *              <td align="center"><span class="fa-sort-alpha-up-alt"></span></td>
 *            </tr>
 *            <tr class="row-odd">
 *              <td align="right"><code>FA_SORT_AMOUNT_DOWN</code></td>
 *              <td align="center"><span class="fa-sort-amount-down"></span></td>
 *            </tr>
 *            <tr class="row-even">
 *              <td align="right"><code>FA_SORT_AMOUNT_DOWN_ALT</code></td>
 *              <td align="center"><span class="fa-sort-amount-down-alt"></span></td>
 *            </tr>
 *            <tr class="row-odd">
 *              <td align="right"><code>FA_SORT_AMOUNT_UP</code></td>
 *              <td align="center"><span class="fa-sort-amount-up"></span></td>
 *            </tr>
 *            <tr class="row-even">
 *              <td align="right"><code>FA_SORT_AMOUNT_UP_ALT</code></td>
 *              <td align="center"><span class="fa-sort-amount-up-alt"></span></td>
 *            </tr>
 *            <tr class="row-odd">
 *              <td align="right"><code>FA_SORT_DOWN</code></td>
 *              <td align="center"><span class="fa-sort-down"></span></td>
 *            </tr>
 *            <tr class="row-even">
 *              <td align="right"><code>FA_SORT_NUMERIC_DOWN</code></td>
 *              <td align="center"><span class="fa-sort-numeric-down"></span></td>
 *            </tr>
 *            <tr class="row-odd">
 *              <td align="right"><code>FA_SORT_NUMERIC_DOWN_ALT</code></td>
 *              <td align="center"><span class="fa-sort-numeric-down-alt"></span></td>
 *            </tr>
 *            <tr class="row-even">
 *              <td align="right"><code>FA_SORT_NUMERIC_UP</code></td>
 *              <td align="center"><span class="fa-sort-numeric-up"></span></td>
 *            </tr>
 *            <tr class="row-odd">
 *              <td align="right"><code>FA_SORT_NUMERIC_UP_ALT</code></td>
 *              <td align="center"><span class="fa-sort-numeric-up-alt"></span></td>
 *            </tr>
 *            <tr class="row-even">
 *              <td align="right"><code>FA_SORT_UP</code></td>
 *              <td align="center"><span class="fa-sort-up"></span></td>
 *            </tr>
 *            <tr class="row-odd">
 *              <td align="right"><code>FA_SPA</code></td>
 *              <td align="center"><span class="fa-spa"></span></td>
 *            </tr>
 *            <tr class="row-even">
 *              <td align="right"><code>FA_SPACE_SHUTTLE</code></td>
 *              <td align="center"><span class="fa-space-shuttle"></span></td>
 *            </tr>
 *            <tr class="row-odd">
 *              <td align="right"><code>FA_SPELL_CHECK</code></td>
 *              <td align="center"><span class="fa-spell-check"></span></td>
 *            </tr>
 *            <tr class="row-even">
 *              <td align="right"><code>FA_SPIDER</code></td>
 *              <td align="center"><span class="fa-spider"></span></td>
 *            </tr>
 *            <tr class="row-odd">
 *              <td align="right"><code>FA_SPINNER</code></td>
 *              <td align="center"><span class="fa-spinner"></span></td>
 *            </tr>
 *            <tr class="row-even">
 *              <td align="right"><code>FA_SPLOTCH</code></td>
 *              <td align="center"><span class="fa-splotch"></span></td>
 *            </tr>
 *            <tr class="row-odd">
 *              <td align="right"><code>FA_SPRAY_CAN</code></td>
 *              <td align="center"><span class="fa-spray-can"></span></td>
 *            </tr>
 *            <tr class="row-even">
 *              <td align="right"><code>FA_SQUARE</code></td>
 *              <td align="center"><span class="fa-square"></span></td>
 *            </tr>
 *            <tr class="row-odd">
 *              <td align="right"><code>FA_SQUARE_FULL</code></td>
 *              <td align="center"><span class="fa-square-full"></span></td>
 *            </tr>
 *            <tr class="row-even">
 *              <td align="right"><code>FA_SQUARE_ROOT_ALT</code></td>
 *              <td align="center"><span class="fa-square-root-alt"></span></td>
 *            </tr>
 *            <tr class="row-odd">
 *              <td align="right"><code>FA_STAMP</code></td>
 *              <td align="center"><span class="fa-stamp"></span></td>
 *            </tr>
 *            <tr class="row-even">
 *              <td align="right"><code>FA_STAR</code></td>
 *              <td align="center"><span class="fa-star"></span></td>
 *            </tr>
 *            <tr class="row-odd">
 *              <td align="right"><code>FA_STAR_AND_CRESCENT</code></td>
 *              <td align="center"><span class="fa-star-and-crescent"></span></td>
 *            </tr>
 *            <tr class="row-even">
 *              <td align="right"><code>FA_STAR_HALF</code></td>
 *              <td align="center"><span class="fa-star-half"></span></td>
 *            </tr>
 *            <tr class="row-odd">
 *              <td align="right"><code>FA_STAR_HALF_ALT</code></td>
 *              <td align="center"><span class="fa-star-half-alt"></span></td>
 *            </tr>
 *            <tr class="row-even">
 *              <td align="right"><code>FA_STAR_OF_DAVID</code></td>
 *              <td align="center"><span class="fa-star-of-david"></span></td>
 *            </tr>
 *            <tr class="row-odd">
 *              <td align="right"><code>FA_STAR_OF_LIFE</code></td>
 *              <td align="center"><span class="fa-star-of-life"></span></td>
 *            </tr>
 *            <tr class="row-even">
 *              <td align="right"><code>FA_STEP_BACKWARD</code></td>
 *              <td align="center"><span class="fa-step-backward"></span></td>
 *            </tr>
 *            <tr class="row-odd">
 *              <td align="right"><code>FA_STEP_FORWARD</code></td>
 *              <td align="center"><span class="fa-step-forward"></span></td>
 *            </tr>
 *            <tr class="row-even">
 *              <td align="right"><code>FA_STETHOSCOPE</code></td>
 *              <td align="center"><span class="fa-stethoscope"></span></td>
 *            </tr>
 *            <tr class="row-odd">
 *              <td align="right"><code>FA_STICKY_NOTE</code></td>
 *              <td align="center"><span class="fa-sticky-note"></span></td>
 *            </tr>
 *            <tr class="row-even">
 *              <td align="right"><code>FA_STOP</code></td>
 *              <td align="center"><span class="fa-stop"></span></td>
 *            </tr>
 *            <tr class="row-odd">
 *              <td align="right"><code>FA_STOP_CIRCLE</code></td>
 *              <td align="center"><span class="fa-stop-circle"></span></td>
 *            </tr>
 *            <tr class="row-even">
 *              <td align="right"><code>FA_STOPWATCH</code></td>
 *              <td align="center"><span class="fa-stopwatch"></span></td>
 *            </tr>
 *            <tr class="row-odd">
 *              <td align="right"><code>FA_STORE</code></td>
 *              <td align="center"><span class="fa-store"></span></td>
 *            </tr>
 *            <tr class="row-even">
 *              <td align="right"><code>FA_STORE_ALT</code></td>
 *              <td align="center"><span class="fa-store-alt"></span></td>
 *            </tr>
 *            <tr class="row-odd">
 *              <td align="right"><code>FA_STREAM</code></td>
 *              <td align="center"><span class="fa-stream"></span></td>
 *            </tr>
 *            <tr class="row-even">
 *              <td align="right"><code>FA_STREET_VIEW</code></td>
 *              <td align="center"><span class="fa-street-view"></span></td>
 *            </tr>
 *            <tr class="row-odd">
 *              <td align="right"><code>FA_STRIKETHROUGH</code></td>
 *              <td align="center"><span class="fa-strikethrough"></span></td>
 *            </tr>
 *            <tr class="row-even">
 *              <td align="right"><code>FA_STROOPWAFEL</code></td>
 *              <td align="center"><span class="fa-stroopwafel"></span></td>
 *            </tr>
 *            <tr class="row-odd">
 *              <td align="right"><code>FA_SUBSCRIPT</code></td>
 *              <td align="center"><span class="fa-subscript"></span></td>
 *            </tr>
 *            <tr class="row-even">
 *              <td align="right"><code>FA_SUBWAY</code></td>
 *              <td align="center"><span class="fa-subway"></span></td>
 *            </tr>
 *            <tr class="row-odd">
 *              <td align="right"><code>FA_SUITCASE</code></td>
 *              <td align="center"><span class="fa-suitcase"></span></td>
 *            </tr>
 *            <tr class="row-even">
 *              <td align="right"><code>FA_SUITCASE_ROLLING</code></td>
 *              <td align="center"><span class="fa-suitcase-rolling"></span></td>
 *            </tr>
 *            <tr class="row-odd">
 *              <td align="right"><code>FA_SUN</code></td>
 *              <td align="center"><span class="fa-sun"></span></td>
 *            </tr>
 *            <tr class="row-even">
 *              <td align="right"><code>FA_SUPERSCRIPT</code></td>
 *              <td align="center"><span class="fa-superscript"></span></td>
 *            </tr>
 *            <tr class="row-odd">
 *              <td align="right"><code>FA_SURPRISE</code></td>
 *              <td align="center"><span class="fa-surprise"></span></td>
 *            </tr>
 *            <tr class="row-even">
 *              <td align="right"><code>FA_SWATCHBOOK</code></td>
 *              <td align="center"><span class="fa-swatchbook"></span></td>
 *            </tr>
 *            <tr class="row-odd">
 *              <td align="right"><code>FA_SWIMMER</code></td>
 *              <td align="center"><span class="fa-swimmer"></span></td>
 *            </tr>
 *            <tr class="row-even">
 *              <td align="right"><code>FA_SWIMMING_POOL</code></td>
 *              <td align="center"><span class="fa-swimming-pool"></span></td>
 *            </tr>
 *            <tr class="row-odd">
 *              <td align="right"><code>FA_SYNAGOGUE</code></td>
 *              <td align="center"><span class="fa-synagogue"></span></td>
 *            </tr>
 *            <tr class="row-even">
 *              <td align="right"><code>FA_SYNC</code></td>
 *              <td align="center"><span class="fa-sync"></span></td>
 *            </tr>
 *            <tr class="row-odd">
 *              <td align="right"><code>FA_SYNC_ALT</code></td>
 *              <td align="center"><span class="fa-sync-alt"></span></td>
 *            </tr>
 *            <tr class="row-even">
 *              <td align="right"><code>FA_SYRINGE</code></td>
 *              <td align="center"><span class="fa-syringe"></span></td>
 *            </tr>
 *            <tr class="row-odd">
 *              <td align="right"><code>FA_TABLE</code></td>
 *              <td align="center"><span class="fa-table"></span></td>
 *            </tr>
 *            <tr class="row-even">
 *              <td align="right"><code>FA_TABLE_TENNIS</code></td>
 *              <td align="center"><span class="fa-table-tennis"></span></td>
 *            </tr>
 *            <tr class="row-odd">
 *              <td align="right"><code>FA_TABLET</code></td>
 *              <td align="center"><span class="fa-tablet"></span></td>
 *            </tr>
 *            <tr class="row-even">
 *              <td align="right"><code>FA_TABLET_ALT</code></td>
 *              <td align="center"><span class="fa-tablet-alt"></span></td>
 *            </tr>
 *            <tr class="row-odd">
 *              <td align="right"><code>FA_TABLETS</code></td>
 *              <td align="center"><span class="fa-tablets"></span></td>
 *            </tr>
 *            <tr class="row-even">
 *              <td align="right"><code>FA_TACHOMETER_ALT</code></td>
 *              <td align="center"><span class="fa-tachometer-alt"></span></td>
 *            </tr>
 *            <tr class="row-odd">
 *              <td align="right"><code>FA_TAG</code></td>
 *              <td align="center"><span class="fa-tag"></span></td>
 *            </tr>
 *            <tr class="row-even">
 *              <td align="right"><code>FA_TAGS</code></td>
 *              <td align="center"><span class="fa-tags"></span></td>
 *            </tr>
 *            <tr class="row-odd">
 *              <td align="right"><code>FA_TAPE</code></td>
 *              <td align="center"><span class="fa-tape"></span></td>
 *            </tr>
 *            <tr class="row-even">
 *              <td align="right"><code>FA_TASKS</code></td>
 *              <td align="center"><span class="fa-tasks"></span></td>
 *            </tr>
 *            <tr class="row-odd">
 *              <td align="right"><code>FA_TAXI</code></td>
 *              <td align="center"><span class="fa-taxi"></span></td>
 *            </tr>
 *            <tr class="row-even">
 *              <td align="right"><code>FA_TEETH</code></td>
 *              <td align="center"><span class="fa-teeth"></span></td>
 *            </tr>
 *            <tr class="row-odd">
 *              <td align="right"><code>FA_TEETH_OPEN</code></td>
 *              <td align="center"><span class="fa-teeth-open"></span></td>
 *            </tr>
 *            <tr class="row-even">
 *              <td align="right"><code>FA_TEMPERATURE_HIGH</code></td>
 *              <td align="center"><span class="fa-temperature-high"></span></td>
 *            </tr>
 *            <tr class="row-odd">
 *              <td align="right"><code>FA_TEMPERATURE_LOW</code></td>
 *              <td align="center"><span class="fa-temperature-low"></span></td>
 *            </tr>
 *            <tr class="row-even">
 *              <td align="right"><code>FA_TENGE</code></td>
 *              <td align="center"><span class="fa-tenge"></span></td>
 *            </tr>
 *            <tr class="row-odd">
 *              <td align="right"><code>FA_TERMINAL</code></td>
 *              <td align="center"><span class="fa-terminal"></span></td>
 *            </tr>
 *            <tr class="row-even">
 *              <td align="right"><code>FA_TEXT_HEIGHT</code></td>
 *              <td align="center"><span class="fa-text-height"></span></td>
 *            </tr>
 *            <tr class="row-odd">
 *              <td align="right"><code>FA_TEXT_WIDTH</code></td>
 *              <td align="center"><span class="fa-text-width"></span></td>
 *            </tr>
 *            <tr class="row-even">
 *              <td align="right"><code>FA_TH</code></td>
 *              <td align="center"><span class="fa-th"></span></td>
 *            </tr>
 *            <tr class="row-odd">
 *              <td align="right"><code>FA_TH_LARGE</code></td>
 *              <td align="center"><span class="fa-th-large"></span></td>
 *            </tr>
 *            <tr class="row-even">
 *              <td align="right"><code>FA_TH_LIST</code></td>
 *              <td align="center"><span class="fa-th-list"></span></td>
 *            </tr>
 *            <tr class="row-odd">
 *              <td align="right"><code>FA_THEATER_MASKS</code></td>
 *              <td align="center"><span class="fa-theater-masks"></span></td>
 *            </tr>
 *            <tr class="row-even">
 *              <td align="right"><code>FA_THERMOMETER</code></td>
 *              <td align="center"><span class="fa-thermometer"></span></td>
 *            </tr>
 *            <tr class="row-odd">
 *              <td align="right"><code>FA_THERMOMETER_EMPTY</code></td>
 *              <td align="center"><span class="fa-thermometer-empty"></span></td>
 *            </tr>
 *            <tr class="row-even">
 *              <td align="right"><code>FA_THERMOMETER_FULL</code></td>
 *              <td align="center"><span class="fa-thermometer-full"></span></td>
 *            </tr>
 *            <tr class="row-odd">
 *              <td align="right"><code>FA_THERMOMETER_HALF</code></td>
 *              <td align="center"><span class="fa-thermometer-half"></span></td>
 *            </tr>
 *            <tr class="row-even">
 *              <td align="right"><code>FA_THERMOMETER_QUARTER</code></td>
 *              <td align="center"><span class="fa-thermometer-quarter"></span></td>
 *            </tr>
 *            <tr class="row-odd">
 *              <td align="right"><code>FA_THERMOMETER_THREE_QUARTERS</code></td>
 *              <td align="center"><span class="fa-thermometer-three-quarters"></span></td>
 *            </tr>
 *            <tr class="row-even">
 *              <td align="right"><code>FA_THUMBS_DOWN</code></td>
 *              <td align="center"><span class="fa-thumbs-down"></span></td>
 *            </tr>
 *            <tr class="row-odd">
 *              <td align="right"><code>FA_THUMBS_UP</code></td>
 *              <td align="center"><span class="fa-thumbs-up"></span></td>
 *            </tr>
 *            <tr class="row-even">
 *              <td align="right"><code>FA_THUMBTACK</code></td>
 *              <td align="center"><span class="fa-thumbtack"></span></td>
 *            </tr>
 *            <tr class="row-odd">
 *              <td align="right"><code>FA_TICKET_ALT</code></td>
 *              <td align="center"><span class="fa-ticket-alt"></span></td>
 *            </tr>
 *            <tr class="row-even">
 *              <td align="right"><code>FA_TIMES</code></td>
 *              <td align="center"><span class="fa-times"></span></td>
 *            </tr>
 *            <tr class="row-odd">
 *              <td align="right"><code>FA_TIMES_CIRCLE</code></td>
 *              <td align="center"><span class="fa-times-circle"></span></td>
 *            </tr>
 *            <tr class="row-even">
 *              <td align="right"><code>FA_TINT</code></td>
 *              <td align="center"><span class="fa-tint"></span></td>
 *            </tr>
 *            <tr class="row-odd">
 *              <td align="right"><code>FA_TINT_SLASH</code></td>
 *              <td align="center"><span class="fa-tint-slash"></span></td>
 *            </tr>
 *            <tr class="row-even">
 *              <td align="right"><code>FA_TIRED</code></td>
 *              <td align="center"><span class="fa-tired"></span></td>
 *            </tr>
 *            <tr class="row-odd">
 *              <td align="right"><code>FA_TOGGLE_OFF</code></td>
 *              <td align="center"><span class="fa-toggle-off"></span></td>
 *            </tr>
 *            <tr class="row-even">
 *              <td align="right"><code>FA_TOGGLE_ON</code></td>
 *              <td align="center"><span class="fa-toggle-on"></span></td>
 *            </tr>
 *            <tr class="row-odd">
 *              <td align="right"><code>FA_TOILET</code></td>
 *              <td align="center"><span class="fa-toilet"></span></td>
 *            </tr>
 *            <tr class="row-even">
 *              <td align="right"><code>FA_TOILET_PAPER</code></td>
 *              <td align="center"><span class="fa-toilet-paper"></span></td>
 *            </tr>
 *            <tr class="row-odd">
 *              <td align="right"><code>FA_TOOLBOX</code></td>
 *              <td align="center"><span class="fa-toolbox"></span></td>
 *            </tr>
 *            <tr class="row-even">
 *              <td align="right"><code>FA_TOOLS</code></td>
 *              <td align="center"><span class="fa-tools"></span></td>
 *            </tr>
 *            <tr class="row-odd">
 *              <td align="right"><code>FA_TOOTH</code></td>
 *              <td align="center"><span class="fa-tooth"></span></td>
 *            </tr>
 *            <tr class="row-even">
 *              <td align="right"><code>FA_TORAH</code></td>
 *              <td align="center"><span class="fa-torah"></span></td>
 *            </tr>
 *            <tr class="row-odd">
 *              <td align="right"><code>FA_TORII_GATE</code></td>
 *              <td align="center"><span class="fa-torii-gate"></span></td>
 *            </tr>
 *            <tr class="row-even">
 *              <td align="right"><code>FA_TRACTOR</code></td>
 *              <td align="center"><span class="fa-tractor"></span></td>
 *            </tr>
 *            <tr class="row-odd">
 *              <td align="right"><code>FA_TRADEMARK</code></td>
 *              <td align="center"><span class="fa-trademark"></span></td>
 *            </tr>
 *            <tr class="row-even">
 *              <td align="right"><code>FA_TRAFFIC_LIGHT</code></td>
 *              <td align="center"><span class="fa-traffic-light"></span></td>
 *            </tr>
 *            <tr class="row-odd">
 *              <td align="right"><code>FA_TRAIN</code></td>
 *              <td align="center"><span class="fa-train"></span></td>
 *            </tr>
 *            <tr class="row-even">
 *              <td align="right"><code>FA_TRAM</code></td>
 *              <td align="center"><span class="fa-tram"></span></td>
 *            </tr>
 *            <tr class="row-odd">
 *              <td align="right"><code>FA_TRANSGENDER</code></td>
 *              <td align="center"><span class="fa-transgender"></span></td>
 *            </tr>
 *            <tr class="row-even">
 *              <td align="right"><code>FA_TRANSGENDER_ALT</code></td>
 *              <td align="center"><span class="fa-transgender-alt"></span></td>
 *            </tr>
 *            <tr class="row-odd">
 *              <td align="right"><code>FA_TRASH</code></td>
 *              <td align="center"><span class="fa-trash"></span></td>
 *            </tr>
 *            <tr class="row-even">
 *              <td align="right"><code>FA_TRASH_ALT</code></td>
 *              <td align="center"><span class="fa-trash-alt"></span></td>
 *            </tr>
 *            <tr class="row-odd">
 *              <td align="right"><code>FA_TRASH_RESTORE</code></td>
 *              <td align="center"><span class="fa-trash-restore"></span></td>
 *            </tr>
 *            <tr class="row-even">
 *              <td align="right"><code>FA_TRASH_RESTORE_ALT</code></td>
 *              <td align="center"><span class="fa-trash-restore-alt"></span></td>
 *            </tr>
 *            <tr class="row-odd">
 *              <td align="right"><code>FA_TREE</code></td>
 *              <td align="center"><span class="fa-tree"></span></td>
 *            </tr>
 *            <tr class="row-even">
 *              <td align="right"><code>FA_TROPHY</code></td>
 *              <td align="center"><span class="fa-trophy"></span></td>
 *            </tr>
 *            <tr class="row-odd">
 *              <td align="right"><code>FA_TRUCK</code></td>
 *              <td align="center"><span class="fa-truck"></span></td>
 *            </tr>
 *            <tr class="row-even">
 *              <td align="right"><code>FA_TRUCK_LOADING</code></td>
 *              <td align="center"><span class="fa-truck-loading"></span></td>
 *            </tr>
 *            <tr class="row-odd">
 *              <td align="right"><code>FA_TRUCK_MONSTER</code></td>
 *              <td align="center"><span class="fa-truck-monster"></span></td>
 *            </tr>
 *            <tr class="row-even">
 *              <td align="right"><code>FA_TRUCK_MOVING</code></td>
 *              <td align="center"><span class="fa-truck-moving"></span></td>
 *            </tr>
 *            <tr class="row-odd">
 *              <td align="right"><code>FA_TRUCK_PICKUP</code></td>
 *              <td align="center"><span class="fa-truck-pickup"></span></td>
 *            </tr>
 *            <tr class="row-even">
 *              <td align="right"><code>FA_TSHIRT</code></td>
 *              <td align="center"><span class="fa-tshirt"></span></td>
 *            </tr>
 *            <tr class="row-odd">
 *              <td align="right"><code>FA_TTY</code></td>
 *              <td align="center"><span class="fa-tty"></span></td>
 *            </tr>
 *            <tr class="row-even">
 *              <td align="right"><code>FA_TV</code></td>
 *              <td align="center"><span class="fa-tv"></span></td>
 *            </tr>
 *            <tr class="row-odd">
 *              <td align="right"><code>FA_UMBRELLA</code></td>
 *              <td align="center"><span class="fa-umbrella"></span></td>
 *            </tr>
 *            <tr class="row-even">
 *              <td align="right"><code>FA_UMBRELLA_BEACH</code></td>
 *              <td align="center"><span class="fa-umbrella-beach"></span></td>
 *            </tr>
 *            <tr class="row-odd">
 *              <td align="right"><code>FA_UNDERLINE</code></td>
 *              <td align="center"><span class="fa-underline"></span></td>
 *            </tr>
 *            <tr class="row-even">
 *              <td align="right"><code>FA_UNDO</code></td>
 *              <td align="center"><span class="fa-undo"></span></td>
 *            </tr>
 *            <tr class="row-odd">
 *              <td align="right"><code>FA_UNDO_ALT</code></td>
 *              <td align="center"><span class="fa-undo-alt"></span></td>
 *            </tr>
 *            <tr class="row-even">
 *              <td align="right"><code>FA_UNIVERSAL_ACCESS</code></td>
 *              <td align="center"><span class="fa-universal-access"></span></td>
 *            </tr>
 *            <tr class="row-odd">
 *              <td align="right"><code>FA_UNIVERSITY</code></td>
 *              <td align="center"><span class="fa-university"></span></td>
 *            </tr>
 *            <tr class="row-even">
 *              <td align="right"><code>FA_UNLINK</code></td>
 *              <td align="center"><span class="fa-unlink"></span></td>
 *            </tr>
 *            <tr class="row-odd">
 *              <td align="right"><code>FA_UNLOCK</code></td>
 *              <td align="center"><span class="fa-unlock"></span></td>
 *            </tr>
 *            <tr class="row-even">
 *              <td align="right"><code>FA_UNLOCK_ALT</code></td>
 *              <td align="center"><span class="fa-unlock-alt"></span></td>
 *            </tr>
 *            <tr class="row-odd">
 *              <td align="right"><code>FA_UPLOAD</code></td>
 *              <td align="center"><span class="fa-upload"></span></td>
 *            </tr>
 *            <tr class="row-even">
 *              <td align="right"><code>FA_USER</code></td>
 *              <td align="center"><span class="fa-user"></span></td>
 *            </tr>
 *            <tr class="row-odd">
 *              <td align="right"><code>FA_USER_ALT</code></td>
 *              <td align="center"><span class="fa-user-alt"></span></td>
 *            </tr>
 *            <tr class="row-even">
 *              <td align="right"><code>FA_USER_ALT_SLASH</code></td>
 *              <td align="center"><span class="fa-user-alt-slash"></span></td>
 *            </tr>
 *            <tr class="row-odd">
 *              <td align="right"><code>FA_USER_ASTRONAUT</code></td>
 *              <td align="center"><span class="fa-user-astronaut"></span></td>
 *            </tr>
 *            <tr class="row-even">
 *              <td align="right"><code>FA_USER_CHECK</code></td>
 *              <td align="center"><span class="fa-user-check"></span></td>
 *            </tr>
 *            <tr class="row-odd">
 *              <td align="right"><code>FA_USER_CIRCLE</code></td>
 *              <td align="center"><span class="fa-user-circle"></span></td>
 *            </tr>
 *            <tr class="row-even">
 *              <td align="right"><code>FA_USER_CLOCK</code></td>
 *              <td align="center"><span class="fa-user-clock"></span></td>
 *            </tr>
 *            <tr class="row-odd">
 *              <td align="right"><code>FA_USER_COG</code></td>
 *              <td align="center"><span class="fa-user-cog"></span></td>
 *            </tr>
 *            <tr class="row-even">
 *              <td align="right"><code>FA_USER_EDIT</code></td>
 *              <td align="center"><span class="fa-user-edit"></span></td>
 *            </tr>
 *            <tr class="row-odd">
 *              <td align="right"><code>FA_USER_FRIENDS</code></td>
 *              <td align="center"><span class="fa-user-friends"></span></td>
 *            </tr>
 *            <tr class="row-even">
 *              <td align="right"><code>FA_USER_GRADUATE</code></td>
 *              <td align="center"><span class="fa-user-graduate"></span></td>
 *            </tr>
 *            <tr class="row-odd">
 *              <td align="right"><code>FA_USER_INJURED</code></td>
 *              <td align="center"><span class="fa-user-injured"></span></td>
 *            </tr>
 *            <tr class="row-even">
 *              <td align="right"><code>FA_USER_LOCK</code></td>
 *              <td align="center"><span class="fa-user-lock"></span></td>
 *            </tr>
 *            <tr class="row-odd">
 *              <td align="right"><code>FA_USER_MD</code></td>
 *              <td align="center"><span class="fa-user-md"></span></td>
 *            </tr>
 *            <tr class="row-even">
 *              <td align="right"><code>FA_USER_MINUS</code></td>
 *              <td align="center"><span class="fa-user-minus"></span></td>
 *            </tr>
 *            <tr class="row-odd">
 *              <td align="right"><code>FA_USER_NINJA</code></td>
 *              <td align="center"><span class="fa-user-ninja"></span></td>
 *            </tr>
 *            <tr class="row-even">
 *              <td align="right"><code>FA_USER_NURSE</code></td>
 *              <td align="center"><span class="fa-user-nurse"></span></td>
 *            </tr>
 *            <tr class="row-odd">
 *              <td align="right"><code>FA_USER_PLUS</code></td>
 *              <td align="center"><span class="fa-user-plus"></span></td>
 *            </tr>
 *            <tr class="row-even">
 *              <td align="right"><code>FA_USER_SECRET</code></td>
 *              <td align="center"><span class="fa-user-secret"></span></td>
 *            </tr>
 *            <tr class="row-odd">
 *              <td align="right"><code>FA_USER_SHIELD</code></td>
 *              <td align="center"><span class="fa-user-shield"></span></td>
 *            </tr>
 *            <tr class="row-even">
 *              <td align="right"><code>FA_USER_SLASH</code></td>
 *              <td align="center"><span class="fa-user-slash"></span></td>
 *            </tr>
 *            <tr class="row-odd">
 *              <td align="right"><code>FA_USER_TAG</code></td>
 *              <td align="center"><span class="fa-user-tag"></span></td>
 *            </tr>
 *            <tr class="row-even">
 *              <td align="right"><code>FA_USER_TIE</code></td>
 *              <td align="center"><span class="fa-user-tie"></span></td>
 *            </tr>
 *            <tr class="row-odd">
 *              <td align="right"><code>FA_USER_TIMES</code></td>
 *              <td align="center"><span class="fa-user-times"></span></td>
 *            </tr>
 *            <tr class="row-even">
 *              <td align="right"><code>FA_USERS</code></td>
 *              <td align="center"><span class="fa-users"></span></td>
 *            </tr>
 *            <tr class="row-odd">
 *              <td align="right"><code>FA_USERS_COG</code></td>
 *              <td align="center"><span class="fa-users-cog"></span></td>
 *            </tr>
 *            <tr class="row-even">
 *              <td align="right"><code>FA_UTENSIL_SPOON</code></td>
 *              <td align="center"><span class="fa-utensil-spoon"></span></td>
 *            </tr>
 *            <tr class="row-odd">
 *              <td align="right"><code>FA_UTENSILS</code></td>
 *              <td align="center"><span class="fa-utensils"></span></td>
 *            </tr>
 *            <tr class="row-even">
 *              <td align="right"><code>FA_VECTOR_SQUARE</code></td>
 *              <td align="center"><span class="fa-vector-square"></span></td>
 *            </tr>
 *            <tr class="row-odd">
 *              <td align="right"><code>FA_VENUS</code></td>
 *              <td align="center"><span class="fa-venus"></span></td>
 *            </tr>
 *            <tr class="row-even">
 *              <td align="right"><code>FA_VENUS_DOUBLE</code></td>
 *              <td align="center"><span class="fa-venus-double"></span></td>
 *            </tr>
 *            <tr class="row-odd">
 *              <td align="right"><code>FA_VENUS_MARS</code></td>
 *              <td align="center"><span class="fa-venus-mars"></span></td>
 *            </tr>
 *            <tr class="row-even">
 *              <td align="right"><code>FA_VIAL</code></td>
 *              <td align="center"><span class="fa-vial"></span></td>
 *            </tr>
 *            <tr class="row-odd">
 *              <td align="right"><code>FA_VIALS</code></td>
 *              <td align="center"><span class="fa-vials"></span></td>
 *            </tr>
 *            <tr class="row-even">
 *              <td align="right"><code>FA_VIDEO</code></td>
 *              <td align="center"><span class="fa-video"></span></td>
 *            </tr>
 *            <tr class="row-odd">
 *              <td align="right"><code>FA_VIDEO_SLASH</code></td>
 *              <td align="center"><span class="fa-video-slash"></span></td>
 *            </tr>
 *            <tr class="row-even">
 *              <td align="right"><code>FA_VIHARA</code></td>
 *              <td align="center"><span class="fa-vihara"></span></td>
 *            </tr>
 *            <tr class="row-odd">
 *              <td align="right"><code>FA_VOICEMAIL</code></td>
 *              <td align="center"><span class="fa-voicemail"></span></td>
 *            </tr>
 *            <tr class="row-even">
 *              <td align="right"><code>FA_VOLLEYBALL_BALL</code></td>
 *              <td align="center"><span class="fa-volleyball-ball"></span></td>
 *            </tr>
 *            <tr class="row-odd">
 *              <td align="right"><code>FA_VOLUME_DOWN</code></td>
 *              <td align="center"><span class="fa-volume-down"></span></td>
 *            </tr>
 *            <tr class="row-even">
 *              <td align="right"><code>FA_VOLUME_MUTE</code></td>
 *              <td align="center"><span class="fa-volume-mute"></span></td>
 *            </tr>
 *            <tr class="row-odd">
 *              <td align="right"><code>FA_VOLUME_OFF</code></td>
 *              <td align="center"><span class="fa-volume-off"></span></td>
 *            </tr>
 *            <tr class="row-even">
 *              <td align="right"><code>FA_VOLUME_UP</code></td>
 *              <td align="center"><span class="fa-volume-up"></span></td>
 *            </tr>
 *            <tr class="row-odd">
 *              <td align="right"><code>FA_VOTE_YEA</code></td>
 *              <td align="center"><span class="fa-vote-yea"></span></td>
 *            </tr>
 *            <tr class="row-even">
 *              <td align="right"><code>FA_VR_CARDBOARD</code></td>
 *              <td align="center"><span class="fa-vr-cardboard"></span></td>
 *            </tr>
 *            <tr class="row-odd">
 *              <td align="right"><code>FA_WALKING</code></td>
 *              <td align="center"><span class="fa-walking"></span></td>
 *            </tr>
 *            <tr class="row-even">
 *              <td align="right"><code>FA_WALLET</code></td>
 *              <td align="center"><span class="fa-wallet"></span></td>
 *            </tr>
 *            <tr class="row-odd">
 *              <td align="right"><code>FA_WAREHOUSE</code></td>
 *              <td align="center"><span class="fa-warehouse"></span></td>
 *            </tr>
 *            <tr class="row-even">
 *              <td align="right"><code>FA_WATER</code></td>
 *              <td align="center"><span class="fa-water"></span></td>
 *            </tr>
 *            <tr class="row-odd">
 *              <td align="right"><code>FA_WAVE_SQUARE</code></td>
 *              <td align="center"><span class="fa-wave-square"></span></td>
 *            </tr>
 *            <tr class="row-even">
 *              <td align="right"><code>FA_WEIGHT</code></td>
 *              <td align="center"><span class="fa-weight"></span></td>
 *            </tr>
 *            <tr class="row-odd">
 *              <td align="right"><code>FA_WEIGHT_HANGING</code></td>
 *              <td align="center"><span class="fa-weight-hanging"></span></td>
 *            </tr>
 *            <tr class="row-even">
 *              <td align="right"><code>FA_WHEELCHAIR</code></td>
 *              <td align="center"><span class="fa-wheelchair"></span></td>
 *            </tr>
 *            <tr class="row-odd">
 *              <td align="right"><code>FA_WIFI</code></td>
 *              <td align="center"><span class="fa-wifi"></span></td>
 *            </tr>
 *            <tr class="row-even">
 *              <td align="right"><code>FA_WIND</code></td>
 *              <td align="center"><span class="fa-wind"></span></td>
 *            </tr>
 *            <tr class="row-odd">
 *              <td align="right"><code>FA_WINDOW_CLOSE</code></td>
 *              <td align="center"><span class="fa-window-close"></span></td>
 *            </tr>
 *            <tr class="row-even">
 *              <td align="right"><code>FA_WINDOW_MAXIMIZE</code></td>
 *              <td align="center"><span class="fa-window-maximize"></span></td>
 *            </tr>
 *            <tr class="row-odd">
 *              <td align="right"><code>FA_WINDOW_MINIMIZE</code></td>
 *              <td align="center"><span class="fa-window-minimize"></span></td>
 *            </tr>
 *            <tr class="row-even">
 *              <td align="right"><code>FA_WINDOW_RESTORE</code></td>
 *              <td align="center"><span class="fa-window-restore"></span></td>
 *            </tr>
 *            <tr class="row-odd">
 *              <td align="right"><code>FA_WINE_BOTTLE</code></td>
 *              <td align="center"><span class="fa-wine-bottle"></span></td>
 *            </tr>
 *            <tr class="row-even">
 *              <td align="right"><code>FA_WINE_GLASS</code></td>
 *              <td align="center"><span class="fa-wine-glass"></span></td>
 *            </tr>
 *            <tr class="row-odd">
 *              <td align="right"><code>FA_WINE_GLASS_ALT</code></td>
 *              <td align="center"><span class="fa-wine-glass-alt"></span></td>
 *            </tr>
 *            <tr class="row-even">
 *              <td align="right"><code>FA_WON_SIGN</code></td>
 *              <td align="center"><span class="fa-won-sign"></span></td>
 *            </tr>
 *            <tr class="row-odd">
 *              <td align="right"><code>FA_WRENCH</code></td>
 *              <td align="center"><span class="fa-wrench"></span></td>
 *            </tr>
 *            <tr class="row-even">
 *              <td align="right"><code>FA_X_RAY</code></td>
 *              <td align="center"><span class="fa-x-ray"></span></td>
 *            </tr>
 *            <tr class="row-odd">
 *              <td align="right"><code>FA_YEN_SIGN</code></td>
 *              <td align="center"><span class="fa-yen-sign"></span></td>
 *            </tr>
 *            <tr class="row-even">
 *              <td align="right"><code>FA_YIN_YANG</code></td>
 *              <td align="center"><span class="fa-yin-yang"></span></td>
 *            </tr>
 *          </tbody>
 *        </table>
 *      </div><!-- wy-table-responsive -->
 *    </center>
 *
 * \endrst
 */

#pragma once

// prevent individual pages from being generated for all of these
#if !defined(DOXYGEN_SHOULD_SKIP_THIS)

#define FA_AD                                   0xf641
#define FA_ADDRESS_BOOK                         0xf2b9
#define FA_ADDRESS_CARD                         0xf2bb
#define FA_ADJUST                               0xf042
#define FA_AIR_FRESHENER                        0xf5d0
#define FA_ALIGN_CENTER                         0xf037
#define FA_ALIGN_JUSTIFY                        0xf039
#define FA_ALIGN_LEFT                           0xf036
#define FA_ALIGN_RIGHT                          0xf038
#define FA_ALLERGIES                            0xf461
#define FA_AMBULANCE                            0xf0f9
#define FA_AMERICAN_SIGN_LANGUAGE_INTERPRETING  0xf2a3
#define FA_ANCHOR                               0xf13d
#define FA_ANGLE_DOUBLE_DOWN                    0xf103
#define FA_ANGLE_DOUBLE_LEFT                    0xf100
#define FA_ANGLE_DOUBLE_RIGHT                   0xf101
#define FA_ANGLE_DOUBLE_UP                      0xf102
#define FA_ANGLE_DOWN                           0xf107
#define FA_ANGLE_LEFT                           0xf104
#define FA_ANGLE_RIGHT                          0xf105
#define FA_ANGLE_UP                             0xf106
#define FA_ANGRY                                0xf556
#define FA_ANKH                                 0xf644
#define FA_APPLE_ALT                            0xf5d1
#define FA_ARCHIVE                              0xf187
#define FA_ARCHWAY                              0xf557
#define FA_ARROW_ALT_CIRCLE_DOWN                0xf358
#define FA_ARROW_ALT_CIRCLE_LEFT                0xf359
#define FA_ARROW_ALT_CIRCLE_RIGHT               0xf35a
#define FA_ARROW_ALT_CIRCLE_UP                  0xf35b
#define FA_ARROW_CIRCLE_DOWN                    0xf0ab
#define FA_ARROW_CIRCLE_LEFT                    0xf0a8
#define FA_ARROW_CIRCLE_RIGHT                   0xf0a9
#define FA_ARROW_CIRCLE_UP                      0xf0aa
#define FA_ARROW_DOWN                           0xf063
#define FA_ARROW_LEFT                           0xf060
#define FA_ARROW_RIGHT                          0xf061
#define FA_ARROW_UP                             0xf062
#define FA_ARROWS_ALT                           0xf0b2
#define FA_ARROWS_ALT_H                         0xf337
#define FA_ARROWS_ALT_V                         0xf338
#define FA_ASSISTIVE_LISTENING_SYSTEMS          0xf2a2
#define FA_ASTERISK                             0xf069
#define FA_AT                                   0xf1fa
#define FA_ATLAS                                0xf558
#define FA_ATOM                                 0xf5d2
#define FA_AUDIO_DESCRIPTION                    0xf29e
#define FA_AWARD                                0xf559
#define FA_BABY                                 0xf77c
#define FA_BABY_CARRIAGE                        0xf77d
#define FA_BACKSPACE                            0xf55a
#define FA_BACKWARD                             0xf04a
#define FA_BACON                                0xf7e5
#define FA_BALANCE_SCALE                        0xf24e
#define FA_BALANCE_SCALE_LEFT                   0xf515
#define FA_BALANCE_SCALE_RIGHT                  0xf516
#define FA_BAN                                  0xf05e
#define FA_BAND_AID                             0xf462
#define FA_BARCODE                              0xf02a
#define FA_BARS                                 0xf0c9
#define FA_BASEBALL_BALL                        0xf433
#define FA_BASKETBALL_BALL                      0xf434
#define FA_BATH                                 0xf2cd
#define FA_BATTERY_EMPTY                        0xf244
#define FA_BATTERY_FULL                         0xf240
#define FA_BATTERY_HALF                         0xf242
#define FA_BATTERY_QUARTER                      0xf243
#define FA_BATTERY_THREE_QUARTERS               0xf241
#define FA_BED                                  0xf236
#define FA_BEER                                 0xf0fc
#define FA_BELL                                 0xf0f3
#define FA_BELL_SLASH                           0xf1f6
#define FA_BEZIER_CURVE                         0xf55b
#define FA_BIBLE                                0xf647
#define FA_BICYCLE                              0xf206
#define FA_BIKING                               0xf84a
#define FA_BINOCULARS                           0xf1e5
#define FA_BIOHAZARD                            0xf780
#define FA_BIRTHDAY_CAKE                        0xf1fd
#define FA_BLENDER                              0xf517
#define FA_BLENDER_PHONE                        0xf6b6
#define FA_BLIND                                0xf29d
#define FA_BLOG                                 0xf781
#define FA_BOLD                                 0xf032
#define FA_BOLT                                 0xf0e7
#define FA_BOMB                                 0xf1e2
#define FA_BONE                                 0xf5d7
#define FA_BONG                                 0xf55c
#define FA_BOOK                                 0xf02d
#define FA_BOOK_DEAD                            0xf6b7
#define FA_BOOK_MEDICAL                         0xf7e6
#define FA_BOOK_OPEN                            0xf518
#define FA_BOOK_READER                          0xf5da
#define FA_BOOKMARK                             0xf02e
#define FA_BORDER_ALL                           0xf84c
#define FA_BORDER_NONE                          0xf850
#define FA_BORDER_STYLE                         0xf853
#define FA_BOWLING_BALL                         0xf436
#define FA_BOX                                  0xf466
#define FA_BOX_OPEN                             0xf49e
#define FA_BOXES                                0xf468
#define FA_BRAILLE                              0xf2a1
#define FA_BRAIN                                0xf5dc
#define FA_BREAD_SLICE                          0xf7ec
#define FA_BRIEFCASE                            0xf0b1
#define FA_BRIEFCASE_MEDICAL                    0xf469
#define FA_BROADCAST_TOWER                      0xf519
#define FA_BROOM                                0xf51a
#define FA_BRUSH                                0xf55d
#define FA_BUG                                  0xf188
#define FA_BUILDING                             0xf1ad
#define FA_BULLHORN                             0xf0a1
#define FA_BULLSEYE                             0xf140
#define FA_BURN                                 0xf46a
#define FA_BUS                                  0xf207
#define FA_BUS_ALT                              0xf55e
#define FA_BUSINESS_TIME                        0xf64a
#define FA_CALCULATOR                           0xf1ec
#define FA_CALENDAR                             0xf133
#define FA_CALENDAR_ALT                         0xf073
#define FA_CALENDAR_CHECK                       0xf274
#define FA_CALENDAR_DAY                         0xf783
#define FA_CALENDAR_MINUS                       0xf272
#define FA_CALENDAR_PLUS                        0xf271
#define FA_CALENDAR_TIMES                       0xf273
#define FA_CALENDAR_WEEK                        0xf784
#define FA_CAMERA                               0xf030
#define FA_CAMERA_RETRO                         0xf083
#define FA_CAMPGROUND                           0xf6bb
#define FA_CANDY_CANE                           0xf786
#define FA_CANNABIS                             0xf55f
#define FA_CAPSULES                             0xf46b
#define FA_CAR                                  0xf1b9
#define FA_CAR_ALT                              0xf5de
#define FA_CAR_BATTERY                          0xf5df
#define FA_CAR_CRASH                            0xf5e1
#define FA_CAR_SIDE                             0xf5e4
#define FA_CARET_DOWN                           0xf0d7
#define FA_CARET_LEFT                           0xf0d9
#define FA_CARET_RIGHT                          0xf0da
#define FA_CARET_SQUARE_DOWN                    0xf150
#define FA_CARET_SQUARE_LEFT                    0xf191
#define FA_CARET_SQUARE_RIGHT                   0xf152
#define FA_CARET_SQUARE_UP                      0xf151
#define FA_CARET_UP                             0xf0d8
#define FA_CARROT                               0xf787
#define FA_CART_ARROW_DOWN                      0xf218
#define FA_CART_PLUS                            0xf217
#define FA_CASH_REGISTER                        0xf788
#define FA_CAT                                  0xf6be
#define FA_CERTIFICATE                          0xf0a3
#define FA_CHAIR                                0xf6c0
#define FA_CHALKBOARD                           0xf51b
#define FA_CHALKBOARD_TEACHER                   0xf51c
#define FA_CHARGING_STATION                     0xf5e7
#define FA_CHART_AREA                           0xf1fe
#define FA_CHART_BAR                            0xf080
#define FA_CHART_LINE                           0xf201
#define FA_CHART_PIE                            0xf200
#define FA_CHECK                                0xf00c
#define FA_CHECK_CIRCLE                         0xf058
#define FA_CHECK_DOUBLE                         0xf560
#define FA_CHECK_SQUARE                         0xf14a
#define FA_CHEESE                               0xf7ef
#define FA_CHESS                                0xf439
#define FA_CHESS_BISHOP                         0xf43a
#define FA_CHESS_BOARD                          0xf43c
#define FA_CHESS_KING                           0xf43f
#define FA_CHESS_KNIGHT                         0xf441
#define FA_CHESS_PAWN                           0xf443
#define FA_CHESS_QUEEN                          0xf445
#define FA_CHESS_ROOK                           0xf447
#define FA_CHEVRON_CIRCLE_DOWN                  0xf13a
#define FA_CHEVRON_CIRCLE_LEFT                  0xf137
#define FA_CHEVRON_CIRCLE_RIGHT                 0xf138
#define FA_CHEVRON_CIRCLE_UP                    0xf139
#define FA_CHEVRON_DOWN                         0xf078
#define FA_CHEVRON_LEFT                         0xf053
#define FA_CHEVRON_RIGHT                        0xf054
#define FA_CHEVRON_UP                           0xf077
#define FA_CHILD                                0xf1ae
#define FA_CHURCH                               0xf51d
#define FA_CIRCLE                               0xf111
#define FA_CIRCLE_NOTCH                         0xf1ce
#define FA_CITY                                 0xf64f
#define FA_CLINIC_MEDICAL                       0xf7f2
#define FA_CLIPBOARD                            0xf328
#define FA_CLIPBOARD_CHECK                      0xf46c
#define FA_CLIPBOARD_LIST                       0xf46d
#define FA_CLOCK                                0xf017
#define FA_CLONE                                0xf24d
#define FA_CLOSED_CAPTIONING                    0xf20a
#define FA_CLOUD                                0xf0c2
#define FA_CLOUD_DOWNLOAD_ALT                   0xf381
#define FA_CLOUD_MEATBALL                       0xf73b
#define FA_CLOUD_MOON                           0xf6c3
#define FA_CLOUD_MOON_RAIN                      0xf73c
#define FA_CLOUD_RAIN                           0xf73d
#define FA_CLOUD_SHOWERS_HEAVY                  0xf740
#define FA_CLOUD_SUN                            0xf6c4
#define FA_CLOUD_SUN_RAIN                       0xf743
#define FA_CLOUD_UPLOAD_ALT                     0xf382
#define FA_COCKTAIL                             0xf561
#define FA_CODE                                 0xf121
#define FA_CODE_BRANCH                          0xf126
#define FA_COFFEE                               0xf0f4
#define FA_COG                                  0xf013
#define FA_COGS                                 0xf085
#define FA_COINS                                0xf51e
#define FA_COLUMNS                              0xf0db
#define FA_COMMENT                              0xf075
#define FA_COMMENT_ALT                          0xf27a
#define FA_COMMENT_DOLLAR                       0xf651
#define FA_COMMENT_DOTS                         0xf4ad
#define FA_COMMENT_MEDICAL                      0xf7f5
#define FA_COMMENT_SLASH                        0xf4b3
#define FA_COMMENTS                             0xf086
#define FA_COMMENTS_DOLLAR                      0xf653
#define FA_COMPACT_DISC                         0xf51f
#define FA_COMPASS                              0xf14e
#define FA_COMPRESS                             0xf066
#define FA_COMPRESS_ARROWS_ALT                  0xf78c
#define FA_CONCIERGE_BELL                       0xf562
#define FA_COOKIE                               0xf563
#define FA_COOKIE_BITE                          0xf564
#define FA_COPY                                 0xf0c5
#define FA_COPYRIGHT                            0xf1f9
#define FA_COUCH                                0xf4b8
#define FA_CREDIT_CARD                          0xf09d
#define FA_CROP                                 0xf125
#define FA_CROP_ALT                             0xf565
#define FA_CROSS                                0xf654
#define FA_CROSSHAIRS                           0xf05b
#define FA_CROW                                 0xf520
#define FA_CROWN                                0xf521
#define FA_CRUTCH                               0xf7f7
#define FA_CUBE                                 0xf1b2
#define FA_CUBES                                0xf1b3
#define FA_CUT                                  0xf0c4
#define FA_DATABASE                             0xf1c0
#define FA_DEAF                                 0xf2a4
#define FA_DEMOCRAT                             0xf747
#define FA_DESKTOP                              0xf108
#define FA_DHARMACHAKRA                         0xf655
#define FA_DIAGNOSES                            0xf470
#define FA_DICE                                 0xf522
#define FA_DICE_D20                             0xf6cf
#define FA_DICE_D6                              0xf6d1
#define FA_DICE_FIVE                            0xf523
#define FA_DICE_FOUR                            0xf524
#define FA_DICE_ONE                             0xf525
#define FA_DICE_SIX                             0xf526
#define FA_DICE_THREE                           0xf527
#define FA_DICE_TWO                             0xf528
#define FA_DIGITAL_TACHOGRAPH                   0xf566
#define FA_DIRECTIONS                           0xf5eb
#define FA_DIVIDE                               0xf529
#define FA_DIZZY                                0xf567
#define FA_DNA                                  0xf471
#define FA_DOG                                  0xf6d3
#define FA_DOLLAR_SIGN                          0xf155
#define FA_DOLLY                                0xf472
#define FA_DOLLY_FLATBED                        0xf474
#define FA_DONATE                               0xf4b9
#define FA_DOOR_CLOSED                          0xf52a
#define FA_DOOR_OPEN                            0xf52b
#define FA_DOT_CIRCLE                           0xf192
#define FA_DOVE                                 0xf4ba
#define FA_DOWNLOAD                             0xf019
#define FA_DRAFTING_COMPASS                     0xf568
#define FA_DRAGON                               0xf6d5
#define FA_DRAW_POLYGON                         0xf5ee
#define FA_DRUM                                 0xf569
#define FA_DRUM_STEELPAN                        0xf56a
#define FA_DRUMSTICK_BITE                       0xf6d7
#define FA_DUMBBELL                             0xf44b
#define FA_DUMPSTER                             0xf793
#define FA_DUMPSTER_FIRE                        0xf794
#define FA_DUNGEON                              0xf6d9
#define FA_EDIT                                 0xf044
#define FA_EGG                                  0xf7fb
#define FA_EJECT                                0xf052
#define FA_ELLIPSIS_H                           0xf141
#define FA_ELLIPSIS_V                           0xf142
#define FA_ENVELOPE                             0xf0e0
#define FA_ENVELOPE_OPEN                        0xf2b6
#define FA_ENVELOPE_OPEN_TEXT                   0xf658
#define FA_ENVELOPE_SQUARE                      0xf199
#define FA_EQUALS                               0xf52c
#define FA_ERASER                               0xf12d
#define FA_ETHERNET                             0xf796
#define FA_EURO_SIGN                            0xf153
#define FA_EXCHANGE_ALT                         0xf362
#define FA_EXCLAMATION                          0xf12a
#define FA_EXCLAMATION_CIRCLE                   0xf06a
#define FA_EXCLAMATION_TRIANGLE                 0xf071
#define FA_EXPAND                               0xf065
#define FA_EXPAND_ARROWS_ALT                    0xf31e
#define FA_EXTERNAL_LINK_ALT                    0xf35d
#define FA_EXTERNAL_LINK_SQUARE_ALT             0xf360
#define FA_EYE                                  0xf06e
#define FA_EYE_DROPPER                          0xf1fb
#define FA_EYE_SLASH                            0xf070
#define FA_FAN                                  0xf863
#define FA_FAST_BACKWARD                        0xf049
#define FA_FAST_FORWARD                         0xf050
#define FA_FAX                                  0xf1ac
#define FA_FEATHER                              0xf52d
#define FA_FEATHER_ALT                          0xf56b
#define FA_FEMALE                               0xf182
#define FA_FIGHTER_JET                          0xf0fb
#define FA_FILE                                 0xf15b
#define FA_FILE_ALT                             0xf15c
#define FA_FILE_ARCHIVE                         0xf1c6
#define FA_FILE_AUDIO                           0xf1c7
#define FA_FILE_CODE                            0xf1c9
#define FA_FILE_CONTRACT                        0xf56c
#define FA_FILE_CSV                             0xf6dd
#define FA_FILE_DOWNLOAD                        0xf56d
#define FA_FILE_EXCEL                           0xf1c3
#define FA_FILE_EXPORT                          0xf56e
#define FA_FILE_IMAGE                           0xf1c5
#define FA_FILE_IMPORT                          0xf56f
#define FA_FILE_INVOICE                         0xf570
#define FA_FILE_INVOICE_DOLLAR                  0xf571
#define FA_FILE_MEDICAL                         0xf477
#define FA_FILE_MEDICAL_ALT                     0xf478
#define FA_FILE_PDF                             0xf1c1
#define FA_FILE_POWERPOINT                      0xf1c4
#define FA_FILE_PRESCRIPTION                    0xf572
#define FA_FILE_SIGNATURE                       0xf573
#define FA_FILE_UPLOAD                          0xf574
#define FA_FILE_VIDEO                           0xf1c8
#define FA_FILE_WORD                            0xf1c2
#define FA_FILL                                 0xf575
#define FA_FILL_DRIP                            0xf576
#define FA_FILM                                 0xf008
#define FA_FILTER                               0xf0b0
#define FA_FINGERPRINT                          0xf577
#define FA_FIRE                                 0xf06d
#define FA_FIRE_ALT                             0xf7e4
#define FA_FIRE_EXTINGUISHER                    0xf134
#define FA_FIRST_AID                            0xf479
#define FA_FISH                                 0xf578
#define FA_FIST_RAISED                          0xf6de
#define FA_FLAG                                 0xf024
#define FA_FLAG_CHECKERED                       0xf11e
#define FA_FLAG_USA                             0xf74d
#define FA_FLASK                                0xf0c3
#define FA_FLUSHED                              0xf579
#define FA_FOLDER                               0xf07b
#define FA_FOLDER_MINUS                         0xf65d
#define FA_FOLDER_OPEN                          0xf07c
#define FA_FOLDER_PLUS                          0xf65e
#define FA_FONT                                 0xf031
#define FA_FONT_AWESOME_LOGO_FULL               0xf4e6
#define FA_FOOTBALL_BALL                        0xf44e
#define FA_FORWARD                              0xf04e
#define FA_FROG                                 0xf52e
#define FA_FROWN                                0xf119
#define FA_FROWN_OPEN                           0xf57a
#define FA_FUNNEL_DOLLAR                        0xf662
#define FA_FUTBOL                               0xf1e3
#define FA_GAMEPAD                              0xf11b
#define FA_GAS_PUMP                             0xf52f
#define FA_GAVEL                                0xf0e3
#define FA_GEM                                  0xf3a5
#define FA_GENDERLESS                           0xf22d
#define FA_GHOST                                0xf6e2
#define FA_GIFT                                 0xf06b
#define FA_GIFTS                                0xf79c
#define FA_GLASS_CHEERS                         0xf79f
#define FA_GLASS_MARTINI                        0xf000
#define FA_GLASS_MARTINI_ALT                    0xf57b
#define FA_GLASS_WHISKEY                        0xf7a0
#define FA_GLASSES                              0xf530
#define FA_GLOBE                                0xf0ac
#define FA_GLOBE_AFRICA                         0xf57c
#define FA_GLOBE_AMERICAS                       0xf57d
#define FA_GLOBE_ASIA                           0xf57e
#define FA_GLOBE_EUROPE                         0xf7a2
#define FA_GOLF_BALL                            0xf450
#define FA_GOPURAM                              0xf664
#define FA_GRADUATION_CAP                       0xf19d
#define FA_GREATER_THAN                         0xf531
#define FA_GREATER_THAN_EQUAL                   0xf532
#define FA_GRIMACE                              0xf57f
#define FA_GRIN                                 0xf580
#define FA_GRIN_ALT                             0xf581
#define FA_GRIN_BEAM                            0xf582
#define FA_GRIN_BEAM_SWEAT                      0xf583
#define FA_GRIN_HEARTS                          0xf584
#define FA_GRIN_SQUINT                          0xf585
#define FA_GRIN_SQUINT_TEARS                    0xf586
#define FA_GRIN_STARS                           0xf587
#define FA_GRIN_TEARS                           0xf588
#define FA_GRIN_TONGUE                          0xf589
#define FA_GRIN_TONGUE_SQUINT                   0xf58a
#define FA_GRIN_TONGUE_WINK                     0xf58b
#define FA_GRIN_WINK                            0xf58c
#define FA_GRIP_HORIZONTAL                      0xf58d
#define FA_GRIP_LINES                           0xf7a4
#define FA_GRIP_LINES_VERTICAL                  0xf7a5
#define FA_GRIP_VERTICAL                        0xf58e
#define FA_GUITAR                               0xf7a6
#define FA_H_SQUARE                             0xf0fd
#define FA_HAMBURGER                            0xf805
#define FA_HAMMER                               0xf6e3
#define FA_HAMSA                                0xf665
#define FA_HAND_HOLDING                         0xf4bd
#define FA_HAND_HOLDING_HEART                   0xf4be
#define FA_HAND_HOLDING_USD                     0xf4c0
#define FA_HAND_LIZARD                          0xf258
#define FA_HAND_MIDDLE_FINGER                   0xf806
#define FA_HAND_PAPER                           0xf256
#define FA_HAND_PEACE                           0xf25b
#define FA_HAND_POINT_DOWN                      0xf0a7
#define FA_HAND_POINT_LEFT                      0xf0a5
#define FA_HAND_POINT_RIGHT                     0xf0a4
#define FA_HAND_POINT_UP                        0xf0a6
#define FA_HAND_POINTER                         0xf25a
#define FA_HAND_ROCK                            0xf255
#define FA_HAND_SCISSORS                        0xf257
#define FA_HAND_SPOCK                           0xf259
#define FA_HANDS                                0xf4c2
#define FA_HANDS_HELPING                        0xf4c4
#define FA_HANDSHAKE                            0xf2b5
#define FA_HANUKIAH                             0xf6e6
#define FA_HARD_HAT                             0xf807
#define FA_HASHTAG                              0xf292
#define FA_HAT_WIZARD                           0xf6e8
#define FA_HAYKAL                               0xf666
#define FA_HDD                                  0xf0a0
#define FA_HEADING                              0xf1dc
#define FA_HEADPHONES                           0xf025
#define FA_HEADPHONES_ALT                       0xf58f
#define FA_HEADSET                              0xf590
#define FA_HEART                                0xf004
#define FA_HEART_BROKEN                         0xf7a9
#define FA_HEARTBEAT                            0xf21e
#define FA_HELICOPTER                           0xf533
#define FA_HIGHLIGHTER                          0xf591
#define FA_HIKING                               0xf6ec
#define FA_HIPPO                                0xf6ed
#define FA_HISTORY                              0xf1da
#define FA_HOCKEY_PUCK                          0xf453
#define FA_HOLLY_BERRY                          0xf7aa
#define FA_HOME                                 0xf015
#define FA_HORSE                                0xf6f0
#define FA_HORSE_HEAD                           0xf7ab
#define FA_HOSPITAL                             0xf0f8
#define FA_HOSPITAL_ALT                         0xf47d
#define FA_HOSPITAL_SYMBOL                      0xf47e
#define FA_HOT_TUB                              0xf593
#define FA_HOTDOG                               0xf80f
#define FA_HOTEL                                0xf594
#define FA_HOURGLASS                            0xf254
#define FA_HOURGLASS_END                        0xf253
#define FA_HOURGLASS_HALF                       0xf252
#define FA_HOURGLASS_START                      0xf251
#define FA_HOUSE_DAMAGE                         0xf6f1
#define FA_HRYVNIA                              0xf6f2
#define FA_I_CURSOR                             0xf246
#define FA_ICE_CREAM                            0xf810
#define FA_ICICLES                              0xf7ad
#define FA_ICONS                                0xf86d
#define FA_ID_BADGE                             0xf2c1
#define FA_ID_CARD                              0xf2c2
#define FA_ID_CARD_ALT                          0xf47f
#define FA_IGLOO                                0xf7ae
#define FA_IMAGE                                0xf03e
#define FA_IMAGES                               0xf302
#define FA_INBOX                                0xf01c
#define FA_INDENT                               0xf03c
#define FA_INDUSTRY                             0xf275
#define FA_INFINITY                             0xf534
#define FA_INFO                                 0xf129
#define FA_INFO_CIRCLE                          0xf05a
#define FA_ITALIC                               0xf033
#define FA_JEDI                                 0xf669
#define FA_JOINT                                0xf595
#define FA_JOURNAL_WHILLS                       0xf66a
#define FA_KAABA                                0xf66b
#define FA_KEY                                  0xf084
#define FA_KEYBOARD                             0xf11c
#define FA_KHANDA                               0xf66d
#define FA_KISS                                 0xf596
#define FA_KISS_BEAM                            0xf597
#define FA_KISS_WINK_HEART                      0xf598
#define FA_KIWI_BIRD                            0xf535
#define FA_LANDMARK                             0xf66f
#define FA_LANGUAGE                             0xf1ab
#define FA_LAPTOP                               0xf109
#define FA_LAPTOP_CODE                          0xf5fc
#define FA_LAPTOP_MEDICAL                       0xf812
#define FA_LAUGH                                0xf599
#define FA_LAUGH_BEAM                           0xf59a
#define FA_LAUGH_SQUINT                         0xf59b
#define FA_LAUGH_WINK                           0xf59c
#define FA_LAYER_GROUP                          0xf5fd
#define FA_LEAF                                 0xf06c
#define FA_LEMON                                0xf094
#define FA_LESS_THAN                            0xf536
#define FA_LESS_THAN_EQUAL                      0xf537
#define FA_LEVEL_DOWN_ALT                       0xf3be
#define FA_LEVEL_UP_ALT                         0xf3bf
#define FA_LIFE_RING                            0xf1cd
#define FA_LIGHTBULB                            0xf0eb
#define FA_LINK                                 0xf0c1
#define FA_LIRA_SIGN                            0xf195
#define FA_LIST                                 0xf03a
#define FA_LIST_ALT                             0xf022
#define FA_LIST_OL                              0xf0cb
#define FA_LIST_UL                              0xf0ca
#define FA_LOCATION_ARROW                       0xf124
#define FA_LOCK                                 0xf023
#define FA_LOCK_OPEN                            0xf3c1
#define FA_LONG_ARROW_ALT_DOWN                  0xf309
#define FA_LONG_ARROW_ALT_LEFT                  0xf30a
#define FA_LONG_ARROW_ALT_RIGHT                 0xf30b
#define FA_LONG_ARROW_ALT_UP                    0xf30c
#define FA_LOW_VISION                           0xf2a8
#define FA_LUGGAGE_CART                         0xf59d
#define FA_MAGIC                                0xf0d0
#define FA_MAGNET                               0xf076
#define FA_MAIL_BULK                            0xf674
#define FA_MALE                                 0xf183
#define FA_MAP                                  0xf279
#define FA_MAP_MARKED                           0xf59f
#define FA_MAP_MARKED_ALT                       0xf5a0
#define FA_MAP_MARKER                           0xf041
#define FA_MAP_MARKER_ALT                       0xf3c5
#define FA_MAP_PIN                              0xf276
#define FA_MAP_SIGNS                            0xf277
#define FA_MARKER                               0xf5a1
#define FA_MARS                                 0xf222
#define FA_MARS_DOUBLE                          0xf227
#define FA_MARS_STROKE                          0xf229
#define FA_MARS_STROKE_H                        0xf22b
#define FA_MARS_STROKE_V                        0xf22a
#define FA_MASK                                 0xf6fa
#define FA_MEDAL                                0xf5a2
#define FA_MEDKIT                               0xf0fa
#define FA_MEH                                  0xf11a
#define FA_MEH_BLANK                            0xf5a4
#define FA_MEH_ROLLING_EYES                     0xf5a5
#define FA_MEMORY                               0xf538
#define FA_MENORAH                              0xf676
#define FA_MERCURY                              0xf223
#define FA_METEOR                               0xf753
#define FA_MICROCHIP                            0xf2db
#define FA_MICROPHONE                           0xf130
#define FA_MICROPHONE_ALT                       0xf3c9
#define FA_MICROPHONE_ALT_SLASH                 0xf539
#define FA_MICROPHONE_SLASH                     0xf131
#define FA_MICROSCOPE                           0xf610
#define FA_MINUS                                0xf068
#define FA_MINUS_CIRCLE                         0xf056
#define FA_MINUS_SQUARE                         0xf146
#define FA_MITTEN                               0xf7b5
#define FA_MOBILE                               0xf10b
#define FA_MOBILE_ALT                           0xf3cd
#define FA_MONEY_BILL                           0xf0d6
#define FA_MONEY_BILL_ALT                       0xf3d1
#define FA_MONEY_BILL_WAVE                      0xf53a
#define FA_MONEY_BILL_WAVE_ALT                  0xf53b
#define FA_MONEY_CHECK                          0xf53c
#define FA_MONEY_CHECK_ALT                      0xf53d
#define FA_MONUMENT                             0xf5a6
#define FA_MOON                                 0xf186
#define FA_MORTAR_PESTLE                        0xf5a7
#define FA_MOSQUE                               0xf678
#define FA_MOTORCYCLE                           0xf21c
#define FA_MOUNTAIN                             0xf6fc
#define FA_MOUSE_POINTER                        0xf245
#define FA_MUG_HOT                              0xf7b6
#define FA_MUSIC                                0xf001
#define FA_NETWORK_WIRED                        0xf6ff
#define FA_NEUTER                               0xf22c
#define FA_NEWSPAPER                            0xf1ea
#define FA_NOT_EQUAL                            0xf53e
#define FA_NOTES_MEDICAL                        0xf481
#define FA_OBJECT_GROUP                         0xf247
#define FA_OBJECT_UNGROUP                       0xf248
#define FA_OIL_CAN                              0xf613
#define FA_OM                                   0xf679
#define FA_OTTER                                0xf700
#define FA_OUTDENT                              0xf03b
#define FA_PAGER                                0xf815
#define FA_PAINT_BRUSH                          0xf1fc
#define FA_PAINT_ROLLER                         0xf5aa
#define FA_PALETTE                              0xf53f
#define FA_PALLET                               0xf482
#define FA_PAPER_PLANE                          0xf1d8
#define FA_PAPERCLIP                            0xf0c6
#define FA_PARACHUTE_BOX                        0xf4cd
#define FA_PARAGRAPH                            0xf1dd
#define FA_PARKING                              0xf540
#define FA_PASSPORT                             0xf5ab
#define FA_PASTAFARIANISM                       0xf67b
#define FA_PASTE                                0xf0ea
#define FA_PAUSE                                0xf04c
#define FA_PAUSE_CIRCLE                         0xf28b
#define FA_PAW                                  0xf1b0
#define FA_PEACE                                0xf67c
#define FA_PEN                                  0xf304
#define FA_PEN_ALT                              0xf305
#define FA_PEN_FANCY                            0xf5ac
#define FA_PEN_NIB                              0xf5ad
#define FA_PEN_SQUARE                           0xf14b
#define FA_PENCIL_ALT                           0xf303
#define FA_PENCIL_RULER                         0xf5ae
#define FA_PEOPLE_CARRY                         0xf4ce
#define FA_PEPPER_HOT                           0xf816
#define FA_PERCENT                              0xf295
#define FA_PERCENTAGE                           0xf541
#define FA_PERSON_BOOTH                         0xf756
#define FA_PHONE                                0xf095
#define FA_PHONE_ALT                            0xf879
#define FA_PHONE_SLASH                          0xf3dd
#define FA_PHONE_SQUARE                         0xf098
#define FA_PHONE_SQUARE_ALT                     0xf87b
#define FA_PHONE_VOLUME                         0xf2a0
#define FA_PHOTO_VIDEO                          0xf87c
#define FA_PIGGY_BANK                           0xf4d3
#define FA_PILLS                                0xf484
#define FA_PIZZA_SLICE                          0xf818
#define FA_PLACE_OF_WORSHIP                     0xf67f
#define FA_PLANE                                0xf072
#define FA_PLANE_ARRIVAL                        0xf5af
#define FA_PLANE_DEPARTURE                      0xf5b0
#define FA_PLAY                                 0xf04b
#define FA_PLAY_CIRCLE                          0xf144
#define FA_PLUG                                 0xf1e6
#define FA_PLUS                                 0xf067
#define FA_PLUS_CIRCLE                          0xf055
#define FA_PLUS_SQUARE                          0xf0fe
#define FA_PODCAST                              0xf2ce
#define FA_POLL                                 0xf681
#define FA_POLL_H                               0xf682
#define FA_POO                                  0xf2fe
#define FA_POO_STORM                            0xf75a
#define FA_POOP                                 0xf619
#define FA_PORTRAIT                             0xf3e0
#define FA_POUND_SIGN                           0xf154
#define FA_POWER_OFF                            0xf011
#define FA_PRAY                                 0xf683
#define FA_PRAYING_HANDS                        0xf684
#define FA_PRESCRIPTION                         0xf5b1
#define FA_PRESCRIPTION_BOTTLE                  0xf485
#define FA_PRESCRIPTION_BOTTLE_ALT              0xf486
#define FA_PRINT                                0xf02f
#define FA_PROCEDURES                           0xf487
#define FA_PROJECT_DIAGRAM                      0xf542
#define FA_PUZZLE_PIECE                         0xf12e
#define FA_QRCODE                               0xf029
#define FA_QUESTION                             0xf128
#define FA_QUESTION_CIRCLE                      0xf059
#define FA_QUIDDITCH                            0xf458
#define FA_QUOTE_LEFT                           0xf10d
#define FA_QUOTE_RIGHT                          0xf10e
#define FA_QURAN                                0xf687
#define FA_RADIATION                            0xf7b9
#define FA_RADIATION_ALT                        0xf7ba
#define FA_RAINBOW                              0xf75b
#define FA_RANDOM                               0xf074
#define FA_RECEIPT                              0xf543
#define FA_RECYCLE                              0xf1b8
#define FA_REDO                                 0xf01e
#define FA_REDO_ALT                             0xf2f9
#define FA_REGISTERED                           0xf25d
#define FA_REMOVE_FORMAT                        0xf87d
#define FA_REPLY                                0xf3e5
#define FA_REPLY_ALL                            0xf122
#define FA_REPUBLICAN                           0xf75e
#define FA_RESTROOM                             0xf7bd
#define FA_RETWEET                              0xf079
#define FA_RIBBON                               0xf4d6
#define FA_RING                                 0xf70b
#define FA_ROAD                                 0xf018
#define FA_ROBOT                                0xf544
#define FA_ROCKET                               0xf135
#define FA_ROUTE                                0xf4d7
#define FA_RSS                                  0xf09e
#define FA_RSS_SQUARE                           0xf143
#define FA_RUBLE_SIGN                           0xf158
#define FA_RULER                                0xf545
#define FA_RULER_COMBINED                       0xf546
#define FA_RULER_HORIZONTAL                     0xf547
#define FA_RULER_VERTICAL                       0xf548
#define FA_RUNNING                              0xf70c
#define FA_RUPEE_SIGN                           0xf156
#define FA_SAD_CRY                              0xf5b3
#define FA_SAD_TEAR                             0xf5b4
#define FA_SATELLITE                            0xf7bf
#define FA_SATELLITE_DISH                       0xf7c0
#define FA_SAVE                                 0xf0c7
#define FA_SCHOOL                               0xf549
#define FA_SCREWDRIVER                          0xf54a
#define FA_SCROLL                               0xf70e
#define FA_SD_CARD                              0xf7c2
#define FA_SEARCH                               0xf002
#define FA_SEARCH_DOLLAR                        0xf688
#define FA_SEARCH_LOCATION                      0xf689
#define FA_SEARCH_MINUS                         0xf010
#define FA_SEARCH_PLUS                          0xf00e
#define FA_SEEDLING                             0xf4d8
#define FA_SERVER                               0xf233
#define FA_SHAPES                               0xf61f
#define FA_SHARE                                0xf064
#define FA_SHARE_ALT                            0xf1e0
#define FA_SHARE_ALT_SQUARE                     0xf1e1
#define FA_SHARE_SQUARE                         0xf14d
#define FA_SHEKEL_SIGN                          0xf20b
#define FA_SHIELD_ALT                           0xf3ed
#define FA_SHIP                                 0xf21a
#define FA_SHIPPING_FAST                        0xf48b
#define FA_SHOE_PRINTS                          0xf54b
#define FA_SHOPPING_BAG                         0xf290
#define FA_SHOPPING_BASKET                      0xf291
#define FA_SHOPPING_CART                        0xf07a
#define FA_SHOWER                               0xf2cc
#define FA_SHUTTLE_VAN                          0xf5b6
#define FA_SIGN                                 0xf4d9
#define FA_SIGN_IN_ALT                          0xf2f6
#define FA_SIGN_LANGUAGE                        0xf2a7
#define FA_SIGN_OUT_ALT                         0xf2f5
#define FA_SIGNAL                               0xf012
#define FA_SIGNATURE                            0xf5b7
#define FA_SIM_CARD                             0xf7c4
#define FA_SITEMAP                              0xf0e8
#define FA_SKATING                              0xf7c5
#define FA_SKIING                               0xf7c9
#define FA_SKIING_NORDIC                        0xf7ca
#define FA_SKULL                                0xf54c
#define FA_SKULL_CROSSBONES                     0xf714
#define FA_SLASH                                0xf715
#define FA_SLEIGH                               0xf7cc
#define FA_SLIDERS_H                            0xf1de
#define FA_SMILE                                0xf118
#define FA_SMILE_BEAM                           0xf5b8
#define FA_SMILE_WINK                           0xf4da
#define FA_SMOG                                 0xf75f
#define FA_SMOKING                              0xf48d
#define FA_SMOKING_BAN                          0xf54d
#define FA_SMS                                  0xf7cd
#define FA_SNOWBOARDING                         0xf7ce
#define FA_SNOWFLAKE                            0xf2dc
#define FA_SNOWMAN                              0xf7d0
#define FA_SNOWPLOW                             0xf7d2
#define FA_SOCKS                                0xf696
#define FA_SOLAR_PANEL                          0xf5ba
#define FA_SORT                                 0xf0dc
#define FA_SORT_ALPHA_DOWN                      0xf15d
#define FA_SORT_ALPHA_DOWN_ALT                  0xf881
#define FA_SORT_ALPHA_UP                        0xf15e
#define FA_SORT_ALPHA_UP_ALT                    0xf882
#define FA_SORT_AMOUNT_DOWN                     0xf160
#define FA_SORT_AMOUNT_DOWN_ALT                 0xf884
#define FA_SORT_AMOUNT_UP                       0xf161
#define FA_SORT_AMOUNT_UP_ALT                   0xf885
#define FA_SORT_DOWN                            0xf0dd
#define FA_SORT_NUMERIC_DOWN                    0xf162
#define FA_SORT_NUMERIC_DOWN_ALT                0xf886
#define FA_SORT_NUMERIC_UP                      0xf163
#define FA_SORT_NUMERIC_UP_ALT                  0xf887
#define FA_SORT_UP                              0xf0de
#define FA_SPA                                  0xf5bb
#define FA_SPACE_SHUTTLE                        0xf197
#define FA_SPELL_CHECK                          0xf891
#define FA_SPIDER                               0xf717
#define FA_SPINNER                              0xf110
#define FA_SPLOTCH                              0xf5bc
#define FA_SPRAY_CAN                            0xf5bd
#define FA_SQUARE                               0xf0c8
#define FA_SQUARE_FULL                          0xf45c
#define FA_SQUARE_ROOT_ALT                      0xf698
#define FA_STAMP                                0xf5bf
#define FA_STAR                                 0xf005
#define FA_STAR_AND_CRESCENT                    0xf699
#define FA_STAR_HALF                            0xf089
#define FA_STAR_HALF_ALT                        0xf5c0
#define FA_STAR_OF_DAVID                        0xf69a
#define FA_STAR_OF_LIFE                         0xf621
#define FA_STEP_BACKWARD                        0xf048
#define FA_STEP_FORWARD                         0xf051
#define FA_STETHOSCOPE                          0xf0f1
#define FA_STICKY_NOTE                          0xf249
#define FA_STOP                                 0xf04d
#define FA_STOP_CIRCLE                          0xf28d
#define FA_STOPWATCH                            0xf2f2
#define FA_STORE                                0xf54e
#define FA_STORE_ALT                            0xf54f
#define FA_STREAM                               0xf550
#define FA_STREET_VIEW                          0xf21d
#define FA_STRIKETHROUGH                        0xf0cc
#define FA_STROOPWAFEL                          0xf551
#define FA_SUBSCRIPT                            0xf12c
#define FA_SUBWAY                               0xf239
#define FA_SUITCASE                             0xf0f2
#define FA_SUITCASE_ROLLING                     0xf5c1
#define FA_SUN                                  0xf185
#define FA_SUPERSCRIPT                          0xf12b
#define FA_SURPRISE                             0xf5c2
#define FA_SWATCHBOOK                           0xf5c3
#define FA_SWIMMER                              0xf5c4
#define FA_SWIMMING_POOL                        0xf5c5
#define FA_SYNAGOGUE                            0xf69b
#define FA_SYNC                                 0xf021
#define FA_SYNC_ALT                             0xf2f1
#define FA_SYRINGE                              0xf48e
#define FA_TABLE                                0xf0ce
#define FA_TABLE_TENNIS                         0xf45d
#define FA_TABLET                               0xf10a
#define FA_TABLET_ALT                           0xf3fa
#define FA_TABLETS                              0xf490
#define FA_TACHOMETER_ALT                       0xf3fd
#define FA_TAG                                  0xf02b
#define FA_TAGS                                 0xf02c
#define FA_TAPE                                 0xf4db
#define FA_TASKS                                0xf0ae
#define FA_TAXI                                 0xf1ba
#define FA_TEETH                                0xf62e
#define FA_TEETH_OPEN                           0xf62f
#define FA_TEMPERATURE_HIGH                     0xf769
#define FA_TEMPERATURE_LOW                      0xf76b
#define FA_TENGE                                0xf7d7
#define FA_TERMINAL                             0xf120
#define FA_TEXT_HEIGHT                          0xf034
#define FA_TEXT_WIDTH                           0xf035
#define FA_TH                                   0xf00a
#define FA_TH_LARGE                             0xf009
#define FA_TH_LIST                              0xf00b
#define FA_THEATER_MASKS                        0xf630
#define FA_THERMOMETER                          0xf491
#define FA_THERMOMETER_EMPTY                    0xf2cb
#define FA_THERMOMETER_FULL                     0xf2c7
#define FA_THERMOMETER_HALF                     0xf2c9
#define FA_THERMOMETER_QUARTER                  0xf2ca
#define FA_THERMOMETER_THREE_QUARTERS           0xf2c8
#define FA_THUMBS_DOWN                          0xf165
#define FA_THUMBS_UP                            0xf164
#define FA_THUMBTACK                            0xf08d
#define FA_TICKET_ALT                           0xf3ff
#define FA_TIMES                                0xf00d
#define FA_TIMES_CIRCLE                         0xf057
#define FA_TINT                                 0xf043
#define FA_TINT_SLASH                           0xf5c7
#define FA_TIRED                                0xf5c8
#define FA_TOGGLE_OFF                           0xf204
#define FA_TOGGLE_ON                            0xf205
#define FA_TOILET                               0xf7d8
#define FA_TOILET_PAPER                         0xf71e
#define FA_TOOLBOX                              0xf552
#define FA_TOOLS                                0xf7d9
#define FA_TOOTH                                0xf5c9
#define FA_TORAH                                0xf6a0
#define FA_TORII_GATE                           0xf6a1
#define FA_TRACTOR                              0xf722
#define FA_TRADEMARK                            0xf25c
#define FA_TRAFFIC_LIGHT                        0xf637
#define FA_TRAIN                                0xf238
#define FA_TRAM                                 0xf7da
#define FA_TRANSGENDER                          0xf224
#define FA_TRANSGENDER_ALT                      0xf225
#define FA_TRASH                                0xf1f8
#define FA_TRASH_ALT                            0xf2ed
#define FA_TRASH_RESTORE                        0xf829
#define FA_TRASH_RESTORE_ALT                    0xf82a
#define FA_TREE                                 0xf1bb
#define FA_TROPHY                               0xf091
#define FA_TRUCK                                0xf0d1
#define FA_TRUCK_LOADING                        0xf4de
#define FA_TRUCK_MONSTER                        0xf63b
#define FA_TRUCK_MOVING                         0xf4df
#define FA_TRUCK_PICKUP                         0xf63c
#define FA_TSHIRT                               0xf553
#define FA_TTY                                  0xf1e4
#define FA_TV                                   0xf26c
#define FA_UMBRELLA                             0xf0e9
#define FA_UMBRELLA_BEACH                       0xf5ca
#define FA_UNDERLINE                            0xf0cd
#define FA_UNDO                                 0xf0e2
#define FA_UNDO_ALT                             0xf2ea
#define FA_UNIVERSAL_ACCESS                     0xf29a
#define FA_UNIVERSITY                           0xf19c
#define FA_UNLINK                               0xf127
#define FA_UNLOCK                               0xf09c
#define FA_UNLOCK_ALT                           0xf13e
#define FA_UPLOAD                               0xf093
#define FA_USER                                 0xf007
#define FA_USER_ALT                             0xf406
#define FA_USER_ALT_SLASH                       0xf4fa
#define FA_USER_ASTRONAUT                       0xf4fb
#define FA_USER_CHECK                           0xf4fc
#define FA_USER_CIRCLE                          0xf2bd
#define FA_USER_CLOCK                           0xf4fd
#define FA_USER_COG                             0xf4fe
#define FA_USER_EDIT                            0xf4ff
#define FA_USER_FRIENDS                         0xf500
#define FA_USER_GRADUATE                        0xf501
#define FA_USER_INJURED                         0xf728
#define FA_USER_LOCK                            0xf502
#define FA_USER_MD                              0xf0f0
#define FA_USER_MINUS                           0xf503
#define FA_USER_NINJA                           0xf504
#define FA_USER_NURSE                           0xf82f
#define FA_USER_PLUS                            0xf234
#define FA_USER_SECRET                          0xf21b
#define FA_USER_SHIELD                          0xf505
#define FA_USER_SLASH                           0xf506
#define FA_USER_TAG                             0xf507
#define FA_USER_TIE                             0xf508
#define FA_USER_TIMES                           0xf235
#define FA_USERS                                0xf0c0
#define FA_USERS_COG                            0xf509
#define FA_UTENSIL_SPOON                        0xf2e5
#define FA_UTENSILS                             0xf2e7
#define FA_VECTOR_SQUARE                        0xf5cb
#define FA_VENUS                                0xf221
#define FA_VENUS_DOUBLE                         0xf226
#define FA_VENUS_MARS                           0xf228
#define FA_VIAL                                 0xf492
#define FA_VIALS                                0xf493
#define FA_VIDEO                                0xf03d
#define FA_VIDEO_SLASH                          0xf4e2
#define FA_VIHARA                               0xf6a7
#define FA_VOICEMAIL                            0xf897
#define FA_VOLLEYBALL_BALL                      0xf45f
#define FA_VOLUME_DOWN                          0xf027
#define FA_VOLUME_MUTE                          0xf6a9
#define FA_VOLUME_OFF                           0xf026
#define FA_VOLUME_UP                            0xf028
#define FA_VOTE_YEA                             0xf772
#define FA_VR_CARDBOARD                         0xf729
#define FA_WALKING                              0xf554
#define FA_WALLET                               0xf555
#define FA_WAREHOUSE                            0xf494
#define FA_WATER                                0xf773
#define FA_WAVE_SQUARE                          0xf83e
#define FA_WEIGHT                               0xf496
#define FA_WEIGHT_HANGING                       0xf5cd
#define FA_WHEELCHAIR                           0xf193
#define FA_WIFI                                 0xf1eb
#define FA_WIND                                 0xf72e
#define FA_WINDOW_CLOSE                         0xf410
#define FA_WINDOW_MAXIMIZE                      0xf2d0
#define FA_WINDOW_MINIMIZE                      0xf2d1
#define FA_WINDOW_RESTORE                       0xf2d2
#define FA_WINE_BOTTLE                          0xf72f
#define FA_WINE_GLASS                           0xf4e3
#define FA_WINE_GLASS_ALT                       0xf5ce
#define FA_WON_SIGN                             0xf159
#define FA_WRENCH                               0xf0ad
#define FA_X_RAY                                0xf497
#define FA_YEN_SIGN                             0xf157
#define FA_YIN_YANG                             0xf6ad

#endif // DOXYGEN_SHOULD_SKIP_THIS
