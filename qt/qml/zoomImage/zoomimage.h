#ifndef ZOOMIMAGE_H
#define ZOOMIMAGE_H

#include <QImage>
#include <QObject>
#include <QPainter>
#include <QQuickPaintedItem>

class ZoomImage : public QQuickPaintedItem
{
    Q_OBJECT

    Q_PROPERTY(QString source READ source WRITE setSource NOTIFY sourceChanged)
    Q_PROPERTY(float speed READ speed WRITE setSpeed)
public:
    explicit ZoomImage(QQuickItem *parent = nullptr);
    virtual ~ZoomImage();

    Q_INVOKABLE void zoom(float scale = 1.0f);

    void paint(QPainter *painter) override;
    void wheelEvent(QWheelEvent *event) override;

    void setSource(const QString &url);
    QString source() const;

    void setSpeed(float speed);
    float speed();

signals:
    void sourceChanged(const QString &newUrl);

private:
    QString m_url;
    float m_scale = 1.0f;
    float m_speed = 0.01f;

    QImage *m_image;
};

#endif // ZOOMIMAGE_H
