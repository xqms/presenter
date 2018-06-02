
import QtQuick 2.0
import QtQuick.Controls 1.2
import QtMultimedia 5.8
import presenter 1.0

Rectangle {
	property var page
	property bool preview: false

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

	Item {
		x: imageView.imageRect.x
		y: imageView.imageRect.y
		width: imageView.imageRect.width
		height: imageView.imageRect.height
		clip: true

		Repeater {
			model: (!preview && page) ? page.videoObjects : 0
			Video {
				source: modelData.url
				autoPlay: modelData.autostart
				loops: modelData.loop ? MediaPlayer.Infinite : 1
				x: modelData.area.left * parent.width
				y: modelData.area.top * parent.height
				width: modelData.area.width * parent.width
				height: modelData.area.height * parent.height
			}
		}
	}
}
