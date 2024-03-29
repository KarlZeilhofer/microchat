#ifndef MICROCHAT_H
#define MICROCHAT_H

#include <QWidget>
#include <stdint.h>
#include <qaesencryption.h>
#include <QMap>

QT_BEGIN_NAMESPACE
class QDialogButtonBox;
class QLabel;
class QPushButton;
class QTimer;
class QUdpSocket;
QT_END_NAMESPACE

namespace Ui {
class MicroChat;
}

class MicroChat : public QWidget
{
    Q_OBJECT
public:
    explicit MicroChat(QWidget *parent = nullptr);
    ~MicroChat();

    void transmit(QString user, QString message);


    QUdpSocket* udpSenderSocket = 0;
    QUdpSocket* udpReceiverSocket = 0;

    QString userName = "mmuster";
    const uint16_t Port = 1111;


signals:

public slots:
private slots:
    void on_pushButton_send_clicked();
    void on_actionSend_triggered();
    void processPendingDatagrams();
    void on_pushButton_delete_clicked();

    void on_pushButton_showKey_clicked(bool checked);
    void on_pushButton_removeGroup_clicked();
    void on_comboBox_group_activated(const QString &group);

    void on_lineEdit_key_editingFinished();

private:
    Ui::MicroChat *ui;
    QAESEncryption *crypto;
    QMap<QString,QString> groups; // Pair of Groups and Keys
};

#endif // MICROCHAT_H
