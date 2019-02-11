
import QtQuick 2.0

Item {
	focus: true
	Keys.onLeftPressed: controller.previousSlide()
	Keys.onRightPressed: controller.nextSlide()
	Keys.onSpacePressed: controller.nextSlide()
	Keys.onTabPressed: controller.slideSelectorActive = !controller.slideSelectorActive

	Keys.onPressed: {
		if(event.key == Qt.Key_Q)
		{
			controller.quit();
			event.accepted = true;
		}
		else if(event.key == Qt.Key_R)
		{
			controller.resetTime();
			event.accepted = true;
		}
		else if(event.key == Qt.Key_MediaNext || event.key == Qt.Key_PageDown)
		{
			controller.nextSlide();
			event.accepted = true;
		}
		else if(event.key == Qt.Key_MediaPrevious || event.key == Qt.Key_PageUp)
		{
			controller.previousSlide();
			event.accepted = true;
		}
	}
}
