// Required for WinMain
#include <Windows.h>

// Oak Nut include
#include <onut/Crypto.h>
#include <onut/Font.h>
#include <onut/onut.h>
#include <onut/Settings.h>
#include <onut/SpriteBatch.h>
#include <onut/Renderer.h>

void init();
void render();

std::string originalText = "One does nut simply";
std::string based64;
uint32_t hash;
std::string sha1;
std::vector<std::pair<std::string, bool>> emails = {
    {"daivuk@gmail.com", false},
    {"email@example.com", false},
    {"firstname.lastname@example.com", false},
    {"email@subdomain.example.com", false},
    {"firstname+lastname@example.com", false},
    {"email@123.123.123.123", false},
    {"email@[123.123.123.123]", false},
    {"\"email\"@example.com", false},
    {"1234567890@example.com", false},
    {"email@example-one.com", false},
    {"_______@example.com", false},
    {"email@example.name", false},
    {"email@example.museum", false},
    {"email@example.co.jp", false},
    {"firstname-lastname@example.com", false},
    {"plainaddress", false},
    {"#@%%#$@#$@#.com", false},
    {"@example.com", false},
    {"Joe Smith <email@example.com>", false},
    {"email.example.com", false},
    {"email@example@example.com", false},
    {".email@example.com", false},
    {"email.@example.com", false},
    {"email..email@example.com", false},
    {"email@example.com (Joe Smith)", false},
    {"email@example", false},
    {"email@-example.com", false},
    {"email@example.web", false},
    {"email@111.222.333.44444", false},
    {"email@example..com", false},
    {"Abc..123@example.com", false}
};

// Main
int CALLBACK WinMain(HINSTANCE appInstance, HINSTANCE prevInstance, LPSTR cmdLine, int cmdCount)
{
    oSettings->setGameName("Crypto Sample");
    ORun(init, nullptr, render);
}

void init()
{
    // Base 64
    based64 = OEncodeBase64(reinterpret_cast<const uint8_t*>(originalText.c_str()), originalText.size());

    // Simple hash
    hash = OHash(originalText);

    // Sha1
    sha1 = OSha1(originalText);

    // Validate emails
    for (auto& email : emails)
    {
        email.second = OValidateEmail(email.first);
    }
}

void render()
{
    // Clear
    oRenderer->clear(OColorHex(1d232d));

    // Draw Info
    auto pFont = OGetFont("font.fnt");

    oSpriteBatch->begin();

    pFont->draw("Original: ", {10, 10});
    pFont->draw(originalText, {200, 10}, OTopLeft, Color(.7f, 1.f));

    pFont->draw("Simple Hash: ", {10, 30});
    pFont->draw(std::to_string(hash), {200, 30}, OTopLeft, Color(.7f, 1.f));

    pFont->draw("Sha1: ", {10, 50});
    pFont->draw(sha1, {200, 50}, OTopLeft, Color(.7f, 1.f));

    pFont->draw("Shitty email validation: ", {10, 70});
    Vector2 pos(200, 70);
    for (auto& email : emails)
    {
        if (email.second)
        {
            pFont->draw(email.first, pos, OTopLeft, Color(0, 1, 0, 1.f));
        }
        else
        {
            pFont->draw(email.first, pos, OTopLeft, Color(1, 0, 0, 1.f));
        }
        pos.y += 12;
    }

    oSpriteBatch->end();
}
