#pragma once
// STL
#include <string>

namespace onut
{
    enum class QuestionBoxAnswer
    {
        Yes,
        No,
        Cancel
    };

    QuestionBoxAnswer showQuestionBox(const std::string& caption, const std::string& message);
    void showMessageBox(const std::string& caption, const std::string& message);
}
