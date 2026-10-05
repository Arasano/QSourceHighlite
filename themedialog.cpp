#include "themedialog.h"

#include <QColorDialog>
#include <QDialogButtonBox>
#include <QFormLayout>
#include <QLineEdit>
#include <QMessageBox>
#include <QPushButton>

ThemeCreateDialog::ThemeCreateDialog(QWidget *parent) : QDialog(parent)
{
    setWindowTitle(tr("Create custom theme"));
    auto *form = new QFormLayout(this);

    m_nameEdit = new QLineEdit(this);
    form->addRow(tr("Theme name:"), m_nameEdit);

    m_bgButton      = new QPushButton(this);
    m_textButton    = new QPushButton(this);
    m_keywordButton = new QPushButton(this);
    m_stringButton  = new QPushButton(this);
    m_commentButton = new QPushButton(this);

    refreshButton(m_bgButton, m_bg);
    refreshButton(m_textButton, m_text);
    refreshButton(m_keywordButton, m_keyword);
    refreshButton(m_stringButton, m_string);
    refreshButton(m_commentButton, m_comment);

    connect(m_bgButton,      &QPushButton::clicked, this, [this] { pickColor(m_bg, m_bgButton); });
    connect(m_textButton,    &QPushButton::clicked, this, [this] { pickColor(m_text, m_textButton); });
    connect(m_keywordButton, &QPushButton::clicked, this, [this] { pickColor(m_keyword, m_keywordButton); });
    connect(m_stringButton,  &QPushButton::clicked, this, [this] { pickColor(m_string, m_stringButton); });
    connect(m_commentButton, &QPushButton::clicked, this, [this] { pickColor(m_comment, m_commentButton);});


    form->addRow(tr("Editor background:"), m_bgButton);
    form->addRow(tr("Editor text:"),       m_textButton);
    form->addRow(tr("Keywords:"),          m_keywordButton);
    form->addRow(tr("Strings:"),           m_stringButton);
    form->addRow(tr("Comments:"),          m_commentButton);

    auto *buttons = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, this);
    connect(buttons, &QDialogButtonBox::accepted, this, &ThemeCreateDialog::acceptValidated);
    connect(buttons, &QDialogButtonBox::rejected, this, &QDialog::reject);
    form->addRow(buttons);
}

void ThemeCreateDialog::pickColor(QColor &target, QPushButton *btn)
{
    const QColor c = QColorDialog::getColor(target, this);
    if (c.isValid()) {
        target = c;
        refreshButton(btn, c);
    }
}

void ThemeCreateDialog::refreshButton(QPushButton *btn, const QColor &c)
{
    btn->setStyleSheet(QString("background-color: %1; min-height: 18px;").arg(c.name()));
}

void ThemeCreateDialog::acceptValidated()
{
    if (m_nameEdit->text().trimmed().isEmpty()) {
        QMessageBox::warning(this, tr("Custom theme"), tr("Theme name must not be empty"));
        return;
    }
    accept();
}

ThemeData ThemeCreateDialog::themeData() const
{
    ThemeData d;
    d.setName(m_nameEdit->text().trimmed());

    // editor
    d.setCodeBlockBackground(m_bg);
    d.setCodeBlockForeground(m_text);
    // syntax
    d.setCodeKeyWordForeground(m_keyword);
    d.setCodeStringForeground(m_string);
    d.setCodeCommentForeground(m_comment);
    // UI surfaces reuse the editor colors so the whole window follows the theme
    d.setMenuBarBackground(m_bg);            d.setMenuBarForeground(m_text);
    d.setToolBarBackground(m_bg);            d.setToolBarForeground(m_text);
    d.setToolBarButtonsBackground(m_bg);     d.setToolBarButtonsForeground(m_text);
    d.setCentralLayoutBackground(m_bg);      d.setCentralLayoutForeground(m_text);
    d.setSettingsBackground(m_bg);           d.setSettingsForeground(m_text);
    d.setStringsTextForeground(m_text);
    d.setButtonsTextForeground(m_text);
    d.setHighlightColor(QColor("#0078d7"));
    return d;
}