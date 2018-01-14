#include <QCoreApplication>

#include <iostream>

#include <QImage>

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    QImage image(50, 50, QImage::Format_RGB32);
    QRgb *data = reinterpret_cast<QRgb *>(image.bits());
    for(int y = 0; y < 50; ++y) {
        for(int x = 0; x < 50; ++x) {
            int red = 255 * (y / 50.f);
            int blue = 255 * (x / 50.f);
            QRgb color = qRgb(red, 0, blue);
            int offset = x + (y * 50);
            data[offset] = color;
        }
    }
    std::cout << image.save("Test.png") << std::endl;
    a.exit();
}
