#pragma once
#include <string>

namespace ProEngine
{
    struct MaterialNodeType
    {
        const char* node_name;
        std::vector<std::string> inputs;
        std::vector<std::string> outputs;

        explicit MaterialNodeType(const char* name, std::vector<std::string> in = {}, std::vector<std::string> out = {})
            : node_name(name), inputs(std::move(in)), outputs(std::move(out))
        {
        }
    };

    struct Texture2DNode : public MaterialNodeType
    {
        Texture2DNode(): MaterialNodeType("Texture2D", {}, {"Color"})
        {
        }
    };

    struct MultiplyNode : public MaterialNodeType
    {
        MultiplyNode(): MaterialNodeType("Multiply", {"A", "B"}, {"Result"})
        {
        }
    };

    struct LitMasterNode : public MaterialNodeType
    {
        LitMasterNode()
            : MaterialNodeType("LitMaster", {"BaseColor"}, {})
        {
        }
    };
}
