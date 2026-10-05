/*
 * Copyright (c) 2019-2020 Waqar Ahmed -- <waqar.17a@gmail.com>
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 *
 */

#include "qsourcehighliterthemes.h"

namespace QSourceHighlite {


// qsourcehighliterthemes.cpp
static QTextCharFormat makeFormat(const QColor &fg, const QColor &bg = QColor())
{
    QTextCharFormat f;
    f.setForeground(fg);
    if (bg.isValid())
        f.setBackground(bg);
    return f;
}


static QHash<QSourceHighliter::Token, QTextCharFormat> formats() {
    QHash<QSourceHighliter::Token, QTextCharFormat> _formats;

    QTextCharFormat defaultFormat = QTextCharFormat();


    _formats[QSourceHighliter::Token::MenuBar] = defaultFormat;
    _formats[QSourceHighliter::Token::MenuBarButtons] = defaultFormat;
    _formats[QSourceHighliter::Token::Sections] = defaultFormat;
    _formats[QSourceHighliter::Token::ToolBar] = defaultFormat;
    _formats[QSourceHighliter::Token::ToolBarButtons] = defaultFormat;
    _formats[QSourceHighliter::Token::CentralLayout] = defaultFormat;
    _formats[QSourceHighliter::Token::SettingsLayout] = defaultFormat;
    _formats[QSourceHighliter::Token::SettingsLayoutBorder] = defaultFormat;
    _formats[QSourceHighliter::Token::StringsText] = defaultFormat;
    _formats[QSourceHighliter::Token::ButtonsText] = defaultFormat;

    _formats[QSourceHighliter::Token::CodeBlock] = defaultFormat;
    _formats[QSourceHighliter::Token::CodeKeyWord] = defaultFormat;
    _formats[QSourceHighliter::Token::CodeString] = defaultFormat;
    _formats[QSourceHighliter::Token::CodeComment] = defaultFormat;
    _formats[QSourceHighliter::Token::CodeType] = defaultFormat;
    _formats[QSourceHighliter::Token::CodeOther] = defaultFormat;
    _formats[QSourceHighliter::Token::CodeNumLiteral] = defaultFormat;
    _formats[QSourceHighliter::Token::CodeBuiltIn] = defaultFormat;

    return _formats;
}
static QHash<QSourceHighliter::Token, QTextCharFormat> monokai() {
    QHash<QSourceHighliter::Token, QTextCharFormat> _formats = formats();

    _formats[QSourceHighliter::Token::MenuBar].setForeground(QColor(33, 33, 31));
    _formats[QSourceHighliter::Token::MenuBar].setBackground(QColor(39, 40, 34));       // #272822

    _formats[QSourceHighliter::Token::MenuBarButtons].setForeground(QColor(33, 33, 31));
    _formats[QSourceHighliter::Token::MenuBarButtons].setBackground(QColor(39, 40, 34));

    _formats[QSourceHighliter::Token::Sections].setForeground(QColor(54, 54, 51));
    _formats[QSourceHighliter::Token::Sections].setBackground(QColor(30, 31, 28));       // темнее основного

    _formats[QSourceHighliter::Token::ToolBar].setForeground(QColor(33, 33, 31));
    _formats[QSourceHighliter::Token::ToolBar].setBackground(QColor(39, 40, 34));

    _formats[QSourceHighliter::Token::ToolBarButtons].setForeground(QColor(33, 33, 31));
    _formats[QSourceHighliter::Token::ToolBarButtons].setBackground(QColor(39, 40, 34));

    _formats[QSourceHighliter::Token::CentralLayout].setForeground(QColor(69, 69, 64));
    _formats[QSourceHighliter::Token::CentralLayout].setBackground(QColor(30, 31, 28));   // #1e1f1c

    _formats[QSourceHighliter::Token::SettingsLayout].setForeground(QColor(84, 84, 80));
    _formats[QSourceHighliter::Token::SettingsLayout].setBackground(QColor(39, 40, 34));

    _formats[QSourceHighliter::Token::SettingsLayoutBorder].setForeground(QColor(255, 255, 255));
    _formats[QSourceHighliter::Token::SettingsLayoutBorder].setBackground(QColor(39, 40, 34));

    _formats[QSourceHighliter::Token::StringsText].setForeground(QColor(255, 255, 255));

    _formats[QSourceHighliter::Token::ButtonsText].setForeground(QColor(255, 255, 255));

    _formats[QSourceHighliter::Token::CodeBlock].setForeground(QColor(227, 226, 214));
    _formats[QSourceHighliter::Token::CodeBlock].setBackground(QColor(39, 40, 34));       // #272822

    _formats[QSourceHighliter::Token::CodeKeyWord].setForeground(QColor(249, 38, 114));
    _formats[QSourceHighliter::Token::CodeString].setForeground(QColor(230, 219, 116));
    _formats[QSourceHighliter::Token::CodeComment].setForeground(QColor(117, 113, 94));
    _formats[QSourceHighliter::Token::CodeType].setForeground(QColor(102, 217, 239));
    _formats[QSourceHighliter::Token::CodeOther].setForeground(QColor(249, 38, 114));
    _formats[QSourceHighliter::Token::CodeNumLiteral].setForeground(QColor(174, 129, 255));
    _formats[QSourceHighliter::Token::CodeBuiltIn].setForeground(QColor(166, 226, 46));

    return _formats;
}

/*
Islands Dark
Background: #191a1c - Deep, comfortable dark
Foreground: #bcbec4 - Soft gray-white
*/
static QHash<QSourceHighliter::Token, QTextCharFormat> darkTheme() {
    QHash<QSourceHighliter::Token, QTextCharFormat> _formats = formats();

           // UI-токены: foreground + background
    _formats[QSourceHighliter::Token::MenuBar].setForeground(QColor(1, 1, 1));
    _formats[QSourceHighliter::Token::MenuBar].setBackground(QColor(25, 26, 28));        // #191a1c

    _formats[QSourceHighliter::Token::MenuBarButtons].setForeground(QColor(1, 1, 1));
    _formats[QSourceHighliter::Token::MenuBarButtons].setBackground(QColor(25, 26, 28));

    _formats[QSourceHighliter::Token::Sections].setForeground(QColor(1, 1, 1));
    _formats[QSourceHighliter::Token::Sections].setBackground(QColor(20, 21, 23));       // темнее

    _formats[QSourceHighliter::Token::ToolBar].setForeground(QColor(27, 27, 27));
    _formats[QSourceHighliter::Token::ToolBar].setBackground(QColor(25, 26, 28));

    _formats[QSourceHighliter::Token::ToolBarButtons].setForeground(QColor(27, 27, 27));
    _formats[QSourceHighliter::Token::ToolBarButtons].setBackground(QColor(25, 26, 28));

    _formats[QSourceHighliter::Token::CentralLayout].setForeground(QColor(7, 7, 7));
    _formats[QSourceHighliter::Token::CentralLayout].setBackground(QColor(20, 21, 23));   // #141517

    _formats[QSourceHighliter::Token::SettingsLayout].setForeground(QColor(56, 55, 55));
    _formats[QSourceHighliter::Token::SettingsLayout].setBackground(QColor(25, 26, 28));

    _formats[QSourceHighliter::Token::SettingsLayoutBorder].setForeground(QColor(56, 55, 55));
    _formats[QSourceHighliter::Token::SettingsLayoutBorder].setBackground(QColor(25, 26, 28));

    _formats[QSourceHighliter::Token::StringsText].setForeground(QColor(255, 255, 255));

    _formats[QSourceHighliter::Token::ButtonsText].setForeground(QColor(255, 255, 255));

    _formats[QSourceHighliter::Token::CodeBlock].setForeground(QColor(188,190,196));      // #191a1c
    _formats[QSourceHighliter::Token::CodeBlock].setBackground(QColor(25, 26, 28));       // фон редактора

    _formats[QSourceHighliter::Token::CodeKeyWord].setForeground(QColor(207,142,109));    // #cf8e6d
    _formats[QSourceHighliter::Token::CodeString].setForeground(QColor(106,171,115));     // #6aab73
    _formats[QSourceHighliter::Token::CodeComment].setForeground(QColor(122,126,133));    // #7a7e85
    _formats[QSourceHighliter::Token::CodeType].setForeground(QColor(102, 217, 239));
    _formats[QSourceHighliter::Token::CodeOther].setForeground(QColor(188,190,196));      // #bcbec4
    _formats[QSourceHighliter::Token::CodeNumLiteral].setForeground(QColor(42,172,184));  // #2aacb8
    _formats[QSourceHighliter::Token::CodeBuiltIn].setForeground(QColor(86,168,245));     // #56a8f5

    return _formats;
}

/*
Islands Light
Background: #ffffff - Pure white
Foreground: #000000 - Rich black
*/
static QHash<QSourceHighliter::Token, QTextCharFormat> lIghtTheme() {
    QHash<QSourceHighliter::Token, QTextCharFormat> _formats = formats();

    _formats[QSourceHighliter::Token::MenuBar].setForeground(QColor(245, 243, 243));
    _formats[QSourceHighliter::Token::MenuBar].setBackground(QColor(255, 255, 255));     // #ffffff

    _formats[QSourceHighliter::Token::MenuBarButtons].setForeground(QColor(245, 243, 243));
    _formats[QSourceHighliter::Token::MenuBarButtons].setBackground(QColor(255, 255, 255));

    _formats[QSourceHighliter::Token::Sections].setForeground(QColor(245, 243, 243));
    _formats[QSourceHighliter::Token::Sections].setBackground(QColor(240, 240, 240));    // светлее основного

    _formats[QSourceHighliter::Token::ToolBar].setForeground(QColor(245, 243, 243));
    _formats[QSourceHighliter::Token::ToolBar].setBackground(QColor(250, 250, 250));     // #fafafa

    _formats[QSourceHighliter::Token::ToolBarButtons].setForeground(QColor(245, 243, 243));
    _formats[QSourceHighliter::Token::ToolBarButtons].setBackground(QColor(250, 250, 250));

    _formats[QSourceHighliter::Token::CentralLayout].setForeground(QColor(231, 228, 228));
    _formats[QSourceHighliter::Token::CentralLayout].setBackground(QColor(255, 255, 255));

    _formats[QSourceHighliter::Token::SettingsLayout].setForeground(QColor(232, 231, 231));
    _formats[QSourceHighliter::Token::SettingsLayout].setBackground(QColor(245, 245, 245)); // #f5f5f5

    _formats[QSourceHighliter::Token::SettingsLayoutBorder].setForeground(QColor(232, 231, 231));
    _formats[QSourceHighliter::Token::SettingsLayoutBorder].setBackground(QColor(245, 245, 245));

    _formats[QSourceHighliter::Token::StringsText].setForeground(QColor(255, 255, 255));

    _formats[QSourceHighliter::Token::ButtonsText].setForeground(QColor(255, 255, 255));

    _formats[QSourceHighliter::Token::CodeBlock].setForeground(QColor(0,0,0));    // #ffffff
    _formats[QSourceHighliter::Token::CodeBlock].setBackground(QColor(255, 255, 255));    // фон редактора

    _formats[QSourceHighliter::Token::CodeKeyWord].setForeground(QColor(0,51,179));       // #0033b3
    _formats[QSourceHighliter::Token::CodeString].setForeground(QColor(6,125,23));        // #067d17
    _formats[QSourceHighliter::Token::CodeComment].setForeground(QColor(140,140,140));    // #8c8c8c
    _formats[QSourceHighliter::Token::CodeType].setForeground(QColor(102, 217, 239));
    _formats[QSourceHighliter::Token::CodeOther].setForeground(QColor(0, 0, 0));          // #000000
    _formats[QSourceHighliter::Token::CodeNumLiteral].setForeground(QColor(0,98,122));    // #1750eb
    _formats[QSourceHighliter::Token::CodeBuiltIn].setForeground(QColor(0,98,122));       // #00627a

    return _formats;
}

QHash<QSourceHighliter::Token, QTextCharFormat>
QSourceHighliterTheme::theme(QSourceHighliter::Themes theme) {
    switch (theme) {
        case QSourceHighliter::Themes::Monokai:
            return monokai();
        case QSourceHighliter::Themes::LightTheme:
            return lIghtTheme();
        case QSourceHighliter::Themes::DarkTheme:
            return darkTheme();
        default:
            return {monokai()};//monokai is default value
    }
}

} // namespace QSourceHighlite
