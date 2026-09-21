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

#include "mainwindow.h"

#include "qsourcehighliter.h"
#include "ui_mainwindow.h"

#include <QDebug>
#include <QDir>
#include <QFileDialog>
#include <QFileInfo>
#include <QJsonObject>
#include <QMessageBox>
#include <QPushButton>

using namespace QSourceHighlite;

QHash<QString, QSourceHighliter::Language> MainWindow::_langStringToEnum;

MainWindow::MainWindow(QWidget* parent) : QMainWindow(parent), ui(new Ui::MainWindow) {
    ui->setupUi(this);

    initLangsEnum();

    initLangsComboBox();
    initThemesComboBox();

    // Init highlighter
    highlighter = new QSourceHighliter(ui->plainTextEdit->document());

    initMainButtons();
    initMenuButtons();

    initEventConnectors();

    setDefaultFont();
    setDefaultInfo();
}

MainWindow::~MainWindow() {
    delete ui;
}

void MainWindow::setDefaultFont() {
    QFont f = QFontDatabase::systemFont(QFontDatabase::FixedFont);
    ui->plainTextEdit->setFont(f);
}

void MainWindow::setDefaultInfo() {
    ui->langComboBox->setCurrentText("Asm");
    languageChanged("Asm");
}

void MainWindow::initMainButtons() {
    // Lang change button
    connect(ui->langComboBox,
            static_cast<void (QComboBox::*)(const QString&)>(&QComboBox::currentTextChanged), this,
            &MainWindow::languageChanged);
    // Theme change button
    connect(ui->themeComboBox,
            static_cast<void (QComboBox::*)(int)>(&QComboBox::currentIndexChanged), this,
            &MainWindow::themeChanged);
    // Print debug info
    // connect(ui->plainTextEdit, &QPlainTextEdit::textChanged, this,
    // &MainWindow::printDebug);
}

void MainWindow::initMenuButtons() {
    // File section
    // Save file
    // as TXT
    connect(ui->actionSaveAsTXT, &QAction::triggered, this, &MainWindow::onSaveAsTXT);
    // as JSON
    connect(ui->actionSaveAsJSON, &QAction::triggered, this, &MainWindow::onSaveAsJSON);

    // Open file
    connect(ui->actionOpenFile, &QAction::triggered, this, &MainWindow::onOpenFile);

    // Exit
    connect(ui->actionExit, &QAction::triggered, this, &MainWindow::onExit);

    // Edit section
    // Search
    connect(ui->actionSearch, &QAction::triggered, this, &MainWindow::onSearch);

    connect(ui->plainTextEdit, &QPlainTextEdit::textChanged, this, [this]() { isTextChanged = 1; });
}

void MainWindow::initEventConnectors() {
    // If text changed
    connect(ui->plainTextEdit, &QPlainTextEdit::textChanged, this, [this]() { isTextChanged = 1; });
}

void MainWindow::closeEvent(QCloseEvent* event) {
    // If exit button pressed
    if (showSaveMessage()) {
        event->accept();
    } else {
        event->ignore();
    }
    ui->statusBar->showMessage("onExit", 3000);
}

bool MainWindow::onSaveAsTXT() {
    QString fileName = QFileDialog::getSaveFileName(this, "Save TXT file", "", "TXT file (*.txt)");

    if (fileName.isEmpty()) {
        ui->statusBar->showMessage("Error: file name is empty", 3000);
        return false;
    };

    QFile file(fileName);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        ui->statusBar->showMessage("Error: " + file.errorString(), 3000);
        return false;
    }

    file.write(ui->plainTextEdit->toPlainText().toUtf8());
    file.close();

    ui->statusBar->showMessage("Save file " + fileName + " successfully", 3000);
    return true;
}

bool MainWindow::onSaveAsJSON() {
    QString fileName =
        QFileDialog::getSaveFileName(this, "Save JSON file", "", "JSON file (*.json)");

    if (fileName.isEmpty()) {
        ui->statusBar->showMessage("Error: file name is empty", 3000);
        return false;
    };

    QFile file(fileName);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        ui->statusBar->showMessage("Error: " + file.errorString(), 3000);
        return false;
    }

    QJsonObject root;
    root["language"] = ui->langComboBox->currentText();
    root["text"] = ui->plainTextEdit->toPlainText();

    QJsonDocument doc(root);
    file.write(doc.toJson());
    file.close();

    ui->statusBar->showMessage("Save file " + fileName + " successfully", 3000);
    return true;
}

void MainWindow::onOpenFile() {
    // Take URL to get suffix
    QUrl url = QFileDialog::getOpenFileUrl(this, "Open file", QUrl(), "Code files (*.json *.txt)");
    if (url.isEmpty()) {
        ui->statusBar->showMessage("Error: file name is empty", 3000);
        return;
    };

    // Take QFileInfo from QUrl to get suffix cuz QUrl doesn't have it
    QFileInfo fileInfo(url.toLocalFile());

    if (!fileInfo.isFile()) {
        ui->statusBar->showMessage("Error: selected item is not file", 3000);
        return;
    };

    // Take Suffix
    QString fileSuffix = fileInfo.suffix();

    if (fileSuffix.isEmpty()) {
        ui->statusBar->showMessage("Error: file doesn't have suffix", 3000);
        return;
    };

    // App supports just 2 extensions
    int error;
    if (fileSuffix == "json") {
        error = loadDataFromJSONFile(fileInfo.filePath());
    } else if (fileSuffix == "txt") {
        error = loadDataFromTXTFile(fileInfo.filePath());
    } else {
        ui->statusBar->showMessage("Error: File doesn't have supported suffix", 3000);
        return;
    }

    if (error == 1) {
        ui->statusBar->showMessage("Error: unsupported error", 3000);
        return;
    }

    isTextChanged = 0;

    ui->statusBar->showMessage("Open file " + fileInfo.filePath() + " successfully", 3000);
}

int MainWindow::loadDataFromTXTFile(const QString& path) {
    QFile file(path);

    if (!file.exists()) {
        ui->statusBar->showMessage("Error: " + file.errorString(), 3000);
        return 1;
    }

    if (!file.open(QIODevice::ReadOnly)) {
        ui->statusBar->showMessage("Error: " + file.errorString(), 3000);
        return 1;
    }

    // Read the text by one line and put in text variable not to
    //  plainTextEdit to have opportunity cancel operation
    QString text;
    text = QTextStream(&file).readAll();
    file.close();

    // Set text in the plain
    ui->plainTextEdit->setPlainText(text);
    return 0;
}

int MainWindow::loadDataFromJSONFile(const QString& path) {
    QFile file(path);

    if (!file.exists()) {
        ui->statusBar->showMessage("Error: file doesn't exist", 3000);
        return 1;
    }

    if (!file.open(QIODevice::ReadWrite | QIODevice::Text)) {
        ui->statusBar->showMessage("Error: " + file.errorString(), 3000);
        return 1;
    }

    // QJsonDocument is preffered tool to work with JSON
    QJsonParseError error;
    QJsonDocument doc = QJsonDocument::fromJson(file.readAll(), &error);
    file.close();

    if (error.error != QJsonParseError::NoError) {
        ui->statusBar->showMessage("Error: " + error.errorString(), 3000);
        return 1;
    }

    // Take data from QJsonDocument(JSON)
    QJsonObject obj = doc.object();
    QString lang = obj["language"].toString();
    QString text = obj["text"].toString();

    // Set text in the plain
    ui->plainTextEdit->setPlainText(text);

    if (lang.isEmpty()) {
        ui->statusBar->showMessage("Error: can't define language", 3000);
        return 1;
    }

    if (!_langStringToEnum.contains(lang)) {
        ui->statusBar->showMessage("Error: file has not supported suffix", 3000);
        ui->langComboBox->setCurrentText(_langStringToEnum.key(QSourceHighliter::CodeAsm));
        return 1;
    }

    ui->langComboBox->setCurrentText(lang);
    return 0;
}

void MainWindow::onExit() {
    if (showSaveMessage()) {
        QApplication::quit();
    }
    ui->statusBar->showMessage("onExit", 3000);
}

bool MainWindow::showSaveMessage() {
    if (isTextChanged) {
        QMessageBox msgBox(this);
        msgBox.setWindowTitle("Unsaved Changes");
        msgBox.setText("The document has been modified.");
        msgBox.setInformativeText("Do you want to save your changes?");
        msgBox.setStandardButtons(QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel);
        msgBox.setDefaultButton(QMessageBox::Save);
        int ret = msgBox.exec();

        switch (ret) {
            case QMessageBox::Save:
                // Open save layout and exit
                if (onSaveAsJSON()) {
                    // Just after proper save close app
                    return true;
                }
                break;
            case QMessageBox::Discard:
                // Just exit
                return true;
                break;
            case QMessageBox::Cancel:
                // Close QMessageBox
                return false;
                break;
            default:
                // Should never be reached
                return false;
                break;
        }
    }
    // Exit app
    return true;
}

void MainWindow::onSearch() {
    ui->statusBar->showMessage("onSearch", 3000);
}

void MainWindow::initLangsEnum() {
    MainWindow::_langStringToEnum = QHash<QString, QSourceHighliter::Language>{
        {QLatin1String("Asm"), QSourceHighliter::CodeAsm},
        {QLatin1String("Bash"), QSourceHighliter::CodeBash},
        {QLatin1String("C"), QSourceHighliter::CodeC},
        {QLatin1String("C++"), QSourceHighliter::CodeCpp},
        {QLatin1String("CMake"), QSourceHighliter::CodeCMake},
        {QLatin1String("CSharp"), QSourceHighliter::CodeCSharp},
        {QLatin1String("Css"), QSourceHighliter::CodeCSS},
        {QLatin1String("Go"), QSourceHighliter::CodeGo},
        {QLatin1String("Html"), QSourceHighliter::CodeXML},
        {QLatin1String("Ini"), QSourceHighliter::CodeINI},
        {QLatin1String("Java"), QSourceHighliter::CodeJava},
        {QLatin1String("Javascript"), QSourceHighliter::CodeJava},
        {QLatin1String("Json"), QSourceHighliter::CodeJSON},
        {QLatin1String("Lua"), QSourceHighliter::CodeLua},
        {QLatin1String("Make"), QSourceHighliter::CodeMake},
        {QLatin1String("Php"), QSourceHighliter::CodePHP},
        {QLatin1String("Python"), QSourceHighliter::CodePython},
        {QLatin1String("Qml"), QSourceHighliter::CodeQML},
        {QLatin1String("Rhai"), QSourceHighliter::CodeRhai},
        {QLatin1String("Rust"), QSourceHighliter::CodeRust},
        {QLatin1String("Sql"), QSourceHighliter::CodeSQL},
        {QLatin1String("Typescript"), QSourceHighliter::CodeTypeScript},
        {QLatin1String("V"), QSourceHighliter::CodeV},
        {QLatin1String("Vex"), QSourceHighliter::CodeVex},
        {QLatin1String("Xml"), QSourceHighliter::CodeXML},
        {QLatin1String("Yaml"), QSourceHighliter::CodeYAML}};
}

void MainWindow::initThemesComboBox() {
    ui->themeComboBox->addItem("Monokai", QSourceHighliter::Themes::Monokai);
}

void MainWindow::initLangsComboBox() {
    ui->langComboBox->addItem("Asm");
    ui->langComboBox->addItem("Bash");
    ui->langComboBox->addItem("C");
    ui->langComboBox->addItem("C++");
    ui->langComboBox->addItem("CMake");
    ui->langComboBox->addItem("CSharp");
    ui->langComboBox->addItem("Css");
    ui->langComboBox->addItem("Go");
    ui->langComboBox->addItem("Html");
    ui->langComboBox->addItem("Ini");
    ui->langComboBox->addItem("Javascript");
    ui->langComboBox->addItem("Java");
    ui->langComboBox->addItem("Lua");
    ui->langComboBox->addItem("Make");
    ui->langComboBox->addItem("Php");
    ui->langComboBox->addItem("Python");
    ui->langComboBox->addItem("Qml");
    ui->langComboBox->addItem("Rust");
    ui->langComboBox->addItem("Sql");
    ui->langComboBox->addItem("Typescript");
    ui->langComboBox->addItem("V");
    ui->langComboBox->addItem("Vex");
    ui->langComboBox->addItem("Xml");
    ui->langComboBox->addItem("Yaml");
}

void MainWindow::themeChanged(int) {
    // Change theme for specific lang
    QSourceHighliter::Themes theme =
        (QSourceHighliter::Themes)ui->themeComboBox->currentData().toInt();
    // Update theme
    highlighter->setTheme(theme);
    highlighter->rehighlight();
}

void MainWindow::languageChanged(const QString& lang) {
    // Change lang
    highlighter->setCurrentLanguage(_langStringToEnum.value(lang));
    // Update highlight in ui
    highlighter->rehighlight();

    ui->statusBar->showMessage("Language is changed to " + lang, 3000);

    // Open default example of new lang
    //  QFile f(QDir::currentPath() + "/../test_files/" + lang + ".txt");
    //  if (f.open(QIODevice::ReadOnly | QIODevice::Text)) {
    //      const auto text = f.readAll();
    //      ui->plainTextEdit->setPlainText(QString::fromUtf8(text));
    //  }
    //  f.close();
}
