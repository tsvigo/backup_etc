// #include <QCoreApplication>

// int main(int argc, char *argv[])
// {
//     QCoreApplication a(argc, argv);

//     return a.exec();
// }
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#include <QCoreApplication>
#include <QFile>
#include <QTextStream>
#include <QDir>
#include <QDebug>
#include <cstdlib> // Для system()

void readNumberFromFile(const QString& filePath, int& number) {
    QFile file(filePath);
    if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QTextStream in(&file);
        in >> number;
        file.close();
    } else {
        qCritical() << "Не удалось открыть файл для чтения:" << filePath;
    }
}

void writeNumberToFile(const QString& filePath, int number) {
    QFile file(filePath);
    if (file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QTextStream out(&file);
        out << number;
        file.close();
    } else {
        qCritical() << "Не удалось открыть файл для записи:" << filePath;
    }
}

void copyDirectory(const QString& sourcePath, const QString& destinationPath) {
    QString command = QString("cp -r %1 %2").arg(sourcePath, destinationPath);
    int result = system(command.toUtf8().constData());
    if (result != 0) {
        qCritical() << "Не удалось скопировать директорию" << sourcePath << "в" << destinationPath;
    }
}

int main(int argc, char *argv[]) {
    QCoreApplication a(argc, argv);

    QString numberFilePath = "/home/viktor/my_projects_qt_2/backup_etc/tekushiy_etc.txt";
    int number;

    readNumberFromFile(numberFilePath, number);
    qDebug() << "Текущее число:" << number;

    QString newBackupFolderPath = QString("/mnt/6017d124-d970-486e-b68f-59b516dd0511/backups/etc%1").arg(number);
    QDir newBackupFolder(newBackupFolderPath);

    if (newBackupFolder.exists()) {
        qDebug() << "Папка уже существует:" << newBackupFolderPath;
    } else {
        if (newBackupFolder.mkpath(".")) {
            qDebug() << "Папка создана:" << newBackupFolderPath;
            copyDirectory("/etc/", newBackupFolderPath);
        } else {
            qCritical() << "Не удалось создать папку:" << newBackupFolderPath;
        }
    }

    number++;
    writeNumberToFile(numberFilePath, number);
    qDebug() << "Новое число записано в файл:" << number;

    return a.exec();
}
