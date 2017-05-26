#include "../include/Framework.h"


int main()
{
    Framework f;
    f.init(696, 758+120, "Pacman");
    f.run();
    f.del();
}