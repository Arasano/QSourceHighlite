#ifndef THEMEDIALOG_H
#define THEMEDIALOG_H

#include <QDialog>
#include "themedata.h"

class QLineEdit;
class QPushButton;

// Dialog for creating a user-defined theme:
// name + main colors (editor background/text, keyword, string, comment)
class ThemeCreateDialog : public QDialog
{
    Q_OBJECT
  public:
    explicit ThemeCreateDialog(QWidget *parent = nullptr);
    ThemeData themeData() const;

  private:
    void pickColor(QColor &target, QPushButton *btn);
    void refreshButton(QPushButton *btn, const QColor &c);
    void acceptValidated();

    QLineEdit   *m_nameEdit;
    QPushButton *m_bgButton, *m_textButton, *m_keywordButton, *m_stringButton, *m_commentButton;

    QColor m_bg      = QColor("#ffffff");
    QColor m_text    = QColor("#000000");
    QColor m_keyword = QColor("#0033b3");
    QColor m_string  = QColor("#067d17");
    QColor m_comment = QColor("#8c8c8c");
};
#endif