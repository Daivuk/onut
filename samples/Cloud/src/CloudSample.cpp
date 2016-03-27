// Required for WinMain
#include <Windows.h>

// Oak Nut include
#include <onut/Anim.h>
#include <onut/Cloud.h>
#include <onut/Crypto.h>
#include <onut/Input.h>
#include <onut/onut.h>
#include <onut/ParticleSystemManager.h>
#include <onut/Random.h>
#include <onut/Renderer.h>
#include <onut/Settings.h>
#include <onut/Sound.h>
#include <onut/SpriteBatch.h>
#include <onut/Texture.h>
#include <onut/Timer.h>
#include <onut/Timing.h>
#include <onut/UIContext.h>
#include <onut/UIControl.h>
#include <onut/UIImage.h>
#include <onut/UILabel.h>
#include <onut/UIPanel.h>
#include <onut/UITextBox.h>

void init();
void update();
void render();
void showLogin();
void showMainMenu();

OUIControlRef pCurrentUI;
OUIControlRef pAchievementPopup;
OUIControlRef pAchievementPopupCard;

enum GameState
{
    LoginPage,
    MainMenu,
    Game,
    GameOver
};

GameState gameState = GameState::LoginPage;

// Main
int CALLBACK WinMain(HINSTANCE appInstance, HINSTANCE prevInstance, LPSTR cmdLine, int cmdCount)
{
    // Those two show up on your game Core App Info on the BrainCloud portal.
    // Visit getbraincloud.com to register a game.
    oSettings->setAppId("10646");
    oSettings->setAppSecret("f4c4a424-c888-4641-af7d-4c2d51195f7d");

    oSettings->setGameName("Cloud Sample");
    ORun(init, update, render);
}

static void onQuitClicked(const OUIControlRef&, const onut::UIMouseEvent&)
{
    OQuit();
}

static void onLoginResult(const OCloud::LoginEvent& event)
{
    OFindUI("logingInDialog")->isVisible = false;
    if (!event.success)
    {
        OFindUI("errorDialog")->isVisible = true;
        return;
    }
    showMainMenu();
}

static void onCreateAccountResult(const OCloud::LoginEvent& event)
{
    OFindUI("logingInDialog")->isVisible = false;
    if (!event.success)
    {
        auto pErr = OFindUI<OUILabel>("lblCreateAccountErrorMsg");
        pErr->textComponent.text = event.errorMessage;
        pErr->isVisible = true;
        return;
    }
    showMainMenu();
}

static void onLoginClicked(const OUIControlRef&, const onut::UIMouseEvent&)
{
    auto email = OFindUI<OUITextBox>("txtEmail")->textComponent.text;
    auto password = OFindUI<OUITextBox>("txtPassword")->textComponent.text;

    if (email.empty() || password.empty())
    {
        OFindUI("errorDialog")->isVisible = true;
        return;
    }

    oSettings->setUserSetting("email", email);
    oSettings->setUserSetting("password", password); // Plain text, not very safe. For example's sake

    // Proceed with login
    OFindUI("logingInDialog")->isVisible = true;
    oCloud->login(email, password, onLoginResult);
}

static void onCreateAccountClicked(const OUIControlRef&, const onut::UIMouseEvent&)
{
    OFindUI("createAccountDialog")->isVisible = true;
    OFindUI("lblCreateAccountErrorMsg")->isVisible = false;
}

static void onCreateAccountCancelClicked(const OUIControlRef&, const onut::UIMouseEvent&)
{
    OFindUI("createAccountDialog")->isVisible = false;
}

static void onCreateAccountCreateClicked(const OUIControlRef&, const onut::UIMouseEvent&)
{
    auto username = OFindUI<OUITextBox>("txtCreateAccountUsername")->textComponent.text;
    auto email = OFindUI<OUITextBox>("txtCreateAccountEmail")->textComponent.text;
    auto password = OFindUI<OUITextBox>("txtCreateAccountPassword")->textComponent.text;
    auto passwordRepeat = OFindUI<OUITextBox>("txtCreateAccountPasswordRepeat")->textComponent.text;

    if (username.empty() || email.empty() || password.empty() || passwordRepeat.empty())
    {
        auto pErr = OFindUI<OUILabel>("lblCreateAccountErrorMsg");
        pErr->textComponent.text = "Please fill all the fields.";
        pErr->isVisible = true;
        return;
    }

    if (password != passwordRepeat)
    {
        auto pErr = OFindUI<OUILabel>("lblCreateAccountErrorMsg");
        pErr->textComponent.text = "Passwords must match.";
        pErr->isVisible = true;
        return;
    }

    if (!OValidateEmail(email))
    {
        auto pErr = OFindUI<OUILabel>("lblCreateAccountErrorMsg");
        pErr->textComponent.text = "Email appears to be invalid.";
        pErr->isVisible = true;
        return;
    }

    OFindUI("logingInDialog")->isVisible = true;
    oCloud->createAccount(username, email, password, onCreateAccountResult);
}

static void onCloseErrorDialogClicked(const OUIControlRef&, const onut::UIMouseEvent&)
{
    OFindUI("errorDialog")->isVisible = false;
}

static void onAchievementsClicked(const OUIControlRef&, const onut::UIMouseEvent&)
{
    auto pAchievementDialog = OFindUI("achievementsDialog");
    auto pAchievementsContainer = pAchievementDialog->getChild("achievementsContainer");
    auto pTemplate = pAchievementsContainer->getChild("achievementCardTemplate");
    auto pSpinner = pAchievementDialog->getChild("spinner");

    pAchievementsContainer->removeAll();
    pAchievementsContainer->add(pTemplate);

    pSpinner->isVisible = true;
    pAchievementDialog->isVisible = true;

    oCloud->getAchievements([=](const OCloud::Achievements& achievements)
    {
        float yPos = pTemplate->rect.y;
        for (auto& achievement : achievements)
        {
            auto pCard = pTemplate->copy();
            pCard->rect.y = yPos;
            pCard->getChild<OUIImage>("imgIcon")->scale9Component.image.pTexture = achievement.iconTexture;
            pCard->getChild<OUILabel>("lblTitle")->textComponent.text = achievement.title;
            pCard->getChild<OUILabel>("lblDescription")->textComponent.text = achievement.description;
            pCard->isVisible = true;
            if (achievement.achieved)
            {
                pCard->getChild<OUIImage>("enable")->isEnabled = true;
            }
            pAchievementsContainer->add(pCard);
            yPos += pCard->rect.w;
        }
        pSpinner->isVisible = false;
    });
}

static void onAchievementsCloseClicked(const OUIControlRef&, const onut::UIMouseEvent&)
{
    OFindUI("achievementsDialog")->isVisible = false;
}

static void onLeaderboardsClicked(const OUIControlRef&, const onut::UIMouseEvent&)
{
    auto pLeaderboardsDialog = OFindUI("leaderboardsDialog");
    auto pLeaderboardsContainer = pLeaderboardsDialog->getChild("leaderboardsContainer");
    std::vector<OUIControlRef> entries = 
    {
        pLeaderboardsContainer->getChild("entry0"),
        pLeaderboardsContainer->getChild("entry1"),
        pLeaderboardsContainer->getChild("entry2"),
        pLeaderboardsContainer->getChild("entry3"),
        pLeaderboardsContainer->getChild("entry4"),
        pLeaderboardsContainer->getChild("entry5"),
        pLeaderboardsContainer->getChild("entry6"),
        pLeaderboardsContainer->getChild("entry7"),
    };
    auto pSpinner = pLeaderboardsDialog->getChild("spinner");

    pLeaderboardsDialog->isVisible = true;
    pSpinner->isVisible = true;
    pLeaderboardsContainer->isVisible = false;

    oCloud->getLeaderboard("score", 8, [=](const OCloud::Leaderboard& leaderboard)
    {
        for (size_t i = 0; i < entries.size(); ++i)
        {
            auto& pUIEntry = entries[i];
            if (i < leaderboard.size())
            {
                pUIEntry->isVisible = true;
                pUIEntry->getChild<OUILabel>("lblPlayer")->textComponent.text = leaderboard[i].name;
                pUIEntry->getChild<OUILabel>("lblScore")->textComponent.text = std::to_string(leaderboard[i].score);
            }
            else
            {
                pUIEntry->isVisible = false;
            }
        }
        pSpinner->isVisible = false;
        pLeaderboardsContainer->isVisible = true;
    });
}

static void onLeaderboardsCloseClicked(const OUIControlRef&, const onut::UIMouseEvent&)
{
    OFindUI("leaderboardsDialog")->isVisible = false;
}

static void onSignOut(const OUIControlRef&, const onut::UIMouseEvent&)
{
    oUI->removeAll();
    oCloud->logout([](const OCloud::LoginEvent& event)
    {
        showLogin();
    });
}

static void onMainMenuClicked(const OUIControlRef&, const onut::UIMouseEvent&)
{
    showMainMenu();
}

struct Monster
{
    Vector2 position;
    Vector2 velocity;
};

struct Coin
{
    using Life = float;

    Vector2 position;
    Vector2 velocity;
    Life life = 1.f;
};

using Monsters = std::vector<Monster>;
using Coins = std::vector<Coin>;
using CoinCount = int;
using SpawnCount = int;
using PlayerLife = int;

Monsters monsters;
Coins coins;
OTimer monsterSpawnTimer;
CoinCount coinCollected;
PlayerLife playerLife = 0;
OTimer gameEventTimer;
SpawnCount spawnCount = 0;

static const auto MONSTER_SPAWN_RATE = std::chrono::milliseconds(400);
static const auto MAX_SPAWN_COUNT = 30000 / 400; // 30 sec

static void spawnMonster()
{
    Monster monster;
    monster.position.x = ORandFloat(32, OScreenWf - 32);
    monster.position.y = -32;
    monster.velocity.x = ORandFloat(-400.f, 400.f);
    monster.velocity.y = ORandFloat(200.f, 500.f);
    monsters.push_back(monster);
    ++spawnCount;
    if (spawnCount < MAX_SPAWN_COUNT)
    {
        monsterSpawnTimer.start(MONSTER_SPAWN_RATE, spawnMonster);
    }
}

static void onPlay(const OUIControlRef&, const onut::UIMouseEvent&)
{
    gameState = GameState::Game;
    if (pCurrentUI) pCurrentUI->remove();
    pCurrentUI = nullptr;
    oUI->add(pAchievementPopup); // Making sure achievement popup is always on top of everything

    monsters.clear();
    coins.clear();
    spawnCount = 0;
    coinCollected = 0;
    playerLife = 3;
    monsterSpawnTimer.start(MONSTER_SPAWN_RATE, spawnMonster);
}

OAnimFloat achievementPopupAnim = 0.f;

static void onAchievementAwarded(const OCloud::Achievement& achievement)
{
    pAchievementPopup->getChild<OUIImage>("imgIcon")->scale9Component.image.pTexture = achievement.iconTexture;
    pAchievementPopup->getChild<OUILabel>("lblTitle")->textComponent.text = achievement.title;
    pAchievementPopup->getChild<OUILabel>("lblDescription")->textComponent.text = achievement.description;

    achievementPopupAnim = pAchievementPopupCard->rect.z;
    achievementPopupAnim.queue(0.f, .3f, OTweenEaseOut);
    achievementPopupAnim.queue(0.f, 3.f);
    achievementPopupAnim.queue(pAchievementPopupCard->rect.z, .3f, OTweenEaseOut);
    achievementPopupAnim.play();
}

void init()
{
    // Add a style for the spinner
    oUIContext->addStyle<OUIPanel>("spinner", [](const OUIPanelRef& pControl, const Rect& rect)
    {
        // Use the user data to store the animation
        auto anim = reinterpret_cast<uintptr_t>(pControl->pUserData);
        auto animf = static_cast<float>(anim) / 100.f;
        animf += oTiming->getRenderDeltaTime() * 360.f;
        anim = static_cast<uintptr_t>(animf * 100.f);
        pControl->pUserData = reinterpret_cast<void*>(anim);

        // Draw the spinner
        auto pNut = OGetTexture("onutLogo.png");
        oSpriteBatch->drawSprite(pNut, rect.Center(), Color::White, animf, .5f);
    });

    pAchievementPopup = OLoadUI("achievementPopup.json");
    pAchievementPopup->isClickThrough = true;
    pAchievementPopupCard = pAchievementPopup->getChild("card");
    achievementPopupAnim = pAchievementPopupCard->rect.z;

    oCloud->setAchievementCallback(onAchievementAwarded);

    showLogin();
}

static void showLogin()
{
    gameState = GameState::LoginPage;

    // Show the login dialog
    if (pCurrentUI) pCurrentUI->remove();
    pCurrentUI = OLoadUI("cloud.json");
    oUI->add(pAchievementPopup); // Making sure achievement popup is always on top of everything

    OFindUI<OUITextBox>("txtEmail")->textComponent.text = oSettings->getUserSetting("email");
    OFindUI<OUITextBox>("txtPassword")->textComponent.text = oSettings->getUserSetting("password");

    // Bind actions
    OFindUI("btnQuit")->onClick = onQuitClicked;
    OFindUI("btnLogin")->onClick = onLoginClicked;
    OFindUI("btnCloseErrorDialog")->onClick = onCloseErrorDialogClicked;
    OFindUI("btnCreateAccount")->onClick = onCreateAccountClicked;
    OFindUI("btnCreateAccountCancel")->onClick = onCreateAccountCancelClicked;
    OFindUI("btnCreateAccountCreate")->onClick = onCreateAccountCreateClicked;
}

static void showMainMenu()
{
    gameState = GameState::MainMenu;

    if (pCurrentUI) pCurrentUI->remove();
    pCurrentUI = OLoadUI("mainMenu.json");
    oUI->add(pAchievementPopup); // Making sure achievement popup is always on top of everything

    OFindUI<OUILabel>("lblWelcome")->textComponent.text = "Welcome ^999" + oCloud->getUsername();
    OFindUI<OUILabel>("lblCoins")->textComponent.text = std::to_string(oCloud->getStat("coins_collected"));

    // Bind actions
    OFindUI("btnQuit")->onClick = onQuitClicked;
    OFindUI("btnAchievements")->onClick = onAchievementsClicked;
    OFindUI("btnCloseAchievements")->onClick = onAchievementsCloseClicked;
    OFindUI("btnLeaderboards")->onClick = onLeaderboardsClicked;
    OFindUI("btnCloseLeaderboards")->onClick = onLeaderboardsCloseClicked;
    OFindUI("btnSignOut")->onClick = onSignOut;
    OFindUI("btnPlay")->onClick = onPlay;
}

static void onShowGameOver()
{
    gameState = GameState::GameOver;

    if (pCurrentUI) pCurrentUI->remove();
    pCurrentUI = OLoadUI("gameOver.json");
    oUI->add(pAchievementPopup); // Making sure achievement popup is always on top of everything

    oCloud->postScore("score", static_cast<OCloud::LeaderboardEntry::Score>(coinCollected));
    OFindUI<OUILabel>("lblCoins")->textComponent.text = std::to_string(coinCollected);
    OFindUI<OUILabel>("lblGameOver")->textComponent.text = playerLife > 0 ? "VICTORY!" : "GAME OVER...";

    // Bind actions
    OFindUI("btnQuit")->onClick = onQuitClicked;
    OFindUI("btnMainMenu")->onClick = onMainMenuClicked;
}

void update()
{
    pAchievementPopupCard->rect.x = achievementPopupAnim;

    if (gameState == GameState::Game)
    {
        if (playerLife > 0)
        {
            // Check if end of game
            if (monsters.empty() && coins.empty() && spawnCount == MAX_SPAWN_COUNT)
            {
                OPlaySound("win.wav");
                oCloud->sendEvent("complete_game");
                onShowGameOver();
            }

            if (OInputJustPressed(OMouse1))
            {
                bool clickConsumed = false;
                for (auto it = monsters.begin(); it != monsters.end();)
                {
                    auto& monster = *it;
                    if (Vector2::Distance(oInput->mousePosf, monster.position) < 64)
                    {
                        OEmitParticles("monsterDie.pfx", monster.position);
                        OPlaySound("monsterDie.wav");
                        Coin coin;
                        coin.position = monster.position;
                        coin.velocity = monster.velocity;
                        coins.push_back(coin);
                        monsters.erase(it);
                        clickConsumed = true;
                        break;
                    }
                    ++it;
                }
                if (!clickConsumed)
                {
                    for (auto it = coins.begin(); it != coins.end();)
                    {
                        auto& coin = *it;
                        if (Vector2::Distance(oInput->mousePosf, coin.position) < 64)
                        {
                            ++coinCollected;
                            OPlaySound("coin.wav");
                            coins.erase(it);
                            oCloud->sendEvent("collect_coin");
                            break;
                        }
                        ++it;
                    }
                }
            }

            for (auto it = monsters.begin(); it != monsters.end();)
            {
                auto& monster = *it;
                monster.position += monster.velocity * ODT;
                if (monster.position.y >= OScreenHf)
                {
                    // Hit player
                    OEmitParticles("monsterDie.pfx", monster.position);
                    OPlaySound("monsterDie.wav");
                    it = monsters.erase(it);
                    --playerLife;
                    if (!playerLife)
                    {
                        OPlaySound("lose.wav");
                        gameEventTimer.start(3, onShowGameOver);
                    }
                    continue;
                }
                // Bounce against screen sides
                if ((monster.position.x <= 32 && monster.velocity.x < 0) ||
                    (monster.position.x >= OScreenWf - 32 && monster.velocity.x > 0))
                {
                    monster.velocity.x = -monster.velocity.x;
                }
                ++it;
            }

            for (auto it = coins.begin(); it != coins.end();)
            {
                auto& coin = *it;
                coin.position += coin.velocity * ODT;
                coin.velocity *= 1.0f - ODT * 4.f; // Slow down
                coin.life -= ODT;
                if (coin.life < 0.f)
                {
                    it = coins.erase(it);
                    continue;
                }
                // Bounce against screen edges
                if ((coin.position.x <= 32 && coin.velocity.x < 0) ||
                    (coin.position.x >= OScreenWf - 32 && coin.velocity.x > 0))
                {
                    coin.velocity.x = -coin.velocity.x;
                }
                if ((coin.position.y <= 32 && coin.velocity.y < 0) ||
                    (coin.position.y >= OScreenHf - 32 && coin.velocity.y > 0))
                {
                    coin.velocity.y = -coin.velocity.y;
                }
                ++it;
            }
        }
    }
}

void render()
{
    // Clear
    oRenderer->clear(OColorHex(1d232d));

    if (gameState == GameState::Game)
    {
        auto pMonsterTexture = OGetTexture("monster.png");
        auto pCoinTexture = OGetTexture("coin.png");
        auto pHeartTexture = OGetTexture("heart.png");

        oSpriteBatch->begin();
        oSpriteBatch->changeFiltering(OFilterNearest);
        oSpriteBatch->changeBlendMode(OBlendAlpha);

        // Map items
        for (auto& coin : coins)
        {
            oSpriteBatch->drawSprite(pCoinTexture, coin.position, Color::White, 0.f, .5f);
        }
        for (auto& monster : monsters)
        {
            oSpriteBatch->drawSprite(pMonsterTexture, monster.position);
        }

        // HUD
        for (auto i = 0; i < playerLife; ++i)
        {
            oSpriteBatch->drawSprite(pHeartTexture, Vector2(48.f + static_cast<float>(i * 64), 48.f), Color::White, 0.f, .75f);
        }

        oSpriteBatch->end();
    }
}
