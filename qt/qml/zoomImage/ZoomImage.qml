import QtQuick 2.3
import QtQuick.Controls 1.2
import QtQuick.Dialogs 1.2
import ZoomImage 1.0

Dialog {
    signal hidden()

    width: 700
    height: 700

    title: "Zoom Image"

    contentItem: ZoomImage {
        id: zoomImage
        source: ":/zoomimage.jpg"
    }
    
    onVisibleChanged: {
        hidden()
    }

    Component.onCompleted: {
        open();
    }
}
