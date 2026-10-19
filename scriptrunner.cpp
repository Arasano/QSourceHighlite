#include "scriptrunner.h"
#include "qdir.h"

#include <QCoreApplication>
#include <QProcess>
#include <QThread>
#include <QStandardPaths>

ScriptRunner::ScriptRunner(QObject* parent) : QObject{parent}{
}

void ScriptRunner::initConnectors() {

    connect(m_flushTimer, &QTimer::timeout, this, &ScriptRunner::flushBuffers);

    connect(mainProcess, &QProcess::finished, this, [this](int code, QProcess::ExitStatus st){
        m_flushTimer->stop();
        flushBuffers();
        emit ScriptRunner::finished(code, st);
    });
    connect(mainProcess, &QProcess::errorOccurred, this, &ScriptRunner::errorOccurred);
    connect(mainProcess, &QProcess::readyReadStandardOutput, this, [this](){
        m_outputBuffer.append(QString::fromUtf8(mainProcess->readAllStandardOutput()));
        if (!m_flushTimer->isActive()) {
            m_flushTimer->start();
        }
    });
    connect(mainProcess, &QProcess::readyReadStandardError, this, [this](){
        m_errorBuffer.append(QString::fromUtf8(mainProcess->readAllStandardError()));
        if (!m_flushTimer->isActive()) {
            m_flushTimer->start();
        }
    });

}

int ScriptRunner::writeInFile(QStringView filePath, QStringView info) {

    QFile file;
    file.setFileName(filePath.toString());

    if(!file.open(QIODevice::WriteOnly|QIODevice::Text)) return 1;

    QTextStream stream(&file);
    stream << info << Qt::endl;

    file.close();
    return !file.exists();

}

int ScriptRunner::runProgram(const QString &filePath, const QString &code) {
    qDebug() << "thread1:" << QThread::currentThread() << "| main is:" << qApp->thread();

    if (!mainProcess) {
        m_flushTimer = new QTimer(this);
        m_flushTimer->setInterval(100);
        mainProcess = new QProcess(this);
        hasPython = isPythonAvailable(pathComp);
        initConnectors();
    }

    if(!hasPython) return 1;

    if(writeInFile(filePath, code)) return 1;

    if (mainProcess && mainProcess->state() != QProcess::NotRunning) return 1;

    if (!mainProcess) {
        mainProcess = new QProcess(this);
    }

    mainProcess->start(pathComp, QStringList() << filePath);

    return 0;
}

int ScriptRunner::stopProgram() {
    if (!mainProcess || mainProcess->state() == QProcess::NotRunning)
        return 1;

    mainProcess->terminate();

    if (!mainProcess->waitForFinished(1000))
        mainProcess->kill();

    return 0;
}

QString ScriptRunner::findPythonInterpreter() const
{
    const QStringList candidates = { QStringLiteral("python3"),
                                    QStringLiteral("python"),
                                    QStringLiteral("py") };
    for (const QString &name : candidates) {
        const QString path = QStandardPaths::findExecutable(name);
        if (!path.isEmpty())
            return path;
    }
    return QString();
}

bool ScriptRunner::isPythonAvailable(QString &path)
{
    const QString python = findPythonInterpreter();
    if (python.isEmpty())
        return false;

    QProcess probe;
    probe.start(python, { QStringLiteral("--version") });

    if (!probe.waitForFinished(3000)) {
        probe.kill();
        probe.waitForFinished(1000);
        return false;
    }
    if (probe.exitStatus() != QProcess::NormalExit || probe.exitCode() != 0)
        return false;

    path = python;
    return true;
}

void ScriptRunner::flushBuffers() {
    if (!m_outputBuffer.isEmpty()) {
        if (m_outputBuffer.size() > MAX_BUFFER_SIZE) {
            m_outputBuffer = m_outputBuffer.right(MAX_BUFFER_SIZE);
        }
        emit standardOutput(m_outputBuffer);
        m_outputBuffer.clear();
    }
    if (!m_errorBuffer.isEmpty()) {
        emit standardError(m_errorBuffer);
        m_errorBuffer.clear();
    }
}

