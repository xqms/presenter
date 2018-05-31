
import QtQuick 2.0
import QtQuick.Controls 1.2
import presenter 1.0

Rectangle {
	property var page

	height: width / imageView.aspectRatio
	color: "black"

	BusyIndicator {
		anchors.centerIn: parent
		width: parent.width / 3
		height: width

		running: page ? !page.ready : false
	}

	ImageView {
		id: imageView
		anchors.fill: parent
		image: page ? page.image : nullImage
	}
}
