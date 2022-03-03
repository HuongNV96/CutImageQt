#include <QCoreApplication>
#include <QDebug>
#include <QImage>
#include <QRect>
#include <QtMath>


std::shared_ptr<QRect> rectFind(QRect &rec1,QRect & rec2)
{
    if (rec1.x() > (rec2.x() + rec2.width())
       || (rec1.x() + rec1.width() < rec2.x())
       || (rec1.y() > (rec2.y() + rec2.height()))
       || (rec1.y() + rec1.height() < rec2.y()))
            return nullptr;
    return std::make_shared<QRect>(
                qMax(rec1.x(),rec2.x()),
                qMax(rec1.y(),rec2.y()),
                qMin(rec1.x() + rec1.width(),rec2.x() + rec2.width()) - qMax(rec1.x(),rec2.x()),
                qMin(rec1.y() + rec1.height(),rec2.y() + rec2.height()) - qMax(rec1.y(),rec2.y())
                );
}
std::shared_ptr<QImage> cutImage(QImage &image,QRect &rec)
{
    auto recImage = image.rect();
    auto rectOverlap = rectFind(recImage,rec);
    if (rectOverlap == nullptr)
        return nullptr;
    std::shared_ptr<QImage> res = std::make_shared<QImage>(image);
    for (int x = recImage.x(); x < recImage.x() + recImage.width(); x++)
        for (int y = recImage.y(); y < recImage.y() + recImage.height(); y++)
        {
            if (x < (rectOverlap->x() + rectOverlap->width())
               && x > rectOverlap->x()
               && y < (rectOverlap->y() + rectOverlap->height())
               && y > rectOverlap->y())
                continue;
            res->setPixelColor(x,y,Qt::black);
        }

    return res;
}

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    QImage myImage;
    const QString path = "/home/huongnv/QT_ws/cutImage/image/";
    auto ret = myImage.load(path + "input.jpeg");
    if (!ret)
        return a.exec();
    QRect rec(0,0,100,100);
    auto out = cutImage(myImage,rec);
    if (out != nullptr)
        out->save(path + "output.jpeg");
    return a.exec();
}
