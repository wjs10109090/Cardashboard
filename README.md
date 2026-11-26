# 车载中控屏幕系统（Qt / C++）

基于 Qt 5.14 的车载中控界面，固定 1024×600 分辨率，模拟车载大屏的导航与车辆控制。用 `QStackedWidget` 承载 4 个页面：主页、主题、空调、油耗。

## 功能

**主页**

- 实时时钟：`QTimer` 每秒刷新，显示 `HH:mm:ss` 与「月/日 周几」
- 音乐播放：`QMediaPlayer` 自动扫描 `music/` 下的 mp3，支持播放/暂停、上一曲/下一曲，读取 ID3 标题与封面图显示
- 四门锁 / 后备箱开关：点击切换状态、图标随之变化，弹 1 秒 Toast 提示，同时把车辆状态经 UDP 上报
- 页面入口：汽车图标进主题页，油耗按钮进油耗页

**主题页**

- 5 张背景缩略图按 3 列网格排列，点击高亮（蓝色描边），确认后切换主页背景
- 选择结果写入 `config.ini`，重启后自动恢复

**空调页**

- 温度 16–30 ℃（`CarConfig` 中做了边界钳制）
- 0–7 档风量：自绘控件，点击档位条直接跳档
- 风向：玻璃 / 身体 / 脚
- 模式：自动、制冷、内循环、通风、除霜

**油耗页**

- `QtCharts` 绘制近 7 天油耗：折线 + 渐变面积图，叠加「同车型均值」水平参考线（5.9）

## 技术要点

- **页面切换**：底部 `BottomBar` 发出 `switchToHome / switchToAir / switchToTheme` 信号，`MainWindow` 用槽切换 `QStackedWidget` 索引
- **配置单例**：`CarConfig` 饿汉式单例 + `QSettings("config.ini", IniFormat)`，统一管理门锁、后备箱、温度、风量、风向与主题路径
- **跨页面同步**：调温通过 `sendNewTemperatuer(int)` 信号广播，主页与空调页同时更新，并顺带触发一次 UDP 上报
- **自绘控件**：`WindSpeedControl` 继承 `QWidget`，重写 `paintEvent`（圆角档位条）与 `mousePressEvent`（按点击 x 坐标换算档位）
- **事件过滤器**：`ThemeScreen` 用 `installEventFilter` + `eventFilter` 统一处理缩略图点击高亮，用 `objectName` 记录选中下标
- **UDP 通信**：`UdpManager` 单例封装 `QUdpSocket`，`QJsonDocument::Compact` 序列化车辆状态，按 `writeDatagram` 返回值判断成败；本地绑定 `serverPort+1` 接收服务端应答，结果回显到状态栏
- **Toast**：`MyLabel` 自定义控件实现 1 秒自动消失的轻提示

## 目录结构

```
MainWindow.*        主窗口，页面切换与信号接线
BottomBar.*         底部导航栏（主页 / 空调 / 主题）
HomeScreen.*        主页：时钟、音乐、门锁、后备箱
AirScreen.*         空调控制页
WindSpeedControl.*  自绘风量档位控件
ThemeScreen.*       主题背景选择页
FuelScreen.*        油耗趋势页（QtCharts）
CarConfig.*         全局配置单例 + ini 持久化
UdpManager.*        UDP 上报与应答接收
MyLabel.*           Toast 提示控件
image.qrc           资源清单（background/ 下 60 张图）
```

## 环境与编译

- Qt 5.14（`core gui widgets multimedia charts network`）+ C++11
- Qt Creator 打开 `CarDashboard_1009.pro` 直接构建即可

运行前把 mp3 放进程序目录的 `music/` 文件夹（代码里另有一个 `D:/qtwork/CarDashboard/music` 的本机路径兜底）；没有音乐也能正常启动，只是播放器没有内容。

> `.pro` 中显式声明 `network` 是为 `QUdpSocket` 准备的；早期版本靠 `charts` / `multimedia` 的传递依赖才链上 `Qt5Network`，显式写出更稳妥。
