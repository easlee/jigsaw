#include "jigsaw/composite/CompositePicture.h"

//-----------------------------------------------------------------------
void jigsaw::composite::PictureView::SetPNG(const QString& _path)
{
    if (0 == _path.length())
        return;

    QPixmap pixmap(_path);

    auto em = this->ToElement<PictureElement>();
    if(em->ptrPicture->movie() != 0)
    {
        em->ptrPicture->movie()->stop();
    }
    em->ptrPicture->setPixmap(pixmap);
}

//-----------------------------------------------------------------------
void jigsaw::composite::PictureView::SetJPG(const QString& _path)
{
    if (0 == _path.length())
        return;

    QPixmap pixmap(_path);

    auto em = this->ToElement<PictureElement>();
    if(em->ptrPicture->movie() != 0)
    {
        em->ptrPicture->movie()->stop();
    }
    em->ptrPicture->setPixmap(pixmap);
}

void jigsaw::composite::PictureView::SetGIF(const QString& _path)
{
    auto em = this->ToElement<PictureElement>();
    em->ptrPicture->movie()->setFileName(_path);
    em->ptrPicture->movie()->start();
}

void jigsaw::composite::PictureView::SetFitJPG(const QString &_path)
{
    if (0 == _path.length())
        return;

    auto em = this->ToElement<PictureElement>();
    if(em->ptrPicture->movie() != 0)
    {
        em->ptrPicture->movie()->stop();
    }
    QImage qImage;
    qImage.load(_path);
    QPixmap qPixmap = QPixmap::fromImage(qImage);
    QPixmap fitPixmap = qPixmap.scaled(em->ptrPicture->width(), em->ptrPicture->height(), Qt::KeepAspectRatio, Qt::SmoothTransformation);
    em->ptrPicture->setScaledContents(false);
    em->ptrPicture->setPixmap(fitPixmap);
}

//-----------------------------------------------------------------------
void jigsaw::composite::PictureView::LoadFromPNG(const QByteArray& _data)
{
	QPixmap pixmap;
	pixmap.loadFromData(_data, "PNG");
    auto em = this->ToElement<PictureElement>();
    em->ptrPicture->movie()->stop();
    em->ptrPicture->setPixmap(pixmap);
}

//-----------------------------------------------------------------------
void jigsaw::composite::PictureView::LoadFromJPG(const QByteArray& _data)
{
    QPixmap pixmap;
    pixmap.loadFromData(_data, "JPG");
    auto em = this->ToElement<PictureElement>();
    em->ptrPicture->movie()->stop();
    em->ptrPicture->setPixmap(pixmap);
}

void jigsaw::composite::PictureView::LoadFromGIF(const QByteArray& _data)
{
    auto em = this->ToElement<PictureElement>();
    QIODevice* device = em->ptrPicture->movie()->device();
	QBuffer* buffer = dynamic_cast<QBuffer*>(device);
	assert(buffer);
	buffer->setData(_data);
    em->ptrPicture->movie()->start();
}

//-----------------------------------------------------------------------
void jigsaw::composite::PictureView::onSetupUI()
{
    PictureElement* element = new PictureElement();
    element->root = View::NewHRoot();
    element->container = element->root;

    element->ptrPicture = new QLabel(element->root);
    element->root->layout()->addWidget(element->ptrPicture);
    element->ptrPicture->setScaledContents(true);
    element->ptrPicture->setAlignment(Qt::AlignCenter);
    QMovie* movie = new QMovie();
	QBuffer* buffer = new QBuffer();
	movie->setDevice(buffer);
	movie->setFormat("GIF");
    element->ptrPicture->setMovie(movie);

    element_ = element;
}

void jigsaw::composite::PictureView::onReset()
{
    auto em = this->ToElement<PictureElement>();
    em->ptrPicture->clear();
}
