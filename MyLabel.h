#ifndef MYLABEL_H
#define MYLABEL_H

#include <QLabel>
#include <QTimer>

//自定义的标签控件,继承了框架中已有的类，好处是：
//1）继承了qt框架中的QLabel类，父类中拥有了标签控件的功能，那么子类继承后，就拥有标签控件的功能
//2）子类在原有功能基础上，可以功能的自定义或者扩展

class MyLabel : public QLabel
{
    Q_OBJECT
public:
    MyLabel(QWidget* parent = nullptr );

    //自定义标签显示内容的函数（显示内容+显示1秒）
    void toastLabel(QString str);

private:
    //自定义定时器
    QTimer* timer;
};

#endif // MYLABEL_H
