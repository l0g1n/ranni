#pragma once

//
// global macros
//

// TODO

//
// x86 / x64 intrinsic headers
//

#include <intrin.h>

//
// C compatibility headers
//

#include <cassert>
#include <cctype>
#include <cerrno>
#include <cfenv>
#include <cfloat>
#include <cinttypes>
#include <climits>
#include <clocale>
#include <cmath>
#include <csetjmp>
#include <csignal>
#include <cstdarg>
#include <cstddef>
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <ctime>
#include <cuchar>
#include <cwchar>
#include <cwctype>

//
// C++ 98/03 headers
//

#include <algorithm>
#include <bitset>
#include <complex>
#include <deque>
#include <exception>
#include <fstream>
#include <functional>
#include <iomanip>
#include <ios>
#include <iosfwd>
#include <iostream>
#include <istream>
#include <iterator>
#include <limits>
#include <list>
#include <locale>
#include <map>
#include <memory>
#include <new>
#include <numeric>
#include <ostream>
#include <queue>
#include <set>
#include <sstream>
#include <stack>
#include <stdexcept>
#include <streambuf>
#include <string>
#include <typeinfo>
#include <utility>
#include <valarray>
#include <vector>

//
// C++ 11 headers
//

#include <array>
#include <atomic>
#include <chrono>
#include <condition_variable>
#include <forward_list>
#include <future>
#include <initializer_list>
#include <mutex>
#include <random>
#include <ratio>
#include <regex>
#include <scoped_allocator>
#include <system_error>
#include <thread>
#include <tuple>
#include <type_traits>
#include <typeindex>
#include <unordered_map>
#include <unordered_set>

//
// C++ 14 headers
//

#include <shared_mutex>

//
// C++ 17 headers
//

#include <any>
#include <charconv>
#include <execution>
#include <filesystem>
#include <memory_resource>
#include <optional>
#include <string_view>
#include <variant>

//
// C++ 20 headers
//

#include <barrier>
#include <bit>
#include <compare>
#include <concepts>
#include <coroutine>
#include <format>
#include <latch>
#include <numbers>
#include <ranges>
#include <semaphore>
#include <source_location>
#include <span>
#include <stop_token>
#include <syncstream>
#include <version>

//
// C / C++ runtime headers
//

#include <corecrt.h>

#include <crtdbg.h>
#include <crtdefs.h>

//
// MFC / ATL headers
//

#define _WIN32_WINNT _WIN32_WINNT_WIN10

#include <afx.h>
#include <afxwin.h>

#include <atlconv.h>
#include <atlcore.h>
#include <atlsafe.h>
#include <atltime.h>

//
// Windows OS-specific headers and libraries
//

#define WIN32_LEAN_AND_MEAN

#include <Windows.h>

#include <d3d11.h>
#include <devioctl.h>
#include <DbgHelp.h>
#include <PathCch.h>
#include <Psapi.h>
#include <ShlObj.h>
#include <ShlObj_core.h>
#include <strsafe.h>
#include <tchar.h>
#include <TlHelp32.h>
#include <WinSock2.h>
#include <WinUser.h>
#include <winternl.h>
#include <WS2tcpip.h>

#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "dbghelp.lib")
#pragma comment(lib, "pathcch.lib")
#pragma comment(lib, "shell32.lib")
#pragma comment(lib, "Version.lib")
#pragma comment(lib, "WS2_32.lib")
