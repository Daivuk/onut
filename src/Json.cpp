#include <onut/Json.h>
#include <onut/Log.h>

#include <fstream>

namespace onut
{
    bool loadJson(Json::Value& out, const std::string& filename)
    {
        std::ifstream file(filename);
        if (!file.is_open())
        {
            OLogE("Failed to load file: " + filename);
            return false;
        }
        try
        {
            file >> out;
        }
        catch (...)
        {
            OLogE("Failed to parse file: " + filename);
            file.close();
            return false;
        }
        file.close();
        return true;
    }

    bool saveJson(const Json::Value &json, const std::string& filename)
    {
        std::ofstream file(filename);
        if (!file.is_open())
        {
            OLogE("Failed to save file: " + filename);
            return false;
        }

        Json::StyledWriter styledWriter;

        auto str = styledWriter.write(json);
        file << str;
        file.close();

        return true;
    }


    Json::Value serializeInt8(int8_t val)
    {
        return val;
    }

    Json::Value serializeUInt8(uint8_t val)
    {
        return val;
    }

    Json::Value serializeInt16(int16_t val)
    {
        return val;
    }

    Json::Value serializeUInt16(uint16_t val)
    {
        return val;
    }

    Json::Value serializeInt32(int32_t val)
    {
        return val;
    }

    Json::Value serializeUInt32(uint32_t val)
    {
        return val;
    }

    Json::Value serializeInt64(int64_t val)
    {
        return val;
    }

    Json::Value serializeUInt64(uint64_t val)
    {
        return val;
    }

    Json::Value serializeFloat(float val)
    {
        return val;
    }

    Json::Value serializeDouble(double val)
    {
        return val;
    }

    Json::Value serializeBool(bool val)
    {
        return val;
    }

    Json::Value serializeString(const std::string &val)
    {
        return val;
    }

    Json::Value serializeFloat2(const float *val)
    {
        Json::Value ret(Json::arrayValue);
        ret.append(val[0]);
        ret.append(val[1]);
        return ret;
    }

    Json::Value serializeFloat3(const float *val)
    {
        Json::Value ret(Json::arrayValue);
        ret.append(val[0]);
        ret.append(val[1]);
        ret.append(val[2]);
        return ret;
    }

    Json::Value serializeFloat4(const float *val)
    {
        Json::Value ret(Json::arrayValue);
        ret.append(val[0]);
        ret.append(val[1]);
        ret.append(val[2]);
        ret.append(val[3]);
        return ret;
    }

    Json::Value serializeInt2(const int *val)
    {
        Json::Value ret(Json::arrayValue);
        ret.append(val[0]);
        ret.append(val[1]);
        return ret;
    }

    Json::Value serializeInt4(const int *val)
    {
        Json::Value ret(Json::arrayValue);
        ret.append(val[0]);
        ret.append(val[1]);
        ret.append(val[2]);
        ret.append(val[3]);
        return ret;
    }

    Json::Value serializeMatrix(const float *val)
    {
        Json::Value ret(Json::arrayValue);
        for (int i = 0; i < 16; ++i)
            ret.append(val[i]);
        return ret;
    }

    Json::Value serializeStringArray(const std::vector<std::string> &val)
    {
        Json::Value ret(Json::arrayValue);
        for (const auto &str : val)
            ret.append(str);
        return ret;
    }

    int8_t deserializeInt8(const Json::Value &json, int8_t in_default)
    {
        if (!json.isInt()) return in_default;
        return (int8_t)json.asInt();
    }

    uint8_t deserializeUInt8(const Json::Value &json, uint8_t in_default)
    {
        if (!json.isUInt()) return in_default;
        return (uint8_t)json.asUInt();
    }

    int16_t deserializeInt16(const Json::Value &json, int16_t in_default)
    {
        if (!json.isInt()) return in_default;
        return (int16_t)json.asInt();
    }

    uint16_t deserializeUInt16(const Json::Value &json, uint16_t in_default)
    {
        if (!json.isUInt()) return in_default;
        return (uint16_t)json.asUInt();
    }

    int32_t deserializeInt32(const Json::Value &json, int32_t in_default)
    {
        if (!json.isInt()) return in_default;
        return (int32_t)json.asInt();
    }

    uint32_t deserializeUInt32(const Json::Value &json, uint32_t in_default)
    {
        if (!json.isUInt()) return in_default;
        return (uint32_t)json.asUInt();
    }

    int64_t deserializeInt64(const Json::Value &json, int64_t in_default)
    {
        if (!json.isInt64()) return in_default;
        return (int64_t)json.asInt64();
    }

    uint64_t deserializeUInt64(const Json::Value &json, uint64_t in_default)
    {
        if (!json.isUInt64()) return in_default;
        return (uint64_t)json.asUInt64();
    }

    float deserializeFloat(const Json::Value &json, float in_default)
    {
        if (!json.isDouble()) return in_default;
        return json.asFloat();
    }

    double deserializeDouble(const Json::Value &json, double in_default)
    {
        if (!json.isDouble()) return in_default;
        return json.asDouble();
    }

    bool deserializeBool(const Json::Value &json, bool in_default)
    {
        if (!json.isBool()) return in_default;
        return json.asBool();
    }

    std::string deserializeString(const Json::Value &json, const std::string &in_default)
    {
        if (!json.isString()) return in_default;
        return json.asString();
    }

    static const float DEFAULT_FLOATS[4]  = {0, 0, 0, 0};
    static const int   DEFAULT_INTS[4]    = {0, 0, 0, 0};
    static const float DEFAULT_MATRIX[16] = {1, 0, 0, 0,
                                                0, 1, 0, 0,
                                                0, 0, 1, 0,
                                                0, 0, 0, 1};

    void deserializeFloat2(float* out, const Json::Value &json, const float *in_default)
    {
        if (in_default == nullptr) in_default = DEFAULT_FLOATS;
        if (!json.isArray() || json.size() != 2)
        {
            memcpy(out, in_default, sizeof(float) * 2);
            return;
        }
        for (int i = 0; i < 2; ++i)
        {
            if (!json[i].isDouble())
            {
                memcpy(out, in_default, sizeof(float) * 2);
                return;
            }
            out[i] = json[i].asFloat();
        }
    }

    void deserializeFloat3(float* out, const Json::Value &json, const float *in_default)
    {
        if (in_default == nullptr) in_default = DEFAULT_FLOATS;
        if (!json.isArray() || json.size() != 3)
        {
            memcpy(out, in_default, sizeof(float) * 3);
            return;
        }
        for (int i = 0; i < 3; ++i)
        {
            if (!json[i].isDouble())
            {
                memcpy(out, in_default, sizeof(float) * 3);
                return;
            }
            out[i] = json[i].asFloat();
        }
    }

    void deserializeFloat4(float* out, const Json::Value &json, const float *in_default)
    {
        if (in_default == nullptr) in_default = DEFAULT_FLOATS;
        if (!json.isArray() || json.size() != 4)
        {
            memcpy(out, in_default, sizeof(float) * 4);
            return;
        }
        for (int i = 0; i < 4; ++i)
        {
            if (!json[i].isDouble())
            {
                memcpy(out, in_default, sizeof(float) * 4);
                return;
            }
            out[i] = json[i].asFloat();
        }
    }

    void deserializeInt2(int* out, const Json::Value &json, const int *in_default)
    {
        if (in_default == nullptr) in_default = DEFAULT_INTS;
        if (!json.isArray() || json.size() != 2)
        {
            memcpy(out, in_default, sizeof(int) * 2);
            return;
        }
        for (int i = 0; i < 2; ++i)
        {
            if (!json[i].isInt())
            {
                memcpy(out, in_default, sizeof(int) * 2);
                return;
            }
            out[i] = (int)json[i].asInt();
        }
    }

    void deserializeInt4(int* out, const Json::Value &json, const int *in_default)
    {
        if (in_default == nullptr) in_default = DEFAULT_INTS;
        if (!json.isArray() || json.size() != 4)
        {
            memcpy(out, in_default, sizeof(int) * 4);
            return;
        }
        for (int i = 0; i < 4; ++i)
        {
            if (!json[i].isInt())
            {
                memcpy(out, in_default, sizeof(float) * 4);
                return;
            }
            out[i] = (int)json[i].asInt();
        }
    }

    void deserializeMatrix(float* out, const Json::Value &json, const float *in_default)
    {
        if (in_default == nullptr) in_default = DEFAULT_MATRIX;
        if (!json.isArray() || json.size() != 16)
        {
            memcpy(out, in_default, sizeof(float) * 16);
            return;
        }
        for (int i = 0; i < 16; ++i)
        {
            if (!json[i].isDouble())
            {
                memcpy(out, in_default, sizeof(float) * 16);
                return;
            }
            out[i] = json[i].asFloat();
        }
    }

    std::vector<std::string> deserializeStringArray(const Json::Value &json, const std::vector<std::string> &in_default)
    {
        if (!json.isArray())
        {
            return in_default;
        }
        std::vector<std::string> ret;
        for (const auto &valJson : json)
        {
            if (valJson.isString())
            {
                ret.push_back(valJson.asString());
            }
            else
            {
                ret.push_back("");
            }
        }
        return ret;
    }

    Json::Value serializeJsonValue(const Vector2& val) { return serializeFloat2(&val.x); }
    Json::Value serializeJsonValue(const Vector3& val) { return serializeFloat3(&val.x); }
    Json::Value serializeJsonValue(const Vector4& val) { return serializeFloat4(&val.x); }
    Json::Value serializeJsonValue(const Point& val) { return serializeInt2(&val.x); }
    Json::Value serializeJsonValue(const iRect& val) { return serializeInt4(&val.left); }
    Json::Value serializeJsonValue(const Matrix& val) { return serializeMatrix(&val._11); }
    Json::Value serializeJsonValue(const std::vector<std::string> &val) { return serializeStringArray(val); }

    template<> Vector2 deserializeJsonValue<Vector2>(const Json::Value &json, const Vector2 &in_default) { Vector2 ret; deserializeFloat2(&ret.x, json, &in_default.x); return ret; }
    template<> Vector3 deserializeJsonValue<Vector3>(const Json::Value &json, const Vector3 &in_default) { Vector3 ret; deserializeFloat3(&ret.x, json, &in_default.x); return ret; }
    template<> Vector4 deserializeJsonValue<Vector4>(const Json::Value &json, const Vector4 &in_default) { Vector4 ret; deserializeFloat4(&ret.x, json, &in_default.x); return ret; }
    template<> Point deserializeJsonValue<Point>(const Json::Value &json, const Point &in_default) { Point ret; deserializeInt2(&ret.x, json, &in_default.x); return ret; }
    template<> iRect deserializeJsonValue<iRect>(const Json::Value &json, const iRect &in_default) { iRect ret; deserializeInt4(&ret.left, json, &in_default.left); return ret; }
    template<> Matrix deserializeJsonValue<Matrix>(const Json::Value &json, const Matrix &in_default) { Matrix ret; deserializeMatrix(&ret._11, json, &in_default._11); return ret; }
    template<> std::vector<std::string> deserializeJsonValue<std::vector<std::string>>(const Json::Value &json, const std::vector<std::string> &in_default) { return deserializeStringArray(json, in_default); }
}
