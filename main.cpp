#include <QCoreApplication>
#include <QCommandLineParser>
#include <QImage>
#include <QtCore>

#include <iostream>

#include "pathtracer.h"
#include "scene/scene.h"

#include <QImage>

#include "util/CS123Common.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    QCommandLineParser parser;
    parser.addHelpOption();
    parser.addPositionalArgument("config", "Path of the config file.");
    parser.process(a);

    auto positionalArgs = parser.positionalArguments();
    if (positionalArgs.size() != 1) {
        std::cerr << "Not enough arguments. Please provide a path to a config file (.ini) as a command-line argument." << std::endl;
        a.exit(1);
        return 1;
    }
    QSettings settings( positionalArgs[0], QSettings::IniFormat );
    QString inputScenePath = settings.value("IO/scene").toString();
    QString outputImagePath = settings.value("IO/output").toString();
    int imageWidth = settings.value("Settings/imageWidth").toInt();
    int imageHeight = settings.value("Settings/imageHeight").toInt();

    QImage image(imageWidth, imageHeight, QImage::Format_RGB32);

    Scene *scene;
    if(!Scene::load(inputScenePath, &scene, imageWidth, imageHeight)) {
        std::cerr << "Error parsing scene file " << inputScenePath.toStdString() << std::endl;
        a.exit(1);
        return 1;
    }

    PathTracer tracer(imageWidth, imageHeight);
    tracer.settings = {
        .samplesPerPixel = settings.value("Settings/samplesPerPixel").toInt(),
        .directLightingOnly = settings.value("Settings/directLightingOnly").toBool(),
        .numDirectLightingSamples = settings.value("Settings/numDirectLightingSamples").toInt(),
        .pathContinuationProb = settings.value("Settings/pathContinuationProb").toFloat(),
    };

    QRgb *data = reinterpret_cast<QRgb *>(image.bits());

    tracer.traceScene(data, *scene);
    delete scene;

    bool success = image.save(outputImagePath);
    if(!success) {
        success = image.save(outputImagePath, "PNG");
    }
    if(success) {
        std::cout << "Wrote rendered image to " << outputImagePath.toStdString() << std::endl;
    } else {
        std::cerr << "Error: failed to write image to " << outputImagePath.toStdString() << std::endl;
    }
    a.exit();
}
