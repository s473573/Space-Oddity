#include <Scene.h>
using namespace std;
using namespace so;
using namespace rapidjson;
using namespace glm;

static inline vec3 parseVec(GenericArray<false, Value> array)
{
    return {array[0].GetFloat(), array[1].GetFloat(), array[2].GetFloat()};
}
static inline vec4 parseVec4(GenericArray<false, Value> array)
{
    return {array[0].GetFloat(), array[1].GetFloat(),
            array[2].GetFloat(), array[3].GetFloat()};
}

static inline Transform parseTransform(GenericObject<false, Value> object)
{
    vec3 position = parseVec(object["position"].GetArray());
    vec3 rotation = parseVec(object["rotation"].GetArray());
    vec3 scale = parseVec(object["scale"].GetArray());
    return Transform(position, rotation, scale);
}

static inline Material parseMaterial(GenericObject<false, Value> object)
{
    vec4 ambient = parseVec4(object["ambient"].GetArray());
    vec4 diffuse = parseVec4(object["diffuse"].GetArray());
    vec4 specular = parseVec4(object["specular"].GetArray());
    float shininess = object["shininess"].GetFloat();
    return Material(ambient, diffuse, specular, shininess);
}
static inline vector<Material> parseMaterials(GenericArray<false, Value> array)
{
    vector<Material> materials;
    for (unsigned i = 0; i < array.Size(); ++i)
    {
        materials.push_back(parseMaterial(array[i].GetObject()));
    }
    return materials;
}

static inline Mesh parseMesh(GenericObject<false, Value> object)
{
    const char *path = object["path"].GetString();
    const char *texturePath = nullptr;
    if (object.HasMember("texture"))
    {
        texturePath = object["texture"].GetString();
    }
    if (object.HasMember("color"))
    {
        vec4 color = parseVec4(object["color"].GetArray());
        return Mesh(path, color, texturePath);
    }
    else
    {
        return Mesh(path, texturePath);
    }
}
static inline vector<shared_ptr<Mesh>> parseMeshes(GenericArray<false, Value> array)
{
    vector<shared_ptr<Mesh>> meshes;
    for (unsigned i = 0; i < array.Size(); ++i)
    {
        Mesh mesh = parseMesh(array[i].GetObject());
        shared_ptr<Mesh> meshPtr(new Mesh(mesh));
        meshes.push_back(meshPtr);
    }
    return meshes;
}

static inline void parseCamera(GenericObject<false, Value> object, Camera &camera)
{
    camera.setFOV(object["fov"].GetFloat(), false);
    camera.transform = parseTransform(object["transform"].GetObject());
}

// vector<shared_ptr<Mesh>> parseAnimation(GenericObject<false, Value> object)
// {
//     // TODO: Implement animation parsing
//     return vector<shared_ptr<Mesh>>();
// }
// 
// static inline vector<vector<shared_ptr<Mesh>>> parseAnimations(GenericArray<false, Value> array)
// {
//     return vector<vector<shader_ptr<Mesh>>>();
// }

// static inline vector<GameObject *> parseGameObjects(GenericArray<false, Value> array,
//                                                     const vector<shared_ptr<Mesh>> &meshes,
//                                                     const vector<Material> &materials,
//                                                     const std::vector<std::vector<std::shared_ptr<aw::Mesh>>> &animations,
//                                                     vec2 mapMinLimit,
//                                                     vec2 mapMaxLimit)
// {
//     return vector<GameObject>();
// }
// 
// static inline vector<Light> parseLights(GenericArray<false, Value> array, const vector<GameObject *> gameObjects)
// {
//     return vector<Light>();
// }

Scene::Scene(const char *path) : camera(45.0f)
{
    fstream sceneStream(path, ios::ate | ios::in);
    vector<char> fileData;
    if (sceneStream.is_open())
    {
        fileData.resize(sceneStream.tellg());
        sceneStream.seekg(0, ios::beg);
        sceneStream.read(fileData.data(), fileData.size());
        sceneStream.flush();
        sceneStream.close();
    }
    else
    {
        throw runtime_error("LEVEL JSON file was not found");
    }
    assert(glGetError() == 0);
    Document json;
    json.Parse(fileData.data(), fileData.size());
    parseCamera(json["camera"].GetObject(), camera);
    auto materials = parseMaterials(json["materials"].GetArray());
    auto meshes = parseMeshes(json["meshes"].GetArray());
    // auto animations = parseAnimations(json["animations"].GetArray());
    auto meshesNAnimations = meshes;
    // for (int i = 0; i < animations.size(); ++i)
    // {
    //     for (int j = 0; j < animations[i].size(); ++j)
    //     {
    //         meshesNAnimations.push_back(animations[i][j]);
    //     }
    // }
    // if (json.HasMember("skybox"))
    // {
    //     skybox = Skybox(&camera, json["skybox"].GetString());
    //     //meshesNAnimations.push_back(skybox.getMesh());
    // }
    Mesh::constructVAO(meshesNAnimations);
    // auto mapMinLimitData = json["mapMinLimit"].GetArray();
    // auto mapMaxLimitData = json["mapMaxLimit"].GetArray();
    // vec2 mapMinLimit = {mapMinLimitData[0].GetFloat(), mapMinLimitData[1].GetFloat()};
    // vec2 mapMaxLimit = {mapMaxLimitData[0].GetFloat(), mapMaxLimitData[1].GetFloat()};
    // gameObjects = parseGameObjects(json["gameobjects"].GetArray(), meshes, materials,
    //                                animations, mapMinLimit, mapMaxLimit);
    // lights = parseLights(json["lights"].GetArray(), gameObjects);
    // Light::constructUniformBuffer(lights);
}
Scene::~Scene()
{
    // for (unsigned i = 0; i < gameObjects.size(); ++i)
    // {
    //     delete gameObjects[i];
    // }
}
void Scene::destroyGameObject(int index)
{
    // delete gameObjects[index];
    // gameObjects.erase(gameObjects.begin() + index);
}