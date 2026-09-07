#ifndef MICROCHAT_H
#define MICROCHAT_H

#include <QWidget>
#include <QMap>
#include <QString>
#include <cstdint>

#include "qaesencryption.h"

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

    void transmit(const QString &user, const QString &message);


    QUdpSocket *udpSenderSocket = nullptr;
    QUdpSocket *udpReceiverSocket = nullptr;

    QString userName = "mmuster";
    static constexpr std::uint16_t Port = 1111;


signals:

public slots:
private slots:
    void on_pushButton_send_clicked();
    void on_actionSend_triggered();
    void processPendingDatagrams();
    void on_pushButton_delete_clicked();

    void on_pushButton_showKey_clicked(bool checked);
    void on_pushButton_removeGroup_clicked();
    void on_comboBox_group_textActivated(const QString &group);

    void on_lineEdit_key_editingFinished();

private:
    Ui::MicroChat *ui;
    QAESEncryption *crypto;
    QMap<QString,QString> groups; // Pair of Groups and Keys
};

#endif // MICROCHAT_H
