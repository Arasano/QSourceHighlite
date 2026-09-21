#ifndef SEARCHDIALOG_H
#define SEARCHDIALOG_H

#include <QDialog>

namespace Ui {
class SearchDialog;
}

class SearchDialog : public QDialog {
    Q_OBJECT

  public:
    explicit SearchDialog(QWidget* parent = nullptr);
    ~SearchDialog();

  private:
    Ui::SearchDialog* ui;
    void initMainButtons();

  signals:
    void findNext(const QString& text) const;
    void replace(const QString& find, const QString& replace) const;
    void replaceAll(const QString& find, const QString& replace) const;

  private slots:
    void onButtonFindNext() const;
    void onButtonReplace() const;
    void onButtonReplaceAll() const;
};

#endif // SEARCHDIALOG_H
