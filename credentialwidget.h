#ifndef CREDENTIALWIDGET_H
#define CREDENTIALWIDGET_H

#include <QWidget>

namespace Ui {
class credentialwidget;
}

class credentialwidget : public QWidget
{
    Q_OBJECT

public:
    explicit credentialwidget(const QString &site, const QString &login, const QString &password, QWidget *parent = nullptr);
    ~credentialwidget();

private:
    Ui::credentialwidget *ui;
};

#endif // CREDENTIALWIDGET_H
