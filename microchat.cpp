#include "microchat.h"
#include "ui_microchat.h"
#include <QtNetwork>
#include "time.h"
#include "fstream"

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
}

MicroChat::~MicroChat()
{
    delete ui;
}


void MicroChat::transmit(QString user, QString message)
{

    QByteArray datagram = user.toUtf8() + "|" + message.toUtf8() +"\n";
    udpSenderSocket->writeDatagram(datagram.data(), datagram.size(),
                             QHostAddress::Broadcast, Port);
}

void MicroChat::on_pushButton_send_clicked()
{
    QString input = ui->plainTextEdit_input->toPlainText();

    if(input.contains("Idiot") || input.contains("Blödmann") || input.contains("Depp")){
        input = QString("**");
    }

    if(input.contains('\n')){
        input = QString("\n")+input;
    }

    if(input.contains('\t')){
        input = QString("Hier wurde nichts eingegeben");
    }

    if(input.contains(' ')){
        input = QString("Hier wurde nichts eingegeben");
    }

    if(input.length() == 0){
        input = QString("Hier wurde nichts eingegeben");
    }

    transmit(userName, input);
    ui->plainTextEdit_input->clear();
    ui->plainTextEdit_input->setFocus();
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

        QString receivedText = QString(datagram).toUtf8();
        QStringList list = receivedText.split('|');
        QString senderName = list[0];
        list.removeFirst();
        receivedText = list.join('|');
        receivedText = receivedText.remove(receivedText.length()-1,1); // strip \r\n
        QString Timestamp = __TIMESTAMP__;
        ui->plainTextEdit_chat->appendPlainText(Timestamp
                                                + " "
                                                + senderName
                                                + ": "
                                                + receivedText);
    }
}


