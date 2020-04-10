#ifndef __INTEROP_HPP__
#define __INTEROP_HPP__

// Handle foreign calls. This function is used inside of C++ manifolds. Any
// changes in the name will require a mirrored change in the morloc code. 
std::string foreign_call(std::string cmd){
    char buffer[256];
    std::string result = "";
    FILE* pipe = popen(cmd.c_str(), "r");
    while (fgets(buffer, sizeof buffer, pipe) != NULL) {
        result += buffer;
    }
    pclose(pipe);
    return(result);
}

#endif
