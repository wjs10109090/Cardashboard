#ifndef CARCONFIG_H
#define CARCONFIG_H

//ini配置文件
#include <QSettings>
#include <QObject>
#include <QJsonObject>

//饿汉式

//车辆信息的配置文件（门锁，后备箱，空调的设置 ）
//单例模式的类
//1)想使用信号与槽的功能，需要当前类是QObject的子类
class CarConfig  : public QObject
{
    //2)添加元对象的宏
    Q_OBJECT
     CarConfig();
     CarConfig(const CarConfig& o);
     //配置文件 读取键值对的对象
     QSettings* setting;
     static CarConfig instacne;

signals:
     //发出新的温度值
    void sendNewTemperatuer(int temp);
public:
     ~CarConfig();
     static CarConfig& getConfig();

     //主题背景图片路径（持久化到 ini）
     void setThemePath(const QString& path);
     QString getThemePath() const;

     //车门锁
     //设置锁
     void setDoorLock( bool lock );
     //获取锁
     bool getDoorLock()const;

     //后备箱
     void setBoxLock( bool lock );
     //获取锁
     bool getBoxLock()const;

     //获取温度值
     int getTemperatuer()const;

     //设置温度值
     void setTemperatuer(int temp);

     //设置空调风量
    void setLevel(int l);
     //获取空调风量
     int getLevel() const;

     //空调一排按钮

     //自动
     bool isAutoOn() const;
     void setAutoOn(bool on);

     // 空调开关
     bool isAcOn() const;
     void setAcOn(bool on);

     //制冷
     bool isAcColdOn() const;
     void setAcColdOn(bool on);

     //内循环
     bool isRECOn() const;
     void setRECOn(bool on);

     //通风 Vent
     bool isVentOn() const;
     void setVentOn(bool on);

     //除霜Defrost
     bool isDEFOn() const;
     void setDEFOn(bool on);

     //空调吹风方向
     //玻璃
     bool isWindowOn() const;
     void setWindowOn(bool on);

     //身体
     bool isBodyOn() const;
     void setBodyOn(bool on);

     //脚
     bool isFootOn() const;
     void setFootOn(bool on);

     //封装json键值对的方法
     //函数功能：1）创建QJsonObject对象，添加多个键值对
     //       2)将带有键值对的对象返回
     /*
      {
         "car_doorlock":0,
         "car_boxlock":1,
         "temp":20
     }
       */
     QJsonObject buildJsonObject();


};

#endif // CARCONFIG_H
