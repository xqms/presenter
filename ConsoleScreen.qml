
import QtQuick 2.0
import presenter 1.0

Rectangle {
	anchors.fill: parent
	color: "black"

	PageView {
		id: currentSlideView

		anchors.top: parent.top
		anchors.left: parent.left
		page: controller.currentPage
		width: (1.0 / 1.618034) * parent.width
	}

	PageView {
		id: nextSlideView

		anchors.left: currentSlideView.right
		page: controller.nextPage
		width: (1.0 - 1.0 / 1.618034) * parent.width
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
