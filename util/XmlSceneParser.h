#ifndef __XMLSCENEPARSER__
#define __XMLSCENEPARSER__

#include "ISceneParser.h"
#include "SceneData.h"

#include <vector>
#include <map>

#include <QtXml>

/**
 * @class XmlSceneParser
 *
 * This class parses the scene graph specified by the Xml file format.
 *
 * The parser is designed to replace the TinyXML parser that was in turn designed to replace the
 * Flex/Yacc/Bison parser.
 */
class XmlSceneParser : public ISceneParser {

public:
    // Create a parser, passing it the scene file.
    XmlSceneParser(const std::string& filename);

    // Clean up all data for the scene
    virtual ~XmlSceneParser();

    // Parse the scene.  Returns false if scene is invalid.
    virtual bool parse();

    virtual bool getGlobalData(SceneGlobalData& data) const;

    virtual bool getCameraData(SceneCameraData& data) const;

    virtual SceneNode* getRootNode() const;

    virtual int getNumLights() const;

    // Returns the ith light data
    virtual bool getLightData(int i, SceneLightData& data) const;


private:
    // The filename should be contained within this parser implementation.
    // If you want to parse a new file, instantiate a different parser.
    bool parseGlobalData(const QDomElement &globaldata);
    bool parseCameraData(const QDomElement &cameradata);
    bool parseLightData(const QDomElement &lightdata);
    bool parseObjectData(const QDomElement &object);
    bool parseTransBlock(const QDomElement &transblock, SceneNode* node);
    bool parsePrimitive(const QDomElement &prim, SceneNode* node);

    std::string file_name;
    mutable std::map<std::string, SceneNode*> m_objects;
    SceneCameraData m_cameraData;
    std::vector<SceneLightData*> m_lights;
    SceneGlobalData m_globalData;
    std::vector<SceneNode*> m_nodes;
};

#endif

