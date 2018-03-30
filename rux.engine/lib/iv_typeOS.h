//
// iv_typeOS.h
//

#ifndef WinLinCoreTypeOS_h_507A63ACB27743cc91626A3A39955790
#	define WinLinCoreTypeOS_h_507A63ACB27743cc91626A3A39955790

# ifdef windows_os
#   undef windows_os
# endif
# ifdef sun_os
#   undef sun_os
# endif
# ifdef linux_os
#   undef linux_os
# endif
# ifdef unix_os
#   undef unix_os
# endif
# ifdef elbrus_os
#   undef elbrus_os
# endif
# ifdef __GLIBCXX__
#   if defined(__sun) || defined(sun)
#     define sun_os
#   else
#     if ( defined(__MACH__) && defined(__APPLE__) ) || defined(__NetBSD__) || defined(__FreeBSD__) || defined(__OpenBSD__)
#       define unix_os
#     else
#       ifdef __e2k__
#         define elbrus_os
#       else
#         define linux_os
#       endif
#     endif
#   endif
# else
#   if defined(_WINDOWS) || defined(WIN32) || defined(_WIN32) || defined (__MINGW32__) || defined(_MSC_VER)
#     define windows_os
#   else
#     if defined(__sun) || defined(sun)
#       define sun_os
#     else
#       if ( defined(__MACH__) && defined(__APPLE__) ) || defined(__NetBSD__) || defined(__FreeBSD__) || defined(__OpenBSD__)
#         define unix_os
#       else
#         if defined(__e2k__) || defined(__elbrus__) || defined(__generic__) || defined(__iset__) || defined(__elbrus_2cplus__) || defined(__elbrus_4c__) || defined(__elbrus_1cplus__) || defined(__elbrus_8c__) || defined(__elbrus_8c2__)
#           define elbrus_os
#         else
#           define linux_os
#         endif
#       endif
#     endif
#   endif
# endif

#endif
