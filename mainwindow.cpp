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
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
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
#include "ui_mainwindow.h"
#include "qsourcehighliter.h"

#include <QDebug>
#include <QDir>

using namespace QSourceHighlite;

QHash<QString, QSourceHighliter::Language> MainWindow::_langStringToEnum;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    initLangsEnum();
    initLangsComboBox();
    initThemesComboBox();

    //Set default font
    QFont f = QFontDatabase::systemFont(QFontDatabase::FixedFont);
    ui->plainTextEdit->setFont(f);

    //Init highlighter
    highlighter = new QSourceHighliter(ui->plainTextEdit->document());

    initMainButtons();
    initMenuButtons();

    setDefaultInfo();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::setDefaultInfo()
{
    ui->langComboBox->setCurrentText("Asm");
    languageChanged("Asm");
}

void MainWindow::initMainButtons()
{
    //Lang change button
    connect(ui->langComboBox,
            static_cast<void (QComboBox::*) (const QString&)>(&QComboBox::currentTextChanged),
            this, &MainWindow::languageChanged);
    //Theme change button
    connect(ui->themeComboBox,
            static_cast<void (QComboBox::*) (int)>(&QComboBox::currentIndexChanged),
            this, &MainWindow::themeChanged);
    //Print debug info
    //connect(ui->plainTextEdit, &QPlainTextEdit::textChanged, this, &MainWindow::printDebug);
}

void MainWindow::initMenuButtons()
{
    //File section
    //Save file
    //as TXT
    connect(ui->actionSaveAsTXT, &QAction::triggered, this, &MainWindow::onSaveAsTXT);
    //as JSON
    connect(ui->actionSaveAsJSON, &QAction::triggered, this, &MainWindow::onSaveAsJSON);
    //as selected language
    connect(ui->actionSaveAsLanguage, &QAction::triggered, this, &MainWindow::onSaveAsLanguage);

    //Open file
    connect(ui->actionOpenFile, &QAction::triggered, this, &MainWindow::onOpenFile);

    //Exit
    connect(ui->actionExit, &QAction::triggered, this, &MainWindow::onExit);

    //Edit section
    //Search
    connect(ui->actionSearch, &QAction::triggered, this, &MainWindow::onSearch);
}

//Add info output
void MainWindow::onSaveAsTXT(){
    ui->statusBar->showMessage("onSaveAsTXT", 3000);
}
void MainWindow::onSaveAsJSON(){
    ui->statusBar->showMessage("onSaveAsJSON", 3000);
}
void MainWindow::onSaveAsLanguage(){
    ui->statusBar->showMessage("onSaveAsLanguage", 3000);
}
void MainWindow::onOpenFile(){
    ui->statusBar->showMessage("onOpenFile", 3000);
}
void MainWindow::onExit(){
    ui->statusBar->showMessage("onExit", 3000);
}
void MainWindow::onSearch(){
    ui->statusBar->showMessage("onSearch", 3000);
}

void MainWindow::initLangsEnum()
{
    MainWindow::_langStringToEnum = QHash<QString, QSourceHighliter::Language> {
        { QLatin1String("Asm"), QSourceHighliter::CodeAsm },
        { QLatin1String("Bash"), QSourceHighliter::CodeBash },
        { QLatin1String("C"), QSourceHighliter::CodeC },
        { QLatin1String("C++"), QSourceHighliter::CodeCpp },
        { QLatin1String("CMake"), QSourceHighliter::CodeCMake },
        { QLatin1String("CSharp"), QSourceHighliter::CodeCSharp },
        { QLatin1String("Css"), QSourceHighliter::CodeCSS },
        { QLatin1String("Go"), QSourceHighliter::CodeGo },
        { QLatin1String("Html"), QSourceHighliter::CodeXML },
        { QLatin1String("Ini"), QSourceHighliter::CodeINI },
        { QLatin1String("Java"), QSourceHighliter::CodeJava },
        { QLatin1String("Javascript"), QSourceHighliter::CodeJava },
        { QLatin1String("Json"), QSourceHighliter::CodeJSON },
        { QLatin1String("Lua"), QSourceHighliter::CodeLua },
        { QLatin1String("Make"), QSourceHighliter::CodeMake },
        { QLatin1String("Php"), QSourceHighliter::CodePHP },
        { QLatin1String("Python"), QSourceHighliter::CodePython },
        { QLatin1String("Qml"), QSourceHighliter::CodeQML },
        { QLatin1String("Rhai"), QSourceHighliter::CodeRhai },
        { QLatin1String("Rust"), QSourceHighliter::CodeRust },
        { QLatin1String("Sql"), QSourceHighliter::CodeSQL },
        { QLatin1String("Typescript"), QSourceHighliter::CodeTypeScript },
        { QLatin1String("V"), QSourceHighliter::CodeV },
        { QLatin1String("Vex"), QSourceHighliter::CodeVex },
        { QLatin1String("Xml"), QSourceHighliter::CodeXML },
        { QLatin1String("Yaml"), QSourceHighliter::CodeYAML }
    };
}

void MainWindow::initThemesComboBox()
{
    ui->themeComboBox->addItem("Monokai", QSourceHighliter::Themes::Monokai);
    ui->themeComboBox->addItem("debug", QSourceHighliter::Themes::Monokai);
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
    //Change theme for specific lang
    QSourceHighliter::Themes theme = (QSourceHighliter::Themes)ui->themeComboBox->currentData().toInt();
    //Update theme
    highlighter->setTheme(theme);
}

void MainWindow::languageChanged(const QString &lang) {
    //Change lang
    highlighter->setCurrentLanguage(_langStringToEnum.value(lang));
    //Update highlight in ui
    highlighter->rehighlight();

    //Open default example of new lang
    // QFile f(QDir::currentPath() + "/../test_files/" + lang + ".txt");
    // if (f.open(QIODevice::ReadOnly | QIODevice::Text)) {
    //     const auto text = f.readAll();
    //     ui->plainTextEdit->setPlainText(QString::fromUtf8(text));
    // }
    // f.close();
}
