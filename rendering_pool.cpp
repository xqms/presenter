// Manages render threads
// Author: Max Schwarz <max.schwarz@online.de>

#include "rendering_pool.h"
#include "video_object.h"

#include <QtConcurrent/QtConcurrent>

#include <boost/lexical_cast.hpp>

#include <iostream>

RenderingPage::RenderingPage(const QUrl& file, Poppler::Page* page, QThreadPool* pool, QObject* parent)
 : QObject(parent)
 , m_page(page)
 , m_pool(pool)
{
	for(auto& link : m_page->links())
	{
		if(link->linkType() == Poppler::Link::Execute)
		{
			auto execLink = reinterpret_cast<Poppler::LinkExecute*>(link);

			VideoObject* obj = new VideoObject(this);
			obj->setArea(execLink->linkArea());
			obj->setupFromLink(file, execLink->fileName());
			m_videoObjects.append(obj);
		}
	}
}

RenderingPage::~RenderingPage()
{
}

bool RenderingPage::ready() const
{
	QMutexLocker locker(&m_mutex);
	return !m_image.isNull();
}

QImage RenderingPage::image() const
{
	QMutexLocker locker(&m_mutex);
	return m_image;
}

void RenderingPage::triggerRender(const QSize& size)
{
	if(!m_future.isFinished())
	{
		m_future.cancel();
		m_future.waitForFinished();
	}

	{
		QMutexLocker locker(&m_mutex);
		m_image = QImage();
		m_size = size;
	}

	m_future = QtConcurrent::run(m_pool, this, &RenderingPage::render);
}

void RenderingPage::render()
{
	QSizeF pageSize = m_page->pageSizeF() / 72.0;
	float dpi = std::min(
		m_size.width() / pageSize.width(),
		m_size.height() / pageSize.height()
	);

	QImage image = m_page->renderToImage(dpi, dpi);
	if(image.isNull())
	{
		std::cerr << "Could not render image";
		std::abort();
	}

	{
		QMutexLocker locker(&m_mutex);
		m_image = image;
		readyChanged();
		imageChanged();
	}
}

RenderingPool::RenderingPool(const QUrl& file, const std::shared_ptr<Poppler::Document>& doc, QObject* parent)
 : QObject(parent)
 , m_doc(doc)
{
	m_pool = new QThreadPool(this);

	bool slideLabelsAreNumbers = true;
	std::vector<int> slideNumbers;
	std::vector<int> slideAnimationIndices;
	std::vector<int> slideAnimationCounts;

	auto collectSlideNumber = [&](RenderingPage* page){
		bool ok = true;
		int num = page->label().toInt(&ok);
		if(!ok)
			return false;

		if(num < 1)
			return false;

		// Check numbers are consecutive
		if(slideNumbers.empty())
		{
			if(num != 1)
				return false;
		}
		else
		{
			int last = slideNumbers.back();
			if(last != num && last + 1 != num)
				return false;
		}

		if(static_cast<std::size_t>(num-1) >= slideAnimationCounts.size())
		{
			slideAnimationIndices.push_back(0);
			slideAnimationCounts.push_back(1);
		}
		else
		{
			slideAnimationIndices.push_back(slideAnimationCounts[num-1]);
			slideAnimationCounts[num-1]++;
		}

		slideNumbers.push_back(num);

		return true;
	};

	for(int i = 0; i < m_doc->numPages(); ++i)
	{
		auto page = new RenderingPage(file, doc->page(i), m_pool, this);
		connect(page, &RenderingPage::readyChanged, this, &RenderingPool::checkFinished);
		append(page);

		page->setSlideNumberInformation(i+1, 0, 1);

		if(slideLabelsAreNumbers)
			slideLabelsAreNumbers = collectSlideNumber(page);
	}

	if(slideLabelsAreNumbers)
	{
		for(int i = 0; i < count(); ++i)
		{
			auto* page = reinterpret_cast<RenderingPage*>((*this)[i]);

			page->setSlideNumberInformation(slideNumbers[i],
				slideAnimationIndices[i],
				slideAnimationCounts[slideNumbers[i]-1]
			);
		}
	}
}

RenderingPool::~RenderingPool()
{
}

void RenderingPool::triggerRender(const QSize& size)
{
	for(auto& obj : *this)
	{
		RenderingPage* page = reinterpret_cast<RenderingPage*>(obj);
		page->triggerRender(size);
	}
}

void RenderingPool::checkFinished()
{
	for(auto& obj : *this)
	{
		RenderingPage* page = reinterpret_cast<RenderingPage*>(obj);
		if(!page->ready())
			return;
	}

	renderingFinished();
}

