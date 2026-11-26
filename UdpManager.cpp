#include "UdpManager.h"
#include "CarConfig.h"
#include <QJsonDocument>
#include <QDebug>

UdpManager::UdpManager(QObject *parent)
    : QObject(parent), serverPort(0)
{
    //创建套接字对象，指定 this 为父对象由 Qt 父子机制自动释放
    socket = new QUdpSocket(this);
}

UdpManager::UdpManager(const UdpManager &) : QObject(nullptr)
{

}

UdpManager::~UdpManager()
{
    //socket 由父对象 this 自动释放
}

UdpManager &UdpManager::getUdp()
{
    static UdpManager m_udp;
    return m_udp;
}

void UdpManager::setServerInfo(QString ip, quint16 port)
{
    this->serverIp = ip;
    this->serverPort = port;

    //绑定本地端口用于接收服务端应答
    if (socket->state() != QAbstractSocket::BoundState) {
        //服务端端口 +1 作为本地接收端口
        if (!socket->bind(QHostAddress::Any, static_cast<quint16>(port + 1))) {
            qDebug() << "UDP 本地端口绑定失败:" << socket->errorString();
        }
    }

    connect(socket, &QUdpSocket::readyRead, this, &UdpManager::onReadyRead);
}

bool UdpManager::sendUdpJsonData()
{
    //1）调用CarConfig的封装键值对函数
    QJsonObject obj = CarConfig::getConfig().buildJsonObject();

    //2）创建QJsonDocument,转为可发送的json格式
    QJsonDocument doc(obj);
    QByteArray arr = doc.toJson(QJsonDocument::Compact);

    //3）通过套接字发送，并检查返回值判断通信结果
    qint64 bytes = socket->writeDatagram(arr, QHostAddress(serverIp), serverPort);
    if (bytes == -1) {
        QString err = socket->errorString();
        qDebug() << "UDP发送失败:" << err;
        emit sendResult(false, err);
        return false;
    }

    qDebug() << "udp发给服务器的json字符串：" << arr << "，字节数：" << bytes;
    emit sendResult(true, QString("已发送 %1 字节").arg(bytes));
    return true;
}

void UdpManager::onReadyRead()
{
    while (socket->hasPendingDatagrams()) {
        QByteArray datagram;
        datagram.resize(static_cast<int>(socket->pendingDatagramSize()));
        QHostAddress sender;
        quint16 senderPort;
        socket->readDatagram(datagram.data(), datagram.size(), &sender, &senderPort);
        qDebug() << "收到服务端应答:" << datagram;
        emit sendResult(true, QString("服务端应答: %1").arg(QString::fromUtf8(datagram)));
    }
}
