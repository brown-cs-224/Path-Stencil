#ifndef PATHTRACER_H
#define PATHTRACER_H

#include <QImage>

class PathTracer
{
public:
    PathTracer();

    void traceScene(QRgb *imageData);
};

#endif // PATHTRACER_H
