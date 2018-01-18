#ifndef PATHTRACER_H
#define PATHTRACER_H

#include <QImage>

#include "scene/scene.h"

class PathTracer
{
public:
    PathTracer(int width, int height);

    void traceScene(QRgb *imageData, const Scene &scene);

private:
    int m_width, m_height;

    QRgb tracePixel(int x, int y, const Scene &scene);

    QRgb traceRay(Ray r, const Scene &scene);
};

#endif // PATHTRACER_H
