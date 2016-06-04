#include <stdio.h>
#include <boost/static_assert.hpp>
#include <string.h>

class Timestamp {
private:
        int64_t m;
};

BOOST_STATIC_ASSERT(sizeof(Timestamp) != sizeof(int64_t));

int main()
{
        
        return 0;
}
