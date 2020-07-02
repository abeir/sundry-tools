#include "zoomimage.h"

#include <QDebug>

ZoomImage::ZoomImage(QQuickItem *parent) : QQuickPaintedItem(parent)
{

}

ZoomImage::~ZoomImage()
{

}

void ZoomImage::zoom(float scale)
{
    m_scale = scale;
    update();
}

void ZoomImage::paint(QPainter *painter)
{
    if(painter == nullptr){
        return;
    }
    if(m_url.isNull() || m_url.isEmpty()){
        return;
    }
    if(m_image == nullptr){
        m_image = new QImage;
        m_image->load(m_url);
    }
    qDebug() << Q_FUNC_INFO << "w:" << m_image->width() << " h:" << m_image->height();

    QImage tmpImage = m_image->scaled(m_image->width() * m_scale, m_image->height() * m_scale, Qt::KeepAspectRatio);

    painter->setRenderHint(QPainter::Antialiasing, true);
    painter->setRenderHint(QPainter::HighQualityAntialiasing, true);
    painter->setRenderHint(QPainter::SmoothPixmapTransform, true );
    painter->drawImage(0,0, tmpImage);
}

void ZoomImage::wheelEvent(QWheelEvent *event)
{
    qDebug() << Q_FUNC_INFO << event->angleDelta().y();

    int direction = event->angleDelta().y() / 120;
    if(direction > 0){
        if(m_scale < 1){
            m_scale += m_speed;
        }
    }else{
        if(m_scale > 0){
            m_scale -= m_speed;
        }
    }
    zoom(m_scale);
}


void ZoomImage::setSource(const QString &url)
{
    qDebug() << Q_FUNC_INFO << url;
    m_url = url;
    if(m_url.isNull() || m_url.isEmpty()){
        return;
    }
    if(m_image == nullptr){
        m_image = new QImage;
    }
    m_image->load(m_url);
}

QString ZoomImage::source() const
{
    return m_url;
}

void ZoomImage::setSpeed(float speed)
{
    if(speed <= 0){
        qWarning() << Q_FUNC_INFO << "speed must gt 0, but is" << speed;
        return;
    }
    m_speed = speed;
}

float ZoomImage::speed()
{
    return m_speed;
}
