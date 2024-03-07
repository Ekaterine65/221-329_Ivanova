#include "credentialwidget.h"
#include "ui_credentialwidget.h"

credentialwidget::credentialwidget(const QString &site, const QString &login, const QString &password, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::credentialwidget)
{
    ui->setupUi(this);
    QPixmap pix("D:/WorkDir/crypto/images/padlock1.bmp");
    ui->icon->setPixmap(pix);
    ui->label->setText(site);
    ui->loginText->setText(login);
    ui->passwordText->setText(password);
}

credentialwidget::~credentialwidget()
{
    delete ui;
}
