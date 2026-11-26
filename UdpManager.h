#ifndef UDPMANAGER_H
#define UDPMANAGER_H

#include <QObject>
#include <QString>
#include <QUdpSocket>
#include <QJsonDocument>

//管理界面 更改车辆状态时，给服务器接收端发送指令
//单例模式的类：懒汉式
class UdpManager : public QObject
{
    Q_OBJECT
    explicit UdpManager(QObject *parent = nullptr);
    UdpManager(const UdpManager&);
    //套接字
    QUdpSocket* socket;
    //服务器ip
    QString serverIp;
    //服务器端口
    quint16 serverPort;

private slots:
    //接收服务端应答
    void onReadyRead();

signals:
    //通信结果反馈：ok 为是否成功写入/收到应答，message 为描述
    void sendResult(bool ok, const QString& message);

public:
    ~UdpManager();
    static UdpManager& getUdp();

    //给 服务器ip和端口号 赋值的函数
    void setServerInfo(QString ip, quint16 port);

    //通过套接字 给服务器接收端发送 json格式字符串
    //返回值：true 表示数据成功写入套接字，false 表示发送失败
    bool sendUdpJsonData();
};

#endif // UDPMANAGER_H
