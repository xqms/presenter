
import QtQuick 2.0
import QtQuick.Controls 1.2
import QtMultimedia 5.8
import presenter 1.0

Rectangle {
	property var page
	property bool preview: false
	property bool isConsole: false

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
			id: videoRepeater
			model: (!preview && page) ? page.videoObjects : 0
			Video {
				source: modelData.url
				autoPlay: modelData.autostart
				loops: modelData.loop ? MediaPlayer.Infinite : 1
				x: modelData.area.left * parent.width
				y: modelData.area.top * parent.height
				width: modelData.area.width * parent.width
				height: modelData.area.height * parent.height

				Connections {
					target: controller
					onVideoPause: function() {
						if(playbackState == MediaPlayer.PlayingState)
							pause();
						else
							play();
					}
					onVideoFaster: function() {
						console.log("playbackRate before:", playbackRate);
						playbackRate = 2 * playbackRate;
						console.log("playbackRate after:", playbackRate);
					}
					onVideoSlower: function() {
						playbackRate = playbackRate / 2;
					}
					onVideoSeekBack: function() {
						seek(position - 5000);
					}
					onVideoSeekFwd: function() {
						seek(position + 5000);
					}
				}

				Rectangle {
					visible: isConsole
					anchors.left: parent.left
					anchors.bottom: parent.bottom

					color: "red"

					width: parent.width * parent.position / parent.duration
					height: 25

					Text {
						x: 0
						y: 0

						Binding on text {
							when: !!parent.playbackRate
							value: parent.playbackRate
						}
						color: "white"
						font.pointSize: 24

						onTextChanged: function() { console.log("new text:", text, parent.playbackRate); }
					}
				}
			}
		}
	}
}
