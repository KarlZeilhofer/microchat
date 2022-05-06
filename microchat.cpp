#include "microchat.h"
#include "ui_microchat.h"
#include <QtNetwork>
#include <bits/functional_hash.h>
#include <QSettings>


MicroChat::MicroChat(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MicroChat)
{
    ui->setupUi(this);
    udpSenderSocket = new QUdpSocket(this);

    udpReceiverSocket = new QUdpSocket(this);
    udpReceiverSocket->bind(Port, QUdpSocket::ShareAddress);

    connect(udpReceiverSocket, SIGNAL(readyRead()),
            this, SLOT(processPendingDatagrams()));

    userName = qgetenv("USER");
    if (userName.isEmpty())
        userName = qgetenv("USERNAME");

    crypto = new QAESEncryption(QAESEncryption::AES_256, QAESEncryption::CBC);

    QSettings set;
    QMap<QString, QString> emptyMap;
    emptyMap["Default"] = "20220506"; // default password
    emptyMap["PlainText"] = "";
    QStringList groupList = set.value("groups", QStringList(emptyMap.keys())).toStringList();
    QStringList passwordList = set.value("passwords", QStringList(emptyMap.values())).toStringList();
    groups.clear();
    for(int i=0; i<groupList.size() && i<passwordList.size(); i++){
        groups[groupList[i]] = passwordList[i];
        ui->comboBox_group->addItem(groupList[i]);
    }
    ui->lineEdit_key->setText(groups[ui->comboBox_group->currentText()]);
}

MicroChat::~MicroChat()
{
    QSettings set;
    set.setValue("groups", QStringList(groups.keys()));
    set.setValue("passwords", QStringList(groups.values()));
    delete ui;
}

// Format: <Groupname>|<User Name>|<Encrypted Text>
void MicroChat::transmit(QString user, QString message)
{
    QByteArray msg = message.toUtf8();
    if(ui->lineEdit_key->text().isEmpty() == false){
        msg = QString("DECRYPTED").toUtf8() + msg;
        QByteArray hashedKey =
                QCryptographicHash::hash(ui->lineEdit_key->text().toUtf8(),
                                         QCryptographicHash::Sha256);
        QByteArray iv ("my-IV-vector");
        QByteArray hashedIV = QCryptographicHash::hash(iv, QCryptographicHash::Md5);
        msg = crypto->encode(msg, hashedKey, hashedIV);
    }
    QByteArray datagram =  ui->comboBox_group->currentText().toUtf8() + "|" +
            user.toUtf8() + "|" +
            msg;
    udpSenderSocket->writeDatagram(datagram.data(), datagram.size(),
                                   QHostAddress::Broadcast, Port);
}

void MicroChat::on_pushButton_send_clicked()
{

    QString input = ui->plainTextEdit_input->toPlainText();
    if(input!=0){
        if(input.contains('\n')){
            input = QString("\n")+input;
        }
        transmit(userName, input);
        ui->plainTextEdit_input->clear();
        ui->plainTextEdit_input->setFocus();
    }
}



void MicroChat::on_actionSend_triggered()
{
    on_pushButton_send_clicked();
}

void MicroChat::processPendingDatagrams()
{
    while (udpReceiverSocket->hasPendingDatagrams()) {
        QByteArray datagram;
        datagram.resize(udpReceiverSocket->pendingDatagramSize());
        udpReceiverSocket->readDatagram(datagram.data(), datagram.size());

        QString receivedText = QString(datagram);
        QStringList list = receivedText.split('|');
        QString senderGroup = list[0];
        QString senderName = list[1];
        list.removeFirst(); // group
        list.removeFirst(); // user
        receivedText = list.join('|');

        bool decrypted = false;
        // try to decrypt:
        if(groups.contains(senderGroup) && groups[senderGroup].isEmpty() == true){
            decrypted = true;
        }else if(groups.contains(senderGroup) && groups[senderGroup].isEmpty() == false){
            QByteArray hashedKey =
                    QCryptographicHash::hash(groups[senderGroup].toUtf8(),
                                             QCryptographicHash::Sha256);
            QByteArray iv ("my-IV-vector");
            QByteArray hashedIV = QCryptographicHash::hash(iv, QCryptographicHash::Md5);

            QList<QByteArray> list = datagram.split('|');
            list.removeFirst();
            list.removeFirst();
            QByteArray msg = list.join('|');

            msg = crypto->decode(msg, hashedKey, hashedIV);
            receivedText = QString(crypto->removePadding(msg));

            if(receivedText.startsWith("DECRYPTED")){
                receivedText = receivedText.right(receivedText.length()-9);
                decrypted = true;
            }
        }


        if(decrypted){
            ui->plainTextEdit_chat->appendPlainText(QTime::currentTime().toString("HH:mm:ss")
                                                    + " "
                                                    + senderName
                                                    + "@"
                                                    + senderGroup
                                                    + ": "
                                                    + receivedText + "\n");
        }
    }
}

void MicroChat::on_pushButton_delete_clicked(){
    ui->plainTextEdit_chat->clear();
}

void MicroChat::on_pushButton_showKey_clicked(bool checked)
{
    if(checked){
        ui->lineEdit_key->setEchoMode(QLineEdit::Normal);
    }else{
        ui->lineEdit_key->setEchoMode(QLineEdit::Password);
    }
}

void MicroChat::on_pushButton_removeGroup_clicked()
{
    if(ui->comboBox_group->currentText() != "Default" &&
            ui->comboBox_group->currentText() != "PlainText"){
        groups.remove(ui->comboBox_group->currentText());
        ui->comboBox_group->removeItem(ui->comboBox_group->currentIndex());
        ui->lineEdit_key->setText(groups[ui->comboBox_group->currentText()]);
    }
}

void MicroChat::on_comboBox_group_activated(const QString &group)
{
    qDebug() << "activated:" << group;
    if(groups.contains(group) == false){
        ui->lineEdit_key->clear();
    }else{
        ui->lineEdit_key->setText(groups[group]);
    }
}

void MicroChat::on_lineEdit_key_editingFinished()
{
    groups[ui->comboBox_group->currentText()] = ui->lineEdit_key->text();
}
