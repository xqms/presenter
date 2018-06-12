
import QtQuick 2.5
import presenter 1.0

Rectangle {
	anchors.fill: parent
	color: "black"
	id: container

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

	Text {
		color: "white"
		text: controller.elapsedTimeString
		font.pointSize: 60
		anchors.bottom: parent.bottom
		anchors.horizontalCenter: parent.horizontalCenter
	}

	Text {
		color: "white"
		font.pointSize: 60
		anchors.bottom: parent.bottom
		anchors.right: parent.right
		text: "%1/%2 %3/%4".arg(controller.currentPage.slideAnimationIndex+1).arg(controller.currentPage.slideAnimationCount).arg(controller.currentSlideNumber + 1).arg(controller.slideCount)
	}

	Rectangle {
		color: "black"
		anchors.fill: parent
		visible: controller.slideSelectorActive

		FontMetrics {
			id: labelMetrics
			font.family: "Arial"
		}

		GridView {
			id: slideSelector
			anchors.fill: parent
			model: controller.allPages
			property real margin : 4
			property real labelHeight: labelMetrics.height
			cellWidth: container.width / 4
			cellHeight: 9.0 / 16.0 * (cellWidth - 2*margin) + 2*margin + labelHeight /* FIXME: fixed ratio */
			focus: controller.slideSelectorActive
			onFocusChanged: currentIndex = controller.currentSlideNumber

			delegate: Item {
				id: delegate
				width: slideSelector.cellWidth
				height: slideSelector.cellHeight

				Item {
					x: slideSelector.margin
					y: slideSelector.margin
					width: parent.width - 2*slideSelector.margin
					height: parent.height - 2*slideSelector.margin

					PageView {
						id: pageView
						width: parent.width
						height: parent.height - slideSelector.labelHeight
						page: modelData
						preview: true

						MouseArea {
							anchors.fill: parent
							onClicked: {
								controller.currentSlideNumber = index;
								controller.slideSelectorActive = false;
							}
						}
					}

					Text {
						id: labelText
						anchors.horizontalCenter: parent.horizontalCenter
						anchors.bottom: parent.bottom
						width: parent.width
						horizontalAlignment: Text.AlignHCenter
						color: "white"
						text: modelData.label
					}

					/* Darken if not current item */
					Rectangle {
						anchors.fill: parent
						visible: !delegate.GridView.isCurrentItem
						color: "black"
						opacity: 0.5
					}
				}
			}

			highlight: Rectangle {
				width: slideSelector.cellWidth
				height: slideSelector.cellHeight
				color: "blue"
			}

			Keys.onPressed: {
				if(event.key == Qt.Key_Q || event.key == Qt.Key_Tab)
				{
					controller.slideSelectorActive = false;
					event.accepted = true;
				}
				else if(event.key == Qt.Key_Return)
				{
					controller.currentSlideNumber = slideSelector.currentIndex;
					controller.slideSelectorActive = false;
					event.accepted = true;
				}
			}
		}
	}

	KeyMap {
		focus: !controller.slideSelectorActive
	}
}
