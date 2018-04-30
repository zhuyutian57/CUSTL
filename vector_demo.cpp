/* file: test_vector.h
 * date: 2018/04/29
 * author: axp (Xiao Peng)
 * 
 * Modified By JHX (2018-04-30 8:26)
 * 
 */

#include <iostream>
#include "assert.h"

#include "vector.h"

struct st {
    int x;
    st(){}
    st(int t):x(t){}
};

int main() {
    custl::vector<st> ve;
    ve.push_back(st(100));
    ve.push_back(st(50));

    assert(ve[0].x == 100);
    assert(ve[1].x == 50);
    assert(ve.size() == 2);

    ve.pop_back();

    assert(ve[0].x == 100);
    assert(ve.size() == 1);

    return 0;
}
