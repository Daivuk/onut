#pragma once
#if defined(WIN32)
// Onut
#include <onut/VideoPlayer.h>

// Third party
#include <Mfmediaengine.h>

// Forward
#include <onut/ForwardDeclaration.h>
OForwardDeclare(MFPlayer);

namespace onut
{
    class MFPlayerNotify final : public IMFMediaEngineNotify
    {
    public:
        MFPlayerNotify(const OMFPlayerRef& pPlayer);

        HRESULT STDMETHODCALLTYPE QueryInterface(REFIID riid, _COM_Outptr_ void __RPC_FAR *__RPC_FAR *ppvObject) override;
        ULONG STDMETHODCALLTYPE AddRef(void) override;
        ULONG STDMETHODCALLTYPE Release(void) override;
        HRESULT STDMETHODCALLTYPE EventNotify(_In_ DWORD event, _In_ DWORD_PTR param1, _In_  DWORD param2) override;

    private:
        ULONG m_ref = 1;
        OMFPlayerWeak m_pPlayer;
    };

    class MFPlayer final : public VideoPlayer, public std::enable_shared_from_this<MFPlayer>
    {
    public:
        ~MFPlayer();

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
        friend class VideoPlayer;

        void init(HWND handle);
        void init(const OTextureRef& pRenderTarget);

        IMFMediaEngine* m_pMediaEngine = nullptr;
        MFPlayerNotify* m_pPlayerNodify;
        OTextureRef m_pRenderTarget = nullptr;
        bool m_isPlaying = false;
        IMFDXGIDeviceManager* m_pDXGIManager = nullptr;
    };
}

#endif
