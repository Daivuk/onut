// Onut
#include <onut/Dialogs.h>

// Third party
#if defined(WIN32)
#include <Windows.h>
#endif

namespace onut
{
#if defined(WIN32)
    QuestionBoxAnswer showQuestionBox(const std::string& caption, const std::string& message)
    {
        auto ret = MessageBoxA(nullptr, message.c_str(), caption.c_str(), MB_YESNOCANCEL);
        switch (ret)
        {
            case IDYES:
                return QuestionBoxAnswer::Yes;
            case IDNO:
                return QuestionBoxAnswer::No;
            default:
                return QuestionBoxAnswer::Cancel;
        }
    }

    void showMessageBox(const std::string& caption, const std::string& message)
    {
        MessageBoxA(nullptr, message.c_str(), caption.c_str(), MB_OK);
    }
#endif
}
