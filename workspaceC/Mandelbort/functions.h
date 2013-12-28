#include <math.h>
#include "mpi.h"
#include "/home/lorenzo/workspaceC/Tests/src/CImg.h"
#include "stdlib.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <dirent.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <algorithm>
#include <cstring>

using namespace std;
using namespace cimg_library;




int free2duchar(unsigned char ***array);
int malloc2duchar(unsigned char ***array, int n, int m);
int malloc3duchar(unsigned char ****array, int n, int m, int i);
double RandomGen (double rangea, double rangeb);

int MandelbortCreation(int width, int height, unsigned char ****matrix);
