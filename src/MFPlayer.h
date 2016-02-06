#if defined(WIN32)
#pragma once

#include "player.h"

#include <Mfmediaengine.h>

namespace onut
{
    class MFPlayer;

    class MFPlayerNotify final : public IMFMediaEngineNotify
    {
    public:
        MFPlayerNotify(MFPlayer* pPlayer);

        HRESULT STDMETHODCALLTYPE QueryInterface(REFIID riid, _COM_Outptr_ void __RPC_FAR *__RPC_FAR *ppvObject) override;
        ULONG STDMETHODCALLTYPE AddRef(void) override;
        ULONG STDMETHODCALLTYPE Release(void) override;
        HRESULT STDMETHODCALLTYPE EventNotify(_In_ DWORD event, _In_ DWORD_PTR param1, _In_  DWORD param2) override;

    private:
        ULONG m_ref = 1;
        MFPlayer* m_pPlayer = nullptr;
    };

    class MFPlayer final : public Player
    {
    public:
        ~MFPlayer();

        void init(HWND handle) override;
        void init(Texture* pRenderTarget) override;
        bool isPlaying() const override;
        void pause() override;
        void play() override;
        void setLoop(bool bLoop) override;
        void setPlayRate(double playRate) override;
        void setSource(const std::string& url) override;
        void setVolume(float volume) override;
        void togglePlayPause() override;
        void update() override;

        void OnEvent(DWORD event, DWORD_PTR param1, DWORD param2);

    private:
        IMFMediaEngine *m_pMediaEngine = nullptr;
        MFPlayerNotify *m_pPlayerNodify = nullptr;
        Texture* m_pRenderTarget = nullptr;
        bool m_isPlaying = false;
        IMFDXGIDeviceManager* m_pDXGIManager = nullptr;
    };
}

#endif
