
import QtQuick 2.0
import QtQuick.Controls 1.2
import presenter 1.0

Rectangle {
	anchors.fill: parent
	color: "black"

	BusyIndicator {
		anchors.centerIn: parent
		width: parent.width / 3
		height: width

		running: !controller.currentPage.ready
	}

	ImageView {
		anchors.fill: parent
		image: controller.currentPage.image
	}

	focus: true
	Keys.onLeftPressed: controller.previousSlide()
	Keys.onRightPressed: controller.nextSlide()

	Keys.onPressed: {
		if(event.key == Qt.Key_Q)
		{
			controller.quit();
			event.accepted = true;
		}
	}
}
