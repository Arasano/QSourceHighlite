#ifndef SCRIPTRUNNER_H
#define SCRIPTRUNNER_H

#include "qobject.h"

#include <QCoreApplication>
#include <QDebug>
#include <QProcess>
#include <QTimer>

class ScriptRunner : public QObject {
    Q_OBJECT
  public:
    explicit ScriptRunner(QObject* parent = nullptr);

    int runProgram(const QString &filePath, const QString &code);
    int stopProgram();

  private:
    QProcess* mainProcess = nullptr;
    void initConnectors();

    QTimer* m_flushTimer = nullptr;
    QString m_outputBuffer;
    QString m_errorBuffer;

    static constexpr int MAX_BUFFER_SIZE = 64 * 1024;
    QString pathComp;
    bool hasPython = false;

    int writeInFile(QStringView filePath, QStringView info);
    QString findPythonInterpreter() const;
    bool isPythonAvailable(QString &path);
    void flushBuffers();

  signals:
    void finished(int code, QProcess::ExitStatus st);
    void errorOccurred(QProcess::ProcessError e);
    void standardError(const QString &error);
    void standardOutput(const QString &output);
};

#endif // SCRIPTRUNNER_H
