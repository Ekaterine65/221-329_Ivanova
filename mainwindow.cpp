#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "credentialwidget.h"

#include <QFile>
#include <openssl/evp.h>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    connect(ui->lineEdit, &QLineEdit::textEdited, this, &MainWindow::filterList);

    readJSON();
    filterList("");

}

MainWindow::~MainWindow()
{
    delete ui;
}

/* функция считывает учётные записи из файла JSON в стурктуру данных QLjst */
bool MainWindow::readJSON()
{
    QFile jsonFile("credentials_en.json");
    jsonFile.open(QFile::ReadOnly);
    if(!jsonFile.isOpen())
        return false;

    QByteArray hexEncryptedBytes = jsonFile.readAll();
    QByteArray encryptedBytes = QByteArray::fromHex(hexEncryptedBytes);
    //qDebug() << encryptedBytes;
    QByteArray decryptedBytes;
    int ret_code = decryptFile(encryptedBytes, decryptedBytes);
    qDebug() << "ret_code = " << ret_code;

    //QJsonDocument jsonDoc = QJsonDocument::fromJson(decryptedBytes);
    //QJsonObject rootObject = jsonDoc.object();
    /*
    for(auto itm : rootObject["credit"].toArray()) {
           qDebug() << "*** itm = " << itm;
       }
    */
    //m_jsonarray = rootObject["credit"].toArray();
    filterList("");
    jsonFile.close();
    return true;
}

void MainWindow::filterList(const QString &text)
{
    ui->listWidget->clear();
    qDebug() << "*** text" << text;

    for (int i = 0; i < m_jsonarray.size(); i++){
        if (m_jsonarray[i].toObject()["site"].toString().contains(text, Qt::CaseInsensitive) || text == "") {
            QListWidgetItem * item = new QListWidgetItem();
            credentialwidget * itemWidget =
                    new credentialwidget(m_jsonarray[i].toObject()["site"].toString(), m_jsonarray[i].toObject()["login"].toString(), m_jsonarray[i].toObject()["password"].toString());
            item->setSizeHint(itemWidget->sizeHint());
            ui->listWidget->addItem(item);
            ui->listWidget->setItemWidget(item, itemWidget);
        }
    }
}

int MainWindow::decryptFile(const QByteArray &encryptedBytes, QByteArray &decryptedBytes){
    // задать ключ и инициализирующий вектор
    QByteArray key_ba = QByteArray::fromHex("a6c284830c59bdea0d6f227758eee57e8e23a93dd8ffcd243b40ca39f00d78d1");
    unsigned char key[32] = {0};
    memcpy(key, key_ba.data(), 32);

    QByteArray iv_ba = QByteArray::fromHex("29f11f244ea40f11facffd580a776e30");
    unsigned char iv[16] = {0};
    memcpy(iv, iv_ba.data(), 16);

    const int BUF_LEN = 256;

    //hex(key) = a6c284830c59bdea0d6f227758eee57e8e23a93dd8ffcd243b40ca39f00d78d1
    //hex(iv) = 29f11f244ea40f11facffd580a776e30

    EVP_CIPHER_CTX *ctx; //заводится контекст
    ctx = EVP_CIPHER_CTX_new();
    if (!EVP_DecryptInit_ex2(ctx, EVP_aes_256_cbc(), key, iv, NULL)) {
        qDebug() << "EVP_DecryptInit_ex2 ERROR";
        EVP_CIPHER_CTX_free(ctx);
        return 0;
    }
    qDebug() << "EVP_DecryptInit_ex2() 0K";

    unsigned char encrypted_buf[BUF_LEN] = {0}, decrypted_buf[BUF_LEN] = {0};
    int encr_len, decr_len;

    QDataStream encrypted_stream(encryptedBytes);
    QDataStream decrypted_stream(&decryptedBytes, QIODevice::ReadWrite);
    encr_len = encrypted_stream.readRawData(reinterpret_cast<char*>(encrypted_buf), BUF_LEN);
    //qDebug() << "***  encr_len =" << encrypted_stream.readRawData(reinterpret_cast<char*>(encrypted_buf), BUF_LEN);
    while(encr_len > 0)
    {

       if (!EVP_DecryptUpdate(ctx, decrypted_buf, &decr_len, encrypted_buf, encr_len)){
           qDebug() << "EVP_DecryptUpdate ERROR";
           EVP_CIPHER_CTX_free(ctx);
           return 0;
        }
        qDebug() << "EVP_DecryptUpdate()" << reinterpret_cast<char*>(decrypted_buf);
        decrypted_stream << QByteArray(reinterpret_cast<char*>(decrypted_buf), decr_len);
        encr_len = encrypted_stream.readRawData(reinterpret_cast<char*>(encrypted_buf), BUF_LEN);

    }

    int tmplen;
    if (!EVP_DecryptFinal_ex(ctx, decrypted_buf + decr_len, &tmplen)) {
        qDebug() << "EVP_DecryptFinal_ex ERROR";
        EVP_CIPHER_CTX_free(ctx);
        return 0;
    }
    //decrypted_stream.writeRawData(reinterpret_cast<char*>(decrypted_buf), decr_len);
    qDebug() << "ALL OK";
    qDebug() << "*** decryptedBytes:" << decryptedBytes;
    EVP_CIPHER_CTX_free(ctx);
    return 1;
}
