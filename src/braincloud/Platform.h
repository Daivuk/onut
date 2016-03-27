//
//  Platform.h
//  BrainCloud
//
//  Created by Preston Jennings on 2015-09-10.
//
//

#ifndef BrainCloud_Platform_h
#define BrainCloud_Platform_h

#include <string>

namespace BrainCloud
{
    class Platform
    {
    public:
        static const Platform & AppleTVOS;
        static const Platform & BlackBerry;
        static const Platform & Facebook;
        static const Platform & GooglePlayAndroid;
        static const Platform & iOS;
        static const Platform & Linux;
        static const Platform & Mac;
        static const Platform & Web;
        static const Platform & Windows;
        static const Platform & WindowsPhone;
        static const Platform & Xbox360;
        static const Platform & PS3;
        static const Platform & XboxOne;
        static const Platform & PS4;
        static const Platform & Wii;
        static const Platform & PSVita;
        static const Platform & Tizen;
        static const Platform & Roku;
        static const Platform & Unknown;
        
        const std::string & toString() const;
        static const Platform & fromString(const std::string & in_platform);
        
    private:
        Platform(const std::string & in_platform);
        std::string m_value;
    };
}

#endif
