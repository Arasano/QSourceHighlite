#ifndef THEMEDATA_H
#define THEMEDATA_H

#include <QColor>
#include <QHash>
#include <QPalette>
#include <QString>
#include <QTextCharFormat>

#include "qjsonobject.h"
#include "qsourcehighliter.h"


// Short alias for the token enum used by the highlighter
using Token = QSourceHighlite::QSourceHighliter::Token;
// Plain data class that stores all colors of a single theme.
// Fill it via setters and convert to Qt types with toPalette()/toTokenFormats().
class ThemeData
{
  public:
    ThemeData() = default;
    ~ThemeData() = default;

           // Theme name (e.g. "Monokai", "Dark", "Light")
    const QString &name() const { return Name; }
    void setName(const QString &name) { Name = name; }

    // themedata.h: replace toPalette() declaration with
    QPalette toPalette(const QPalette &base) const{
            QPalette pal = base;                       // standard palette as a fallback
            auto setIfValid = [&pal](QPalette::ColorRole role, const QColor &c) {
                if (c.isValid()) pal.setColor(role, c);   // skip unset colors
            };
            setIfValid(QPalette::Window,     CentralLayoutBackground);
            setIfValid(QPalette::WindowText, StringsTextForeground);
            setIfValid(QPalette::Base,       CodeBlockBackground);
            setIfValid(QPalette::Text,       CodeBlockForeground);
            setIfValid(QPalette::Button,     ToolBarButtonsBackground);
            setIfValid(QPalette::ButtonText, ButtonsTextForeground);
            setIfValid(QPalette::Highlight,  HighlightColor);
            if (HighlightColor.isValid()) pal.setColor(QPalette::HighlightedText, Qt::white);
            return pal;
        }

    // themedata.h / .cpp
    bool isValid() const
    {
        return !Name.isEmpty()
        && CodeBlockBackground.isValid()      // editor background
            && CodeBlockForeground.isValid()      // editor text
            && StringsTextForeground.isValid()    // UI labels
            && ButtonsTextForeground.isValid();   // buttons / combos text
    }

    // simple contrast guard: background and text must actually differ
    static bool hasContrast(const QColor &bg, const QColor &fg)
    {
        return bg.isValid() && fg.isValid()
        && qAbs(bg.lightness() - fg.lightness()) > 40;   // 0..255 scale
    }

    bool isConsistent() const
    {
        return isValid()
        && hasContrast(CodeBlockBackground, CodeBlockForeground)
            && hasContrast(CentralLayoutBackground, StringsTextForeground);
    }

    // add serialization for custom themes
    QJsonObject toJson() const
    {
        QJsonObject o;
        o["name"]                   = Name;
        o["codeBlockBackground"]    = CodeBlockBackground.name();
        o["codeBlockForeground"]    = CodeBlockForeground.name();
        o["codeKeyWordForeground"]  = CodeKeyWordForeground.name();
        o["codeStringForeground"]   = CodeStringForeground.name();
        o["codeCommentForeground"]  = CodeCommentForeground.name();
        o["centralLayoutBackground"]= CentralLayoutBackground.name();
        o["stringsTextForeground"]  = StringsTextForeground.name();
        o["buttonsTextForeground"]  = ButtonsTextForeground.name();
        o["highlightColor"]         = HighlightColor.name();
        return o;
    }

    static bool fromJson(const QJsonObject &o, ThemeData &out)
    {
        // helper: read a color, 'required' fields must exist and be valid
        auto readColor = [](const QJsonObject &obj, const char *key, QColor &dst, bool required) {
            const QJsonValue v = obj.value(QLatin1String(key));
            if (!v.isString()) return !required;
            QColor c(v.toString());
            if (!c.isValid())  return !required;
            dst = c;
            return true;
        };

        const QJsonValue nameVal = o.value(QLatin1String("name"));
        if (!nameVal.isString() || nameVal.toString().isEmpty())
            return false;                                   // unnamed theme is invalid
        out.setName(nameVal.toString());

               // required: editor background and text
        if (!readColor(o, "codeBlockBackground", out.CodeBlockBackground, true)) return false;
        if (!readColor(o, "codeBlockForeground", out.CodeBlockForeground, true)) return false;

               // optional: syntax colors, skipped silently if broken
        readColor(o, "codeKeyWordForeground",  out.CodeKeyWordForeground,  false);
        readColor(o, "codeStringForeground",   out.CodeStringForeground,   false);
        readColor(o, "codeCommentForeground",  out.CodeCommentForeground,  false);
        readColor(o, "centralLayoutBackground",out.CentralLayoutBackground,false);
        readColor(o, "stringsTextForeground",  out.StringsTextForeground,  false);
        readColor(o, "buttonsTextForeground",  out.ButtonsTextForeground,  false);
        readColor(o, "highlightColor",         out.HighlightColor,         false);
        return true;
    }

    // Build highlighter formats from the stored colors
    QHash<QSourceHighlite::QSourceHighliter::Token, QTextCharFormat> toTokenFormats() const
    {
        QHash<Token, QTextCharFormat> formats;

        // Helper: create a format from foreground and optional background
        auto put = [&formats](Token t, const QColor &fg, const QColor &bg = QColor()) {
            QTextCharFormat f;
            if (fg.isValid()) f.setForeground(fg);   // skip invalid colors
            if (bg.isValid()) f.setBackground(bg);
            formats.insert(t, f);
        };

               // UI tokens: text + surface background
        put(Token::MenuBar,              MenuBarForeground,              MenuBarBackground);
        put(Token::MenuBarButtons,       MenuBarForeground,              MenuBarBackground);
        put(Token::Sections,             SectionsForeground,             SectionsBackground);
        put(Token::ToolBar,              ToolBarForeground,              ToolBarBackground);
        put(Token::ToolBarButtons,       ToolBarButtonsForeground,       ToolBarButtonsBackground);
        put(Token::CentralLayout,        CentralLayoutForeground,        CentralLayoutBackground);
        put(Token::SettingsLayout,       SettingsForeground,             SettingsBackground);
        put(Token::SettingsLayoutBorder, SettingsLayoutBorderForeground, SettingsLayoutBorderBackground);

               // Text-only roles, no background
        put(Token::StringsText, StringsTextForeground);
        put(Token::ButtonsText, ButtonsTextForeground);

               // Code editor: block background + plain text color
        put(Token::CodeBlock, CodeBlockForeground, CodeBlockBackground);

               // Syntax tokens: foreground only
        put(Token::CodeKeyWord,    CodeKeyWordForeground);
        put(Token::CodeString,     CodeStringForeground);
        put(Token::CodeComment,    CodeCommentForeground);
        put(Token::CodeType,       CodeTypeForeground);
        put(Token::CodeOther,      CodeOtherForeground);
        put(Token::CodeNumLiteral, CodeNumLiteralForeground);
        put(Token::CodeBuiltIn,    CodeBuiltInForeground);

        return formats;
    }

           // --- UI colors: getters / setters ---
    const QColor &menuBarForeground() const { return MenuBarForeground; }
    void setMenuBarForeground(const QColor &c) { MenuBarForeground = c; }

    const QColor &menuBarBackground() const { return MenuBarBackground; }
    void setMenuBarBackground(const QColor &c) { MenuBarBackground = c; }

    const QColor &sectionsForeground() const { return SectionsForeground; }
    void setSectionsForeground(const QColor &c) { SectionsForeground = c; }

    const QColor &sectionsBackground() const { return SectionsBackground; }
    void setSectionsBackground(const QColor &c) { SectionsBackground = c; }

    const QColor &toolBarForeground() const { return ToolBarForeground; }
    void setToolBarForeground(const QColor &c) { ToolBarForeground = c; }

    const QColor &toolBarBackground() const { return ToolBarBackground; }
    void setToolBarBackground(const QColor &c) { ToolBarBackground = c; }

    const QColor &toolBarButtonsForeground() const { return ToolBarButtonsForeground; }
    void setToolBarButtonsForeground(const QColor &c) { ToolBarButtonsForeground = c; }

    const QColor &toolBarButtonsBackground() const { return ToolBarButtonsBackground; }
    void setToolBarButtonsBackground(const QColor &c) { ToolBarButtonsBackground = c; }

    const QColor &centralLayoutForeground() const { return CentralLayoutForeground; }
    void setCentralLayoutForeground(const QColor &c) { CentralLayoutForeground = c; }

    const QColor &centralLayoutBackground() const { return CentralLayoutBackground; }
    void setCentralLayoutBackground(const QColor &c) { CentralLayoutBackground = c; }

    const QColor &settingsForeground() const { return SettingsForeground; }
    void setSettingsForeground(const QColor &c) { SettingsForeground = c; }

    const QColor &settingsBackground() const { return SettingsBackground; }
    void setSettingsBackground(const QColor &c) { SettingsBackground = c; }

    const QColor &settingsLayoutBorderForeground() const { return SettingsLayoutBorderForeground; }
    void setSettingsLayoutBorderForeground(const QColor &c) { SettingsLayoutBorderForeground = c; }

    const QColor &settingsLayoutBorderBackground() const { return SettingsLayoutBorderBackground; }
    void setSettingsLayoutBorderBackground(const QColor &c) { SettingsLayoutBorderBackground = c; }

           // Text-only colors (no background)
    const QColor &stringsTextForeground() const { return StringsTextForeground; }
    void setStringsTextForeground(const QColor &c) { StringsTextForeground = c; }

    const QColor &buttonsTextForeground() const { return ButtonsTextForeground; }
    void setButtonsTextForeground(const QColor &c) { ButtonsTextForeground = c; }

           // --- Code colors: getters / setters ---
    const QColor &codeBlockForeground() const { return CodeBlockForeground; }
    void setCodeBlockForeground(const QColor &c) { CodeBlockForeground = c; }

    const QColor &codeBlockBackground() const { return CodeBlockBackground; }
    void setCodeBlockBackground(const QColor &c) { CodeBlockBackground = c; }

    const QColor &codeKeyWordForeground() const { return CodeKeyWordForeground; }
    void setCodeKeyWordForeground(const QColor &c) { CodeKeyWordForeground = c; }

    const QColor &codeStringForeground() const { return CodeStringForeground; }
    void setCodeStringForeground(const QColor &c) { CodeStringForeground = c; }

    const QColor &codeCommentForeground() const { return CodeCommentForeground; }
    void setCodeCommentForeground(const QColor &c) { CodeCommentForeground = c; }

    const QColor &codeTypeForeground() const { return CodeTypeForeground; }
    void setCodeTypeForeground(const QColor &c) { CodeTypeForeground = c; }

    const QColor &codeOtherForeground() const { return CodeOtherForeground; }
    void setCodeOtherForeground(const QColor &c) { CodeOtherForeground = c; }

    const QColor &codeNumLiteralForeground() const { return CodeNumLiteralForeground; }
    void setCodeNumLiteralForeground(const QColor &c) { CodeNumLiteralForeground = c; }

    const QColor &codeBuiltInForeground() const { return CodeBuiltInForeground; }
    void setCodeBuiltInForeground(const QColor &c) { CodeBuiltInForeground = c; }

           // Selection color, used for QPalette::Highlight
    const QColor &highlightColor() const { return HighlightColor; }
    void setHighlightColor(const QColor &c) { HighlightColor = c; }

  private:

    QString Name;

    QColor MenuBarForeground;
    QColor MenuBarBackground;

    QColor SectionsForeground;
    QColor SectionsBackground;

    QColor ToolBarForeground;
    QColor ToolBarBackground;

    QColor ToolBarButtonsForeground;
    QColor ToolBarButtonsBackground;

    QColor CentralLayoutForeground;
    QColor CentralLayoutBackground;

    QColor SettingsForeground;
    QColor SettingsBackground;

    QColor SettingsLayoutBorderForeground;
    QColor SettingsLayoutBorderBackground;

    QColor StringsTextForeground;
    QColor ButtonsTextForeground;

    QColor CodeBlockForeground;
    QColor CodeBlockBackground;

    QColor CodeKeyWordForeground;
    QColor CodeStringForeground;
    QColor CodeCommentForeground;
    QColor CodeTypeForeground;
    QColor CodeOtherForeground;
    QColor CodeNumLiteralForeground;
    QColor CodeBuiltInForeground;

    QColor HighlightColor;
};

#endif // THEMEDATA_H