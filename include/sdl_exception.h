#ifndef SDL_EXCEPTION_H_INCLUDED
#define SDL_EXCEPTION_H_INCLUDED

#include <exception>
#include <SDL2/SDL.h>

/** Class for defining an SDL exception.
 * This is meant to be thrown right after a failed SDL operation.
 * The effective error code returned may also be passed to the exception,
 * but this behavior is not mandatory.
 */
 struct sdl_exception : public std::exception {

    int code;
    const char* msg;

    /// Default constructor, error code not provided
    sdl_exception(void)
    : code(0)
    , msg(SDL_GetError()) {
    }

    /// Int-argument constructor, with error code
    sdl_exception(int code)
    : code(code)
    , msg(SDL_GetError()) {
    }

    sdl_exception(const sdl_exception& other) = default;
    sdl_exception(sdl_exception&& other) = default;
    sdl_exception& operator=(const sdl_exception& other) = default;
    sdl_exception& operator=(sdl_exception&& other) = default;

    virtual const char* what(void) const noexcept {
        return msg;
    }
};

#endif // SDL_EXCEPTION_H_INCLUDED
