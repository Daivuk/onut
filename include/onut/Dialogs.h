#ifndef DIALOGS_H_INCLUDED
#define DIALOGS_H_INCLUDED

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

#endif
