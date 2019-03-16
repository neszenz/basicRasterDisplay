#include "brd_util.hpp"

/** Log an SDL error with error msg to output stream of our choice
 * @param os - the output stream to write the message to
 * @param msg - error msg to write, format will be "msg error: SDL_GetError()"
 */
void logSDLError(std::ostream &os, const std::string msg) {
    // padding ensures a space between msg and rest of log string
    char padding = '\0';
    if(msg.back() != ' ') padding = ' ';

    os << msg << padding << "error: " << SDL_GetError() << std::endl;
}
void logSDLError(const std::string msg) {
    // padding ensures a space between msg and rest of log string
    char padding = '\0';
    if(msg.back() != ' ') padding = ' ';

    std::cerr << msg << padding << "error: " << SDL_GetError() << std::endl;
}
void throwSDLError(const std::string msg) {
    // padding ensures a space between msg and rest of log string
    char padding = '\0';
    if(msg.back() != ' ') padding = ' ';

    std::stringstream ss;
    ss << msg << padding << "error: " << SDL_GetError() << std::endl;
    throw std::runtime_error(ss.str());
}
