#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonValue>
#include <QJsonArray>
#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    bool readJSON();

private:
    Ui::MainWindow *ui;
    QJsonArray m_jsonarray; //структура данных, содержащая учетные данные
public slots:
    int decryptFile(const QByteArray &encryptedBytes, QByteArray &decryptedBytes);
private slots:
    void filterList(const QString &text);
};
#endif // MAINWINDOW_H
