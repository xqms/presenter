
import QtQuick 2.0
import presenter 1.0

Rectangle {
	anchors.fill: parent
	color: "black"

	PageView {
		id: currentSlideView

		anchors.fill: parent
		page: controller.currentPage
	}

	KeyMap {}
}
