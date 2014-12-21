//--------------------------------------------------------------------------------------
// File: DynamicSoundEffectInstance.cpp
//
// THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF
// ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO
// THE IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A
// PARTICULAR PURPOSE.
//
// Copyright (c) Microsoft Corporation. All rights reserved.
//
// http://go.microsoft.com/fwlink/?LinkId=248929
//--------------------------------------------------------------------------------------

#include "pch.h"
#include "SoundCommon.h"

using namespace DirectX;


//======================================================================================
// DynamicSoundEffectInstance
//======================================================================================

// Internal object implementation class.
class DynamicSoundEffectInstance::Impl : public IVoiceNotify
{
public:
    Impl( _In_ AudioEngine* engine,
          _In_ DynamicSoundEffectInstance* object, _In_opt_ std::function<void(DynamicSoundEffectInstance*)> bufferNeeded,
          int sampleRate, int channels, int sampleBits, SOUND_EFFECT_INSTANCE_FLAGS flags ) :
        mBase(),
        mBufferEvent( INVALID_HANDLE_VALUE ),
        mBufferNeeded( nullptr ),
        mObject( object )
    {
        if ( ( sampleRate < XAUDIO2_MIN_SAMPLE_RATE )
             || ( sampleRate > XAUDIO2_MAX_SAMPLE_RATE ) )
        {
            DebugTrace( "DynamicSoundEffectInstance sampleRate must be in range %u...%u\n", XAUDIO2_MIN_SAMPLE_RATE, XAUDIO2_MAX_SAMPLE_RATE );
            throw std::invalid_argument( "DynamicSoundEffectInstance" );
        }

        if ( !channels || ( channels > 8 ) )
        {
            DebugTrace( "DynamicSoundEffectInstance channels must be in range 1...8\n" );
            throw std::invalid_argument( "DynamicSoundEffectInstance" );
        }

        switch ( sampleBits )
        {
        case 8:
        case 16:
            break;

        default:
            DebugTrace( "DynamicSoundEffectInstance sampleBits must be 8-bit or 16-bit\n" );
            throw std::invalid_argument( "DynamicSoundEffectInstance" );
        }

#if (_WIN32_WINNT >= _WIN32_WINNT_VISTA)
        mBufferEvent = CreateEventEx( nullptr, nullptr, 0, EVENT_MODIFY_STATE | SYNCHRONIZE );
#else
        mBufferEvent = CreateEvent( nullptr, FALSE, FALSE, nullptr );
#endif
        if ( !mBufferEvent )
        {
            throw std::exception( "CreateEvent" );
        }

        CreateIntegerPCM( &mWaveFormat, sampleRate, channels, sampleBits );

        assert( engine != 0 );
        engine->RegisterNotify( this, true );

        mBase.Initialize( engine, &mWaveFormat, flags );

        mBufferNeeded = bufferNeeded;
    }

    ~Impl()
    {
        mBase.DestroyVoice();

        if ( mBase.engine )
        {
            mBase.engine->UnregisterNotify( this, false, true );
            mBase.engine = nullptr;
        }

        CloseHandle( mBufferEvent );
    }

    void Play();

    void Resume();

    void SubmitBuffer( _In_reads_bytes_(audioBytes) const uint8_t* pAudioData, uint32_t offset, size_t audioBytes );

    const WAVEFORMATEX* GetFormat() const { return &mWaveFormat; } ;

    // IVoiceNotify
    virtual void __cdecl OnBufferEnd() override
    {
        SetEvent( mBufferEvent );
    }

    virtual void __cdecl OnCriticalError() override
    {
        mBase.OnCriticalError();
    }

    virtual void __cdecl OnReset() override
    {
        mBase.OnReset();
    }

    virtual void __cdecl OnUpdate() override;

    virtual void __cdecl OnDestroyEngine() override
    {
        mBase.OnDestroy();
    }

    virtual void __cdecl OnTrim() override
    {
        mBase.OnTrim();
    }

    virtual void __cdecl GatherStatistics( AudioStatistics& stats ) const override
    {
        mBase.GatherStatistics(stats);
    }

    SoundEffectInstanceBase                             mBase;

private:
    HANDLE                                              mBufferEvent;
    std::function<void(DynamicSoundEffectInstance*)>    mBufferNeeded;
    DynamicSoundEffectInstance*                         mObject;
    WAVEFORMATEX                                        mWaveFormat;
};


void DynamicSoundEffectInstance::Impl::Play()
{
    if ( !mBase.voice )
    {
        mBase.AllocateVoice( &mWaveFormat );
    }

    (void)mBase.Play();

    if ( mBase.voice && ( mBase.state == PLAYING ) && ( mBase.GetPendingBufferCount() <= 2 ) )
    {
        SetEvent( mBufferEvent );
    }
}


void DynamicSoundEffectInstance::Impl::Resume()
{
    if ( mBase.voice && ( mBase.state == PAUSED ) )
    {
        mBase.Resume();

        if ( ( mBase.state == PLAYING ) && ( mBase.GetPendingBufferCount() <= 2 ) )
        {
            SetEvent( mBufferEvent );
        }
    }
}


_Use_decl_annotations_
void DynamicSoundEffectInstance::Impl::SubmitBuffer( const uint8_t* pAudioData, uint32_t offset, size_t audioBytes )
{
    if ( !pAudioData || !audioBytes )
        throw std::exception( "Invalid audio data buffer" );

#ifdef _M_X64
    if ( audioBytes > 0xFFFFFFFF )
        throw std::out_of_range( "SubmitBuffer" );
#endif

    XAUDIO2_BUFFER buffer;
    memset( &buffer, 0, sizeof(buffer) );

    buffer.AudioBytes = static_cast<UINT32>( audioBytes );
    buffer.pAudioData = pAudioData;

    if( offset )
    {
        assert( mWaveFormat.wFormatTag == WAVE_FORMAT_PCM );
        buffer.PlayBegin = offset / mWaveFormat.nBlockAlign;
        buffer.PlayLength = static_cast<UINT32>( ( audioBytes - offset ) / mWaveFormat.nBlockAlign );
    }

    buffer.pContext = this;

    HRESULT hr = mBase.voice->SubmitSourceBuffer( &buffer, nullptr );
    if ( FAILED(hr) )
    {
#ifdef _DEBUG
        DebugTrace( "ERROR: DynamicSoundEffectInstance failed (%08X) when submitting buffer:\n", hr );

        DebugTrace( "\tFormat Tag %u, %u channels, %u-bit, %u Hz, %Iu bytes [%u offset)\n", mWaveFormat.wFormatTag, 
                    mWaveFormat.nChannels, mWaveFormat.wBitsPerSample, mWaveFormat.nSamplesPerSec, audioBytes, offset );
#endif
        throw std::exception( "SubmitSourceBuffer" );
    }
}


void DynamicSoundEffectInstance::Impl::OnUpdate()
{
    DWORD result = WaitForSingleObjectEx( mBufferEvent, 0, FALSE );
    switch( result )
    {
    case WAIT_TIMEOUT:
        break;

    case WAIT_OBJECT_0:
        if( mBufferNeeded )
        {
            // This callback happens on the same thread that called AudioEngine::Update()
            mBufferNeeded( mObject );
        }
        break;
    
    case WAIT_FAILED:
        throw std::exception( "WaitForSingleObjectEx" );
    }
}



//--------------------------------------------------------------------------------------
// DynamicSoundEffectInstance
//--------------------------------------------------------------------------------------

#pragma warning( disable : 4355 )

// Public constructors
_Use_decl_annotations_
DynamicSoundEffectInstance::DynamicSoundEffectInstance( AudioEngine* engine,
    std::function<void(DynamicSoundEffectInstance*)> bufferNeeded,
    int sampleRate, int channels, int sampleBits, SOUND_EFFECT_INSTANCE_FLAGS flags ) :
    pImpl( new Impl( engine, this, bufferNeeded, sampleRate, channels, sampleBits, flags ) )
{
}


// Move constructor.
DynamicSoundEffectInstance::DynamicSoundEffectInstance(DynamicSoundEffectInstance&& moveFrom)
  : pImpl(std::move(moveFrom.pImpl))
{
}


// Move assignment.
DynamicSoundEffectInstance& DynamicSoundEffectInstance::operator= (DynamicSoundEffectInstance&& moveFrom)
{
    pImpl = std::move(moveFrom.pImpl);
    return *this;
}


// Public destructor.
DynamicSoundEffectInstance::~DynamicSoundEffectInstance()
{
}


// Public methods.
void DynamicSoundEffectInstance::Play()
{
    pImpl->Play();
}


void DynamicSoundEffectInstance::Stop( bool immediate )
{
    bool looped = false;
    pImpl->mBase.Stop( immediate, looped );
}


void DynamicSoundEffectInstance::Pause()
{
    pImpl->mBase.Pause();
}


void DynamicSoundEffectInstance::Resume()
{
    pImpl->Resume();
}


void DynamicSoundEffectInstance::SetVolume( float volume )
{
    pImpl->mBase.SetVolume( volume );
}


void DynamicSoundEffectInstance::SetPitch( float pitch )
{
    pImpl->mBase.SetPitch( pitch );
}


void DynamicSoundEffectInstance::SetPan( float pan )
{
    pImpl->mBase.SetPan( pan );
}


void DynamicSoundEffectInstance::Apply3D( const AudioListener& listener, const AudioEmitter& emitter )
{
    pImpl->mBase.Apply3D( listener, emitter );
}


_Use_decl_annotations_
void DynamicSoundEffectInstance::SubmitBuffer( const uint8_t* pAudioData, size_t audioBytes )
{
    pImpl->SubmitBuffer( pAudioData, 0, audioBytes );
}


_Use_decl_annotations_
void DynamicSoundEffectInstance::SubmitBuffer( const uint8_t* pAudioData, uint32_t offset, size_t audioBytes )
{
    pImpl->SubmitBuffer( pAudioData, offset, audioBytes );
}


// Public accessors.
SoundState DynamicSoundEffectInstance::GetState()
{
    return pImpl->mBase.GetState( false );
}


size_t DynamicSoundEffectInstance::GetSampleDuration( size_t bytes ) const
{
    auto wfx = pImpl->GetFormat();
    if ( !wfx || !wfx->wBitsPerSample || !wfx->nChannels )
        return 0;

    return static_cast<size_t>( ( uint64_t( bytes ) * 8 )
                                / uint64_t( wfx->wBitsPerSample * wfx->nChannels ) );
}


size_t DynamicSoundEffectInstance::GetSampleDurationMS( size_t bytes ) const
{
    auto wfx = pImpl->GetFormat();
    if ( !wfx || !wfx->nAvgBytesPerSec )
        return 0;

    return static_cast<size_t>( ( uint64_t(bytes) * 1000 ) / wfx->nAvgBytesPerSec );
}


size_t DynamicSoundEffectInstance::GetSampleSizeInBytes( uint64_t duration ) const
{
    auto wfx = pImpl->GetFormat();
    if ( !wfx || !wfx->nSamplesPerSec )
        return 0;

    return static_cast<size_t>( ( ( duration * wfx->nSamplesPerSec ) / 1000 ) * wfx->nBlockAlign );
}


int DynamicSoundEffectInstance::GetPendingBufferCount() const
{
    return pImpl->mBase.GetPendingBufferCount();
}


const WAVEFORMATEX* DynamicSoundEffectInstance::GetFormat() const
{
    return pImpl->GetFormat();
}