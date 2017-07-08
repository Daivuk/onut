#include <onut/ContentManager.h>
#include <onut/Files.h>
#include <onut/onut.h>
#include <onut/Settings.h>
#include <onut/Strings.h>
#include <onut/Timing.h>

#include <fstream>

void initSettings()
{
    std::string path = ".";
    if (!OArguments.empty()) path = OArguments.back();

    path = onut::getPath(path);
    auto workingDirectory = path;

    // Set content manager path
    oContentManager = OContentManager::create();
    oContentManager->clearSearchPaths();
    oContentManager->addSearchPath(workingDirectory);

    // Load settings
    auto settingsFile = onut::findFile("settings.txt", workingDirectory);
    if (!settingsFile.empty())
    {
        std::ifstream fic(settingsFile);
        std::string line;
        while (!fic.eof())
        {
            std::getline(fic, line);
            if (line.empty()) continue;
            auto split = onut::splitString(line, '=', false);
            if (split.size() != 2) continue;
            auto what = onut::toUpper(onut::trim(split[0]));
            auto value = onut::trim(split[1]);
            if (what == "RESOLUTION")
            {
                auto split2 = onut::splitString(value, ",x \t\n\r");
                if (split2.size() != 2) continue;
                Point res;
                try
                {
                    res.x = std::stoi(split2[0]);
                    res.y = std::stoi(split2[1]);
                    oSettings->setResolution(res);
                }
                catch (...) {}
            }
            else if (what == "RETRORESOLUTION")
            {
                auto split2 = onut::splitString(value, ",x \t\n\r");
                if (split2.size() != 2) continue;
                Point res;
                try
                {
                    res.x = std::stoi(split2[0]);
                    res.y = std::stoi(split2[1]);
                    oSettings->setRetroResolution(res);
                }
                catch (...) {}
            }
            else if (what == "RETROMODE")
            {
                value = onut::toUpper(value);
                oSettings->setIsRetroMode(value == "TRUE" || value == "1" || value == "ON");
            }
            else if (what == "GAMENAME")
            {
                oSettings->setGameName(value);
            }
            else if (what == "RESIZABLEWINDOW")
            {
                value = onut::toUpper(value);
                oSettings->setIsResizableWindow(value == "TRUE" || value == "1" || value == "ON");
            }
            else if (what == "FULLSCREEN")
            {
                value = onut::toUpper(value);
                oSettings->setBorderlessFullscreen(value == "TRUE" || value == "1" || value == "ON");
            }
            else if (what == "SHOWFPS")
            {
                value = onut::toUpper(value);
                oSettings->setShowFPS(value == "TRUE" || value == "1" || value == "ON");
            }
            else if (what == "FIXEDSTEP")
            {
                value = onut::toUpper(value);
                oSettings->setIsFixedStep(value == "TRUE" || value == "1" || value == "ON");
            }
            else if (what == "UPDATEFPS")
            {
                try
                {
                    auto fps = std::stoi(value);
                    oTiming = OTiming::create((uint32_t)fps);
                }
                catch (...) {}
            }
            else if (what == "APPID")
            {
                oSettings->setAppId(value);
            }
            else if (what == "APPSECRET")
            {
                oSettings->setAppSecret(value);
            }
            else if (what == "ONSCREENLOG")
            {
                value = onut::toUpper(value);
                oSettings->setShowOnScreenLog(value == "TRUE" || value == "1" || value == "ON");
            }
            else if (what == "EDITORMODE")
            {
                value = onut::toUpper(value);
                oSettings->setIsEditorMode(value == "TRUE" || value == "1" || value == "ON");
            }
        }
        fic.close();
    }
}

void init()
{
}

void update()
{
}

void render()
{
}

void postRender()
{
}
