#pragma once

// a debug MACRO for loggin printing of variables  
#if DEBUG
#include <iostream>
#include <glm/glm.hpp>
#define LOGVAR(x,v) std::cout << x << " " << v << "\n"
#define LOG(x) std::cout<<x<<"\n"
#define LOGVEC2(name, x) std::cout<< name << "x = "<<x.x << "y = "<< x.y << "\n"
#define LOGVEC3(name, vec) std::cout<<name<<"x="<<vec.x << " y="<<vec.y<<" z="<<vec.z<<"\n"
#else
#define LOG(x,v)
#endif
