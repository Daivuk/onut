#ifndef JSON_H_INCLUDED
#define JSON_H_INCLUDED

#include <onut/iRect.h>
#include <onut/Point.h>
#include <onut/Rect.h>
#include <onut/Vector2.h>
#include <onut/Vector3.h>
#include <onut/Vector4.h>
#include <onut/Matrix.h>

#include <json/json.h>

#include <string>

namespace onut
{
    bool loadJson(Json::Value &out, const std::string& filename);
    bool saveJson(const Json::Value &json, const std::string& filename);

    Json::Value serializeInt8(int8_t val);
    Json::Value serializeUInt8(uint8_t val);
    Json::Value serializeInt16(int16_t val);
    Json::Value serializeUInt16(uint16_t val);
    Json::Value serializeInt32(int32_t val);
    Json::Value serializeUInt32(uint32_t val);
    Json::Value serializeInt64(int64_t val);
    Json::Value serializeUInt64(uint64_t val);
    Json::Value serializeFloat(float val);
    Json::Value serializeDouble(double val);
    Json::Value serializeBool(bool val);
    Json::Value serializeString(const std::string &val);
    Json::Value serializeFloat2(const float *val);
    Json::Value serializeFloat3(const float *val);
    Json::Value serializeFloat4(const float *val);
    Json::Value serializeInt2(const int *val);
    Json::Value serializeInt4(const int *val);
    Json::Value serializeMatrix(const float *val);
    Json::Value serializeStringArray(const std::vector<std::string> &val);

    int8_t deserializeInt8(const Json::Value &json, int8_t in_default = 0);
    uint8_t deserializeUInt8(const Json::Value &json, uint8_t in_default = 0);
    int16_t deserializeInt16(const Json::Value &json, int16_t in_default = 0);
    uint16_t deserializeUInt16(const Json::Value &json, uint16_t in_default = 0);
    int32_t deserializeInt32(const Json::Value &json, int32_t in_default = 0);
    uint32_t deserializeUInt32(const Json::Value &json, uint32_t in_default = 0);
    int64_t deserializeInt64(const Json::Value &json, int64_t in_default = 0);
    uint64_t deserializeUInt64(const Json::Value &json, uint64_t in_default = 0);
    float deserializeFloat(const Json::Value &json, float in_default = 0.0f);
    double deserializeDouble(const Json::Value &json, double in_default = 0.0);
    bool deserializeBool(const Json::Value &json, bool in_default = false);
    std::string deserializeString(const Json::Value &json, const std::string &in_default = "");
    void deserializeFloat2(float* out, const Json::Value &json, const float *in_default = nullptr);
    void deserializeFloat3(float* out, const Json::Value &json, const float *in_default = nullptr);
    void deserializeFloat4(float* out, const Json::Value &json, const float *in_default = nullptr);
    void deserializeInt2(int* out, const Json::Value &json, const int *in_default = nullptr);
    void deserializeInt4(int* out, const Json::Value &json, const int *in_default = nullptr);
    void deserializeMatrix(float* out, const Json::Value &json, const float *in_default = nullptr); // Null = identity
    std::vector<std::string> deserializeStringArray(const Json::Value &json, const std::vector<std::string> &in_default = {});
        
    Json::Value serializeJsonValue(const Vector2& val);
    Json::Value serializeJsonValue(const Vector3& val);
    Json::Value serializeJsonValue(const Vector4& val);
    Json::Value serializeJsonValue(const Point& val);
    Json::Value serializeJsonValue(const iRect& val);
    Json::Value serializeJsonValue(const Matrix& val);
    Json::Value serializeJsonValue(const std::vector<std::string> &val);

    template<typename T> T deserializeJsonValue(const Json::Value &json, const T &in_default);
}

#endif
