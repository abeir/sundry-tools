# 说明

## zoomimage.h  zoomimage.cpp
自定义的 `ZoomImage` 组件，继承 `QQuickPaintedItem`，
实现虚函数 `void paint(QPainter *painter)` 绘制缩放的图片，
实现虚函数 `void wheelEvent(QWheelEvent *event)` 捕获鼠标滚轮事件计算缩放比例，并重绘图片

### 属性
`source`  设置位置，可使用qrc(使用冒号":"作为前缀，不可使用"qrc:")

`speed`  设置鼠标滚动缩放的速度

### 方法
`void zoom(float scale = 1.0f)`  设置缩放比例并重绘图片


## ZoomImage.qml  zoomimage.jpg
ZoomImage 的使用示例

# 使用

在主代码中注册组件，并在需要使用到的 qml 中 import

例如：

注册组件
```c++
qmlRegisterType<ZoomImage>("ZoomImage", 1, 0, "ZoomImage");
```

使用
```c++
import QtQuick 2.5
import ZoomImage 1.0

Rectangle {
  ZoomImage {
      id: zoomImage
      source: ":/zoomimage.jpg"
  }
}
```

