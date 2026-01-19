/**
 * @file ISceneParser
 *
 * Interface for scene files.
 *
 * The current file format is an xml format and the parser implementation
 * is XmlSceneParser.  A user could implement a different scene format
 * and if it implements this interface, it should be fully compatible with
 * their other code.
 */

#ifndef __ISCENEPARSER_HEADER__
#define __ISCENEPARSER_HEADER__

// Forward declare structs to contain parsed data.
struct SceneCameraData;
struct SceneNode;
struct SceneGlobalData;
struct SceneLightData;

// Interface for accessing parsed scenegraph data.
// Subclasses will have file format specific implementations.
class ISceneParser {
public:
    // On return, data will contain the camera data
    virtual bool getCameraData(SceneCameraData& data) const = 0;

    // Returns the number of lights in the scene
    virtual int getNumLights() const = 0;

    // On return, data will contain the information for the ith light.
    virtual bool getLightData(
        const int i, SceneLightData& data) const = 0;

    // On return data will contain the global scene data
    virtual bool getGlobalData(SceneGlobalData& data) const = 0;

    // Returns the root node of the scene graph
    virtual SceneNode* getRootNode() const = 0;

    virtual ~ISceneParser() {}

protected:
  ISceneParser() {}
};

#endif

